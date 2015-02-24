#ifndef __BOARD_955X_H
#define __BOARD_955X_H

#include <config.h>

#undef MTDPARTS_DEFAULT

#undef CFG_HZ

#include <atheros_hw29764530p4p0p32p3x3p0.h>

#ifndef FLASH_SIZE
#define FLASH_SIZE 4
#endif
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#if (FLASH_SIZE == 16)
#define CFG_MAX_FLASH_SECT	256	/* max number of sectors on one chip */
#define ATH_MTDPARTS_MIB0	"8256k(mib0)"
#elif (FLASH_SIZE == 8)
#define CFG_MAX_FLASH_SECT	128	/* max number of sectors on one chip */
#define ATH_MTDPARTS_MIB0	"64k(mib0)"
#else
#define CFG_MAX_FLASH_SECT	64	/* max number of sectors on one chip */
#define ATH_MTDPARTS_MIB0	"64k(mib0)"
#endif

#define CFG_FLASH_SECTOR_SIZE	(64*1024)
#if (FLASH_SIZE == 16)
#define CFG_FLASH_SIZE		0x01000000	/* Total flash size */
#elif (FLASH_SIZE == 8)
#define CFG_FLASH_SIZE		0x00800000	/* max number of sectors on one chip */
#else
#define CFG_FLASH_SIZE		0x00400000	/* Total flash size */
#endif

#ifndef COMPRESSED_UBOOT
#define ENABLE_DYNAMIC_CONF	1
#endif

#undef CFG_ATHRS26_PHY

#if (CFG_MAX_FLASH_SECT * CFG_FLASH_SECTOR_SIZE) != CFG_FLASH_SIZE
#	error "Invalid flash configuration"
#endif

#define CFG_FLASH_WORD_SIZE	unsigned short

#if defined(CONFIG_ATH_NAND_BR) && defined(COMPRESSED_UBOOT)
#define CFG_FLASH_BASE			0xa0100000
#else
/* NOR Flash start address */
#define CFG_FLASH_BASE			0x9f000000
#endif

#ifdef COMPRESSED_UBOOT
#define BOOTSTRAP_TEXT_BASE		CFG_FLASH_BASE
#define BOOTSTRAP_CFG_MONITOR_BASE	BOOTSTRAP_TEXT_BASE
#endif

#define CONFIG_PCI_CONFIG_DATA_IN_OTP

/*
 * Defines to change flash size on reboot
 */
#ifdef ENABLE_DYNAMIC_CONF
#define UBOOT_FLASH_SIZE		(192 * 1024)
#define UBOOT_ENV_SEC_START		(CFG_FLASH_BASE + UBOOT_FLASH_SIZE)

#define CFG_FLASH_MAGIC			0xaabacada
#define CFG_FLASH_MAGIC_F		(UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x20)
#define CFG_FLASH_SECTOR_SIZE_F		*(volatile int *)(CFG_FLASH_MAGIC_F + 0x4)
#define CFG_FLASH_SIZE_F		*(volatile int *)(CFG_FLASH_MAGIC_F + 0x8) /* Total flash size */
#define CFG_MAX_FLASH_SECT_F		(CFG_FLASH_SIZE / CFG_FLASH_SECTOR_SIZE) /* max number of sectors on one chip */
#else
#define CFG_FLASH_SIZE_F		CFG_FLASH_SIZE
#define CFG_FLASH_SECTOR_SIZE_F		CFG_FLASH_SECTOR_SIZE
#endif


#define CFG_DDR_REFRESH_VAL		0x4138

/*
 * The following #defines are needed to get flash environment right
 */
#define	CFG_MONITOR_BASE	TEXT_BASE
#define	CFG_MONITOR_LEN		(192 << 10)

#undef CONFIG_BOOTARGS

#define __gen_cmd(n, a, f, ec, cc, el)		\
	#n "=tftp 0x80060000 ${dir}" #f "&&"	\
	#ec " " #a " " #el "&&"			\
	#cc " $fileaddr " #a " $filesize\0"

#define gen_cmd(n, a, f)			\
	__gen_cmd(n, a, f, erase, cp.b, +$filesize)

