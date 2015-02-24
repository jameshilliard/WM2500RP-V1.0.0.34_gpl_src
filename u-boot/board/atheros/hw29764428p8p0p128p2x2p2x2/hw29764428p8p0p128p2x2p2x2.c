#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>
#include <atheros_hw29764428p8p0p128p2x2p2x2.h>
#ifdef CFG_NMRP
extern int NmrpState;
extern ulong NmrpAliveTimerStart;
extern ulong NmrpAliveTimerBase;
extern int NmrpAliveTimerTimeout;
#endif

extern int ath_ddr_initial_config(uint32_t refresh);
extern int ath_ddr_find_size(void);
extern flash_info_t flash_info[];

#ifdef COMPRESSED_UBOOT
#	define prmsg(...)
#	define args		char *s
#	define board_str(a)	do {			\
	char ver[] = "0";				\
	strcpy(s, a " - Scorpion 1.");			\
	ver[0] += ath_reg_rd(RST_REVISION_ID_ADDRESS)	\
						& 0xf;	\
	strcat(s, ver);					\
} while (0)
#else
#	define prmsg	printf
#	define args		void
#	define board_str(a)				\
	printf(a " - Scorpion 1.%d", ath_reg_rd		\
			(RST_REVISION_ID_ADDRESS) & 0xf)
#endif

void
ath_usb1_initial_config(void)
{
#define unset(a)	(~(a))

	ath_reg_wr_nf(SWITCH_CLOCK_SPARE_ADDRESS,
		ath_reg_rd(SWITCH_CLOCK_SPARE_ADDRESS) |
		SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_SET(5));
	udelay(1000);

	ath_reg_rmw_set(RST_RESET_ADDRESS,
				RST_RESET_USB_PHY_SUSPEND_OVERRIDE_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_PHY_RESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_PHY_ARESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_HOST_RESET_SET(1));
	udelay(1000);

	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_PHY_PLL_PWD_EXT_SET(1));
	udelay(10);

	ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_USB1_EXT_PWR_SEQ_SET(1));
	udelay(10);
}

void
ath_usb2_initial_config(void)
{
	if (is_drqfn()) {
		return;
	}

	ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_USB2_MODE_SET(1));
	udelay(10);
	ath_reg_rmw_set(RST_RESET2_ADDRESS,
				RST_RESET2_USB_PHY2_SUSPEND_OVERRIDE_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_PHY2_RESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_PHY2_ARESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_HOST2_RESET_SET(1));
	udelay(1000);

	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_PHY2_PLL_PWD_EXT_SET(1));
	udelay(10);

	ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_USB2_EXT_PWR_SEQ_SET(1));
	udelay(10);
}

#define ath_gpio_set(_mask)    ath_reg_wr(GPIO_SET_ADDRESS, _mask)
#define ath_gpio_clear(_mask)  ath_reg_wr(GPIO_CLEAR_ADDRESS, _mask)
#define ath_led_on(_led_mask)  ath_gpio_clear(_led_mask)
#define ath_led_off(_led_mask) ath_gpio_set(_led_mask)

void ath_gpio_config(void)
{
	/* disable JTAG so that WPS LED can be used on GPIO_1 */
	ath_reg_rmw_set(GPIO_FUNCTION_ADDRESS, GPIO_FUNCTION_DISABLE_JTAG_MASK);

	ath_reg_rmw_clear(GPIO_OUT_FUNCTION0_ADDRESS, 0x0000ff00); /* 03, 02, 01, 00 */
//	ath_reg_rmw_clear(GPIO_OUT_FUNCTION1_ADDRESS, 0x00000000); /* 07, 06, 05, 04 */
	ath_reg_rmw_clear(GPIO_OUT_FUNCTION2_ADDRESS, 0xff000000); /* 11, 10, 09, 08 */
	ath_reg_rmw_clear(GPIO_OUT_FUNCTION3_ADDRESS, 0xffffff00); /* 15, 14, 13, 12 */
	ath_reg_rmw_clear(GPIO_OUT_FUNCTION4_ADDRESS, 0xffff00ff); /* 19, 18, 17, 16 */
	ath_reg_rmw_clear(GPIO_OUT_FUNCTION5_ADDRESS, 0x000000ff); /* 23, 22, 21, 20 */

	/* Let gpio pins of all leds be output enabled. */
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, ALL_LED_GPIO_MASK);
	/* Let gpio pins of all buttons be input enabled. */
	ath_reg_rmw_set(GPIO_OE_ADDRESS, ALL_BUTTON_GPIO_MASK);

	ath_led_off(ALL_LED_GPIO_MASK);
}

