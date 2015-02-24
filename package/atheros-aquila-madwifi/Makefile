# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5196 2006-10-17 20:52:40Z nbd $
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=atheros-aquila-madwifi
PKG_VERSION:=
PKG_RELEASE:=1

PKG_SOURCE:=
PKG_SOURCE_URL:=
PKG_MD5SUM:=
PKG_CAT:=zcat

PKG_CO_TAG=atheros-br
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

AR531X_KERNEL_CROSS:=mips-linux-

include $(INCLUDE_DIR)/package.mk

ifdef CONFIG_ATHEROS_MADWIFI_SUPPORT_DFS
  DFS_ENABLE:=1
  DFS_MODULE:=$(PKG_BUILD_DIR)/lmac/dfs/ath_dfs.$(LINUX_KMOD_SUFFIX)
else
  DFS_ENABLE:=0
endif

ifdef CONFIG_ATHEROS_MADWIFI_PLATFORM_PATCH
  PLATFORM_PATCH_DIR:=./patches/patches-$(BOARD)
else
  PLATFORM_PATCH_DIR:=./patches/patches
endif

RATE_CONTROL:=atheros

ifeq ($(ARCH),mips)
  HAL_TARGET:=mipsisa32-be-elf
  KERNEL_ARCH=mips
endif
ifeq ($(ARCH),mipsel)
  HAL_TARGET:=mips-le-elf
  KERNEL_ARCH=mips
endif
ifeq ($(ARCH),i386)
  HAL_TARGET:=i386-elf
  KERNEL_ARCH=i386
endif

BUS:=PCI
ifneq ($(CONFIG_LINUX_2_6_AR71XX),)
  BUS:=PCI
endif
ifneq ($(CONFIG_LINUX_2_6_ARUBA),)
  BUS:=PCI AHB	# no suitable HAL for AHB yet.
endif


ATH_TOOLS:= athstats athstatsclr wlanconfig cgiMain apstats

MADWIFI_AUTOLOAD:= \
	adf \
	asf \
	ath_hal \
	ath_rate_atheros \
	ath_dev \
	umac

ifdef CONFIG_ATHEROS_MADWIFI_SUPPORT_PKTLOG
  PKTLOG_MODULE:=$(PKG_BUILD_DIR)/lmac/ath_pktlog/ath_pktlog.$(LINUX_KMOD_SUFFIX)
  ATH_TOOLS+=pktlogconf pktlogdump
  MADWIFI_AUTOLOAD+=ath_pktlog
  REMOVE_PKTLOG=0
else
  REMOVE_PKTLOG=1
endif

ifdef CONFIG_ATHEROS_MADWIFI_SUPPORT_SPECTRAL
  SPECTRAL_MODULE=$(PKG_BUILD_DIR)/lmac/spectral/ath_spectral.$(LINUX_KMOD_SUFFIX)
  ATH_TOOLS+=spectraltool
  SUPPORT_SPECTRAL=1
else
  SUPPORT_SPECTRAL=0
endif