#define gen_cmd_el(n, a, f, el)			\
	__gen_cmd(n, a, f, erase, cp.b, +el)

#define nand_gen_cmd(n, a, f, s)		\
	__gen_cmd(n, a, f, nand erase, nand write, s)

#define __fs_name(x, y)		x ## y
#define _fs_name(x, y)		__fs_name(x, y)
#define fs_name(y)		_fs_name(__CONFIG_BOARD_NAME, y)

#ifdef COMPRESSED_UBOOT
#	define ATH_U_FILE	tuboot.bin
#else
#	define ATH_U_FILE	u-boot.bin
#endif


#ifdef CONFIG_ATH_NAND_SUPPORT
#	ifdef CONFIG_ATH_NAND_BR	// nand boot rom
#		if defined(COMPRESSED_UBOOT)
#			define ATH_U_CMD	nand_gen_cmd(lu, 0x0, 2fw.bin, 0x20000)
#			define MTDPARTS_DEFAULT	"mtdparts=ath-nand:128k(u-boot),384k(free),1280k(uImage),7m(rootfs),128k(dummy),128k(caldata)"
#		else
#		define ATH_U_CMD	nand_gen_cmd(lu, 0x0, 2fw.bin, 0x40000)
#		define MTDPARTS_DEFAULT "mtdparts=ath-nand:256k(u-boot),256k(u-boot-env),1280k(uImage),7m(rootfs),128k(dummy),128k(caldata)"
#		endif
#		define ATH_ROOT_DEV	"31:03"
#		define CFG_ENV_ADDR	0x00040000
#	else //dual flash
#		define ATH_U_CMD	gen_cmd(lu, 0x9f000000, ATH_U_FILE)
#		define MTDPARTS_DEFAULT "mtdparts=ath-nor0:320k(u-boot-and-env),6336k(free);ath-nand:256k(u-boot),256k(u-boot-env),1280k(uImage),7m(rootfs),128k(dummy),128k(caldata)"
#		define ATH_ROOT_DEV	"31:05"
#		define CFG_ENV_ADDR	0x9f040000
#	endif
#	define ATH_F_FILE		fs_name(${bc}-nand-jffs2)
#	define ATH_F_LEN		0x700000
#	define ATH_F_ADDR		0x1c0000
#	define ATH_K_FILE		vmlinux${bc}.lzma.uImage
#	define ATH_K_ADDR		0x80000
#	define ATH_F_CMD		nand_gen_cmd(lf, ATH_F_ADDR, ATH_F_FILE, ATH_F_LEN)
#	define ATH_K_CMD		nand_gen_cmd(lk, ATH_K_ADDR, ATH_K_FILE, 0x140000)
#	define ATH_EXTRA_ENV		"bootdevice=0\0"
#else
#	define ATH_F_FILE		fs_name(${bc}-jffs2)
#	define ATH_F_LEN		$filesize
#	define ATH_F_ADDR		0x9f040000
#	define ATH_K_FILE		vmlinux${bc}.lzma.uImage
#	define ATH_K_ADDR		0x9f2f0000
#	define MTDPARTS_DEFAULT		"mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),6336k(rootfs),1408k(uImage)," ATH_MTDPARTS_MIB0 ",64k(ART)"
#endif /*CONFIG_MI124*/
#undef MTDPARTS_DEFAULT
#define MTDPARTS_DEFAULT		"mtdparts=ath-nor0:192k(u-boot),64k(u-boot-env),2752k(rootfs),1024k(uImage),64k(ART)"

#ifndef ATH_ROOT_DEV
#	define ATH_ROOT_DEV	"31:02"
#endif

#ifndef ATH_EXTRA_ENV
#	define ATH_EXTRA_ENV
#endif

#ifndef ATH_U_CMD
#	define ATH_U_CMD	gen_cmd(lu, 0x9f000000, ATH_U_FILE)
#endif

#ifndef ATH_F_CMD
#	define ATH_F_CMD	gen_cmd_el(lf, ATH_F_ADDR, ATH_F_FILE, ATH_F_LEN)
#endif

