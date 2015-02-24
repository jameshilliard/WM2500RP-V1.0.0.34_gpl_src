#ifndef EXPORT_SYMTAB
#define EXPORT_SYMTAB
#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <asm/types.h>
#include <asm/irq.h>
#include <asm/delay.h>
#include <asm/system.h>

#include <atheros.h>

#define ATH_FACTORY_RESET		0x89ABCDEF

static atomic_t ath_fr_status = ATOMIC_INIT(0);
static volatile int ath_fr_opened = 0;
static wait_queue_head_t ath_fr_wq;
static u_int32_t push_time = 0;
struct timer_list os_timer_t;
static int initial_led_state = 0;
struct timer_list os_button_timer_t;
static int reset_button_led_status;
struct timer_list reset_button_timer_t;
struct timer_list  os_diswps_t; /* WPS LED blinking while WPS error occured,
				   like push button is pressed if WPS was disabled */
static int diswps_blink_count = 0; /* max value: 50, blinking 5 seconds */
static int diswps = 0;	/* 0 => WPS is enabled, 1 => WPS is disabled */

typedef enum {
	free = 0,	/* release button, pop up */
	push = 1,	/* press button */
	hold = 2,	/* press button and hold pushed state */
}but_state_t;
static but_state_t wifibut = free;	/* WLAN switch */
static but_state_t pushbut = free;	/* Push button */
static but_state_t resetbut = free;	/* Reset button */

#ifdef POWER_ON_RLED_GPIO
#define POWER_ON_TIMEOUT            60      /* 60 * 0.5 seconds */
#define POWER_LED_BLINK_INTERVAL    500     /* microseconds */
static volatile int power_on_finish = 0;
struct timer_list power_on_timer;
struct timer_list  os_butdet_t; /* button detect */
static struct proc_dir_entry *power_on_proc_entry = NULL;
#endif

#define ACTIVE_HIGH_VOLTAGE	0x3
#define LED_ON(gpio)	(((1 << gpio) & ACTIVE_HIGH_VOLTAGE) ? 1 : 0)
#define LED_OFF(gpio)	(((1 << gpio) & ACTIVE_HIGH_VOLTAGE) ? 0 : 1)

#define frdbg printk

#define WPS_LED_OFF	1
#define WPS_LED_ON	0

#define USB_LED_OFF 1
#define USB_LED_ON 0

#define POWER_LED_OFF 1
#define POWER_LED_ON 0

#define READY_LED_OFF 1
#define READY_LED_ON 0

#define SIMPLE_CONFIG_OFF	1
#define SIMPLE_CONFIG_ON	2
#define SIMPLE_CONFIG_BLINK	3
#define SIMPLE_CONFIG_FAIL  4

/* WM2500RP GPIOs Define Start */
#define WLAN_BUTTON_GPIO	23
#define WPS_BUTTON_GPIO		17
#define RESET_BUTTON_GPIO   2
#define STATUS_LED_GPIO			14
#define POWER_LED_GPIO			19
#define MOCA_AMBER_LED_GPIO		15
#define MOCA_GREEN_LED_GPIO		16
#define WLAN_2G_LED_GPIO		13
#define WLAN_5G_LED_GPIO		11
#define WPS_LED_GPIO			1
#define LAN_1G_LED_GPIO			18
#define LAN_10_100_LED_GPIO		20
//-----------------------------------------
#define POLL_INTERVAL			100	/* millisecond */
#define PUSH_BUTTON_TIME        100
#define RESET_BUTTON_TIME       5000
#define WLAN_SWITCH_TIME        100
#define RESET_BUTTON_COUNT      (RESET_BUTTON_TIME/POLL_INTERVAL)
#define WPS_LED_ERR_INTERVAL	100		//millisecond
#define TEST_LED_ONTIME         250     //millisecond
#define TEST_LED_OFFTIME        750     //millisecond
/* WM2500RP GPIOs Define END */

#define OS_TIMER_FUNC(_fn)	\
	void _fn(unsigned long timer_arg)

#define OS_GET_TIMER_ARG(_arg, _type)	\
	(_arg) = (_type)(timer_arg)

#define OS_INIT_TIMER(_osdev, _timer, _fn, _arg)	\
do {							\
	init_timer(_timer);				\
	(_timer)->function = (_fn);			\
	(_timer)->data = (unsigned long)(_arg);		\
} while (0)

#define OS_SET_TIMER(_timer, _ms)	\
	mod_timer(_timer, jiffies + ((_ms)*HZ)/1000)

#define OS_CANCEL_TIMER(_timer)		del_timer(_timer)
/*
 * GPIO interrupt stuff
 */
typedef enum {
    INT_TYPE_EDGE,
    INT_TYPE_LEVEL,
} ath_gpio_int_type_t;

typedef enum {
    INT_POL_ACTIVE_LOW,
    INT_POL_ACTIVE_HIGH,
} ath_gpio_int_pol_t;
/*
** Simple Config stuff
*/
typedef irqreturn_t (*sc_callback_t) (int, void *, void *, void *);

/*
 * Multiple Simple Config callback support
 * For multiple radio scenarios, we need to post the button push to
 * all radios at the same time.  However, there is only 1 button, so
 * we only have one set of GPIO callback pointers.
 *
 * Creating a structure that contains each callback, tagged with the
 * name of the device registering the callback.  The unregister routine
 * will need to determine which element to "unregister", so the device
 * name will have to be passed to unregister also
 */

typedef struct {
	char		*name;
	sc_callback_t	registered_cb;
	void		*cb_arg1;
	void		*cb_arg2;
} multi_callback_t;

/*
 * Specific instance of the callback structure
 */
static multi_callback_t sccallback[3];
static volatile int ignore_pushbutton = 1;
static struct proc_dir_entry *simple_config_entry = NULL;
static struct proc_dir_entry *simulate_push_button_entry = NULL;
static struct proc_dir_entry *simple_config_led_entry = NULL;
static struct proc_dir_entry *simple_config_all_led_entry = NULL;
static struct proc_dir_entry *button_state = NULL;
static struct proc_dir_entry *disable_wps = NULL;	/* WPS is disabled? write into: 1 => disable, 0 => enable */
static struct proc_dir_entry *button_test = NULL;
static int reset_count=0;
static int wps_count=0;
static int wireless_count=0;
static int wps_led_blinking = 0;

