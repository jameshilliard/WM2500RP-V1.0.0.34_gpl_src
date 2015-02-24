# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.2 2006/11/14 08:57:31 ronger Exp $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=atheros-enet
PKG_VERSION:=
PKG_RELEASE:=

PKG_SOURCE:=
PKG_SOURCE_URL:=
PKG_MD5SUM:=
PKG_CAT:=zcat

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/atheros-enet

include $(INCLUDE_DIR)/package.mk

ENET_AUTOLOAD := athrs_gmac.$(LINUX_KMOD_SUFFIX)
KNFILES := $(PKG_BUILD_DIR)/athrs_gmac.$(LINUX_KMOD_SUFFIX)
ifeq (,$(findstring $(CONFIG_TARGET_BOARD),"wndr3400v3" "wm2500rp"))
  ENET_AUTOLOAD += dni_enet.$(LINUX_KMOD_SUFFIX)
  KNFILES += $(PKG_BUILD_DIR)/dni_enet.$(LINUX_KMOD_SUFFIX)
endif

define KernelPackage/atheros-enet
  SUBMENU:=Ethernet Drivers
  TITLE:=Driver for Atheros ethernet
  URL:=http://www.atheros.com/
  VERSION:=$(LINUX_VERSION)+$(PKG_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:=$(KNFILES)
  AUTOLOAD:=$(call AutoLoad,50,$(ENET_AUTOLOAD))
endef

ENET_MAKEOPTS=  \
		PATH="$(KERNEL_STAGING_DIR)/bin:$(TARGET_PATH)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		TOOLPREFIX="mips-linux-" \
		TOOLPATH="mips-linux-" \
		ENETDIR=$(PKG_BUILD_DIR)

DNI_ENET_BOARD=$(call qstrip,$(CONFIG_ATHEROS_ENET_BOARD))
ifeq ($(CONFIG_ATHEROS_ENET_BOARD),"db120-wndr3700v4")
  export BOOT_FROM_NAND=1
  export ETH_CONFIG=_s17
  include ./configs/config.db12x
  CONFILES:=./files/files
else ifeq ($(CONFIG_ATHEROS_ENET_BOARD),"db120-wndr3400v3")
  include ./configs/config.db12x-$(CONFIG_ATHEROS_ENET_BOARD)
  export ATH_PHY_SWAP=1
  CONFILES:=./files/files2
else ifeq ($(CONFIG_ATHEROS_ENET_BOARD),"db120-r6000")
  export 11AC_OFFLOAD=1
  ifdef CONFIG_NAND_FLASH
  export BOOT_FROM_NAND=1
  endif
  include ./configs/config.db12x-lsdk-10.1.332
  export ATH_PHY_SWAP=1
  CONFILES:=./files/files2
else ifeq ($(CONFIG_ATHEROS_ENET_BOARD),"aph131-wm2500rp")
  export CFG_BOARD_TYPE=aph131
  include ./configs/config.board955x-lsdk-9.5.3.15
  export ATH_GMAC0_MII=ATHR_RGMII
  CONFILES:=./files/files-$(DNI_ENET_BOARD)
else ifeq ($(CONFIG_ATHEROS_ENET_BOARD),"ap123-wnr2000v4")
  export BUILD_CONFIG=_ap123
  include ./configs/config.db12x
  export ATH_PHY_SWAP=0
  CONFILES:=./files/files-$(DNI_ENET_BOARD)
else ifeq ($(CONFIG_ATHEROS_ENET_BOARD),"ap121-wnr1000v4")
endif
PLATFORM_PATCH_DIR:=./patches/patches-$(DNI_ENET_BOARD)

# dni related features
export DNI_ENET_BOARD
export DNI_SIOCGMIIREG=1
export DNI_DEFAULT_MAC=1
export DNI_SWITCH_PHY=1
export DNI_ETH_LED=1
export DNI_QOS=1
ifdef CONFIG_DNI_FACTORY_MODE
export DNI_LED_FACTORY=1
endif

define KernelPackage/atheros-enet/config
	menu "Configurations"
		depends on PACKAGE_kmod-atheros-enet
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/atheros-enet.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Build/InstallDev
endef

define Build/UninstallDev
endef

define KernelPackage/atheros-enet/install
	mkdir -p $(1)/etc/init.d
	install -m0755 ./$(CONFILES)/init.enet $(1)/etc/init.d/
	install -m0755 ./$(CONFILES)/net-lan $(1)/etc/init.d/
	install -m0755 ./$(CONFILES)/net-wan $(1)/etc/init.d/
	-[ -f ./$(CONFILES)/$(CONFIG_ATHEROS_ENET_BOARD).init.enet.0 ] && install -m0755 ./$(CONFILES)/$(CONFIG_ATHEROS_ENET_BOARD).init.enet.0 $(1)/etc/init.d/init.enet.0
	mkdir -p $(1)/sbin
	-[ -f $(PKG_BUILD_DIR)/tools2/ethreg ] && install -m0755 $(PKG_BUILD_DIR)/tools2/ethreg $(1)/sbin/
	-[ -f $(PKG_BUILD_DIR)/tools2/dni_eth ] && install -m0755 $(PKG_BUILD_DIR)/tools2/dni_eth $(1)/sbin/
	-[ -f ./$(CONFILES)/net-iptv ] && install -m0755 ./$(CONFILES)/net-iptv $(1)/etc/init.d/
	-[ -f ./$(CONFILES)/$(CONFIG_ATHEROS_ENET_BOARD).vlan-set ] && install -m0755 ./$(CONFILES)/$(CONFIG_ATHEROS_ENET_BOARD).vlan-set $(1)/sbin/vlan-set
endef

$(eval $(call KernelPackage,atheros-enet))
