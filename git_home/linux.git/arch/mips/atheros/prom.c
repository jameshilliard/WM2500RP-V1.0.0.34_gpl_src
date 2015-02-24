#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/bootmem.h>

#include <asm/bootinfo.h>
#include <asm/addrspace.h>

#include <atheros.h>

int __ath_flash_size;

#ifdef CONFIG_IMAGE_CMDLINE_HACK
extern char __image_cmdline[];

static int __init DNI_use__image_cmdline(void)
{
	char *p = __image_cmdline;
	int replace = 0;

	if (*p == '-') {
		replace = 1;
		p++;
	}

	if (*p == '\0')
		return 0;

	if (replace) {
		strlcpy(arcs_cmdline, p, sizeof(arcs_cmdline));
	} else {
		strlcat(arcs_cmdline, " ", sizeof(arcs_cmdline));
		strlcat(arcs_cmdline, p, sizeof(arcs_cmdline));
	}

	return 1;
}
#else
static int inline DNI_use__image_cmdline(void) { return 0; }
#endif

void __init prom_init(void)
{
	int memsz = 0x8000000, argc = 0, i;
	char **arg = (char **)fw_arg1;

	printk("flash_size passed from bootloader = %d\n", (int)fw_arg3);
	__ath_flash_size = fw_arg3;

	/*
	 * if user passes kernel args, ignore the default one
	 */
	if (argc > 1) {
		arcs_cmdline[0] = '\0';

		for (i = 1; i < argc; i++)
			printk("arg %d: %s\n", i, arg[i]);

		/*
		 * arg[0] is "g", the rest is boot parameters
		 */
		for (i = 1; i < argc; i++) {
			if (strlen(arcs_cmdline) + strlen(arg[i] + 1)
			    >= sizeof(arcs_cmdline))
				break;
			strcat(arcs_cmdline, arg[i]);
			strcat(arcs_cmdline, " ");
		}
	}

	DNI_use__image_cmdline();

	// mips_machgroup = MACH_GROUP_ATHEROS;

	mips_machtype = CONFIG_ATH_MACH_TYPE;

	/*
	 * By default, use all available memory. You can override this
	 * to use, say, 8MB by specifying "mem=8M" as an argument on the
	 * linux bootup command line.
	 */
	add_memory_region(0, memsz, BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
}
