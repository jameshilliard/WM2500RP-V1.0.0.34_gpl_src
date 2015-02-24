/*
 *	Copyright 1994, 1995, 2000 Neil Russell.
 *	(See License)
 *	Copyright 2000, 2001 DENX Software Engineering, Wolfgang Denk, wd@denx.de
 */

#include <common.h>
#include <command.h>
#include <net.h>
#include "tftp.h"
#include "bootp.h"
#include <dni_common.h>

#ifdef CFG_NMRP
#include "nmrp.h"
#endif
#undef	ET_DEBUG

#if (CONFIG_COMMANDS & CFG_CMD_NET)

#define WELL_KNOWN_PORT	69		/* Well known TFTP port #		*/
#define TIMEOUT		5		/* Seconds to timeout for a lost pkt	*/
#ifndef	CONFIG_NET_RETRY_COUNT
# define TIMEOUT_COUNT	10		/* # of timeouts before giving up  */
#else
# define TIMEOUT_COUNT  (CONFIG_NET_RETRY_COUNT * 2)
#endif
					/* (for checking the image size)	*/
#define HASHES_PER_LINE	65		/* Number of "loading" hashes per line	*/

/*
 *	TFTP operations.
 */
#define TFTP_RRQ	1
#define TFTP_WRQ	2
#define TFTP_DATA	3
#define TFTP_ACK	4
#define TFTP_ERROR	5
#define TFTP_OACK	6


static int	TftpServerPort;		/* The UDP port at their end		*/
static int	TftpOurPort;		/* The UDP port at our end		*/
static int	TftpTimeoutCount;
static ulong	TftpBlock;		/* packet sequence number		*/
static ulong	TftpLastBlock;		/* last packet sequence number received */
static ulong	TftpBlockWrap;		/* count of sequence number wraparounds */
static ulong	TftpBlockWrapOffset;	/* memory offset due to wrapping	*/
static int	TftpState;

#define STATE_RRQ	1
#define STATE_DATA	2
#define STATE_TOO_LARGE	3
#define STATE_BAD_MAGIC	4
#define STATE_OACK	5

#ifdef CFG_NMRP
uchar TestNmrpIP[2] = {192,168};
static int Nmrp_Waiting_TimeoutCount = 0;
static int nmrpconfflag = 1;
#endif

#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
#define STATE_WRQ 6
#define STATE_BAD_IMAGE_TYPE 7
#define STATE_BAD_IMAGE_CHKSUM 8
static int TftpClientPort;
static uchar ImageCheckSum = 0;
#endif

#define TFTP_BLOCK_SIZE		512		    /* default TFTP block size	*/
#define TFTP_SEQUENCE_SIZE	((ulong)(1<<16))    /* sequence number is 16 bit */

#define DEFAULT_NAME_LEN	(8 + 4 + 1)
static char default_filename[DEFAULT_NAME_LEN];
static char *tftp_filename;

#ifdef CFG_DIRECT_FLASH_TFTP
extern flash_info_t flash_info[];
#endif

static __inline__ void
store_block (unsigned block, uchar * src, unsigned len)
{
	ulong offset = block * TFTP_BLOCK_SIZE + TftpBlockWrapOffset;
	ulong newsize = offset + len;
#ifdef CFG_DIRECT_FLASH_TFTP
	int i, rc = 0;

	for (i=0; i<CFG_MAX_FLASH_BANKS; i++) {
		/* start address in flash? */
		if (load_addr + offset >= flash_info[i].start[0]) {
			rc = 1;
			break;
		}
	}

	if (rc) { /* Flash is destination for this packet */
		rc = flash_write ((char *)src, (ulong)(load_addr+offset), len);
		if (rc) {
			flash_perror (rc);
			NetState = NETLOOP_FAIL;
			return;
		}
	}
	else
#endif /* CFG_DIRECT_FLASH_TFTP */
	{
#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
		if(NetRunTftpServer)
		{
			uchar *dest=(uchar *)(load_addr + offset);
			while(len--)
			{
				ImageCheckSum = (ImageCheckSum + (*src)) & 0xFF;
				*dest++=*src++;
			}
		}
		else
#endif
		(void)memcpy((void *)(load_addr + offset), src, len);
	}

	if (NetBootFileXferSize < newsize)
		NetBootFileXferSize = newsize;
}

