/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <asm/inca-ip.h>
#include <asm/mipsregs.h>

#if defined(CONFIG_AR7100)
#include <asm/addrspace.h>
#include <ar7100_soc.h>
#endif

#if defined(CONFIG_AR7240)
#include <asm/addrspace.h>
#if defined(CONFIG_HW29763847P16P64) || defined(CONFIG_HW29764037P8P64) || defined(CONFIG_WNDR3700V3) || defined(CONFIG_WNR1000V2) || defined(CONFIG_WNR1000V4) || defined(CONFIG_WNR1100) || defined(CONFIG_WNR2000V3) || defined(CONFIG_WNR2200)
#include <ar7240_soc.h>
#endif
#if defined(CONFIG_HW29763948P128P128)
#include <ar7240_soc_hw29763948p128p128.h>
#endif
#if defined(CONFIG_HW29763948P0P128P128P2X2P3X3)
#include <ar7240_soc_hw29763948p0p128p128p2x2p3x3.h>
#endif
#if defined(CONFIG_HW29764434P0P128P128P2X2P1X1)
#include <ar7240_soc_hw29764434p0p128p128p2x2p1x1.h>
#endif
#if defined(CONFIG_HW29764434P0P128P128P2X2P2X2)
#include <ar7240_soc_hw29764434p0p128p128p2x2p2x2.h>
#endif
#if defined(CONFIG_HW29763904P4P32)
#include <ar7240_soc_hw29763904p4p32.h>
#endif
#if defined(CONFIG_HW29763906P8P0P64P2x2P0)
#include <ar7240_soc_hw29763906p8p0p64p2x2p0.h>
#endif
#endif

#include <asm/addrspace.h>
#include <atheros_hw29764428p8p0p128p2x2p2x2.h>


int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	while (1) {
		ath_reg_wr(RST_RESET_ADDRESS, RST_RESET_FULL_CHIP_RESET_SET(1));
	}
#ifndef COMPRESSED_UBOOT
	fprintf(stderr, "*** reset failed ***\n");
#endif  /* #ifndef COMPRESSED_UBOOT */
	return 0;
}

void flush_cache (ulong start_addr, ulong size)
{
	u32 end, a;
    int i;

    a = start_addr & ~(CFG_CACHELINE_SIZE - 1);
    size = (size + CFG_CACHELINE_SIZE - 1) & ~(CFG_CACHELINE_SIZE - 1);
    end = a + size;

    dcache_flush_range(a, end);
}

void write_one_tlb( int index, u32 pagemask, u32 hi, u32 low0, u32 low1 ){
	write_32bit_cp0_register(CP0_ENTRYLO0, low0);
	write_32bit_cp0_register(CP0_PAGEMASK, pagemask);
	write_32bit_cp0_register(CP0_ENTRYLO1, low1);
	write_32bit_cp0_register(CP0_ENTRYHI, hi);
	write_32bit_cp0_register(CP0_INDEX, index);
	tlb_write_indexed();
}