void ath_gpio_config_int(int gpio,
			 ath_gpio_int_type_t type,
			 ath_gpio_int_pol_t polarity)
{
	u32 val;

	/*
	 * allow edge sensitive/rising edge too
	 */
	if (type == INT_TYPE_LEVEL) {
		/* level sensitive */
		ath_reg_rmw_set(ATH_GPIO_INT_TYPE, (1 << gpio));
	} else {
		/* edge triggered */
		val = ath_reg_rd(ATH_GPIO_INT_TYPE);
		val &= ~(1 << gpio);
		ath_reg_wr(ATH_GPIO_INT_TYPE, val);
	}

	if (polarity == INT_POL_ACTIVE_HIGH) {
		ath_reg_rmw_set(ATH_GPIO_INT_POLARITY, (1 << gpio));
	} else {
		val = ath_reg_rd(ATH_GPIO_INT_POLARITY);
		val &= ~(1 << gpio);
		ath_reg_wr(ATH_GPIO_INT_POLARITY, val);
	}

	ath_reg_rmw_set(ATH_GPIO_INT_ENABLE, (1 << gpio));
}

void ath_gpio_config_output(int gpio)
{
#if defined(CONFIG_MACH_AR934x) || \
    defined(CONFIG_MACH_QCA955x)
	ath_reg_rmw_clear(ATH_GPIO_OE, (1 << gpio));
#else
	ath_reg_rmw_set(ATH_GPIO_OE, (1 << gpio));
#endif
}
EXPORT_SYMBOL(ath_gpio_config_output);

void ath_gpio_config_input(int gpio)
{
#if defined(CONFIG_MACH_AR934x) || \
    defined(CONFIG_MACH_QCA955x)
	ath_reg_rmw_set(ATH_GPIO_OE, (1 << gpio));
#else
	ath_reg_rmw_clear(ATH_GPIO_OE, (1 << gpio));
#endif
}

void ath_gpio_out_val(int gpio, int val)
{
	if (val & 0x1) {
		ath_reg_rmw_set(ATH_GPIO_OUT, (1 << gpio));
	} else {
		ath_reg_rmw_clear(ATH_GPIO_OUT, (1 << gpio));
	}
}
EXPORT_SYMBOL(ath_gpio_out_val);

int ath_gpio_in_val(int gpio)
{
	return ((1 << gpio) & (ath_reg_rd(ATH_GPIO_IN)));
}

static void
ath_gpio_intr_enable(unsigned int irq)
{
	ath_reg_rmw_set(ATH_GPIO_INT_MASK,
				(1 << (irq - ATH_GPIO_IRQ_BASE)));
}

static void
ath_gpio_intr_disable(unsigned int irq)
{
	ath_reg_rmw_clear(ATH_GPIO_INT_MASK,
				(1 << (irq - ATH_GPIO_IRQ_BASE)));
}

static unsigned int
ath_gpio_intr_startup(unsigned int irq)
{
	ath_gpio_intr_enable(irq);
	return 0;
}

static void
ath_gpio_intr_shutdown(unsigned int irq)
{
	ath_gpio_intr_disable(irq);
}

static void
ath_gpio_intr_ack(unsigned int irq)
{
	ath_gpio_intr_disable(irq);
}

static void
ath_gpio_intr_end(unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED | IRQ_INPROGRESS)))
		ath_gpio_intr_enable(irq);
}

static int
ath_gpio_intr_set_affinity(unsigned int irq, const struct cpumask *dest)
{
	/*
	 * Only 1 CPU; ignore affinity request
	 */
	return 0;
}

struct irq_chip /* hw_interrupt_type */ ath_gpio_intr_controller = {
	.name = "ATH GPIO",
	.startup = ath_gpio_intr_startup,
	.shutdown = ath_gpio_intr_shutdown,
	.enable = ath_gpio_intr_enable,
	.disable = ath_gpio_intr_disable,
	.ack = ath_gpio_intr_ack,
	.end = ath_gpio_intr_end,
	.eoi = ath_gpio_intr_end,
	.set_affinity = ath_gpio_intr_set_affinity,
};

void ath_gpio_irq_init(int irq_base)
{
	int i;

	for (i = irq_base; i < irq_base + ATH_GPIO_IRQ_COUNT; i++) {
		irq_desc[i].status = IRQ_DISABLED;
		irq_desc[i].action = NULL;
		irq_desc[i].depth = 1;
		//irq_desc[i].chip = &ath_gpio_intr_controller;
		set_irq_chip_and_handler(i, &ath_gpio_intr_controller,
					 handle_percpu_irq);
	}
}

void
ath_gpio_set_fn(int gpio, int fn)
{
#define gpio_fn_reg(x)	((x) / 4)
#define gpio_lsb(x)	(((x) % 4) * 8)
#define gpio_msb(x)	(gpio_lsb(x) + 7)
#define gpio_mask(x)	(0xffu << gpio_lsb(x))
#define gpio_set(x, f)	(((f) & 0xffu) << gpio_lsb(x))

	uint32_t *reg = ((uint32_t *)GPIO_OUT_FUNCTION0_ADDRESS) +
					gpio_fn_reg(gpio);

	ath_reg_wr(reg, (ath_reg_rd(reg) & ~gpio_mask(gpio)) | gpio_set(gpio, fn));
}

int32_t register_simple_config_callback(char *cbname, void *callback, void *arg1, void *arg2)
{
	int i;
	int cbnum = sizeof(sccallback)/sizeof(multi_callback_t);

	printk("SC Callback Registration for %s\n", cbname);

	for (i = 0; i< cbnum; i++)
	{
		if (!sccallback[i].name) {
			sccallback[i].name = (char*)kmalloc(strlen(cbname), GFP_KERNEL);
			strcpy(sccallback[i].name, cbname);
			sccallback[i].registered_cb = (sc_callback_t) callback;
			sccallback[i].cb_arg1 = arg1;
			sccallback[i].cb_arg2 = arg2;
			break;
		}
	}

	if (i == cbnum)
	{
		printk("@@@@ Failed SC Callback Registration for %s\n", cbname);
		return -1;
	}
	return 0;
}
EXPORT_SYMBOL(register_simple_config_callback);

int32_t unregister_simple_config_callback(char *cbname)
{
	int i;
	int cbnum = sizeof(sccallback)/sizeof(multi_callback_t);

	for (i = 0; i< cbnum; i++)
	{
		if (sccallback[i].name && strcmp(sccallback[i].name, cbname) == 0) {
			kfree(sccallback[i].name);
			sccallback[i].name = NULL;
			sccallback[i].registered_cb = NULL;
			sccallback[i].cb_arg1 = NULL;
			sccallback[i].cb_arg2 = NULL;
			break;
		}
	}

	if (i == cbnum){
		printk("!&!&!&!& ERROR: Unknown callback name %s\n", cbname);
		return -1;
	}
	return 0;
}
EXPORT_SYMBOL(unregister_simple_config_callback);