static void TftpSend (void);
static void TftpTimeout (void);

/**********************************************************************/

static void
TftpSend (void)
{
	volatile uchar *	pkt;
	volatile uchar *	xp;
	int			len = 0;
	volatile ushort *s;

	/*
	 *	We will always be sending some sort of packet, so
	 *	cobble together the packet headers now.
	 */
	pkt = NetTxPacket + NetEthHdrSize() + IP_HDR_SIZE;

	switch (TftpState) {

	case STATE_RRQ:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_RRQ);
		pkt = (uchar *)s;
		strcpy ((char *)pkt, tftp_filename);
		pkt += strlen(tftp_filename) + 1;
		strcpy ((char *)pkt, "octet");
		pkt += 5 /*strlen("octet")*/ + 1;
		strcpy ((char *)pkt, "timeout");
		pkt += 7 /*strlen("timeout")*/ + 1;
		sprintf((char *)pkt, "%d", TIMEOUT);
#ifdef ET_DEBUG
		printf("send option \"timeout %s\"\n", (char *)pkt);
#endif
		pkt += strlen((char *)pkt) + 1;
		len = pkt - xp;
		break;

	case STATE_DATA:
	case STATE_OACK:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ACK);
		*s++ = htons(TftpBlock);
		pkt = (uchar *)s;
		len = pkt - xp;
		break;

	case STATE_TOO_LARGE:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ERROR);
		*s++ = htons(3);
		pkt = (uchar *)s;
		strcpy ((char *)pkt, "File too large");
		pkt += 14 /*strlen("File too large")*/ + 1;
		len = pkt - xp;
		break;

	case STATE_BAD_MAGIC:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ERROR);
		*s++ = htons(2);
		pkt = (uchar *)s;
		strcpy ((char *)pkt, "File has bad magic");
		pkt += 18 /*strlen("File has bad magic")*/ + 1;
		len = pkt - xp;
		break;
#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
		case STATE_BAD_IMAGE_TYPE:
			xp = pkt;
			s = (ushort *)pkt;
			*s++=htons(TFTP_ERROR);
			*s++=htons(2);
			strcpy ((char *)pkt, "No binary mode used");
			pkt += 19/*strlen("No binary mode used")*/ + 1;
			len = pkt - xp;
			break;
		case STATE_BAD_IMAGE_CHKSUM:
			xp = pkt;
			s=(ushort *)pkt;
			*s++=htons(TFTP_ERROR);
			*s++=htons(2);
			strcpy ((char *)pkt, "File has bad checksum");
			pkt += 21 /*"File has bad checksum"*/ +1;
			len = pkt -xp;
			break;
#endif
	}

#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
	if (NetRunTftpServer)
	{
		NetSendUDPPacket(TftpClientEther, TftpClientIP, TftpClientPort,TftpOurPort,len);
		if (TftpState == STATE_BAD_IMAGE_TYPE)
		{
			puts("\nNo binary mode used!\n");
			ResetTftpServer();
		}
		else if (TftpState == STATE_BAD_IMAGE_CHKSUM)
		{
			puts("File has bad checksum!\n");
			ResetTftpServer();
		}
	}
	else
#endif
	NetSendUDPPacket(NetServerEther, NetServerIP, TftpServerPort, TftpOurPort, len);
}


