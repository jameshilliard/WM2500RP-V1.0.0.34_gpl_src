include $(TOPDIR)/config.mk

LIB	= lib$(BOARD).a

OBJS	=			\
	$(BOARD).o		\
	flash.o			\
	extra.o \
	../hw29764530p4p0p32p3x3p0_private/init-955x.o	\
	../hw29764530p4p0p32p3x3p0_private/serial.o	\
	../hw29764530p4p0p32p3x3p0_private/ath_pci.o	\
	../hw29764530p4p0p32p3x3p0_private/qca-eth-955x.o

#
# The files included in EXTRA_OBJS use some functions that are not
# available in the `bootstrap' portion of tuboot.bin resulting in
# compile/link errors. Hence have them separate (i.e. don't include
# into lib$(BOARD).a and include them only in `u-boot' portion of
# tuboot.bin.
#

ifeq ($(strip $(COMPRESSED_UBOOT)),0) #{
OBJS	+= $(BOARD_EXTRA_OBJS_tmp)
else
EXTRA_OBJS = $(BOARD_EXTRA_OBJS_tmp)
endif #}

ifeq ($(ATH_DUAL_FLASH),1)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/ath_nand.o ../../../drivers/nand/nand_ids.o
endif

ifndef BOOT_FROM_NAND
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/ath_flash.o
endif

ifeq ($(ATH_RGMII_CAL),1)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/rgmii-cal-955x.o
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs17_lpbk.o
endif

ifeq ($(ETH_CONFIG2), _s17)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs17_phy.o
endif

ifeq ($(ETH_CONFIG), _s17)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs17_phy.o
endif

ifeq ($(ETH_CONFIG2), _s17_hwaccel)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs17_phy.o
endif

ifeq ($(ETH_CONFIG), _s17_hwaccel)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs17_phy.o
endif

ifeq ($(ETH_CONFIG), _s16)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs16_phy.o
endif

ifeq ($(ETH_CONFIG), _f1e)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrsf1_phy.o
endif

ifeq ($(ETH_CONFIG), _f2e)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrsf2_phy.o
endif

ifeq ($(ETH_CONFIG), _vir)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs_vir_phy.o
endif

ifeq ($(ETH_CONFIG2), _vir)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs_vir_phy.o
endif

ifeq ($(ETH_CONFIG), _s27)
#OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athrs27_phy.o
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athr_s27_phy.o
endif

ifeq ($(ETH_CONFIG2), _ar8033)
OBJS	+= ../hw29764530p4p0p32p3x3p0_private/athr_ar8033_phy.o
endif


SOBJS	= ../hw29764530p4p0p32p3x3p0_private/955x.o ../hw29764530p4p0p32p3x3p0_private/tap-955x.o


$(LIB):	.depend $(OBJS) $(SOBJS) $(EXTRA_OBJS)
	$(AR) crv $@ $(OBJS) $(SOBJS)

#########################################################################

.depend:	Makefile $(SOBJS:.o=.S) $(OBJS:.o=.c)
		$(CC) -M $(CFLAGS) $(SOBJS:.o=.S) $(OBJS:.o=.c) > $@

sinclude .depend

#########################################################################
