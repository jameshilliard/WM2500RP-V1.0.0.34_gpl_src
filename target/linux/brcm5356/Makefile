#
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mipsel
BOARD:=brcm5356
BOARDNAME:=Broadcom BCM5356/7/8
CUSTOMER:=netgear
FEATURES:=squashfs

LINUX_VERSION:=2.6.22
LINUX_TREEISH:=$(CONFIG_TARGET_LINUX_GIT_TREEISH)
NETFILTER_TREEISH:=$(CONFIG_TARGET_NETFILTER_GIT_TREEISH)
LSDK_TREEISH:=$(CONFIG_LSDK_TREEISH)

include $(INCLUDE_DIR)/target.mk
DEFAULT_PACKAGES += brcm-nvram

define Kernel/Prepare
	test x$(GIT_HOME) != x
	@echo "Plz git clone $(CONFIG_TARGET_LINUX_GIT_URL) $(CONFIG_TARGET_LINUX_GIT_REPOSITORY)"
	@echo "Plz git clone $(CONFIG_TARGET_NETFILTER_GIT_URL) $(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY)"
	test -d $(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY)
	test -d $(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY)
	(cd $(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY); git-cat-file -e $(LINUX_TREEISH))
	(cd $(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY); git-cat-file -e $(NETFILTER_TREEISH))
	(cd $(KERNEL_BUILD_DIR); git-archive --format=tar --prefix=linux-$(LINUX_VERSION)/ --remote=$(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY) $(LINUX_TREEISH) | tar -xvf -)
	(cd $(KERNEL_BUILD_DIR); rm -rf linux-$(LINUX_VERSION)/net/ipv4/netfilter linux-$(LINUX_VERSION)/net/netfilter linux-$(LINUX_VERSION)/include/linux/netfilter_ipv4 linux-$(LINUX_VERSION)/include/linux/netfilter linux-$(LINUX_VERSION)/net/ipv6/netfilter linux-$(LINUX_VERSION)/include/net/netfilter)
	(cd $(KERNEL_BUILD_DIR); git-archive --format=tar --prefix=linux-$(LINUX_VERSION)/ --remote=$(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY) $(NETFILTER_TREEISH) | tar -xvf - linux-$(LINUX_VERSION)/net/ipv4/netfilter linux-$(LINUX_VERSION)/net/netfilter linux-$(LINUX_VERSION)/include/linux/netfilter_ipv4 linux-$(LINUX_VERSION)/include/linux/netfilter linux-$(LINUX_VERSION)/net/ipv6/netfilter linux-$(LINUX_VERSION)/include/net/netfilter linux-$(LINUX_VERSION)/include/linux/netfilter.h)
	$(Kernel/Patch)
	$(INSTALL_DIR) $(BUILD_DIR_BASE)
	$(INSTALL_DIR) $(BUILD_DIR)
	ln -sf ../include $(BUILD_DIR)/include
	(cd $(BUILD_DIR_BASE); rm -rf bcm57xx include shared router emf et wl tools wps; git-archive --format=tar --remote=$(GIT_HOME)/$(CONFIG_LSDK_REPOSITORY) $(LSDK_TREEISH) | tar -xvf - bcm57xx include shared router emf et wl tools wps)
endef

define Target/Description
	Build firmware images for Broadcom based routers
	(e.g. brcm5356 router)
endef

$(eval $(call BuildTarget))
