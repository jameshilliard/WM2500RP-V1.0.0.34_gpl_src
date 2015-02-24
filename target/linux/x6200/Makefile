#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

modelname_val:=x6200
ifneq "x$(CONFIG_TARGET_MODELNAME)" "x"
  modelname_val:=$(call qstrip,$(CONFIG_TARGET_MODELNAME))
endif

ARCH:=arm
BOARD:=x6200
BOARDNAME:=DNI $(modelname_val)
CUSTOMER:=netgear
FEATURES:=squashfs usb
CFLAGS:= -Os -pipe -march=armv7-a -mtune=cortex-a9 -funit-at-a-time

LINUX_VERSION:=2.6.36
LINUX_TREEISH:=$(CONFIG_TARGET_LINUX_GIT_TREEISH)
NETFILTER_TREEISH:=$(CONFIG_TARGET_NETFILTER_GIT_TREEISH)
LSDK_TREEISH:=$(CONFIG_LSDK_TREEISH)

export SRCBASE:=$(BUILD_DIR_BASE)

define Target/Description
	Build firmware images for DNI x6200. Broadcom BCM47081 (ARM A9)
endef

include $(INCLUDE_DIR)/target.mk
define Kernel/Prepare
	test x$(GIT_HOME) != x
	@echo "Plz git clone $(CONFIG_LSDK_REPOSITORY)"
	test -d $(GIT_HOME)/$(CONFIG_LSDK_REPOSITORY)
	(cd $(GIT_HOME)/$(CONFIG_LSDK_REPOSITORY); git-cat-file -e $(LINUX_TREEISH))
	(mkdir -p $(LINUX_DIR); cd $(LINUX_DIR); git-archive --format=tar --remote=$(GIT_HOME)/$(CONFIG_LSDK_REPOSITORY) $(LINUX_TREEISH) | tar -xvf - components/opensource/linux/linux-2.6.36; mv components/opensource/linux/linux-2.6.36/* ./; rm -rf components)
	$(Kernel/Patch)
	$(INSTALL_DIR) $(BUILD_DIR_BASE)
	$(INSTALL_DIR) $(BUILD_DIR)
	ln -sf ../include $(BUILD_DIR)/include
	ln -sf build_dir $(TOPDIR)/src
	(cd $(BUILD_DIR_BASE); rm -rf bcm57xx include shared router emf et wl tools wps bcmcrypto makefiles; git-archive --format=tar --remote=$(GIT_HOME)/$(CONFIG_LSDK_REPOSITORY) $(LSDK_TREEISH) | tar -xvf - src/bcm57xx src/include src/shared src/router src/emf src/et src/wl src/tools src/wps src/bcmcrypto src/makefiles; mv src/* ./; rm -rf src)
	(cd $(BUILD_DIR); git-archive --format=tar --remote=$(GIT_HOME)/$(CONFIG_LSDK_REPOSITORY) $(LSDK_TREEISH) | tar -xvf - tools)
	cp ./sdk_files/bcmconfig.h $(BUILD_DIR_BASE)/router/shared/
endef

$(eval $(call BuildTarget))