#define WPS_TIME_OUT 120
static OS_TIMER_FUNC(wps_led_blink)
{
	static int WPSled = WPS_LED_ON, sec = 0;
	ath_gpio_out_val(WPS_LED_GPIO, WPSled);
	WPSled = !WPSled;
	sec++;
	if (sec < WPS_TIME_OUT) {
		OS_SET_TIMER(&os_timer_t, 1000);
	} else {
		sec = 0;
		wps_led_blinking = 0;
		OS_CANCEL_TIMER(&os_timer_t);
		ath_gpio_out_val(WPS_LED_GPIO, initial_led_state);
	}
}

static OS_TIMER_FUNC(wps_led_fail)
{
	static int WPSled = WPS_LED_ON, sec = 0;
	ath_gpio_out_val(WPS_LED_GPIO, WPSled);
	WPSled = !WPSled;
	sec++;
	if (sec < 250 * 5) {//Keep blinking for 250 seconds & timer callback kicks in every 200 ms
		OS_SET_TIMER(&os_timer_t, 200);
	} else {
		sec = 0;
		wps_led_blinking = 0;
		OS_CANCEL_TIMER(&os_timer_t);
		ath_gpio_out_val(WPS_LED_GPIO, initial_led_state);
	}
}

static OS_TIMER_FUNC(diswps_led_blink)
{
	static int onoff = 0;

	if (!diswps ||  diswps_blink_count <= 0)
		return;

	if (onoff) {
		ath_gpio_out_val(WPS_LED_GPIO, LED_OFF(WPS_LED_GPIO));
	} else {
		ath_gpio_out_val(WPS_LED_GPIO, LED_ON(WPS_LED_GPIO));
	}
	onoff = 1 - onoff;
	diswps_blink_count--;
	if (diswps_blink_count == 0) {
		ath_gpio_out_val(WPS_LED_GPIO, LED_OFF(WPS_LED_GPIO));
	}
	OS_SET_TIMER(&os_diswps_t, WPS_LED_ERR_INTERVAL);
}

static OS_TIMER_FUNC(wps_led_success)
{
    wps_led_blinking = 0;
    OS_CANCEL_TIMER(&os_timer_t);
    ath_gpio_out_val(WPS_LED_GPIO, initial_led_state);
}

static OS_TIMER_FUNC(reset_button_led_action)
{
	if (reset_button_led_status == POWER_LED_OFF) {
		ath_gpio_out_val(STATUS_LED_GPIO, POWER_LED_OFF);
		reset_button_led_status = POWER_LED_ON;
		OS_SET_TIMER(&reset_button_timer_t, TEST_LED_OFFTIME);
	} else {
		ath_gpio_out_val(STATUS_LED_GPIO, POWER_LED_ON);
		reset_button_led_status = POWER_LED_OFF;
		OS_SET_TIMER(&reset_button_timer_t, TEST_LED_ONTIME);
	}
}

static OS_TIMER_FUNC(button_detect)
{
	static int staled_s = 0;
	static int wifibut_s = 0;
	static int wlan_count = 0;
	static int pushbut_s = 0;
	static int rstbut_count = 0;

	static int reset_push = 0;
	static int wps_push = 0;
	static int wlan_push = 0;

	/* GPIO 23: WIRELESS ON/OFF(Input Pin / Active LOW) */
	if (!ath_gpio_in_val(WLAN_BUTTON_GPIO)) {
		wlan_count++;
		if(!wlan_push)
			wlan_push = 1;
		/* [Bug 24969]According to NTGR's requirement, it should only trigger enabling or
		 * disabling the wireless radio after the WLAN switch button is pressed and held
		 * for more than 1 second.
		 * Frequency of GPIO state detecting is 100 milliseconds, so if detect that WLAN
		 * switch button is pressed and held more than 9 times, just trigger the
		 * functionality to trun on/off WLAN.
		 */
		if (wlan_count > 9 && !wifibut_s) {
			wifibut_s = 1;
			wifibut = push;
			OS_CANCEL_TIMER(&os_diswps_t);
		}
	} else {
		wlan_count = 0;
		if (wifibut_s) {
			wifibut_s = 0;
		}
		if (wlan_push){
			wlan_push = 0;
			wireless_count++;
		}
	}

	/* GPIO 11: JUMPSTART (Input Pin / Active HIGH) */
	if (!ath_gpio_in_val(WPS_BUTTON_GPIO)) {
		if (!pushbut_s) {
			pushbut_s = 1;
			pushbut = push;
			/* blink WPS LED more than 5 seconds if WPS is disable */
			
			if (diswps) {
				diswps_blink_count = 50;
				OS_SET_TIMER(&os_diswps_t, WPS_LED_ERR_INTERVAL);
			}
			
		}
		/* if WPS was disabled, and push button is pressed and hold, just blink WPS LED
		 * until push button is free, and then continue blink WPS LED 5 seconds */
		if (diswps)
			diswps_blink_count = 50;
	} else {
		if (pushbut_s) {
			pushbut_s = 0;
			wps_count++;
		}
	}

	/* GPIO 2: RESET SWITCH (Input Pin/Active LOW) */
	if (!ath_gpio_in_val(RESET_BUTTON_GPIO)) {
		rstbut_count++;
		if(!reset_push)
			reset_push = 1;
		/* turn off POWER LED, turn on STATUS LED (amber), set timer, blinking STATUS LED */
		if (!staled_s) {
			ath_gpio_out_val(POWER_LED_GPIO, LED_OFF(POWER_LED_GPIO));
			ath_gpio_out_val(STATUS_LED_GPIO, LED_ON(STATUS_LED_GPIO));
			reset_button_led_status = POWER_LED_OFF;
    		OS_SET_TIMER(&reset_button_timer_t, RESET_BUTTON_TIME);
			staled_s = 1;
		}
	} else {
		if (RESET_BUTTON_COUNT <= rstbut_count)
			resetbut = hold;
		else if (0 < rstbut_count && RESET_BUTTON_COUNT > rstbut_count)
			resetbut = push;
		/* turn on POWER, turn off STATUS LEDs, cancel timer */
		if (staled_s) {
			OS_CANCEL_TIMER(&reset_button_timer_t);
			ath_gpio_out_val(POWER_LED_GPIO, LED_ON(POWER_LED_GPIO));
			ath_gpio_out_val(STATUS_LED_GPIO, LED_OFF(STATUS_LED_GPIO));
			staled_s = 0;
		}
		rstbut_count = 0;
		if (reset_push){
			reset_push = 0;
			reset_count++;
		}
	}

	OS_SET_TIMER(&os_button_timer_t, POLL_INTERVAL);
}

