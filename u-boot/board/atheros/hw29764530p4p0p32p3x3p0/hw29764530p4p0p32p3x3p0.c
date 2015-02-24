#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>
#include <atheros_hw29764530p4p0p32p3x3p0.h>
#ifdef CFG_NMRP
extern int NmrpState;
extern ulong NmrpAliveTimerStart;
extern ulong NmrpAliveTimerBase;
extern int NmrpAliveTimerTimeout;
#endif
extern flash_info_t flash_info[];

extern int ath_ddr_initial_config(uint32_t refresh);
extern int ath_ddr_find_size(void);

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


/* LED GPIO functions */
#define ath_gpio_set(_pin)    ath_reg_rmw_set(GPIO_OUT_SET_ADDRESS, (1 << _pin))
#define ath_gpio_clear(_pin)  ath_reg_rmw_set(GPIO_OUT_CLEAR_ADDRESS, (1 << _pin))
#define ath_power_led_on()    ath_gpio_clear(POWER_LED_GPIO)
#define ath_power_led_off()   ath_gpio_set(POWER_LED_GPIO)
#define ath_status_led_on()   ath_gpio_clear(STATUS_LED_GPIO)
#define ath_status_led_off()  ath_gpio_set(STATUS_LED_GPIO)

void ath_gpio_init(void)
{
	unsigned int rddata;

	/* gpio 0~3 are default to JTAG pins, we must disable JTAG function. */
	ath_reg_rmw_set(GPIO_FUNCTION_ADDRESS, (1 << 1));

	rddata = ath_reg_rd(GPIO_OUT_FUNCTION0_ADDRESS);
	ath_reg_wr(GPIO_OUT_FUNCTION0_ADDRESS, (rddata & 0xff000000)); /* 03, 02, 01, 00 */
//	rddata = ath_reg_rd(GPIO_OUT_FUNCTION1_ADDRESS);
//	ath_reg_wr(GPIO_OUT_FUNCTION1_ADDRESS, (rddata & 0xffffffff)); /* 07, 06, 05, 04 */
//	rddata = ath_reg_rd(GPIO_OUT_FUNCTION2_ADDRESS);
//	ath_reg_wr(GPIO_OUT_FUNCTION2_ADDRESS, (rddata & 0xffffffff)); /* 11, 10, 09, 08 */
//	rddata = ath_reg_rd(GPIO_OUT_FUNCTION3_ADDRESS);
//	ath_reg_wr(GPIO_OUT_FUNCTION3_ADDRESS, 0x2b2a2900);            /* 15, 14, 13, 12 */
//	rddata = ath_reg_rd(GPIO_OUT_FUNCTION4_ADDRESS);
//	ath_reg_wr(GPIO_OUT_FUNCTION4_ADDRESS, 0x00002d2c);            /* 19, 18, 17, 16 */
//	rddata = ath_reg_rd(GPIO_OUT_FUNCTION5_ADDRESS);
//	ath_reg_wr(GPIO_OUT_FUNCTION5_ADDRESS, (rddata & 0xff000000)); /* 23, 22, 21, 20 */

	/* let gpio pin of all leds be output enabled. */
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, ALL_LED_GPIO_MASK);

	/* off all leds */
	ath_reg_rmw_set(GPIO_OUT_SET_ADDRESS, ALL_LED_GPIO_MASK);

	/* Setting one of pin 4 and pin 13 is necessary for manipulating LEDs.
	 * Don't know why. */
	ath_reg_rmw_set(GPIO_OUT_SET_ADDRESS, (1 << 4));

	/* let gpio pin of reset button be input enabled. */
	ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << RESET_BUTTON_GPIO));
}

void ath_gpio_config(void)
{
	ath_gpio_init();
	/* disable the CLK_OBS on GPIO_4 and set GPIO4 as input */
	/*ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << 4));*/
	/*ath_reg_rmw_clear(GPIO_OUT_FUNCTION1_ADDRESS, GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_MASK);*/
	/*ath_reg_rmw_set(GPIO_OUT_FUNCTION1_ADDRESS, GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_SET(0x80));*/
	/*ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 4));*/
	/* Set GPIO 13 as input for LED functionality to be OFF during bootup */
	/*ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 13));*/
	/* Turn off JUMPST_LED and 5Gz LED during bootup */
	/*ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 15));*/
	/*ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 12));*/
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
	board_str("U-boot dni29 V0.9 for DNI HW ID: 29764530 flash 4MB RAM 32MB 1st Radio 3x3\n");
	return 0;
}

/*ledstat 0:on; 1:off*/
void board_power_led(int ledstat)
{
	if (ledstat == 1){
		ath_power_led_off();
	} else {
		ath_status_led_off();
		ath_power_led_on();
	}
}

/*ledstat 0:on; 1:off*/
void board_test_led(int ledstat)
{
	if (ledstat == 1){
		ath_status_led_off();
	} else {
		ath_power_led_off();
		ath_status_led_on();
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

/*return value 0: not pressed, 1 pressed*/
static int ath_reset_button_is_press(void)
{
	if (ath_reg_rd(GPIO_IN_ADDRESS) & (1 << RESET_BUTTON_GPIO))
		return 0;

	return 1;
}

/*return value 0: not pressed, 1: pressed*/
int board_reset_button_is_press()
{
	return ath_reset_button_is_press();
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
	ath_status_led_on();
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

    /* set different CFG_STRING_TABLE_ADDR_BEGIN for JNR3300 */
    char board_model_id[BOARD_MODEL_ID_LENGTH + 1];
    unsigned int cfg_string_table_addr_begin = CFG_STRING_TABLE_ADDR_BEGIN;
    memset(board_model_id, 0, sizeof(board_model_id));
    get_board_data(BOARD_MODEL_ID_OFFSET, BOARD_MODEL_ID_LENGTH, (u8 *)board_model_id);
    if(strcmp(board_model_id,"JNR3300") == 0)
        cfg_string_table_addr_begin = CFG_FLASH_BASE + 0x3b0000;

    /* Read whole string table partition from Flash to RAM(load_addr+128k) */
    string_table_addr = load_addr + 0x20000;
    memset(string_table_addr, 0, CFG_STRING_TABLE_TOTAL_LEN);
    memcpy(string_table_addr, cfg_string_table_addr_begin, CFG_STRING_TABLE_TOTAL_LEN);

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
    printf("updating %x to %x\n", cfg_string_table_addr_begin, CFG_STRING_TABLE_ADDR_END);
    update_data(string_table_addr, CFG_STRING_TABLE_TOTAL_LEN, cfg_string_table_addr_begin, 1);
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