#ifndef ATH_K_CMD
#	define ATH_K_CMD	gen_cmd(lk, ATH_K_ADDR, ATH_K_FILE)
#endif

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"dir=\0" ATH_U_CMD ATH_F_CMD ATH_K_CMD ""

#define	CONFIG_BOOTARGS		"console=ttyS0,115200 root=" ATH_ROOT_DEV " rootfstype=jffs2 init=/sbin/init " MTDPARTS_DEFAULT

//#define CFG_PLL_FREQ    CFG_PLL_720_600_200

/*
 * timeout values are in ticks
 */
#define CFG_FLASH_ERASE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

/*
 * Cache lock for stack
 */
#define CFG_INIT_SP_OFFSET	0x1000

#ifdef CONFIG_ATH_NAND_SUPPORT
#	define CONFIG_BOOTCOMMAND	"nboot 0x81000000 0 0x80000"
#else
#	define CFG_ENV_ADDR		0x9f030000
#	define CONFIG_BOOTCOMMAND	"mw 0xb8116cc4 0x10380000; mw 0x1900005c 0x0007efcf; nmrp; nor_two_part_fw_integrity_check 0x9f040000; bootm 0x9f040000"
#endif



#ifdef ENABLE_DYNAMIC_CONF
#define CFG_DDR_MAGIC		0xaabacada
#define CFG_DDR_MAGIC_F		(UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x30)
#define CFG_DDR_CONFIG_VAL_F	*(volatile int *)(CFG_DDR_MAGIC_F + 4)
#define CFG_DDR_CONFIG2_VAL_F	*(volatile int *)(CFG_DDR_MAGIC_F + 8)
#define CFG_DDR_EXT_MODE_VAL_F	*(volatile int *)(CFG_DDR_MAGIC_F + 12)
#endif

#define CONFIG_NET_MULTI
#define CONFIG_MEMSIZE_IN_BYTES
#undef CONFIG_PCI

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#ifndef COMPRESSED_UBOOT
#define ATH_CFG_COMMANDS	((			\
				CONFIG_CMD_DFL	|	\
				CFG_CMD_DHCP	|	\
				CFG_CMD_ELF	|	\
				CFG_CMD_PCI	|	\
				CFG_CMD_FLS	|	\
				CFG_CMD_MII	|	\
				CFG_CMD_PING	|	\
				CFG_CMD_NET	|	\
				CFG_CMD_ENV	|	\
				CFG_CMD_PLL	|	\
				CFG_CMD_FLASH	|	\
				CFG_CMD_RUN	|	\
				CFG_CMD_ELF	|	\
				CFG_CMD_DDR	|	\
				CFG_CMD_ETHREG		\
				) & ~(			\
				CFG_CMD_IMLS	|	\
				CFG_CMD_FLASH		\
				))
#else
#	ifdef CONFIG_ATH_NAND_BR
#		define ATH_CFG_COMMANDS		((			\
						CONFIG_CMD_DFL	|	\
						CFG_CMD_PING	|	\
						CFG_CMD_NET) & ~(	\
						CFG_CMD_FLASH		\
						))
#	else
#		define ATH_CFG_COMMANDS		(CONFIG_CMD_DFL	|	\
				CFG_CMD_PING	|	\
				CFG_CMD_NET)
#	endif
#endif /* #ifndef COMPRESSED_UBOOT */

#ifdef CONFIG_ATH_NAND_SUPPORT
#	ifdef CONFIG_ATH_NAND_BR
#		define CFG_ENV_IS_IN_NAND	1
#		define CFG_ENV_OFFSET		0x40000u
#		define CFG_ENV_SIZE		0x40000u
#		define ATH_EXTRA_CMD		CFG_CMD_NAND
#	else
#		define CFG_ENV_IS_IN_FLASH	1
#		define CFG_ENV_SIZE		CFG_FLASH_SECTOR_SIZE
#		define ATH_EXTRA_CMD		(CFG_CMD_NAND | CFG_CMD_FLASH)
#	endif
#	define NAND_MAX_CHIPS			1
#	define CFG_MAX_NAND_DEVICE		1
#else
#	define ATH_EXTRA_CMD			CFG_CMD_FLASH
#	define CFG_ENV_IS_IN_FLASH		1
#	define CFG_ENV_SIZE			CFG_FLASH_SECTOR_SIZE
#endif