int ath_simple_config_invoke_cb(int simplecfg_only, int irq_enable, int cpl)
{
	int i;
	int cbnum = sizeof(sccallback)/sizeof(multi_callback_t);

	printk("%s: sc %d, irq %d, ignorepb %d, jiffies %lu\n", __func__,
		simplecfg_only, irq_enable, ignore_pushbutton, jiffies);
	if (simplecfg_only) {
		if (ignore_pushbutton) {
			ath_gpio_config_int(JUMPSTART_GPIO, INT_TYPE_LEVEL,
						INT_POL_ACTIVE_HIGH);
			ignore_pushbutton = 0;
			push_time = jiffies;
			return IRQ_HANDLED;
		}

		ath_gpio_config_int(JUMPSTART_GPIO, INT_TYPE_LEVEL,
					INT_POL_ACTIVE_LOW);
		ignore_pushbutton = 1;
	}

	if (irq_enable)
		local_irq_enable();

	if (push_time) {
		push_time = jiffies - push_time;
	}
	printk ("simple_config callback.. push dur in sec %d\n", push_time/HZ);


	for (i = 0; i< cbnum; i++)
	{
		if (sccallback[i].registered_cb) {
			if (sccallback[i].cb_arg2) {
				*(u_int32_t *)sccallback[i].cb_arg2 = push_time/HZ;
			}
			sccallback[i].registered_cb (cpl, sccallback[i].cb_arg1, NULL, sccallback[i].cb_arg2);
		}

	}

	return IRQ_HANDLED;
}

/*
 * Irq for front panel SW jumpstart switch
 * Connected to XSCALE through GPIO4
 */
irqreturn_t jumpstart_irq(int cpl, void *dev_id)
{
#ifndef AP_RESET_GPIO
	unsigned int delay;
	if (atomic_read(&ath_fr_status)) {
		local_irq_disable();

#define UDELAY_COUNT 4000
		push_time = jiffies;

		for (delay = UDELAY_COUNT; delay; delay--) {
			if (ath_gpio_in_val(JUMPSTART_GPIO)) {
				break;
			}
			udelay(1000);
		}

		if (!delay) {
			atomic_dec(&ath_fr_status);
			/*
			 * since we are going to reboot the board, we
			 * don't need the interrupt handler anymore,
			 * so disable it.
			 */
			disable_irq(ATH_GPIO_IRQn(JUMPSTART_GPIO));
			wake_up(&ath_fr_wq);
			printk("\nath: factory configuration restored..\n");
			local_irq_enable();
			return IRQ_HANDLED;
		} else {
			return (ath_simple_config_invoke_cb
				(0, 1, cpl));
		}
	} else
#endif
		return (ath_simple_config_invoke_cb(1, 0, cpl));
}

#ifdef AP_RESET_GPIO
irqreturn_t ath_reset_irq(int cpl, void *dev_id)
{
    local_irq_disable();

    if(push_time == 0){
        ath_gpio_config_int(AP_RESET_GPIO, INT_TYPE_LEVEL,
                 INT_POL_ACTIVE_HIGH);
        push_time = jiffies;
		local_irq_enable();
        return IRQ_HANDLED;
    }else{
        ath_gpio_config_int(AP_RESET_GPIO, INT_TYPE_LEVEL,
                INT_POL_ACTIVE_LOW);
        push_time = jiffies - push_time;
    }

    if(push_time/HZ > 3){
		/*
		 * since we are going to reboot the board, we
		 * don't need the interrupt handler anymore,
		 * so disable it.
		 */
		disable_irq(ATH_GPIO_IRQn(AP_RESET_GPIO));
		wake_up(&ath_fr_wq);
		printk("\nath: factory configuration restored..\n");
        push_time = 0;
		local_irq_enable();
		return IRQ_HANDLED;
	} else if(push_time/HZ < 1){
        push_time = 0;
		local_irq_enable();
		return IRQ_HANDLED;
    }else{
		extern void ath_restart(char *);
		ath_restart(NULL);
		return IRQ_HANDLED;
	}
}
#endif

static int push_button_read(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	return 0;
}

static int push_button_write(struct file *file, const char *buf,
				unsigned long count, void *data)
{
	int i;
	int cbnum = sizeof(sccallback)/sizeof(multi_callback_t);

	for (i = 0; i< cbnum; i++){
		if (sccallback[i].registered_cb) {
			sccallback[i].registered_cb (0, sccallback[i].cb_arg1, 0, sccallback[i].cb_arg2);
		}
	}
	return count;
}

typedef enum {
	LED_STATE_OFF = 1,
	LED_STATE_ON = 2,
	LED_STATE_BLINKING = 3,
	LED_STATE_NORMAL = 4,
} led_state_e;

static led_state_e simple_config_led_state = LED_STATE_OFF;

static int gpio_simple_config_led_read(char *page, char **start, off_t off,
					int count, int *eof, void *data)
{
	return sprintf(page, "%d\n", simple_config_led_state);
}

static int gpio_simple_config_led_write(struct file *file, const char *buf,
					unsigned long count, void *data)
{
	u_int32_t val;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;
    
    if(val == SIMPLE_CONFIG_BLINK){
        if( ath_gpio_in_val(WPS_LED_GPIO) == 0 ){
            initial_led_state = WPS_LED_ON;
        }else{ 
            initial_led_state = WPS_LED_OFF;
        }
    }

	if ((val == SIMPLE_CONFIG_BLINK) && !wps_led_blinking) { /* wps LED blinking */
		wps_led_blinking = 1;
		simple_config_led_state = SIMPLE_CONFIG_BLINK;
		ath_gpio_out_val(WPS_LED_GPIO, WPS_LED_ON);
		OS_CANCEL_TIMER(&os_timer_t);
		OS_INIT_TIMER(NULL, &os_timer_t, wps_led_blink, &os_timer_t);
		OS_SET_TIMER(&os_timer_t, 1000);
	} else if (val == SIMPLE_CONFIG_FAIL) {	/* WPS failed */
		wps_led_blinking = 0;
		simple_config_led_state = SIMPLE_CONFIG_FAIL;
		ath_gpio_out_val(WPS_LED_GPIO, WPS_LED_ON);
		OS_CANCEL_TIMER(&os_timer_t);
		OS_INIT_TIMER(NULL, &os_timer_t, wps_led_fail, &os_timer_t);
		OS_SET_TIMER(&os_timer_t, 200);
	} else if (val == SIMPLE_CONFIG_ON) {	/* WPS Success */
		wps_led_blinking = 0;
		simple_config_led_state = SIMPLE_CONFIG_ON;
		OS_CANCEL_TIMER(&os_timer_t);
		ath_gpio_out_val(WPS_LED_GPIO, WPS_LED_ON);
		OS_INIT_TIMER(NULL, &os_timer_t, wps_led_success, &os_timer_t);
		OS_SET_TIMER(&os_timer_t, 120000);
	} else if (val == SIMPLE_CONFIG_OFF) {	/* wps LED off */
		wps_led_blinking = 0;
		simple_config_led_state = SIMPLE_CONFIG_OFF;
		OS_CANCEL_TIMER(&os_timer_t);
		ath_gpio_out_val(WPS_LED_GPIO, WPS_LED_OFF);
	}

	return count;
}