int
ath_mem_config(void)
{
	unsigned int type, reg32, *tap;
	extern uint32_t *ath_ddr_tap_cal(void);

#if !defined(CONFIG_ATH_EMULATION)

#if !defined(CONFIG_ATH_NAND_BR)
	type = ath_ddr_initial_config(CFG_DDR_REFRESH_VAL);

	tap = ath_ddr_tap_cal();
	prmsg("tap = 0x%p\n", tap);

	tap = (uint32_t *)0xbd007f10;
	prmsg("Tap (low, high) = (0x%x, 0x%x)\n", tap[0], tap[1]);

	tap = (uint32_t *)TAP_CONTROL_0_ADDRESS;
	prmsg("Tap values = (0x%x, 0x%x, 0x%x, 0x%x)\n",
		tap[0], tap[2], tap[2], tap[3]);

	/* Take WMAC out of reset */
	reg32 = ath_reg_rd(RST_RESET_ADDRESS);
	reg32 = reg32 & ~RST_RESET_RTC_RESET_SET(1);
	ath_reg_wr_nf(RST_RESET_ADDRESS, reg32);
#endif

	ath_usb1_initial_config();
	ath_usb2_initial_config();

	ath_gpio_config();
#endif /* !defined(CONFIG_ATH_EMULATION) */

	return ath_ddr_find_size();
}

long int initdram(int board_type)
{
	return (ath_mem_config());
}

int	checkboard(args)
{
	board_str("DNI HW ID: hw29764428p8p0p128p2x2p2x2 NOR flash 8MB RAM 128MB U-boot dni29 V0.6\n");
	return 0;
}

/*ledstat 0:on; 1:off*/
void board_power_led(int ledstat)
{
	if (ledstat == 1){
		ath_led_off(POWER_LED_GPIO_MASK);
	} else {
		ath_led_off(STATUS_LED_GPIO_MASK);
		ath_led_on(POWER_LED_GPIO_MASK);
	}
}

/*ledstat 0:on; 1:off*/
void board_test_led(int ledstat)
{
	if (ledstat == 1){
		ath_led_off(STATUS_LED_GPIO_MASK);
	} else {
		ath_led_off(POWER_LED_GPIO_MASK);
		ath_led_on(STATUS_LED_GPIO_MASK);
	}
}

void board_reset_default_LedSet(void)
{
	static int DiagnosLedCount = 1;
	if ((DiagnosLedCount++ % 2) == 1) {
		/*power on test led 0.25s */
		board_test_led(0);
		NetSetTimeout((CFG_HZ * 1) / 4, board_reset_default_LedSet);
	} else {
		/*power off test led 0.75s */
		board_test_led(1);
		NetSetTimeout((CFG_HZ * 3) / 4, board_reset_default_LedSet);
	}
}

/*return value 0: not pressed, 1: pressed*/
int board_reset_button_is_press()
{
    return 0;
}

extern int flash_sect_erase (ulong, ulong);

/*erase the config sector of flash*/
void board_reset_default()
{
	printf("Restore to factory default\n");
	flash_sect_erase(CFG_FLASH_CONFIG_BASE,
	      CFG_FLASH_CONFIG_BASE + CFG_FLASH_CONFIG_PARTITION_SIZE - 1);
#ifdef CFG_NMRP
	if (NmrpState != 0)
		return;
#endif
	printf("Rebooting...\n");
	do_reset(NULL, 0, 0, NULL);
}

int misc_init_r(void)
{
	ath_led_on(STATUS_LED_GPIO_MASK);
	return 0;
}