#define DEBUG

#ifdef COMPRESSED_UBOOT
#undef  CFG_ENV_IS_IN_FLASH
#undef  CFG_ENV_IS_IN_NAND
#define CFG_ENV_IS_NOWHERE		1
#endif

#define CONFIG_COMMANDS			(ATH_CFG_COMMANDS | ATH_EXTRA_CMD)

#define CONFIG_IPADDR			192.168.1.1
#define CONFIG_SERVERIP			192.168.1.10
#define CONFIG_ETHADDR			0x00:0xaa:0xbb:0xcc:0xdd:0xee
#define CFG_FAULT_ECHO_LINK_DOWN	1

#define CFG_PHY_ADDR			0
#define CFG_GMII			0
#define CFG_MII0_RMII			1
#define CFG_AG7100_GE0_RMII		1

#define CFG_BOOTM_LEN			(16 << 20) /* 16 MB */
#define CFG_HUSH_PARSER
#define CFG_PROMPT_HUSH_PS2		"hush>"

/*
** Parameters defining the location of the calibration/initialization
** information for the two Merlin devices.
** NOTE: **This will change with different flash configurations**
*/

#define WLANCAL			0x9f3f1000
#define BOARDCAL			0x9f3f0000
#define ATHEROS_PRODUCT_ID		137
#define CAL_SECTOR			(CFG_MAX_FLASH_SECT - 1)

/* For Merlin, both PCI, PCI-E interfaces are valid */
#define ATH_ART_PCICFG_OFFSET		12

#include <cmd_confdefs_hw29764530p4p0p32p3x3p0.h>

#define CONFIG_JFFS2_PART_OFFSET	(CFG_IMAGE_BASE_ADDR - CFG_FLASH_BASE + 0x40)
#define FIRMWARE_RECOVER_FROM_TFTP_SERVER 1
#define CFG_NMRP 1
#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
/*change image length to 0x370000, it is consistent with bootargs rootfs mtd partion */
#define CFG_IMAGE_LEN   0x360000
#define CFG_IMAGE_BASE_ADDR (CFG_FLASH_BASE + 0x40000)
#define CFG_IMAGE_ADDR_BEGIN (CFG_IMAGE_BASE_ADDR)
#define CFG_IMAGE_ADDR_END   (CFG_IMAGE_BASE_ADDR + CFG_IMAGE_LEN)
#define CFG_FLASH_CONFIG_BASE               (CFG_FLASH_BASE + 0x3e0000)
#define CFG_FLASH_CONFIG_PARTITION_SIZE     0x10000
#define CFG_STRING_TABLE_LEN 0x19000 /* Each string table takes 100K to save */
#define CFG_STRING_TABLE_NUMBER 1
#define CFG_STRING_TABLE_TOTAL_LEN 0x20000 /* Totally allocate 1024K to save all string tables */
#define CFG_STRING_TABLE_BASE_ADDR (CFG_FLASH_BASE + 0x3a0000)
#define CFG_STRING_TABLE_ADDR_BEGIN (CFG_STRING_TABLE_BASE_ADDR)
#define CFG_STRING_TABLE_ADDR_END   (CFG_STRING_TABLE_ADDR_BEGIN + CFG_STRING_TABLE_TOTAL_LEN)
#endif
#define CFG_NMRP                1
#define CFG_SINGLE_FIRMWARE     1
#define TFTP_RECOVER_MODE_PINGABLE 1
#define DO_BOARD_PARAMETERS_SET 1

#define CONFIG_MISC_INIT_R 1

#define LAN_MAC_OFFSET			0x00
#define WAN_MAC_OFFSET			0x06
#define WLAN_MAC_OFFSET			0x0c

/* For Merlin, both PCI, PCI-E interfaces are valid */
#define AR7240_ART_PCICFG_OFFSET        12

/* 6(WAN mac) + 6(LAN mac) + (WLAN mac) = 18(0x12) */
#define WPSPIN_OFFSET           0x12
#define WPSPIN_LENGTH           8