void ap_usb_led_on(void)
{
#ifdef CONFIG_MACH_AR934x
#if !defined(CONFIG_I2S) && defined(AP_USB_LED_GPIO)
	unsigned int rddata;

	if (AP_USB_LED_GPIO == 4) {
		rddata = ath_reg_rd(ATH_GPIO_OUT_FUNCTION1); //87- for USB suspend
		rddata = rddata & 0xffffff00;
		rddata = rddata | ATH_GPIO_OUT_FUNCTION1_ENABLE_GPIO_4(0x0);
		ath_reg_wr(ATH_GPIO_OUT_FUNCTION1, rddata);
	} else if (AP_USB_LED_GPIO == 11) {
		rddata = ath_reg_rd(ATH_GPIO_OUT_FUNCTION2); //87- for USB suspend
		rddata = rddata & 0x00ffffff;
		rddata = rddata | ATH_GPIO_OUT_FUNCTION2_ENABLE_GPIO_11(0x0);
		ath_reg_wr(ATH_GPIO_OUT_FUNCTION2, rddata);
	}

	ath_reg_rmw_clear(ATH_GPIO_OE, (1<<AP_USB_LED_GPIO));
	ath_reg_rmw_clear(ATH_GPIO_OUT, (1<<AP_USB_LED_GPIO));
#endif
#else
#	ifdef AP_USB_LED_GPIO
	ath_gpio_config_output(AP_USB_LED_GPIO);
	ath_gpio_set_fn(AP_USB_LED_GPIO, 0);
	ath_gpio_out_val(AP_USB_LED_GPIO, USB_LED_ON);
#	endif
#endif
}

EXPORT_SYMBOL(ap_usb_led_on);

void ap_usb_led_off(void)
{
#ifdef CONFIG_MACH_AR934x
#if !defined(CONFIG_I2S) && defined(AP_USB_LED_GPIO)
	ath_reg_rmw_set(ATH_GPIO_OUT, (1<<AP_USB_LED_GPIO));
#endif
#else
#	ifdef AP_USB_LED_GPIO
	ath_gpio_out_val(AP_USB_LED_GPIO, USB_LED_OFF);
#	endif
#endif
}
EXPORT_SYMBOL(ap_usb_led_off);


void ap_ready_led_on(void)
{
#ifdef AP_READY_LED_GPIO
	ath_gpio_out_val(AP_READY_LED_GPIO, READY_LED_ON);
#endif
}
EXPORT_SYMBOL(ap_ready_led_on);

void ap_ready_led_off(void)
{
#ifdef AP_READY_LED_GPIO
	ath_gpio_out_val(AP_READY_LED_GPIO, READY_LED_OFF);
#endif
}
EXPORT_SYMBOL(ap_ready_led_off);

void ath_config_slave_mdio_gpios(void)
{
        unsigned int mask;

        /* Slave MDIO phy address setting */
        ath_reg_wr(MDIO_PHY_ADDR_ADDRESS, ATH_MDIO_PHY_ADDR);

        /* Set the Clock Divisor */
        mask = ath_reg_rd(ATH_MDIO_MAC_MII_MGMT_CFG) & ~(0xf);
        mask |= ATH_MDIO_MGMT_CFG_CLK_DIV_28;
        ath_reg_wr(ATH_MDIO_MAC_MII_MGMT_CFG, mask);

        /* Set External MDIO Multiplexing Register */
        mask = ath_reg_rd(ATH_GPIO_IN_ENABLE3) &
                      ~(GPIO_IN_ENABLE3_BOOT_EXT_MDC_MASK |
                        GPIO_IN_ENABLE3_BOOT_EXT_MDO_MASK);
        mask |= GPIO_IN_ENABLE3_BOOT_EXT_MDC_SET(ATH_MDC_GPIO_PIN) |
                GPIO_IN_ENABLE3_BOOT_EXT_MDO_SET(ATH_MDIO_GPIO_PIN);
        ath_reg_wr(ATH_GPIO_IN_ENABLE3, mask);

        /* Configure the Direction of GPIO Pins */
        ath_gpio_config_input(ATH_MDC_GPIO_PIN);
        ath_gpio_config_output(ATH_MDIO_GPIO_PIN);

        /* Configure GPIO Output function as GPIOs */
        ath_gpio_set_fn(ATH_MDIO_GPIO_PIN, ATH_MDIO_OUPUT_FUNC);
}
EXPORT_SYMBOL(ath_config_slave_mdio_gpios);

#ifdef POWER_ON_RLED_GPIO
static int power_on_finish_read(char *page, char **start, off_t off,
        int count, int *eof, void *data)
{
	return sprintf(page, "%d\n", power_on_finish);
}

static int power_on_finish_write(struct file *file, const char *buf,
        unsigned long count, void *data)
{
	u_int32_t val;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

    power_on_finish = 1;

	return count;
}

static OS_TIMER_FUNC(power_led_blink)
{
	static int power_led_status = POWER_LED_OFF, power_on_timeout = 0;

    OS_CANCEL_TIMER(&power_on_timer);

    if (power_on_finish) {
		ath_gpio_out_val(POWER_ON_GLED_GPIO, POWER_LED_ON);
    } else if (++power_on_timeout >= POWER_ON_TIMEOUT) {
        ath_gpio_out_val(POWER_ON_GLED_GPIO, POWER_LED_OFF);  
        ath_gpio_config_input(POWER_ON_GLED_GPIO);
        ath_gpio_config_output(POWER_ON_RLED_GPIO);
        ath_gpio_out_val(POWER_ON_RLED_GPIO, POWER_LED_ON);  
    } else {
		ath_gpio_out_val(POWER_ON_GLED_GPIO, power_led_status);
	    power_led_status = !power_led_status;
		OS_SET_TIMER(&power_on_timer, POWER_LED_BLINK_INTERVAL);
    }
}
#endif