static void
TftpHandler (uchar * pkt, unsigned dest, unsigned src, unsigned len)
{
	ushort proto;
	ushort *s;

	if (dest != TftpOurPort) {
		return;
	}
	if (TftpState != STATE_RRQ && src != TftpServerPort) {
		return;
	}

	if (len < 2) {
		return;
	}
	len -= 2;
	/* warning: don't use increment (++) in ntohs() macros!! */
	s = (ushort *)pkt;
	proto = *s++;
	pkt = (uchar *)s;
	switch (ntohs(proto)) {

	case TFTP_RRQ:
	case TFTP_WRQ:
	case TFTP_ACK:
		break;
	default:
		break;

	case TFTP_OACK:
#ifdef ET_DEBUG
		printf("Got OACK: %s %s\n", pkt, pkt+strlen(pkt)+1);
#endif
		TftpState = STATE_OACK;
		TftpServerPort = src;
		TftpSend (); /* Send ACK */
		break;
	case TFTP_DATA:
		if (len < 2)
			return;
		len -= 2;
		TftpBlock = ntohs(*(ushort *)pkt);

		/*
		 * RFC1350 specifies that the first data packet will
		 * have sequence number 1. If we receive a sequence
		 * number of 0 this means that there was a wrap
		 * around of the (16 bit) counter.
		 */
		if (TftpBlock == 0) {
			TftpBlockWrap++;
			TftpBlockWrapOffset += TFTP_BLOCK_SIZE * TFTP_SEQUENCE_SIZE;
			printf ("\n\t %lu MB reveived\n\t ", TftpBlockWrapOffset>>20);
		} else {
			if (((TftpBlock - 1) % 10) == 0) {
				putc ('#');
			} else if ((TftpBlock % (10 * HASHES_PER_LINE)) == 0) {
				puts ("\n\t ");
			}
		}

#ifdef ET_DEBUG
		if (TftpState == STATE_RRQ) {
			puts ("Server did not acknowledge timeout option!\n");
		}
#endif

		if (TftpState == STATE_RRQ || TftpState == STATE_OACK) {
			/* first block received */
			TftpState = STATE_DATA;
			TftpServerPort = src;
			TftpLastBlock = 0;
			TftpBlockWrap = 0;
			TftpBlockWrapOffset = 0;

			if (TftpBlock != 1) {	/* Assertion */
				printf ("\nTFTP error: "
					"First block is not block 1 (%ld)\n"
					"Starting again\n\n",
					TftpBlock);
				NetStartAgain ();
				break;
			}
		}

		if (TftpBlock == TftpLastBlock) {
			/*
			 *	Same block again; ignore it.
			 */
			break;
		}

		TftpLastBlock = TftpBlock;
		NetSetTimeout (TIMEOUT * CFG_HZ, TftpTimeout);

		store_block (TftpBlock - 1, pkt + 2, len);

		/*
		 *	Acknoledge the block just received, which will prompt
		 *	the server for the next one.
		 */
		TftpSend ();

		if (len < TFTP_BLOCK_SIZE) {
			/*
			 *	We received the whole thing.  Try to
			 *	run it.
			 */
			puts ("\ndone\n");
			NetState = NETLOOP_SUCCESS;
		}
		break;

	case TFTP_ERROR:
		printf ("\nTFTP error: '%s' (%d)\n",
					pkt + 2, ntohs(*(ushort *)pkt));
		puts ("Starting again\n\n");
		NetStartAgain ();
		break;
	}
}


static void
TftpTimeout (void)
{
	if (++TftpTimeoutCount > TIMEOUT_COUNT) {
		puts ("\nRetry count exceeded; starting again\n");
		NetStartAgain ();
	} else {
		puts ("T ");
		NetSetTimeout (TIMEOUT * CFG_HZ, TftpTimeout);
		TftpSend ();
	}
}


