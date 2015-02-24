#ifndef __eth_defs_asm_h
#define __eth_defs_asm_h

/*
 * This file is autogenerated from
 *   file:           ../../inst/eth/rtl/eth_regs.r
 *     id:           eth_regs.r,v 1.11 2005/02/09 10:48:38 kriskn Exp
 *     last modfied: Mon Apr 11 16:07:03 2005
 *
 *   by /n/asic/design/tools/rdesc/src/rdes2c -asm --outfile asm/eth_defs_asm.h ../../inst/eth/rtl/eth_regs.r
 *      id: $Id: //depot/sw/releases/9.5.3/linux/kernels/mips-linux-2.6.31/arch/cris/include/arch-v32/arch/hwregs/asm/eth_defs_asm.h#1 $
 * Any changes here will be lost.
 *
 * -*- buffer-read-only: t -*-
 */

#ifndef REG_FIELD
#define REG_FIELD( scope, reg, field, value ) \
  REG_FIELD_X_( value, reg_##scope##_##reg##___##field##___lsb )
#define REG_FIELD_X_( value, shift ) ((value) << shift)
#endif

#ifndef REG_STATE
#define REG_STATE( scope, reg, field, symbolic_value ) \
  REG_STATE_X_( regk_##scope##_##symbolic_value, reg_##scope##_##reg##___##field##___lsb )
#define REG_STATE_X_( k, shift ) (k << shift)
#endif

#ifndef REG_MASK
#define REG_MASK( scope, reg, field ) \
  REG_MASK_X_( reg_##scope##_##reg##___##field##___width, reg_##scope##_##reg##___##field##___lsb )
#define REG_MASK_X_( width, lsb ) (((1 << width)-1) << lsb)
#endif

#ifndef REG_LSB
#define REG_LSB( scope, reg, field ) reg_##scope##_##reg##___##field##___lsb
#endif

#ifndef REG_BIT
#define REG_BIT( scope, reg, field ) reg_##scope##_##reg##___##field##___bit
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) REG_ADDR_X_(inst, reg_##scope##_##reg##_offset)
#define REG_ADDR_X_( inst, offs ) ((inst) + offs)
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
         REG_ADDR_VECT_X_(inst, reg_##scope##_##reg##_offset, index, \
			 STRIDE_##scope##_##reg )
#define REG_ADDR_VECT_X_( inst, offs, index, stride ) \
                          ((inst) + offs + (index) * stride)
#endif

/* Register rw_ma0_lo, scope eth, type rw */
#define reg_eth_rw_ma0_lo___addr___lsb 0
#define reg_eth_rw_ma0_lo___addr___width 32
#define reg_eth_rw_ma0_lo_offset 0

/* Register rw_ma0_hi, scope eth, type rw */
#define reg_eth_rw_ma0_hi___addr___lsb 0
#define reg_eth_rw_ma0_hi___addr___width 16
#define reg_eth_rw_ma0_hi_offset 4

/* Register rw_ma1_lo, scope eth, type rw */
#define reg_eth_rw_ma1_lo___addr___lsb 0
#define reg_eth_rw_ma1_lo___addr___width 32
#define reg_eth_rw_ma1_lo_offset 8

/* Register rw_ma1_hi, scope eth, type rw */
#define reg_eth_rw_ma1_hi___addr___lsb 0
#define reg_eth_rw_ma1_hi___addr___width 16
#define reg_eth_rw_ma1_hi_offset 12

/* Register rw_ga_lo, scope eth, type rw */
#define reg_eth_rw_ga_lo___table___lsb 0
#define reg_eth_rw_ga_lo___table___width 32
#define reg_eth_rw_ga_lo_offset 16

/* Register rw_ga_hi, scope eth, type rw */
#define reg_eth_rw_ga_hi___table___lsb 0
#define reg_eth_rw_ga_hi___table___width 32
#define reg_eth_rw_ga_hi_offset 20

/* Register rw_gen_ctrl, scope eth, type rw */
#define reg_eth_rw_gen_ctrl___en___lsb 0
#define reg_eth_rw_gen_ctrl___en___width 1
#define reg_eth_rw_gen_ctrl___en___bit 0
#define reg_eth_rw_gen_ctrl___phy___lsb 1
#define reg_eth_rw_gen_ctrl___phy___width 2
#define reg_eth_rw_gen_ctrl___protocol___lsb 3
#define reg_eth_rw_gen_ctrl___protocol___width 1
#define reg_eth_rw_gen_ctrl___protocol___bit 3
#define reg_eth_rw_gen_ctrl___loopback___lsb 4
#define reg_eth_rw_gen_ctrl___loopback___width 1
#define reg_eth_rw_gen_ctrl___loopback___bit 4
#define reg_eth_rw_gen_ctrl___flow_ctrl_dis___lsb 5
#define reg_eth_rw_gen_ctrl___flow_ctrl_dis___width 1
#define reg_eth_rw_gen_ctrl___flow_ctrl_dis___bit 5
#define reg_eth_rw_gen_ctrl_offset 24

/* Register rw_rec_ctrl, scope eth, type rw */
#define reg_eth_rw_rec_ctrl___ma0___lsb 0
#define reg_eth_rw_rec_ctrl___ma0___width 1
#define reg_eth_rw_rec_ctrl___ma0___bit 0
#define reg_eth_rw_rec_ctrl___ma1___lsb 1
#define reg_eth_rw_rec_ctrl___ma1___width 1
#define reg_eth_rw_rec_ctrl___ma1___bit 1
#define reg_eth_rw_rec_ctrl___individual___lsb 2
#define reg_eth_rw_rec_ctrl___individual___width 1
#define reg_eth_rw_rec_ctrl___individual___bit 2
#define reg_eth_rw_rec_ctrl___broadcast___lsb 3
#define reg_eth_rw_rec_ctrl___broadcast___width 1
#define reg_eth_rw_rec_ctrl___broadcast___bit 3
#define reg_eth_rw_rec_ctrl___undersize___lsb 4
#define reg_eth_rw_rec_ctrl___undersize___width 1
#define reg_eth_rw_rec_ctrl___undersize___bit 4
#define reg_eth_rw_rec_ctrl___oversize___lsb 5
#define reg_eth_rw_rec_ctrl___oversize___width 1
#define reg_eth_rw_rec_ctrl___oversize___bit 5
#define reg_eth_rw_rec_ctrl___bad_crc___lsb 6
#define reg_eth_rw_rec_ctrl___bad_crc___width 1
#define reg_eth_rw_rec_ctrl___bad_crc___bit 6
#define reg_eth_rw_rec_ctrl___duplex___lsb 7
#define reg_eth_rw_rec_ctrl___duplex___width 1
#define reg_eth_rw_rec_ctrl___duplex___bit 7
#define reg_eth_rw_rec_ctrl___max_size___lsb 8
#define reg_eth_rw_rec_ctrl___max_size___width 1
#define reg_eth_rw_rec_ctrl___max_size___bit 8
#define reg_eth_rw_rec_ctrl_offset 28

/* Register rw_tr_ctrl, scope eth, type rw */
#define reg_eth_rw_tr_ctrl___crc___lsb 0
#define reg_eth_rw_tr_ctrl___crc___width 1
#define reg_eth_rw_tr_ctrl___crc___bit 0
#define reg_eth_rw_tr_ctrl___pad___lsb 1
#define reg_eth_rw_tr_ctrl___pad___width 1
#define reg_eth_rw_tr_ctrl___pad___bit 1
#define reg_eth_rw_tr_ctrl___retry___lsb 2
#define reg_eth_rw_tr_ctrl___retry___width 1
#define reg_eth_rw_tr_ctrl___retry___bit 2
#define reg_eth_rw_tr_ctrl___ignore_col___lsb 3
#define reg_eth_rw_tr_ctrl___ignore_col___width 1
#define reg_eth_rw_tr_ctrl___ignore_col___bit 3
#define reg_eth_rw_tr_ctrl___cancel___lsb 4
#define reg_eth_rw_tr_ctrl___cancel___width 1
#define reg_eth_rw_tr_ctrl___cancel___bit 4
#define reg_eth_rw_tr_ctrl___hsh_delay___lsb 5
#define reg_eth_rw_tr_ctrl___hsh_delay___width 1
#define reg_eth_rw_tr_ctrl___hsh_delay___bit 5
#define reg_eth_rw_tr_ctrl___ignore_crs___lsb 6
#define reg_eth_rw_tr_ctrl___ignore_crs___width 1
#define reg_eth_rw_tr_ctrl___ignore_crs___bit 6
#define reg_eth_rw_tr_ctrl_offset 32

/* Register rw_clr_err, scope eth, type rw */
#define reg_eth_rw_clr_err___clr___lsb 0
#define reg_eth_rw_clr_err___clr___width 1
#define reg_eth_rw_clr_err___clr___bit 0
#define reg_eth_rw_clr_err_offset 36

/* Register rw_mgm_ctrl, scope eth, type rw */
#define reg_eth_rw_mgm_ctrl___mdio___lsb 0
#define reg_eth_rw_mgm_ctrl___mdio___width 1
#define reg_eth_rw_mgm_ctrl___mdio___bit 0
#define reg_eth_rw_mgm_ctrl___mdoe___lsb 1
#define reg_eth_rw_mgm_ctrl___mdoe___width 1
#define reg_eth_rw_mgm_ctrl___mdoe___bit 1
#define reg_eth_rw_mgm_ctrl___mdc___lsb 2
#define reg_eth_rw_mgm_ctrl___mdc___width 1
#define reg_eth_rw_mgm_ctrl___mdc___bit 2
#define reg_eth_rw_mgm_ctrl___phyclk___lsb 3
#define reg_eth_rw_mgm_ctrl___phyclk___width 1
#define reg_eth_rw_mgm_ctrl___phyclk___bit 3
#define reg_eth_rw_mgm_ctrl___txdata___lsb 4
#define reg_eth_rw_mgm_ctrl___txdata___width 4
#define reg_eth_rw_mgm_ctrl___txen___lsb 8
#define reg_eth_rw_mgm_ctrl___txen___width 1
#define reg_eth_rw_mgm_ctrl___txen___bit 8
#define reg_eth_rw_mgm_ctrl_offset 40

/* Register r_stat, scope eth, type r */
#define reg_eth_r_stat___mdio___lsb 0
#define reg_eth_r_stat___mdio___width 1
#define reg_eth_r_stat___mdio___bit 0
#define reg_eth_r_stat___exc_col___lsb 1
#define reg_eth_r_stat___exc_col___width 1
#define reg_eth_r_stat___exc_col___bit 1
#define reg_eth_r_stat___urun___lsb 2
#define reg_eth_r_stat___urun___width 1
#define reg_eth_r_stat___urun___bit 2
#define reg_eth_r_stat___phyclk___lsb 3
#define reg_eth_r_stat___phyclk___width 1
#define reg_eth_r_stat___phyclk___bit 3
#define reg_eth_r_stat___txdata___lsb 4
#define reg_eth_r_stat___txdata___width 4
#define reg_eth_r_stat___txen___lsb 8
#define reg_eth_r_stat___txen___width 1
#define reg_eth_r_stat___txen___bit 8
#define reg_eth_r_stat___col___lsb 9
#define reg_eth_r_stat___col___width 1
#define reg_eth_r_stat___col___bit 9
#define reg_eth_r_stat___crs___lsb 10
#define reg_eth_r_stat___crs___width 1
#define reg_eth_r_stat___crs___bit 10
#define reg_eth_r_stat___txclk___lsb 11
#define reg_eth_r_stat___txclk___width 1
#define reg_eth_r_stat___txclk___bit 11
#define reg_eth_r_stat___rxdata___lsb 12
#define reg_eth_r_stat___rxdata___width 4
#define reg_eth_r_stat___rxer___lsb 16
#define reg_eth_r_stat___rxer___width 1
#define reg_eth_r_stat___rxer___bit 16
#define reg_eth_r_stat___rxdv___lsb 17
#define reg_eth_r_stat___rxdv___width 1
#define reg_eth_r_stat___rxdv___bit 17
#define reg_eth_r_stat___rxclk___lsb 18
#define reg_eth_r_stat___rxclk___width 1
#define reg_eth_r_stat___rxclk___bit 18
#define reg_eth_r_stat_offset 44

/* Register rs_rec_cnt, scope eth, type rs */
#define reg_eth_rs_rec_cnt___crc_err___lsb 0
#define reg_eth_rs_rec_cnt___crc_err___width 8
#define reg_eth_rs_rec_cnt___align_err___lsb 8
#define reg_eth_rs_rec_cnt___align_err___width 8
#define reg_eth_rs_rec_cnt___oversize___lsb 16
#define reg_eth_rs_rec_cnt___oversize___width 8
#define reg_eth_rs_rec_cnt___congestion___lsb 24
#define reg_eth_rs_rec_cnt___congestion___width 8
#define reg_eth_rs_rec_cnt_offset 48

/* Register r_rec_cnt, scope eth, type r */
#define reg_eth_r_rec_cnt___crc_err___lsb 0
#define reg_eth_r_rec_cnt___crc_err___width 8
#define reg_eth_r_rec_cnt___align_err___lsb 8
#define reg_eth_r_rec_cnt___align_err___width 8
#define reg_eth_r_rec_cnt___oversize___lsb 16
#define reg_eth_r_rec_cnt___oversize___width 8
#define reg_eth_r_rec_cnt___congestion___lsb 24
#define reg_eth_r_rec_cnt___congestion___width 8
#define reg_eth_r_rec_cnt_offset 52

/* Register rs_tr_cnt, scope eth, type rs */
#define reg_eth_rs_tr_cnt___single_col___lsb 0
#define reg_eth_rs_tr_cnt___single_col___width 8
#define reg_eth_rs_tr_cnt___mult_col___lsb 8
#define reg_eth_rs_tr_cnt___mult_col___width 8
#define reg_eth_rs_tr_cnt___late_col___lsb 16
#define reg_eth_rs_tr_cnt___late_col___width 8
#define reg_eth_rs_tr_cnt___deferred___lsb 24
#define reg_eth_rs_tr_cnt___deferred___width 8
#define reg_eth_rs_tr_cnt_offset 56

/* Register r_tr_cnt, scope eth, type r */
#define reg_eth_r_tr_cnt___single_col___lsb 0
#define reg_eth_r_tr_cnt___single_col___width 8
#define reg_eth_r_tr_cnt___mult_col___lsb 8
#define reg_eth_r_tr_cnt___mult_col___width 8
#define reg_eth_r_tr_cnt___late_col___lsb 16
#define reg_eth_r_tr_cnt___late_col___width 8
#define reg_eth_r_tr_cnt___deferred___lsb 24
#define reg_eth_r_tr_cnt___deferred___width 8
#define reg_eth_r_tr_cnt_offset 60

/* Register rs_phy_cnt, scope eth, type rs */
#define reg_eth_rs_phy_cnt___carrier_loss___lsb 0
#define reg_eth_rs_phy_cnt___carrier_loss___width 8
#define reg_eth_rs_phy_cnt___sqe_err___lsb 8
#define reg_eth_rs_phy_cnt___sqe_err___width 8
#define reg_eth_rs_phy_cnt_offset 64

/* Register r_phy_cnt, scope eth, type r */
#define reg_eth_r_phy_cnt___carrier_loss___lsb 0
#define reg_eth_r_phy_cnt___carrier_loss___width 8
#define reg_eth_r_phy_cnt___sqe_err___lsb 8
#define reg_eth_r_phy_cnt___sqe_err___width 8
#define reg_eth_r_phy_cnt_offset 68

/* Register rw_test_ctrl, scope eth, type rw */
#define reg_eth_rw_test_ctrl___snmp_inc___lsb 0
#define reg_eth_rw_test_ctrl___snmp_inc___width 1
#define reg_eth_rw_test_ctrl___snmp_inc___bit 0
#define reg_eth_rw_test_ctrl___snmp___lsb 1
#define reg_eth_rw_test_ctrl___snmp___width 1
#define reg_eth_rw_test_ctrl___snmp___bit 1
#define reg_eth_rw_test_ctrl___backoff___lsb 2
#define reg_eth_rw_test_ctrl___backoff___width 1
#define reg_eth_rw_test_ctrl___backoff___bit 2
#define reg_eth_rw_test_ctrl_offset 72

/* Register rw_intr_mask, scope eth, type rw */
#define reg_eth_rw_intr_mask___crc___lsb 0
#define reg_eth_rw_intr_mask___crc___width 1
#define reg_eth_rw_intr_mask___crc___bit 0
#define reg_eth_rw_intr_mask___align___lsb 1
#define reg_eth_rw_intr_mask___align___width 1
#define reg_eth_rw_intr_mask___align___bit 1
#define reg_eth_rw_intr_mask___oversize___lsb 2
#define reg_eth_rw_intr_mask___oversize___width 1
#define reg_eth_rw_intr_mask___oversize___bit 2
#define reg_eth_rw_intr_mask___congestion___lsb 3
#define reg_eth_rw_intr_mask___congestion___width 1
#define reg_eth_rw_intr_mask___congestion___bit 3
#define reg_eth_rw_intr_mask___single_col___lsb 4
#define reg_eth_rw_intr_mask___single_col___width 1
#define reg_eth_rw_intr_mask___single_col___bit 4
#define reg_eth_rw_intr_mask___mult_col___lsb 5
#define reg_eth_rw_intr_mask___mult_col___width 1
#define reg_eth_rw_intr_mask___mult_col___bit 5
#define reg_eth_rw_intr_mask___late_col___lsb 6
#define reg_eth_rw_intr_mask___late_col___width 1
#define reg_eth_rw_intr_mask___late_col___bit 6
#define reg_eth_rw_intr_mask___deferred___lsb 7
#define reg_eth_rw_intr_mask___deferred___width 1
#define reg_eth_rw_intr_mask___deferred___bit 7
#define reg_eth_rw_intr_mask___carrier_loss___lsb 8
#define reg_eth_rw_intr_mask___carrier_loss___width 1
#define reg_eth_rw_intr_mask___carrier_loss___bit 8
#define reg_eth_rw_intr_mask___sqe_test_err___lsb 9
#define reg_eth_rw_intr_mask___sqe_test_err___width 1
#define reg_eth_rw_intr_mask___sqe_test_err___bit 9
#define reg_eth_rw_intr_mask___orun___lsb 10
#define reg_eth_rw_intr_mask___orun___width 1
#define reg_eth_rw_intr_mask___orun___bit 10
#define reg_eth_rw_intr_mask___urun___lsb 11
#define reg_eth_rw_intr_mask___urun___width 1
#define reg_eth_rw_intr_mask___urun___bit 11
#define reg_eth_rw_intr_mask___excessive_col___lsb 12
#define reg_eth_rw_intr_mask___excessive_col___width 1
#define reg_eth_rw_intr_mask___excessive_col___bit 12
#define reg_eth_rw_intr_mask___mdio___lsb 13
#define reg_eth_rw_intr_mask___mdio___width 1
#define reg_eth_rw_intr_mask___mdio___bit 13
#define reg_eth_rw_intr_mask_offset 76

/* Register rw_ack_intr, scope eth, type rw */
#define reg_eth_rw_ack_intr___crc___lsb 0
#define reg_eth_rw_ack_intr___crc___width 1
#define reg_eth_rw_ack_intr___crc___bit 0
#define reg_eth_rw_ack_intr___align___lsb 1
#define reg_eth_rw_ack_intr___align___width 1
#define reg_eth_rw_ack_intr___align___bit 1
#define reg_eth_rw_ack_intr___oversize___lsb 2
#define reg_eth_rw_ack_intr___oversize___width 1
#define reg_eth_rw_ack_intr___oversize___bit 2
#define reg_eth_rw_ack_intr___congestion___lsb 3
#define reg_eth_rw_ack_intr___congestion___width 1
#define reg_eth_rw_ack_intr___congestion___bit 3
#define reg_eth_rw_ack_intr___single_col___lsb 4
#define reg_eth_rw_ack_intr___single_col___width 1
#define reg_eth_rw_ack_intr___single_col___bit 4
#define reg_eth_rw_ack_intr___mult_col___lsb 5
#define reg_eth_rw_ack_intr___mult_col___width 1
#define reg_eth_rw_ack_intr___mult_col___bit 5
#define reg_eth_rw_ack_intr___late_col___lsb 6
#define reg_eth_rw_ack_intr___late_col___width 1
#define reg_eth_rw_ack_intr___late_col___bit 6
#define reg_eth_rw_ack_intr___deferred___lsb 7
#define reg_eth_rw_ack_intr___deferred___width 1
#define reg_eth_rw_ack_intr___deferred___bit 7
#define reg_eth_rw_ack_intr___carrier_loss___lsb 8
#define reg_eth_rw_ack_intr___carrier_loss___width 1
#define reg_eth_rw_ack_intr___carrier_loss___bit 8
#define reg_eth_rw_ack_intr___sqe_test_err___lsb 9
#define reg_eth_rw_ack_intr___sqe_test_err___width 1
#define reg_eth_rw_ack_intr___sqe_test_err___bit 9
#define reg_eth_rw_ack_intr___orun___lsb 10
#define reg_eth_rw_ack_intr___orun___width 1
#define reg_eth_rw_ack_intr___orun___bit 10
#define reg_eth_rw_ack_intr___urun___lsb 11
#define reg_eth_rw_ack_intr___urun___width 1
#define reg_eth_rw_ack_intr___urun___bit 11
#define reg_eth_rw_ack_intr___excessive_col___lsb 12
#define reg_eth_rw_ack_intr___excessive_col___width 1
#define reg_eth_rw_ack_intr___excessive_col___bit 12
#define reg_eth_rw_ack_intr___mdio___lsb 13
#define reg_eth_rw_ack_intr___mdio___width 1
#define reg_eth_rw_ack_intr___mdio___bit 13
#define reg_eth_rw_ack_intr_offset 80

/* Register r_intr, scope eth, type r */
#define reg_eth_r_intr___crc___lsb 0
#define reg_eth_r_intr___crc___width 1
#define reg_eth_r_intr___crc___bit 0
#define reg_eth_r_intr___align___lsb 1
#define reg_eth_r_intr___align___width 1
#define reg_eth_r_intr___align___bit 1
#define reg_eth_r_intr___oversize___lsb 2
#define reg_eth_r_intr___oversize___width 1
#define reg_eth_r_intr___oversize___bit 2
#define reg_eth_r_intr___congestion___lsb 3
#define reg_eth_r_intr___congestion___width 1
#define reg_eth_r_intr___congestion___bit 3
#define reg_eth_r_intr___single_col___lsb 4
#define reg_eth_r_intr___single_col___width 1
#define reg_eth_r_intr___single_col___bit 4
#define reg_eth_r_intr___mult_col___lsb 5
#define reg_eth_r_intr___mult_col___width 1
#define reg_eth_r_intr___mult_col___bit 5
#define reg_eth_r_intr___late_col___lsb 6
#define reg_eth_r_intr___late_col___width 1
#define reg_eth_r_intr___late_col___bit 6
#define reg_eth_r_intr___deferred___lsb 7
#define reg_eth_r_intr___deferred___width 1
#define reg_eth_r_intr___deferred___bit 7
#define reg_eth_r_intr___carrier_loss___lsb 8
#define reg_eth_r_intr___carrier_loss___width 1
#define reg_eth_r_intr___carrier_loss___bit 8
#define reg_eth_r_intr___sqe_test_err___lsb 9
#define reg_eth_r_intr___sqe_test_err___width 1
#define reg_eth_r_intr___sqe_test_err___bit 9
#define reg_eth_r_intr___orun___lsb 10
#define reg_eth_r_intr___orun___width 1
#define reg_eth_r_intr___orun___bit 10
#define reg_eth_r_intr___urun___lsb 11
#define reg_eth_r_intr___urun___width 1
#define reg_eth_r_intr___urun___bit 11
#define reg_eth_r_intr___excessive_col___lsb 12
#define reg_eth_r_intr___excessive_col___width 1
#define reg_eth_r_intr___excessive_col___bit 12
#define reg_eth_r_intr___mdio___lsb 13
#define reg_eth_r_intr___mdio___width 1
#define reg_eth_r_intr___mdio___bit 13
#define reg_eth_r_intr_offset 84

/* Register r_masked_intr, scope eth, type r */
#define reg_eth_r_masked_intr___crc___lsb 0
#define reg_eth_r_masked_intr___crc___width 1
#define reg_eth_r_masked_intr___crc___bit 0
#define reg_eth_r_masked_intr___align___lsb 1
#define reg_eth_r_masked_intr___align___width 1
#define reg_eth_r_masked_intr___align___bit 1
#define reg_eth_r_masked_intr___oversize___lsb 2
#define reg_eth_r_masked_intr___oversize___width 1
#define reg_eth_r_masked_intr___oversize___bit 2
#define reg_eth_r_masked_intr___congestion___lsb 3
#define reg_eth_r_masked_intr___congestion___width 1
#define reg_eth_r_masked_intr___congestion___bit 3
#define reg_eth_r_masked_intr___single_col___lsb 4
#define reg_eth_r_masked_intr___single_col___width 1
#define reg_eth_r_masked_intr___single_col___bit 4
#define reg_eth_r_masked_intr___mult_col___lsb 5
#define reg_eth_r_masked_intr___mult_col___width 1
#define reg_eth_r_masked_intr___mult_col___bit 5
#define reg_eth_r_masked_intr___late_col___lsb 6
#define reg_eth_r_masked_intr___late_col___width 1
#define reg_eth_r_masked_intr___late_col___bit 6
#define reg_eth_r_masked_intr___deferred___lsb 7
#define reg_eth_r_masked_intr___deferred___width 1
#define reg_eth_r_masked_intr___deferred___bit 7
#define reg_eth_r_masked_intr___carrier_loss___lsb 8
#define reg_eth_r_masked_intr___carrier_loss___width 1
#define reg_eth_r_masked_intr___carrier_loss___bit 8
#define reg_eth_r_masked_intr___sqe_test_err___lsb 9
#define reg_eth_r_masked_intr___sqe_test_err___width 1
#define reg_eth_r_masked_intr___sqe_test_err___bit 9
#define reg_eth_r_masked_intr___orun___lsb 10
#define reg_eth_r_masked_intr___orun___width 1
#define reg_eth_r_masked_intr___orun___bit 10
#define reg_eth_r_masked_intr___urun___lsb 11
#define reg_eth_r_masked_intr___urun___width 1
#define reg_eth_r_masked_intr___urun___bit 11
#define reg_eth_r_masked_intr___excessive_col___lsb 12
#define reg_eth_r_masked_intr___excessive_col___width 1
#define reg_eth_r_masked_intr___excessive_col___bit 12
#define reg_eth_r_masked_intr___mdio___lsb 13
#define reg_eth_r_masked_intr___mdio___width 1
#define reg_eth_r_masked_intr___mdio___bit 13
#define reg_eth_r_masked_intr_offset 88


/* Constants */
#define regk_eth_discard                          0x00000000
#define regk_eth_ether                            0x00000000
#define regk_eth_full                             0x00000001
#define regk_eth_half                             0x00000000
#define regk_eth_hsh                              0x00000001
#define regk_eth_mii                              0x00000001
#define regk_eth_mii_clk                          0x00000000
#define regk_eth_mii_rec                          0x00000002
#define regk_eth_no                               0x00000000
#define regk_eth_rec                              0x00000001
#define regk_eth_rw_ga_hi_default                 0x00000000
#define regk_eth_rw_ga_lo_default                 0x00000000
#define regk_eth_rw_gen_ctrl_default              0x00000000
#define regk_eth_rw_intr_mask_default             0x00000000
#define regk_eth_rw_ma0_hi_default                0x00000000
#define regk_eth_rw_ma0_lo_default                0x00000000
#define regk_eth_rw_ma1_hi_default                0x00000000
#define regk_eth_rw_ma1_lo_default                0x00000000
#define regk_eth_rw_mgm_ctrl_default              0x00000000
#define regk_eth_rw_test_ctrl_default             0x00000000
#define regk_eth_size1518                         0x00000000
#define regk_eth_size1522                         0x00000001
#define regk_eth_yes                              0x00000001
#endif /* __eth_defs_asm_h */
