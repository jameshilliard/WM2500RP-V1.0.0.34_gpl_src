#include <ar7240_soc_hw29763906p8p0p64p2x2p0.h>
#include <common.h>
#include <command.h>
#include <asm/addrspace.h>

#define LED_DATA    17
#define LED_SH      16
#define LED_ST      11
#define LED_RST     15
#define WPS_LED		18
#define STATUS_LED_GPIO 13
#define POWER_LED_GPIO  14

#define LED_OFF		1
#define LED_ON		0


static void ath_gpio_config_output(int gpio)
{
	ar7240_reg_rmw_clear(AR7240_GPIO_OE, (1 << gpio));
}

static void ath_gpio_out_val(int gpio, int val)
{
	if( val & 0x1 )
	{
		ar7240_reg_rmw_set(AR7240_GPIO_OUT, (1 << gpio));
	}
	else
	{
		ar7240_reg_rmw_clear(AR7240_GPIO_OUT, (1 << gpio));
	}
}

void serial_led_output(int val)
{
	int i;
	int bit_end;

	for(i=0;i<16;i++)       //shift serial_led_flag to register
	{
		bit_end = ((val>>i) & 1)?1:0;
		ath_gpio_out_val(LED_DATA,bit_end);
		ath_gpio_out_val(LED_SH,1);
		ath_gpio_out_val(LED_SH,0);
	}
	//shift to data register, output to led
	ath_gpio_out_val(LED_ST,1);
	ath_gpio_out_val(LED_ST,0);
}

int do_ledclr (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int val;

	if (argc < 2) {
		val = 0;
	} else {
		val = simple_strtol(argv[1], NULL, 16);
	}

	ath_gpio_config_output(LED_ST);              //gpio 11
	ath_gpio_out_val(LED_ST,0);

	ath_gpio_config_output(LED_RST);             //gpio 15
	ath_gpio_out_val(LED_RST,1);

	ath_gpio_config_output(LED_SH);              //gpio 16
	ath_gpio_out_val(LED_SH,0);

	ath_gpio_config_output(LED_DATA);            //gpio 17
	ath_gpio_out_val(LED_DATA,0);

	if (val == 0) {
		serial_led_output(0xffff);
	} else {
		serial_led_output(0);
	}

	ath_gpio_config_output(WPS_LED);
	ath_gpio_out_val(WPS_LED,LED_OFF);

	ath_gpio_config_output(STATUS_LED_GPIO);
	ath_gpio_out_val(STATUS_LED_GPIO, LED_ON);

	ath_gpio_config_output(POWER_LED_GPIO);
	ath_gpio_out_val(POWER_LED_GPIO, LED_OFF);

	return 0;
}

U_BOOT_CMD(
		ledclr,2, 0, do_ledclr,
		"ledclr		- Clear LEDs\n",
		NULL
		);
