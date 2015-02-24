#
# Copyright (C) 2008-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

modelname_val:=WNDR3400v3
ifneq "x$(CONFIG_TARGET_MODELNAME)" "x"
  modelname_val:=$(call qstrip,$(CONFIG_TARGET_MODELNAME))
endif

ARCH:=mips
BOARD:=wndr3400v3
BOARDNAME:=DNI $(modelname_val)
CUSTOMER:=dni
FEATURES:=squashfs

LINUX_VERSION:=2.6.31
LINUX_TREEISH:=$(CONFIG_TARGET_LINUX_GIT_TREEISH)
NETFILTER_TREEISH:=$(CONFIG_TARGET_NETFILTER_GIT_TREEISH)
include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += \
	kmod-leds-gpio kmod-input-core kmod-input-polldev \
	kmod-input-gpio-buttons kmod-button-hotplug

define Kernel/Prepare
	$(call Kernel/Prepare/Default)	
endef

define Target/Description
	Build firmware images for DNI WNDR3400v3
endef

$(eval $(call BuildGitTarget))