#if defined(CFG_SINGLE_FIRMWARE)
void board_upgrade_string_table(unsigned char *load_addr, int table_number, unsigned int file_size)
{
    unsigned char *string_table_addr, *addr2;
    unsigned long offset;
    unsigned int table_length;
    unsigned char high_bit, low_bit;
    unsigned long passed;
    int offset_num;
    uchar *src_addr;
    ulong target_addr;

    /* Read whole string table partition from Flash to RAM(load_addr+128k) */
    string_table_addr = load_addr + 0x20000;
    memset(string_table_addr, 0, CFG_STRING_TABLE_TOTAL_LEN);
    memcpy(string_table_addr, CFG_STRING_TABLE_ADDR_BEGIN, CFG_STRING_TABLE_TOTAL_LEN);

    /* Save string table checksum to (100K - 1) */
    memcpy(load_addr + CFG_STRING_TABLE_LEN - 1, load_addr + file_size- 1, 1);
    /* Remove checksum from string table file's tail */
    memset(load_addr + file_size - 1, 0, 1);

    table_length = file_size - 1;
    printf("string table length is %d\n", table_length);

    /* Save (string table length / 1024)to 100K-3 */
    high_bit = table_length / 1024;
    addr2 = load_addr + CFG_STRING_TABLE_LEN - 3;
    memcpy(addr2, &high_bit, sizeof(high_bit));

    /* Save (string table length % 1024)to 100K-2 */
    low_bit = table_length % 1024;
    addr2 = load_addr + CFG_STRING_TABLE_LEN - 2;
    memcpy(addr2, &low_bit, sizeof(low_bit));

    /* Copy processed string table from load_addr to RAM */
    offset = (table_number - 1) * CFG_STRING_TABLE_LEN;
    memcpy(string_table_addr + offset, load_addr, CFG_STRING_TABLE_LEN);

    /* Write back string tables from RAM to Flash */
    printf("updating %x to %x\n", CFG_STRING_TABLE_ADDR_BEGIN, CFG_STRING_TABLE_ADDR_END);
    update_data(string_table_addr, CFG_STRING_TABLE_TOTAL_LEN, CFG_STRING_TABLE_ADDR_BEGIN, 1);
    return;
}
#endif

#if defined(NETGEAR_BOARD_ID_SUPPORT)
/*
 * item_name_want could be "device" to get Model Id, "version" to get Version
 * or "hd_id" to get Hardware ID.
 */
void board_get_image_info(ulong fw_image_addr, char *item_name_want, char *buf)
{
    char image_header[HEADER_LEN];
    char item_name[HEADER_LEN];
    char *item_value;
    char *parsing_string;

    memset(image_header, 0, HEADER_LEN);
    memcpy(image_header, fw_image_addr, HEADER_LEN);

    parsing_string = strtok(image_header, "\n");
    while (parsing_string != NULL) {
       memset(item_name, 0, sizeof(item_name));
       strncpy(item_name, parsing_string, (int)(strchr(parsing_string, ':') - parsing_string));

       if (strcmp(item_name, item_name_want) == 0) {
           item_value = strstr(parsing_string, ":") + 1;

           memcpy(buf, item_value, strlen(item_value));
       }

       parsing_string = strtok(NULL, "\n");
    }
}

int board_match_image_hw_id (ulong fw_image_addr)
{
    char board_hw_id[BOARD_HW_ID_LENGTH + 1];
    char image_hw_id[BOARD_HW_ID_LENGTH + 1];
    char image_model_id[BOARD_MODEL_ID_LENGTH + 1];
    char image_version[20];

    /*get hardward id from board */
    memset(board_hw_id, 0, sizeof(board_hw_id));
    memcpy(board_hw_id, (void *)(BOARDCAL + BOARD_HW_ID_OFFSET),
           BOARD_HW_ID_LENGTH);
    printf("HW ID on board: %s\n", board_hw_id);

    /*get hardward id from image */
    memset(image_hw_id, 0, sizeof(image_hw_id));
    board_get_image_info(fw_image_addr, "hd_id", (char*)image_hw_id);
    printf("HW ID on image: %s\n", image_hw_id);

    if (memcmp(board_hw_id, image_hw_id, BOARD_HW_ID_LENGTH) != 0) {
            printf("Firmware Image HW ID do not match Board HW ID\n");
            return 0;
    }
    printf("Firmware Image HW ID matched Board HW ID\n\n");
    return 1;
}

int board_match_image_model_id (ulong fw_image_addr)
{
    char board_model_id[BOARD_MODEL_ID_LENGTH + 1];
    char image_model_id[BOARD_MODEL_ID_LENGTH + 1];

    /*get hardward id from board */
    memset(board_model_id, 0, sizeof(board_model_id));
    memcpy(board_model_id, (void *)(BOARDCAL + BOARD_MODEL_ID_OFFSET),
           BOARD_MODEL_ID_LENGTH);
    printf("MODEL ID on board: %s\n", board_model_id);

    /*get hardward id from image */
    memset(image_model_id, 0, sizeof(image_model_id));
    board_get_image_info(fw_image_addr, "device", (char*)image_model_id);
    printf("MODEL ID on image: %s\n", image_model_id);

    if (memcmp(board_model_id, image_model_id, BOARD_MODEL_ID_LENGTH) != 0) {
            printf("Firmware Image MODEL ID do not match Board model ID\n");
            return 0;
    }
    printf("Firmware Image MODEL ID matched Board model ID\n\n");
    return 1;
}