void
TftpStart (void)
{
#ifdef CONFIG_TFTP_PORT
	char *ep;             /* Environment pointer */
#endif

	if (BootFile[0] == '\0') {
		sprintf(default_filename, "%02lX%02lX%02lX%02lX.img",
			NetOurIP & 0xFF,
			(NetOurIP >>  8) & 0xFF,
			(NetOurIP >> 16) & 0xFF,
			(NetOurIP >> 24) & 0xFF	);
		tftp_filename = default_filename;

		printf ("*** Warning: no boot file name; using '%s'\n",
			tftp_filename);
	} else {
		tftp_filename = BootFile;
	}

#if defined(CONFIG_NET_MULTI)
	printf ("Using %s device\n", eth_get_name());
#endif
	puts ("TFTP from server ");	print_IPaddr (NetServerIP);
	puts ("; our IP address is ");	print_IPaddr (NetOurIP);

	/* Check if we need to send across this subnet */
	if (NetOurGatewayIP && NetOurSubnetMask) {
	    IPaddr_t OurNet 	= NetOurIP    & NetOurSubnetMask;
	    IPaddr_t ServerNet 	= NetServerIP & NetOurSubnetMask;

	    if (OurNet != ServerNet) {
		puts ("; sending through gateway ");
		print_IPaddr (NetOurGatewayIP) ;
	    }
	}
	putc ('\n');

	printf ("Filename '%s'.", tftp_filename);

	if (NetBootFileSize) {
		printf (" Size is 0x%x Bytes = ", NetBootFileSize<<9);
		print_size (NetBootFileSize<<9, "");
	}

	putc ('\n');

	printf ("Load address: 0x%lx\n", load_addr);

	puts ("Loading: *\b");

	NetSetTimeout (TIMEOUT * CFG_HZ, TftpTimeout);
	NetSetHandler (TftpHandler);

	TftpServerPort = WELL_KNOWN_PORT;
	TftpTimeoutCount = 0;
	TftpState = STATE_RRQ;
	/* Use a pseudo-random port unless a specific port is set */
	TftpOurPort = 1024 + (get_timer(0) % 3072);
#ifdef CONFIG_TFTP_PORT
	if ((ep = getenv("tftpdstp")) != NULL) {
		TftpServerPort = simple_strtol(ep, NULL, 10);
	}
	if ((ep = getenv("tftpsrcp")) != NULL) {
		TftpOurPort= simple_strtol(ep, NULL, 10);
	}
#endif
	TftpBlock = 0;

	/* zero out server ether in case the server ip has changed */
	memset(NetServerEther, 0, 6);

	TftpSend ();
}

#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
static int TftpServerWaiting = 1;
static uint TftpLedCount = 0;

static void
TftpServerTimeout (void)
{
	if(TftpServerWaiting)
	{
#ifdef CFG_NMRP
		if(NmrpState == STATE_CONFIGING )
		{
			if(nmrpconfflag){
				puts("\nwaiting nmrp server to upload firmware!\n");
				nmrpconfflag = 0;
			}
			if( ++Nmrp_Waiting_TimeoutCount > NMRP_MAX_RETRY_TFTP_UL)
			{
				puts("\n retry tftp_upload count exceeded;\n");
				Nmrp_Waiting_TimeoutCount = 0;
				NmrpState = STATE_LISTENING;
				NmrpSend();
			}else{
				puts("T");
				NmrpSend();
				NetSetTimeout((NMRP_TIMEOUT_REQ*CFG_HZ)/2,TftpServerTimeout);
			}
		}else{
#endif
		TftpLedCount++;
		if ((TftpLedCount % 2)== 1)
		{
			printf("Upgrade Mode\b\b\b\b\b\b\b\b\b\b\b\b");
			/*power LED (GREEN) 0.25 second on */
#if defined(CONFIG_WNR1000V2)||defined(CONFIG_WNR1100)
			power_led(0);
#else
			board_power_led(0);
#endif
			NetSetTimeout ((CFG_HZ*1)/4, TftpServerTimeout);
		}else{
			/* power LED (GREEN) 0.75 second off */
#if defined(CONFIG_WNR1000V2)||defined(CONFIG_WNR1100)
			power_led(1);
#else
			board_power_led(1);
#endif
			printf("            \b\b\b\b\b\b\b\b\b\b\b\b");
			NetSetTimeout ((CFG_HZ*3)/4, TftpServerTimeout);
		}
#ifdef CFG_NMRP
	}
#endif
	}
	else{
		if (++TftpTimeoutCount > TIMEOUT_COUNT) {
			puts ("\nRetry to wait TFTP Client's Data count exceeded; starting again\n");
			ResetTftpServer ();
		} else {
			puts ("T ");
			NetSetTimeout (TIMEOUT * CFG_HZ, TftpServerTimeout);
			TftpSend ();
		}
	}
}