static int button_state_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
	/**
	 * return states of all buttons.
	 * return value: <PUSHBUTTONstate><WIFIBUTTONstate><RESETBUTTONstate>
	 *   <PUSHBUTTONstate>=1x	#'x' value: 0 - free, 1 - pressed
	 *   <WIFIBUTTONstate>=2x	#'x' value: 0 - free, 1 - pressed
	 *   <RESETBUTTONstate>=3x	#'x' value: 0 - free, 1 - pressed(time<5s), 2 - pressed(time>5s)
	 **/
	 return sprintf(page, "1%u2%u3%u\n", \
	 	(u_int32_t)pushbut, \
		(u_int32_t)wifibut, \
		(u_int32_t)resetbut);
}

static int button_state_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
	/**
	 * set state of button.
	 * valid value: <Button number><value>
	 *   <Button number>: 1 - Push button, 2 - WiFi button, 3 - Reset button
	 *   <value>: should be 0, that indicate the event of button pressed is handled by upper process.
	 **/
	 u_int32_t num, val;
	 char *msg = "Error: valid value inputted into /proc/simple_config/button_state should as below.\n" \
	 		"  <Button number><value>\n" \
			"\t<Button number>: 1 - Push button, 2 - WiFi button, 3 - Reset button.\n" \
			"\t<value> should be 0.\n";

	 num = (u_int32_t)(*buf - '0');
	 val = (u_int32_t)(*(buf + 1) - '0');

	 if (val != 0) {
		printk("%s", msg);
		return count;
	 }

	 switch (num) {
	 case 1:
		pushbut = free;
	 	break;
	 case 2:
		wifibut = free;
	 	break;
	 case 3:
	 	resetbut = free;
	 	break;
	 default:
		printk("%s", msg);
	 	break;
	 }

	 return count;
}

static int disable_wps_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
	return 0;
}

static int disable_wps_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
	u_int32_t val;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

	if (val == 1) { /* WPS was disabled */
		diswps = 1;
	} else if (val == 0) { /* WPS was enabled */
		diswps = 0;
		OS_CANCEL_TIMER(&os_diswps_t);
	}

	return count;
}

void ap_wlan_2g_led_on_off(int status)
{
#ifdef WLAN_2G_LED_GPIO
	ath_gpio_out_val(WLAN_2G_LED_GPIO, status);
#endif
}
EXPORT_SYMBOL(ap_wlan_2g_led_on_off);

void ap_wlan_5g_led_on_off(int status)
{
#ifdef WLAN_5G_LED_GPIO
	ath_gpio_out_val(WLAN_5G_LED_GPIO, status);
#endif
}
EXPORT_SYMBOL(ap_wlan_5g_led_on_off);

void ap_power_led_on(void)
{
#ifdef POWER_LED_GPIO
	ath_gpio_out_val(POWER_LED_GPIO, READY_LED_ON);
#endif
}
EXPORT_SYMBOL(ap_power_led_on);

void ap_power_led_off(void)
{
#ifdef POWER_LED_GPIO

	ath_gpio_out_val(POWER_LED_GPIO, READY_LED_OFF);
#endif
}
EXPORT_SYMBOL(ap_power_led_off);

void ap_status_led_on(void)
{
#ifdef STATUS_LED_GPIO
	ath_gpio_out_val(STATUS_LED_GPIO, READY_LED_ON);
#endif
}
EXPORT_SYMBOL(ap_status_led_on);

void ap_status_led_off(void)
{
#ifdef STATUS_LED_GPIO
	ath_gpio_out_val(STATUS_LED_GPIO, READY_LED_OFF);
#endif
}
EXPORT_SYMBOL(ap_status_led_off);

void ap_moca_led_green_on(void)
{
#ifdef MOCA_GREEN_LED_GPIO
	ath_gpio_out_val(MOCA_GREEN_LED_GPIO, READY_LED_ON);
#endif
}
EXPORT_SYMBOL(ap_moca_led_green_on);

void ap_moca_led_green_off(void)
{
#ifdef MOCA_GREEN_LED_GPIO
	ath_gpio_out_val(MOCA_GREEN_LED_GPIO, READY_LED_OFF);
#endif
}
EXPORT_SYMBOL(ap_moca_led_green_off);

void ap_moca_led_amber_on(void)
{
#ifdef MOCA_AMBER_LED_GPIO
	ath_gpio_out_val(MOCA_AMBER_LED_GPIO, READY_LED_ON);
#endif
}
EXPORT_SYMBOL(ap_moca_led_amber_on);

void ap_moca_led_amber_off(void)
{
#ifdef MOCA_AMBER_LED_GPIO
	ath_gpio_out_val(MOCA_AMBER_LED_GPIO, READY_LED_OFF);
#endif
}
EXPORT_SYMBOL(ap_moca_led_amber_off);

static led_state_e simple_config_all_led_state = LED_STATE_NORMAL;
EXPORT_SYMBOL(simple_config_all_led_state);

static int gpio_simple_config_all_led_read (char *page, char **start, off_t off,
                               int count, int *eof, void *data)
{
	if(simple_config_all_led_state == LED_STATE_OFF)	
    	return sprintf (page, "off\n");
	else if(simple_config_all_led_state == LED_STATE_ON)	
    	return sprintf (page, "on\n");
	else 
    	return sprintf (page, "normal\n");

    return 0;
}

