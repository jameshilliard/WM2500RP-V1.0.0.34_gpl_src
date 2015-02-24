#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>
#include "ar7240_soc_hw29763906p8p0p64p2x2p0.h"
#ifdef CFG_NMRP
extern int NmrpState;
extern ulong NmrpAliveTimerStart;
extern ulong NmrpAliveTimerBase;
extern int NmrpAliveTimerTimeout;
#endif

extern int wasp_ddr_initial_config(uint32_t refresh);
extern int ar7240_ddr_find_size(void);

extern void serial_led_output(int val);
extern flash_info_t flash_info[];

void
wasp_usb_initial_config(void)
{
#define unset(a)	(~(a))

	if ((ar7240_reg_rd(WASP_BOOTSTRAP_REG) & WASP_REF_CLK_25) == 0) {
		ar7240_reg_wr_nf(AR934X_SWITCH_CLOCK_SPARE,
			ar7240_reg_rd(AR934X_SWITCH_CLOCK_SPARE) |
			SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_SET(2));
	} else {
		ar7240_reg_wr_nf(AR934X_SWITCH_CLOCK_SPARE,
			ar7240_reg_rd(AR934X_SWITCH_CLOCK_SPARE) |
			SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_SET(5));
	}

	udelay(1000);
	ar7240_reg_wr(AR7240_RESET,
		ar7240_reg_rd(AR7240_RESET) |
		RST_RESET_USB_PHY_SUSPEND_OVERRIDE_SET(1));
	udelay(1000);
	ar7240_reg_wr(AR7240_RESET,
		ar7240_reg_rd(AR7240_RESET) &
		unset(RST_RESET_USB_PHY_RESET_SET(1)));
	udelay(1000);
	ar7240_reg_wr(AR7240_RESET,
		ar7240_reg_rd(AR7240_RESET) &
		unset(RST_RESET_USB_PHY_ARESET_SET(1)));
	udelay(1000);
	ar7240_reg_wr(AR7240_RESET,
		ar7240_reg_rd(AR7240_RESET) &
		unset(RST_RESET_USB_HOST_RESET_SET(1)));
	udelay(1000);
	if ((ar7240_reg_rd(AR7240_REV_ID) & 0xf) == 0) {
		/* Only for WASP 1.0 */
		ar7240_reg_wr(0xb8116c84 ,
			ar7240_reg_rd(0xb8116c84) & unset(1<<20));
	}
}

/* GPIO */
#define ATH_GPIO_OE              AR7240_GPIO_BASE
#define ATH_GPIO_IN              AR7240_GPIO_BASE+0x04
#define ATH_GPIO_OUT             AR7240_GPIO_BASE+0x08
#define ATH_GPIO_FUNCTION        AR7240_GPIO_BASE+0x6c
#define ATH_GPIO_OUT_FUNCTION0   AR7240_GPIO_BASE+0x2c
#define ATH_GPIO_OUT_FUNCTION1   AR7240_GPIO_BASE+0x30
#define ATH_GPIO_OUT_FUNCTION2   AR7240_GPIO_BASE+0x34
#define ATH_GPIO_OUT_FUNCTION3   AR7240_GPIO_BASE+0x38
#define ATH_GPIO_OUT_FUNCTION4   AR7240_GPIO_BASE+0x3c
#define ATH_GPIO_OUT_FUNCTION5   AR7240_GPIO_BASE+0x40

#define ath_gpio_set(_pin)    ar7240_reg_rmw_set(ATH_GPIO_OUT, (1 << _pin))
#define ath_gpio_clear(_pin)  ar7240_reg_rmw_clear(ATH_GPIO_OUT, (1 << _pin))
#define ath_power_led_on()    ath_gpio_clear(POWER_LED_GPIO)
#define ath_power_led_off()   ath_gpio_set(POWER_LED_GPIO)
#define ath_status_led_on()   ath_gpio_clear(STATUS_LED_GPIO)
#define ath_status_led_off()  ath_gpio_set(STATUS_LED_GPIO)

/* return : 0 (not pressed) / 1 (pressed) */
static int ath_reset_button_is_press(void)
{
	if ( ar7240_reg_rd(ATH_GPIO_IN) & (1 << RESET_BUTTON_GPIO))
		return 0;
	return 1;
}