define KernelPackage/atheros-aquila-madwifi
  SUBMENU:=Wireless Drivers
  TITLE:=Driver for Atheros wireless chipsets
  URL:=http://madwifi.org/
  VERSION:=$(LINUX_VERSION)+$(PKG_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
		$(PKG_BUILD_DIR)/adf/adf.$(LINUX_KMOD_SUFFIX) \
		$(PKG_BUILD_DIR)/asf/asf.$(LINUX_KMOD_SUFFIX) \
		$(PKG_BUILD_DIR)/lmac/ath_dev/ath_dev.$(LINUX_KMOD_SUFFIX) \
		$(PKG_BUILD_DIR)/lmac/ext/wlan_ext.$(LINUX_KMOD_SUFFIX) \
		$(PKG_BUILD_DIR)/os/linux/ath_hal/ath_hal.$(LINUX_KMOD_SUFFIX) \
		$(PKTLOG_MODULE) \
		$(PKG_BUILD_DIR)/lmac/ratectrl/ath_rate_atheros.$(LINUX_KMOD_SUFFIX) \
		$(DFS_MODULE) \
		$(SPECTRAL_MODULE) \
		$(PKG_BUILD_DIR)/umac/umac.$(LINUX_KMOD_SUFFIX)
  # AUTOLOAD:=$(call AutoLoad,50,$(MADWIFI_AUTOLOAD))
endef

ifneq ($(CONFIG_TARGET_dniwndr3700u2631),)
  MADWIFI_DRIVER_SPEC_OPTS+= \
	DNI_EXTRA_HAL_INCS="-I$(LINUX_DIR)/arch/mips/include/asm/mach-ar71xx" \
	USEMARCH="mips32"
  BOARD_FLAGS+=-DWORKAROUND_FOR_OPENWRT_WNDR3700
endif

ifdef CONFIG_ATHEROS_MADWIFI_2G_MAC_ADDR_LOC
  BOARD_FLAGS+=-DWLAN_2G_MAC_ADDR=$(CONFIG_ATHEROS_MADWIFI_2G_MAC_ADDR_LOC)
endif

ifdef CONFIG_ATHEROS_MADWIFI_5G_MAC_ADDR_LOC
  BOARD_FLAGS+=-DWLAN_5G_MAC_ADDR=$(CONFIG_ATHEROS_MADWIFI_5G_MAC_ADDR_LOC)
endif

ifdef CONFIG_ATHEROS_MADWIFI_USE_CLIENT_BRIDGE
  BOARD_FLAGS+=-DENABLE_CLIENT_BRIDGE
  BOARD_FLAGS+=-DDNI_EXTEND_FUNCS
endif

ifdef CONFIG_ATHEROS_MADWIFI_DISPLAY_SCAN_IN_PROC
  BOARD_FLAGS+=-DDISPLAY_SCAN_RESULT_IN_PROC
  BOARD_FLAGS+=-DENABLE_DNI_SYSCTL
endif

ifdef CONFIG_ATHEROS_NETGEAR_ACL
  BOARD_FLAGS+=-DNETGEAR_ACL
endif

ifdef CONFIG_NAND_FLASH
  export BOOT_FROM_NAND=1
endif

ifeq ($(BOARD),r6000d)
  export 11AC_OFFLOAD=1
  include qca-configs/config.db12x
  export ATH_SUPPORT_SPECTRAL=$(SUPPORT_SPECTRAL)
  BOARD_OPTIONS+= AH_CAL_LOCATIONS_PCI=$(CONFIG_ATHEROS_MADWIFI_CAL_LOCATIONS_PCI) \
		AH_CAL_LOCATIONS_AHB=$(CONFIG_ATHEROS_MADWIFI_CAL_LOCATIONS_AHB) \
		ATH_LED_CUSTOMER=CUSTOMER_NETGEAR \
		GPIO_PIN_FUNC_0=0 \
		GPIO_PIN_FUNC_1=0 \
		GPIO_PIN_FUNC_2=0
endif

ifeq ($(BOARD),wm2500rp)
  export CFG_BOARD_TYPE=aph131
  include qca-configs/config.board955x
  BOARD_OPTIONS+= AH_CAL_LOCATIONS_PCI=$(CONFIG_ATHEROS_MADWIFI_CAL_LOCATIONS_PCI) \
		AH_CAL_LOCATIONS_AHB=$(CONFIG_ATHEROS_MADWIFI_CAL_LOCATIONS_AHB) \
		ATH_LED_CUSTOMER=CUSTOMER_NETGEAR \
		GPIO_PIN_FUNC_0=0 \
		GPIO_PIN_FUNC_1=0 \
		GPIO_PIN_FUNC_2=0
define Build/Compile/wm2500rp/extratools/acfg
endef

define Build/install/wm2500rp/extratools/acfg
	$(CP) $(PKG_BUILD_DIR)/qca-apps/acfg/src/libacfg.a $(1)/lib
	$(CP) $(PKG_BUILD_DIR)/qca-apps/acfg_test_app/acfg_tool $(1)/sbin
	$(CP) $(PKG_BUILD_DIR)/qca-apps/acfg_test_app/acfg_sample.conf $(1)/etc
	$(CP) $(PKG_BUILD_DIR)/qca-apps/acfg/src/acfg_common.conf $(1)/etc
endef
endif

# Add each board own BOARD_OPTIONS above here, or a default value will be used.
ifeq ($(BOARD_OPTIONS),)
  BOARD_OPTIONS+= AH_CAL_IN_FLASH=1 \
		AH_CAL_RADIOS=$(CONFIG_ATHEROS_MADWIFI_RADIOS) \
		AH_CAL_LOCATIONS=$(CONFIG_ATHEROS_MADWIFI_CAL_LOCATIONS) \
		AH_SUPPORT_OWL=0
  ifdef CONFIG_ATHEROS_MADWIFI_BUS_AS_AHB
    BUS=AHB
    BOARD_OPTIONS+=AH_SUPPORT_SOWL=0 \
	AH_SUPPORT_EEPROM_AR9330=1 \
	ATH_TRAFFIC_FAST_RECOVER=1 \
	ATH_WDS_INTEROP=1
  endif
endif


MADWIFI_DRIVER_SPEC_OPTS+= \
		KERNELPATH="$(LINUX_DIR)" \
		KERNELARCH=$(KERNEL_ARCH) \
		KERNELVER=$(LINUX_VERSION) \
		TARGET="$(HAL_TARGET)"

MADWIFI_ENVOPTS:= \
		PATH="$(KERNEL_STAGING_DIR)/bin:$(TARGET_PATH)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_KERNEL_CROSS)" \
		TOOLPREFIX="$(TARGET_KERNEL_CROSS)" \
		TOOLPATH="$(TARGET_KERNEL_CROSS)" \
		ATHEROSPATH="$(PKG_BUILD_DIR)" \
		ATH_RATE="$(PKG_BUILD_DIR)/lmac/ratectrl" \
		HAL="$(PKG_BUILD_DIR)/hal" \
		COMMON_CFLAGS="-DCONFIG_NO_SAVE_CALDATA -DNETGEAR_WDS -DNETGEAR_CUSTOMISED_LED -DNETGEAR_WLAN_LOG -DENABLE_DNI_SYSCTL -DWPS_NOTIFICATION -DDNI_HOSTAP_JUDGE_PIN_LENGTH -DNETGEAR_CUSTOMISED_WEP_KEY -DNETGEAR_CUSTOM_MIC -DNETGEAR_RTS_DEF -DWLAN_GET_MAC_ADDR_FROM_PARAMS -DNETGEAR_DISABLE_WPS_IE -DMORE_TPSCALE -D__EXPORTED_HEADERS__ $(BOARD_FLAGS) -DDNI_SHOW_VAP_STATUS"

MADWIFI_ATHOPTS:= \
		ATH_SUPPORT_DFS=$(DFS_ENABLE) \
		$(BOARD_OPTIONS)

MADWIFI_DRIVEROPTS:= $(MADWIFI_DRIVER_SPEC_OPTS) $(MADWIFI_ENVOPTS) $(MADWIFI_ATHOPTS)
MADWIFI_TOOLOPTS:= $(MADWIFI_ENVOPTS) $(MADWIFI_ATHOPTS)

# for wndr3400v3, it should set 'CONFIG_ATHEROS_MADWIFI_BUS_AS_AHB' 
# and 'CONFIG_ATHEROS_MADWIFI_BUS_AS_DUAL' both as y
# Because it need do 'MADWIFI_TOOLOPTS+=KERNELPATH="$(LINUX_DIR)"'
ifdef CONFIG_ATHEROS_MADWIFI_BUS_AS_AHB
  MADWIFI_TOOLOPTS+=KERNELPATH="$(LINUX_DIR)"
endif

define KernelPackage/atheros-aquila-madwifi/config
	menu "Configurations"
		depends on PACKAGE_kmod-atheros-aquila-madwifi
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Build/InstallDev
endef

define Build/UninstallDev
endef

define KernelPackage/atheros-aquila-madwifi/install
	mkdir -p $(1)/etc/init.d
	mkdir -p $(1)/lib/modules/$(LINUX_VERSION)
	mkdir -p $(1)/sbin
	$(CP) $(foreach file,$(ATH_TOOLS),$(PKG_BUILD_DIR)/tools/$(file)) $(1)/sbin
	ln -sf /sbin/cgiMain $(1)/sbin/cfg
	$(call Build/install/$(BOARD)/extratools/acfg,$(1))
endef

$(eval $(call KernelPackage,atheros-aquila-madwifi))