static int gpio_simple_config_all_led_write (struct file *file, const char *buf,
                                        unsigned long count, void *data)
{
	if (strncmp(buf,"on",2) == 0)
	{
		simple_config_all_led_state = LED_STATE_ON;
	}
	else if (strncmp(buf,"off",3) == 0)
	{
		simple_config_all_led_state = LED_STATE_OFF;
	}
	else if (strncmp(buf, "normal",6) == 0)
	{
		simple_config_all_led_state = LED_STATE_NORMAL;
	}

	if(simple_config_all_led_state == LED_STATE_OFF){
		ap_status_led_off();
		ap_power_led_off();
		ap_moca_led_green_off();
		ap_moca_led_amber_off();
		ap_wlan_5g_led_on_off(READY_LED_OFF);
		ap_wlan_2g_led_on_off(READY_LED_OFF);
		ath_gpio_out_val(LAN_10_100_LED_GPIO, POWER_LED_OFF);
		ath_gpio_out_val(LAN_1G_LED_GPIO, POWER_LED_OFF);
		ath_gpio_out_val(WPS_LED_GPIO, WPS_LED_OFF);
	}else if (simple_config_all_led_state == LED_STATE_ON){
		ap_status_led_on();
		ap_power_led_on();
		ap_moca_led_green_on();
		ap_moca_led_amber_on();
		ap_wlan_5g_led_on_off(READY_LED_ON);
		ap_wlan_2g_led_on_off(READY_LED_ON);
		ath_gpio_out_val(LAN_10_100_LED_GPIO, POWER_LED_ON);
		ath_gpio_out_val(LAN_1G_LED_GPIO, POWER_LED_ON);
		ath_gpio_out_val(WPS_LED_GPIO, WPS_LED_ON);
	}
	return count;
}

static int button_test_read (char *page, char **start, off_t off,
		int count, int *eof, void *data)
{
	return sprintf(page, "%d:%d:%d\n", reset_count, wps_count, wireless_count);
}

static int button_test_write (struct file *file, const char *buf,
		unsigned long count, void *data)
{
	if (strncmp(buf, "reset", 5) == 0) {
		reset_count = 0;
		wps_count = 0;
		wireless_count = 0;
	}

	return count;
}

static int create_simple_config_led_proc_entry(void)
{
	if (simple_config_entry != NULL) {
		printk("Already have a proc entry for /proc/simple_config!\n");
		return -ENOENT;
	}

	simple_config_entry = proc_mkdir("simple_config", NULL);
	if (!simple_config_entry)
		return -ENOENT;

	simulate_push_button_entry = create_proc_entry("push_button", 0644,
							simple_config_entry);
	if (!simulate_push_button_entry)
		return -ENOENT;

	simulate_push_button_entry->write_proc = push_button_write;
	simulate_push_button_entry->read_proc = push_button_read;

	simple_config_led_entry = create_proc_entry("simple_config_led", 0644,
							simple_config_entry);
	if (!simple_config_led_entry)
		return -ENOENT;

	simple_config_led_entry->write_proc = gpio_simple_config_led_write;
	simple_config_led_entry->read_proc = gpio_simple_config_led_read;

    simple_config_all_led_entry = create_proc_entry ("all_led", 0644,
                                            simple_config_entry);
    if (!simple_config_all_led_entry)
        return -ENOENT;

    simple_config_all_led_entry->write_proc = gpio_simple_config_all_led_write;
    simple_config_all_led_entry->read_proc = gpio_simple_config_all_led_read;

    button_test = create_proc_entry ("button_test", 0644,
                                            simple_config_entry);
    if (!button_test)
        return -ENOENT;

    button_test->write_proc = button_test_write;
    button_test->read_proc = button_test_read;

    button_state = create_proc_entry ("button_state", 0644,
                                            simple_config_entry);
    if (!button_state)
        return -ENOENT;

    button_state->write_proc = button_state_write;
    button_state->read_proc = button_state_read;

    disable_wps = create_proc_entry ("diswps", 0644,
                                            simple_config_entry);
    if (!disable_wps)
        return -ENOENT;

    disable_wps->write_proc = disable_wps_write;
    disable_wps->read_proc = disable_wps_read;

    ath_gpio_config_input(WPS_BUTTON_GPIO);
    ath_gpio_config_input(WLAN_BUTTON_GPIO);
    ath_gpio_config_input(RESET_BUTTON_GPIO);

	OS_INIT_TIMER(NULL, &os_button_timer_t, button_detect, &os_button_timer_t);
	OS_INIT_TIMER(NULL, &reset_button_timer_t, reset_button_led_action, &reset_button_timer_t);
    OS_INIT_TIMER(NULL, &os_diswps_t, diswps_led_blink, &os_diswps_t);
	OS_SET_TIMER(&os_button_timer_t, POLL_INTERVAL);
#ifdef POWER_ON_RLED_GPIO
	power_on_proc_entry = create_proc_entry("power_on_finish", 0644,
							simple_config_entry);
	if (!power_on_proc_entry)
		return -ENOENT;

	power_on_proc_entry->write_proc = power_on_finish_write;
	power_on_proc_entry->read_proc = power_on_finish_read;
#endif
	return 0;
}

static int
athfr_open(struct inode *inode, struct file *file)
{
	if (MINOR(inode->i_rdev) != FACTORY_RESET_MINOR) {
		return -ENODEV;
	}

	if (ath_fr_opened) {
		return -EBUSY;
	}

	ath_fr_opened = 1;
	return nonseekable_open(inode, file);
}

static int
athfr_close(struct inode *inode, struct file *file)
{
	if (MINOR(inode->i_rdev) != FACTORY_RESET_MINOR) {
		return -ENODEV;
	}

	ath_fr_opened = 0;
	return 0;
}

static ssize_t
athfr_read(struct file *file, char *buf, size_t count, loff_t * ppos)
{
	return -ENOTSUPP;
}

static ssize_t
athfr_write(struct file *file, const char *buf, size_t count, loff_t * ppos)
{
	return -ENOTSUPP;
}

static int
athfr_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
		unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
	case ATH_FACTORY_RESET:
#ifndef AP_RESET_GPIO
		atomic_inc(&ath_fr_status);
#endif
		sleep_on(&ath_fr_wq);
		break;

	default:
		ret = -EINVAL;
	}

	return ret;
}

static struct file_operations athfr_fops = {
	read:	athfr_read,
	write:	athfr_write,
	ioctl:	athfr_ioctl,
	open:	athfr_open,
	release:athfr_close
};

static struct miscdevice athfr_miscdev = {
	FACTORY_RESET_MINOR,
	"Factory reset",
	&athfr_fops
};