void board_update_image_model_id (ulong fw_image_addr)
{
    char board_model_id[BOARD_MODEL_ID_LENGTH + 1];
    char image_model_id[BOARD_MODEL_ID_LENGTH + 1];
    char sectorBuff[CFG_FLASH_SECTOR_SIZE];

    /* Read boarddata */
    memcpy(sectorBuff, (void *)BOARDCAL, CFG_FLASH_SECTOR_SIZE);

    /*get model id from board */
    memset(board_model_id, 0, sizeof(board_model_id));
    memcpy(board_model_id, (void *)(BOARDCAL + BOARD_MODEL_ID_OFFSET),
           BOARD_MODEL_ID_LENGTH);
    printf("Original board MODEL ID: %s\n", board_model_id);

    /*get model id from image */
    memset(image_model_id, 0, sizeof(image_model_id));
    board_get_image_info(fw_image_addr, "device", (char*)image_model_id);
    printf("New MODEL ID from image: %s\n", image_model_id);

    printf("Updating MODEL ID\n");
    memcpy(sectorBuff + BOARD_MODEL_ID_OFFSET, image_model_id,
	    BOARD_MODEL_ID_LENGTH);
    flash_erase(flash_info, CAL_SECTOR, CAL_SECTOR);
    write_buff(flash_info, sectorBuff, BOARDCAL, CFG_FLASH_SECTOR_SIZE);

    printf("done\n\n");
}
#endif	/* BOARD_ID */

int do_ledctl(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ath_reg_t mask;

	if (argc != 3)
		goto error;

	if(0 == strcmp(argv[1],"all"))
		mask = ALL_LED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"green"))
		mask = ALL_GREEN_LED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"power"))
		mask = POWER_LED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"status"))
		mask = STATUS_LED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"wps"))
		mask = WPS_LED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"wlan2g"))
		mask = WLAN_LED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"wlan5g"))
		mask = WLAN_5G_LED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"moca_g"))
		mask = MOCA_GLED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"moca_a"))
		mask = MOCA_ALED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"eth_g"))
		mask = PHY_GLED_GPIO_MASK;
	else if(0 == strcmp(argv[1],"eth_a"))
		mask = PHY_OLED_GPIO_MASK;
	else
		goto error;

	if(0 == strcmp(argv[2],"on"))
		ath_led_on(mask);
	else if(0 == strcmp(argv[2],"off"))
		ath_led_off(mask);
	else
		goto error;

	return 0;

error:
	printf("%s\n",cmdtp->usage);
	return 1;
}

U_BOOT_CMD(
	ledctl, 3, 0, do_ledctl,
	"ledctl \t- set LED on/off.\n",
	"<LED_PIN> <ON_OFF>    - set LED on/off.\n"
	"   <LED_PIN> : all, green, power, status, wps\n"
	"               wlan2g, wlan5g, moca_g, moca_a, eth_g, eth_a\n"
	"   <ON_OFF> : on, off\n"
	"e.g. :\n"
	"   ledctl all on\n"
	"   ledctl green off\n"
);

int do_buttonstatusshow(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int reg32;

	reg32 = ath_reg_rd(GPIO_IN_ADDRESS);

	printf("reset button : ");
	if (reg32 & RESET_BUTTON_GPIO_MASK)
		printf("off");
	else
		printf("on");

	printf("\nwifi button : ");
	if (reg32 & WIFI_BUTTON_GPIO_MASK)
		printf("off");
	else
		printf("on");

	printf("\nwps button : ");
	if (reg32 & WPS_BUTTON_GPIO_MASK)
		printf("off");
	else
		printf("on");

	printf("\n");
	return 0;
}

U_BOOT_CMD(
	buttonstatusshow, 1, 0, do_buttonstatusshow,
	"buttonstatusshow \t- show button (RESET, WPS, WIFI) status.\n",
	"    - show button (RESET, WPS, WIFI) status.\n"
);
