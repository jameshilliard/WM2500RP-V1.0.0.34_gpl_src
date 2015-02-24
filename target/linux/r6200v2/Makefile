#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

modelname_val:=r6200v2
ifneq "x$(CONFIG_TARGET_MODELNAME)" "x"
  modelname_val:=$(call qstrip,$(CONFIG_TARGET_MODELNAME))
endif

ARCH:=arm
BOARD:=r6200v2
BOARDNAME:=DNI $(modelname_val)
CUSTOMER:=netgear
FEATURES:=squashfs usb
CFLAGS:= -Os -pipe -march=armv7-a -mtune=cortex-a9 -funit-at-a-time

LINUX_VERSION:=2.6.36.4brcmarm
LINUX_TREEISH:=$(CONFIG_TARGET_LINUX_GIT_TREEISH)
NETFILTER_TREEISH:=$(CONFIG_TARGET_NETFILTER_GIT_TREEISH)
LSDK_TREEISH:=$(CONFIG_LSDK_TREEISH)

NETFILTER_FILES:=linux-$(LINUX_VERSION)/net/ipv4/netfilter linux-$(LINUX_VERSION)/net/netfilter linux-$(LINUX_VERSION)/include/linux/netfilter_ipv4 linux-$(LINUX_VERSION)/include/linux/netfilter linux-$(LINUX_VERSION)/net/ipv6/netfilter linux-$(LINUX_VERSION)/include/net/netfilter linux-$(LINUX_VERSION)/include/linux/netfilter.h

export SRCBASE:=$(BUILD_DIR_BASE)

define Target/Description
	Build firmware images for DNI r6200v2. Broadcom BCM47081 (ARM A9)
endef

include $(INCLUDE_DIR)/target.mk
define Kernel/Prepare
	test x$(GIT_HOME) != x
	@echo "Plz sync with $(CONFIG_TARGET_LINUX_GIT_URL) $(CONFIG_TARGET_LINUX_GIT_REPOSITORY)"
	@echo "Plz sync with $(CONFIG_TARGET_NETFILTER_GIT_URL) $(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY)"
	@echo "Plz sync with $(CONFIG_TARGET_LSDK_GIT_URL) $(CONFIG_TARGET_LSDK_GIT_REPOSITORY)"
	test -d $(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY)
	test -d $(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY)
	test -d $(GIT_HOME)/$(CONFIG_TARGET_LSDK_GIT_REPOSITORY)
	(cd $(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY); git-cat-file -e $(LINUX_TREEISH))
	(cd $(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY); git-cat-file -e $(NETFILTER_TREEISH))
	(cd $(KERNEL_BUILD_DIR); git-archive --format=tar --prefix=linux-$(LINUX_VERSION)/ --remote=$(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY) $(LINUX_TREEISH) | tar -xvf -)
	(cd $(KERNEL_BUILD_DIR); rm -rf $(NETFILTER_FILES))
	(cd $(KERNEL_BUILD_DIR); git-archive --format=tar --prefix=linux-$(LINUX_VERSION)/ --remote=$(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY) $(NETFILTER_TREEISH) | tar -xvf - $(NETFILTER_FILES))
	if [ -d ./patches -a $$$$(ls ./patches | wc -l) -gt 0 ]; then \
		$(PATCH) $(LINUX_DIR) ./patches; \
	fi
	$(INSTALL_DIR) $(BUILD_DIR_BASE)
	$(INSTALL_DIR) $(BUILD_DIR)
	ln -sf ../include $(BUILD_DIR)/include
	ln -sf build_dir $(TOPDIR)/src
	(cd $(BUILD_DIR_BASE); rm -rf bcm57xx include shared router emf et wl tools wps bcmcrypto makefiles; git-archive --format=tar --remote=$(GIT_HOME)/brcm_sdk.git $(LSDK_TREEISH) | tar -xvf - src/bcm57xx src/include src/shared src/router src/emf src/et src/wl src/tools src/wps src/bcmcrypto src/makefiles; mv src/* ./; rm -rf src)
	(cd $(BUILD_DIR); git-archive --format=tar --remote=$(GIT_HOME)/$(CONFIG_TARGET_LSDK_GIT_REPOSITORY) $(LSDK_TREEISH) | tar -xvf - tools)
	cp ./sdk_files/bcmconfig.h $(BUILD_DIR_BASE)/router/shared/
endef

$(eval $(call BuildTarget))