/* 18(WPSPIN_OFFSET) + 8(WPSPIN_LENGTH) = 26(0x1a) */
#define SERIAL_NUMBER_OFFSET		0x1a
#define SERIAL_NUMBER_LENGTH		13

/* 26(SERIAL_NUMBER_OFFSET) + 13(SERIAL_NUMBER_LENGTH) = 39(0x27) */
#define REGION_NUMBER_OFFSET        0x27
#define REGION_NUMBER_LENGTH        2

/*
 * It's HW_ID should be "29764530+4+0+32+3x3+0".
 * "(8 MSB of PCB number)+(Flash size)+(SDRam size)", length should be 21
 */
#define BOARD_HW_ID_OFFSET          (REGION_NUMBER_OFFSET + REGION_NUMBER_LENGTH)
#define BOARD_HW_ID_LENGTH          24

#define BOARD_MODEL_ID_OFFSET       (BOARD_HW_ID_OFFSET + BOARD_HW_ID_LENGTH)
#define BOARD_MODEL_ID_LENGTH       12

#define BOARD_SSID_OFFSET       (BOARD_MODEL_ID_OFFSET + BOARD_MODEL_ID_LENGTH)
#define BOARD_SSID_LENGTH         32
#define BOARD_PASSPHRASE_OFFSET (BOARD_SSID_OFFSET + BOARD_SSID_LENGTH)
#define BOARD_PASSPHRASE_LENGTH   64

#define NETGEAR_BOARD_ID_SUPPORT    1

/* Skip Atheros Ethernet Address Checking*/
#define SKIP_ATHEROS_ETHADDR_CHECKING 1

#define WORKAROUND_QCA955X_GMAC_NMRP_HANG 1


/* GPIO */
#define	WLAN_LED_GPIO		0	/* WLAN led, green (Output Pin/Active LOW) */
#define	STATUS_LED_GPIO		2	/* ready status led, amber (Output Pin/Active LOW) */
#define	S_WIFI_LED_GPIO		3	/* Super Wifi mode led, green (Output Pin/Active LOW) */
#define	LAN1_10M_LED_GPIO	11	/* LAN1 10M led, amber (Output Pin/Active LOW) */
#define	LAN2_10M_LED_GPIO	12	/* LAN2 10M led, amber (Output Pin/Active LOW) */
#define	LAN3_10M_LED_GPIO	18	/* LAN3 10M led, amber (Output Pin/Active LOW) */
#define	LAN4_10M_LED_GPIO	20	/* LAN4 10M led, amber (Output Pin/Active LOW) */
#define	USB_ENABLE_GPIO		14	/* Enable USB device (Output Pin/Active HIGH) */
#define	WPS_LED_GPIO		15	/* WPS led, green (Output Pin/Active LOW) */
#define	WPS_BUTTON_GPIO		16	/* WPS button (Input Pin/Active LOW) */
#define	RESET_BUTTON_GPIO	17	/* reset button (Input Pin/Active LOW) */
#define	POWER_LED_GPIO		19	/* Power led, green (Output Pin/Active LOW) */
#define	S_WIFI_BUTTON_GPIO	21	/* Super Wifi mode button (Input Pin/Active LOW) */
#define	WAN_GLED_GPIO		22	/* WAN led, green (Output Pin/Active LOW) */
#define	WAN_ALED_GPIO		23	/* WAN led, amber (Output Pin/Active LOW) */

/*#define ALL_LED_GPIO_MASK	0x00dc981d*/
#define	ALL_LED_GPIO_MASK	(1 << WLAN_LED_GPIO) | (1 << STATUS_LED_GPIO) | \
				(1 << S_WIFI_LED_GPIO) | (1 << LAN1_10M_LED_GPIO) | \
				(1 << LAN2_10M_LED_GPIO) | (1 << LAN3_10M_LED_GPIO) | \
				(1 << LAN4_10M_LED_GPIO) | (1 << WPS_LED_GPIO) | \
				(1 << POWER_LED_GPIO) | (1 << WAN_GLED_GPIO) | \
				(1 << WAN_ALED_GPIO)
#endif	/* __BOARD_955X_H */