static void ath_gpio_init(void)
{
	unsigned int rddata;
	
	/* gpio 0~3 are default to JTAG pins, we must disable JTAG function. */
	ar7240_reg_rmw_set(ATH_GPIO_FUNCTION, (1 << 1));

	rddata = ar7240_reg_rd(ATH_GPIO_OUT_FUNCTION0);
	ar7240_reg_wr(ATH_GPIO_OUT_FUNCTION0, (rddata & 0xff000000)); /* 03, 02, 01, 00 */
	rddata = ar7240_reg_rd(ATH_GPIO_OUT_FUNCTION1);
	ar7240_reg_wr(ATH_GPIO_OUT_FUNCTION1, (rddata & 0xffffff00)); /* 07, 06, 05, 04 */
	rddata = ar7240_reg_rd(ATH_GPIO_OUT_FUNCTION2);
	ar7240_reg_wr(ATH_GPIO_OUT_FUNCTION2, (rddata & 0x00ffffff)); /* 11, 10, 09, 08 */
	rddata = ar7240_reg_rd(ATH_GPIO_OUT_FUNCTION3);
	ar7240_reg_wr(ATH_GPIO_OUT_FUNCTION3, (rddata & 0x000000ff)); /* 15, 14, 13, 12 */
	rddata = ar7240_reg_rd(ATH_GPIO_OUT_FUNCTION4);
	ar7240_reg_wr(ATH_GPIO_OUT_FUNCTION4, (rddata & 0xff000000)); /* 19, 18, 17, 16 */
	rddata = ar7240_reg_rd(ATH_GPIO_OUT_FUNCTION5);
	ar7240_reg_wr(ATH_GPIO_OUT_FUNCTION5, (rddata & 0xffffff00)); /* 23, 22, 21, 20 */

	/* let gpio pin of power, status, serial leds be output enabled. */
	ar7240_reg_rmw_clear(ATH_GPIO_OE,
				(1 << POWER_LED_GPIO) | (1 << STATUS_LED_GPIO) |
				(1 << SERIAL_LED_DATA) | (1 << SERIAL_LED_SH) |
				(1 << SERIAL_LED_ST) | (1 << SERIAL_LED_RST) |
				(1 << WLAN_LED_GPIO) | (1 << WPS_LED_GPIO) );
	/* let gpio pin of reset button be input enabled. */
	ar7240_reg_rmw_set(ATH_GPIO_OE, (1 << RESET_BUTTON_GPIO));
	serial_led_output(0);

}

void wasp_gpio_config(void)
{
	ath_gpio_init();
#if 0
	/* Disable clock obs */
	ar7240_reg_wr (AR7240_GPIO_FUNC, (ar7240_reg_rd(AR7240_GPIO_FUNC) & 0xffe7e0ff));
	/* Enable eth Switch LEDs */
#ifdef CONFIG_K31
	ar7240_reg_wr (AR7240_GPIO_FUNC, (ar7240_reg_rd(AR7240_GPIO_FUNC) | 0xd8));
#else
	ar7240_reg_wr (AR7240_GPIO_FUNC, (ar7240_reg_rd(AR7240_GPIO_FUNC) | 0xfa));
#endif
#endif
}

int
wasp_mem_config(void)
{
	extern void ath_ddr_tap_cal(void);
	unsigned int type, reg32;

	type = wasp_ddr_initial_config(CFG_DDR_REFRESH_VAL);

	ath_ddr_tap_cal();

#ifndef COMPRESSED_UBOOT
	printf("Tap value selected = 0x%x [0x%x - 0x%x]\n",
		ar7240_reg_rd(AR7240_DDR_TAP_CONTROL0),
		ar7240_reg_rd(0xbd007f10), ar7240_reg_rd(0xbd007f14));
#endif

	/* Take WMAC out of reset */
	reg32 = ar7240_reg_rd(AR7240_RESET);
	reg32 = reg32 &  ~AR7240_RESET_WMAC;
	ar7240_reg_wr_nf(AR7240_RESET, reg32);

	/* Switching regulator settings */
	ar7240_reg_wr_nf(0x18116c40, 0x633c8176); /* AR_PHY_PMU1 */
	ar7240_reg_wr_nf(0x18116c44, 0x10380000); /* AR_PHY_PMU2 */

	wasp_usb_initial_config();

	wasp_gpio_config();

	reg32 = ar7240_ddr_find_size();

	return reg32;
}

long int initdram(int board_type)
{
	return (wasp_mem_config());
}

int	checkboard(void)
{
	printf("DNI HW ID: 29763906+8+0+64+2X2+0 U-boot 1.1.4 dni29 V0.3\n");
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