static void
TftpServerHandler (uchar * pkt, unsigned dest , unsigned src ,unsigned len )
{
	ushort  proto;
	ushort *s;

	if (dest != TftpOurPort) {
		return;
	}
	if (TftpState != STATE_WRQ && src != TftpClientPort) {
		return;
	}

	if (len < 2) {
		return;
	}
	len -= 2;
	/* warning: don't use increment (++) in ntohs() macros!! */
	s = (ushort *)pkt;
	proto = *s++;
	pkt = (uchar *)s;
	switch(ntohs(proto)){
		case TFTP_RRQ:
		case TFTP_ACK:
		case TFTP_OACK:
			break;

		case TFTP_WRQ:
			TftpClientPort = src;
			TftpServerWaiting = 0;

			pkt += strlen((char *)pkt) + 1;

			if(strcmp(pkt, "octet"))
			{
				TftpState = STATE_BAD_IMAGE_TYPE;
			}
			else
			{
				TftpState = STATE_OACK;
				puts ("\nRcv:\n\t");
			}
			TftpSend (); /* Send ACK */
			break;

		case TFTP_DATA:
			if (len < 2)
				return;
			len -= 2;
			TftpBlock = ntohs(*(ushort *)pkt);

			if (TftpBlock == 0)
			{
				TftpBlockWrap++;
				TftpBlockWrapOffset += TFTP_BLOCK_SIZE * TFTP_SEQUENCE_SIZE;
				printf ("\n\t %lu MB reveived\n\t ", TftpBlockWrapOffset>>20);
			}

			else
			{
				if (((TftpBlock - 1) % 10) == 0)
				{
					putc ('.');
				}
				else if ((TftpBlock % (10 * HASHES_PER_LINE)) == 0)
				{
					puts ("\n\t");
				}
			}
			if (TftpState == STATE_OACK)
			{
				/* first block received */
				TftpState = STATE_DATA;
				TftpLastBlock = 0;
				TftpBlockWrap = 0;
				TftpBlockWrapOffset = 0;

				if (TftpBlock != 1)
				{
					/* Assertion */
					printf ("\nTFTP error: "
						"First block is not block 1 (%ld)\n"
						"Starting again\n\n",
						TftpBlock);
					ResetTftpServer ();
					break;
				}
			}

			if (TftpBlock == TftpLastBlock)
			{
				break;
			}

			TftpLastBlock = TftpBlock;
			NetSetTimeout (TIMEOUT * CFG_HZ, TftpServerTimeout);

			store_block (TftpBlock - 1, pkt + 2, len);
			if(len < TFTP_BLOCK_SIZE)
			{
				ImageCheckSum = ~ImageCheckSum;
				if ((ImageCheckSum != 0x00) &&
						(!image_match_open_source_fw_id(load_addr) && !board_model_id_match_open_source_id()))
				{
					TftpState = STATE_BAD_IMAGE_CHKSUM;
				}
			}
			TftpSend ();
			if (len < TFTP_BLOCK_SIZE && TftpState == STATE_DATA)
			{
				puts ("\nDone!\n");
				NetState = NETLOOP_SUCCESS;
			}

			break;

		case TFTP_ERROR:
			printf ("\nTFTP error: '%s' (%d)\n", pkt + 2, ntohs(*(ushort *)pkt));
			puts ("Starting again\n\n");
			ResetTftpServer();
			break;
	}
}

void
TftpServerStart(void)
{
#ifdef CFG_NMRP
	if(NmrpState != STATE_CONFIGING)
#endif
	puts ("\nThe Router is in TFTP Server Firmware Recovery mode NOW!\n");

	puts ("Listening on Port : 69, IP Address: ");
	print_IPaddr (NetOurIP);
	puts ("...\n");
	NetSetTimeout (CFG_HZ/10, TftpServerTimeout);
	NetSetHandler (TftpServerHandler);

	TftpOurPort  = WELL_KNOWN_PORT;
	TftpTimeoutCount = 0;
	TftpClientPort = 0;
	TftpState = STATE_WRQ;
	TftpBlock = 0;

	TftpServerWaiting = 1;
	TftpLedCount = 0;

	ImageCheckSum = 0;

	/* zero out client ether in case the client ip has changed */
	memset(TftpClientEther, 0, 6);
	TftpClientIP = 0;
}
#endif

#endif /* CFG_CMD_NET */
