#ifndef _BR_IGMP_H
#define _BR_IGMP_H

#include <linux/types.h>
#include <linux/in.h>
#include <linux/kernel.h> 
#include <linux/netfilter.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/if_vlan.h>

/* IGMPv1/IGMPv2 report format */
typedef struct _igmpr_t {
  u_char          igmpr_type;  		/* version & type */
  u_char          igmpr_code;  		/* unused */
  u_short         igmpr_cksum; 		/* IP-style checksum */
  struct in_addr  igmpr_group; 		/* group address being reported */
} igmpr_t;

/* IGMPv3 group record format */
typedef struct _igmp_grouprec_t {
  u_char          igmpg_type;		/* record type */
  u_char          igmpg_datalen;	/* amount of aux data */
  u_short         igmpg_numsrc;		/* number of sources */
  struct in_addr  igmpg_group;		/* the group being reported */
  struct in_addr  igmpg_sources[0];	/* source addresses */
} igmp_grouprec_t;

/* IGMPv3 report format */
typedef struct _igmp_report_t {
  u_char          igmpr_type;		/* version & type of IGMP message */
  u_char          igmpr_code;		/* subtype for routing msgs */
  u_short         igmpr_cksum; 		/* IP-style checksum */
  u_short         igmpr_rsv;		/* reserved */
  u_short         igmpr_numgrps;        /* number of groups*/
  igmp_grouprec_t igmpr_group[0]; 	/* group records */
} igmp_report_t;

#define IGMP_GMI 260
#define PORT_GMI 260
#define PORT_INIT 1000
#define PORT_ONE 2
#define PORT_SEC 4
#define PORT_THI 8
#define PORT_FOU 16

#define IGMP_V1_MEMBERSHIP_REPORT   0x12
#define IGMP_V2_MEMBERSHIP_REPORT   0x16
#define IGMP_V2_MEMBERSHIP_LEAVE    0x17
#define IGMP_V3_MEMBERSHIP_REPORT   0x22

/*
 * table:
 *
 *	group_ip1---src_mac1--src_mac2--src_mac3
 *	group_ip2---src_mac1--src_mac2--src_mac3
 *	group_ip3---src_mac1--src_mac2--src_mac3
 * 
 * */
/*source mac*/
struct source_mac
{
	unsigned char src_mac[ETH_ALEN];/*source mac*/
	uint8_t port;			/*source port*/
	struct source_mac *next;
};

/*igmp snooping struct*/
struct igmp_group_snooping
{
	struct in_addr group_addr;	/*group ip*/
	struct source_mac *s_mac;	/*source mac*/
	int group_time;			/*group timer*/
	int port_time[4];
	unsigned char mac_cast[6];/*multicast mac*/
	uint8_t port;			/*group ---> port*/
	struct igmp_group_snooping *next;
};

#endif
