#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/cpumask.h>
#include <linux/delay.h>
#include <linux/irq.h>

#include <asm/delay.h>
#include <asm/irq.h>

#include <atheros.h>
/*
 * Support for Atheros pci interrupt and core pci initialization
 */
/*
 * PCI interrupts.
 * roughly the interrupts flow is:
 *
 * - save flags
 * - CLI (disable all)
 * - IC->ack (mask out the source)
 * - EI (enable all, except the source that was masked of course)
 * - action (ISR)
 * - IC->enable (unmask the source)
 *
 * The reason we have a separate PCI IC is beacause of the following:
 * If we dont, then Throughout the "action" of a PCI slot, the
 * entire PCI "IP" on the cpu will remain disabled. Which means that we cant
 * prioritize between PCI interrupts. Normally this should be ok, if all PCI
 * interrupts are considered equal. However, creating a PCI IC gives
 * the flexibility to prioritize.
 */

static void
ath_pci_irq_enable(unsigned int irq)
{
	ATH_DECL_PCI_IM_ARR(r);

	ath_reg_rmw_set(r[irq - ATH_PCI_IRQ_BASE], PCIE_INT_MASK_INTAL_MASK);
}

static void
ath_pci_irq_disable(unsigned int irq)
{
	ATH_DECL_PCI_IM_ARR(r1);
	ATH_DECL_PCI_IS_ARR(r2);

	ath_reg_rmw_clear(r1[irq - ATH_PCI_IRQ_BASE], PCIE_INT_MASK_INTAL_MASK);
	ath_reg_rmw_clear(r2[irq - ATH_PCI_IRQ_BASE], PCIE_INT_STATUS_INTAL_MASK);
}

static unsigned int
ath_pci_irq_startup(unsigned int irq)
{
	ath_pci_irq_enable(irq);
	return 0;
}

static void
ath_pci_irq_shutdown(unsigned int irq)
{
	ath_pci_irq_disable(irq);
}

static void
ath_pci_irq_ack(unsigned int irq)
{
	ath_pci_irq_disable(irq);
}

static void
ath_pci_irq_end(unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED | IRQ_INPROGRESS)))
		ath_pci_irq_enable(irq);
}

static int
ath_pci_irq_set_affinity(unsigned int irq, const struct cpumask *mask)
{
	/*
	 * Only 1 CPU; ignore affinity request
	 */
	return 0;
}

struct irq_chip /* hw_interrupt_type */ ath_pci_irq_controller = {
	.name		= "ATH PCI",
	.startup	= ath_pci_irq_startup,
	.shutdown	= ath_pci_irq_shutdown,
	.enable		= ath_pci_irq_enable,
	.disable	= ath_pci_irq_disable,
	.ack		= ath_pci_irq_ack,
	.end		= ath_pci_irq_end,
	.eoi		= ath_pci_irq_end,
	.set_affinity	= ath_pci_irq_set_affinity,
};

void
ath_pci_irq_init(int irq_base)
{
	int i;

	for (i = irq_base; i < irq_base + ATH_PCI_IRQ_COUNT; i++) {
		irq_desc[i].status = IRQ_DISABLED;
		irq_desc[i].action = NULL;
		irq_desc[i].depth = 1;
		//irq_desc[i].chip = &ath_pci_irq_controller;
		set_irq_chip_and_handler(i, &ath_pci_irq_controller,
					 handle_percpu_irq);
	}
}

#ifndef CONFIG_ATH_EMULATION
/*
 * init the pci controller
 */

#define ATH_PCI_RES(x, n, s, e, f)			\
	{ .name = n, .start = s, .end = e, .flags = f }