int __init ath_simple_config_init(void)
{
#ifdef CONFIG_CUS100
	u32 mask = 0;
#endif
	int req, ret;
#ifdef AP_RESET_GPIO
    int req2;
#endif
	ret = misc_register(&athfr_miscdev);

	if (ret < 0) {
		printk("*** ath misc_register failed %d *** \n", ret);
		return -1;
	}

#ifdef AP_RESET_GPIO
    ath_gpio_config_input(AP_RESET_GPIO);
    ath_gpio_config_int(AP_RESET_GPIO, INT_TYPE_LEVEL, INT_POL_ACTIVE_LOW);
    printk("%s (%s) AP_RESET_GPIO: %d\n", __FILE__, __func__, AP_RESET_GPIO);
#endif

#ifdef CONFIG_CUS100
	mask = ath_reg_rd(ATH_MISC_INT_MASK);
	ath_reg_wr(ATH_MISC_INT_MASK, mask | (1 << 2));
	ath_gpio_config_int(JUMPSTART_GPIO, INT_TYPE_LEVEL,
				INT_POL_ACTIVE_HIGH);
	ath_gpio_intr_enable(JUMPSTART_GPIO);
	ath_gpio_config_input(JUMPSTART_GPIO);
#else
	ath_gpio_config_input(JUMPSTART_GPIO);
	/* configure Jumpstart GPIO as level triggered interrupt */
	ath_gpio_config_int(JUMPSTART_GPIO, INT_TYPE_LEVEL,
				INT_POL_ACTIVE_LOW);
	printk("%s (%s) JUMPSTART_GPIO: %d\n", __FILE__, __func__,
		JUMPSTART_GPIO);
#ifndef CONFIG_MACH_AR934x
	ath_reg_rmw_clear(ATH_GPIO_FUNCTIONS, (1 << 2));
	ath_reg_rmw_clear(ATH_GPIO_FUNCTIONS, (1 << 16));
	ath_reg_rmw_clear(ATH_GPIO_FUNCTIONS, (1 << 20));
#endif
#endif

	req = request_irq(ATH_GPIO_IRQn(JUMPSTART_GPIO), jumpstart_irq, 0,
#ifdef AP_RESET_GPIO
			"SW JUMPSTART", NULL);
#else
			"SW JUMPSTART/FACTORY RESET", NULL);
#endif
	if (req != 0) {
		printk("request_irq for jumpstart failed (error %d)\n", req);
		misc_deregister(&athfr_miscdev);
		ath_gpio_intr_shutdown(ATH_GPIO_IRQn(JUMPSTART_GPIO));
		return -1;
	}
#ifdef AP_RESET_GPIO
    req2 = request_irq(ATH_GPIO_IRQn(AP_RESET_GPIO), ath_reset_irq, 0,
            "FACTORY RESET", NULL);
    if (req2 != 0) {
        printk("request_irq for factory reset failed (error %d)\n", req);
        misc_deregister(&athfr_miscdev);
        free_irq(req, NULL);
        return -1;
    }
#endif
#if !defined(CONFIG_I2S) && defined(AP_USB_LED_GPIO)
	ath_gpio_config_output(AP_USB_LED_GPIO);
#endif

#ifdef AP_READY_LED_GPIO
    ath_gpio_config_output(AP_READY_LED_GPIO);
    ap_ready_led_on();
#endif
	init_waitqueue_head(&ath_fr_wq);

	create_simple_config_led_proc_entry();

#ifdef POWER_ON_GLED_GPIO
	printk("%s (%s) POWER_ON_GLED_GPIO: %d\n", __FILE__, __func__, POWER_ON_GLED_GPIO);
    ath_gpio_config_output(POWER_ON_GLED_GPIO);
    ath_gpio_out_val(POWER_ON_GLED_GPIO, POWER_LED_ON);
#endif

#ifdef POWER_ON_RLED_GPIO
	printk("%s (%s) POWER_ON_RLED_GPIO: %d\n", __FILE__, __func__, POWER_ON_RLED_GPIO);
    ath_gpio_config_output(POWER_ON_RLED_GPIO);
    ath_gpio_out_val(POWER_ON_RLED_GPIO, POWER_LED_OFF);
    OS_INIT_TIMER(NULL, &power_on_timer, power_led_blink, NULL);
    OS_SET_TIMER(&power_on_timer, POWER_LED_BLINK_INTERVAL);
#endif
	/***** WM2500RP: Set GPIO FUNCTION *****/
	unsigned int rddata, setdata;
	/* Set GPIO 0~3 FUNCTION */
	ath_reg_wr(ATH_GPIO_OUT_FUNCTION0, 0x0);
	/* Disable JTAG FUNCTION */
	rddata = ath_reg_rd(ATH_GPIO_FUNCTIONS);
	setdata = rddata | 0x02;
	ath_reg_wr(ATH_GPIO_FUNCTIONS, setdata);
	/* Set GPIO 11 FUNCTION */
	rddata = ath_reg_rd(ATH_GPIO_OUT_FUNCTION2);
	setdata = rddata & 0x00ffffff;
	ath_reg_wr(ATH_GPIO_OUT_FUNCTION2, setdata);
	/* Set GPIO 12~15 FUNCTION */
	ath_reg_wr(ATH_GPIO_OUT_FUNCTION3, 0x0);
	/* Set GPIO 16~19 FUNCTION */
	ath_reg_wr(ATH_GPIO_OUT_FUNCTION4, 0x0);
	/* Set GPIO 20~23 FUNCTION */
	rddata = ath_reg_rd(ATH_GPIO_OUT_FUNCTION5);
	setdata = rddata & 0x00ffff00;
	ath_reg_wr(ATH_GPIO_OUT_FUNCTION5, setdata);
	/***** WM2500RP: Set Default GPIO Status *****/
	ath_gpio_config_output(WPS_LED_GPIO);
	ath_gpio_out_val(WPS_LED_GPIO, WPS_LED_OFF);
	ap_power_led_off();
	ath_gpio_config_output(POWER_LED_GPIO);
	ap_status_led_on();
	ath_gpio_config_output(STATUS_LED_GPIO);
	ap_moca_led_green_off();
	ath_gpio_config_output(MOCA_GREEN_LED_GPIO);
	ap_moca_led_amber_off();
	ath_gpio_config_output(MOCA_AMBER_LED_GPIO);
	ap_wlan_2g_led_on_off(READY_LED_OFF);
	ath_gpio_config_output(WLAN_2G_LED_GPIO);
	ap_wlan_5g_led_on_off(READY_LED_OFF);
	ath_gpio_config_output(WLAN_5G_LED_GPIO);
	ath_gpio_out_val(LAN_10_100_LED_GPIO, POWER_LED_OFF);
	ath_gpio_config_output(LAN_10_100_LED_GPIO);
	ath_gpio_out_val(LAN_1G_LED_GPIO, POWER_LED_OFF);
	ath_gpio_config_output(LAN_1G_LED_GPIO);

	return 0;
}

/*
 * used late_initcall so that misc_register will succeed
 * otherwise, misc driver won't be in a initializated state
 * thereby resulting in misc_register api to fail.
 */
#if !defined(CONFIG_ATH_EMULATION)
late_initcall(ath_simple_config_init);
#endif