#define ATH_PCI_RES_IO(x, s, e)				\
	ATH_PCI_RES(x, "PCI I/O - " # x, s, e, IORESOURCE_IO)

#define ATH_PCI_RES_MEM(x)				\
	ATH_PCI_RES(x, "PCI MEM - " # x,		\
	ATH_PCI_MEM_BASE(x),				\
	ATH_PCI_MEM_BASE(x) + ATH_PCI_WINDOW - 1,	\
	IORESOURCE_MEM)

/*
 * We don't use I/O transaction ability.
 */
static struct resource ath_io_resource[] = {
	ATH_DECL_PCI_IO_RES
};

static struct resource ath_mem_resource[] = {
	ATH_DECL_PCI_MEM_RES
};


extern struct pci_ops ath_pci_ops;

#define ATH_PCI_CTRL_DESCRIPTOR(x)			\
	{						\
		.pci_ops	= &ath_pci_ops,		\
		.mem_resource	= &ath_mem_resource[x],	\
		.io_resource	= &ath_io_resource[x],	\
	}

static struct pci_controller ath_pci_controller[] = {
	ATH_DECL_PCI_CTRLR
};

#define ATH_NUM_PCI_CONTROLLER	\
	(sizeof(ath_pci_controller) / sizeof(ath_pci_controller[0]))

int ath_pci_link[ATH_NUM_PCI_CONTROLLER];

/*
 * We want a 1:1 mapping between PCI and DDR for inbound and outbound.
 * The PCI<---AHB decoding works as follows:
 *
 * 8 registers in the DDR unit provide software configurable 32 bit offsets
 * for each of the eight 16MB PCI windows in the 128MB. The offsets will be
 * added to any address in the 16MB segment before being sent to the PCI unit.
 *
 * Essentially  for any AHB address generated by the CPU,
 * 1. the MSB  four bits are stripped off, [31:28],
 * 2. Bit 27 is used to decide between the lower 128Mb (PCI) or the rest of
 *    the AHB space
 * 3. Bits 26:24 are used to access one of the 8 window registers and are
 *    masked off.
 * 4. If it is a PCI address, then the WINDOW offset in the WINDOW register
 *    corresponding to the next 3 bits (bit 26:24) is ADDED to the address,
 *    to generate the address to PCI unit.
 *
 *     eg. CPU address = 0x100000ff
 *         window 0 offset = 0x10000000
 *         This points to lowermost 16MB window in PCI space.
 *         So the resulting address would be 0x000000ff+0x10000000
 *         = 0x100000ff
 *
 *         eg2. CPU address = 0x120000ff
 *         WINDOW 2 offset = 0x12000000
 *         resulting address would be 0x000000ff+0x12000000
 *                         = 0x120000ff
 *
 * There is no translation for inbound access (PCI device as a master)
 */

static int __init ath_pcibios_init(void)
{
	uint32_t i;
	ATH_DECL_PCI_RST_ARR(pcie_reset);

#if ATH_LOW_POWER_ENABLE
	ATH_DECL_PCI_ASPM_SUPP_ARR(aspm_sup);
	ATH_DECL_PCI_ASPM_EN_ARR(aspm_en);
#endif

#ifdef CONFIG_MACH_AR934x
	if (is_ar9341()) {
		return 0;
	}
#endif

	for (i = 0; i < ATH_NUM_PCI_CONTROLLER; i++) {
		printk("%s: bus %d\n", __func__, i);
		/*
		 * Check if the WLAN PCI-E H/W is present, If the
		 * WLAN H/W is not present, skip the PCI
		 * initialization code and just return.
		 */
		if (!(ath_reg_rd(pcie_reset[i]) & PCIE_RESET_LINK_UP_MASK)) {
			printk("***** Warning PCIe %d H/W not found !!!\n", i);
		} else {
			ath_pci_link[i] = 1;

#ifndef CONFIG_PCI_INIT_IN_MONITOR

#define ATH_PCI_CMD_INIT	(PCI_COMMAND_MEMORY |		\
				 PCI_COMMAND_MASTER |		\
				 PCI_COMMAND_INVALIDATE |	\
				 PCI_COMMAND_PARITY |		\
				 PCI_COMMAND_SERR |		\
				 PCI_COMMAND_FAST_BACK)

			printk("%s(%d): PCI %d CMD write: 0x%x\n",
				__func__, __LINE__, i, ATH_PCI_CMD_INIT);

			ath_local_write_config(i, PCI_COMMAND, 4, ATH_PCI_CMD_INIT);
#endif

#if ATH_LOW_POWER_ENABLE
			// Enable L0 & L1 ASPM Support
			ath_reg_rmw_set(aspm_sup[i], ATH_PCIE_RC_SUPP_VAL);
			// Enable L0 & L1
			ath_reg_rmw_set(aspm_en[i], ATH_PCIE_RC_EN_VAL);
#endif
		}
		register_pci_controller(&ath_pci_controller[i]);
	}
	return 0;
}

arch_initcall(ath_pcibios_init);
#endif
