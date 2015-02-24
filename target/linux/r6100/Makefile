#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

modelname_val:=R6100
ifneq "x$(CONFIG_TARGET_MODELNAME)" "x"
	modelname_val:=$(call qstrip,$(CONFIG_TARGET_MODELNAME))
endif

ARCH:=rlx
BOARD:=r6100
BOARDNAME:=DNI $(modelname_val)
CUSTOMER:=dni
FEATURES:=squashfs tgz


LINUX_VERSION:=2.6.30
LINUX_TREEISH:=$(CONFIG_TARGET_LINUX_GIT_TREEISH)


include $(INCLUDE_DIR)/target.mk

KERNEL_MAKEOPTS := -C $(LINUX_DIR) \
         DIR_LINUX="$(LINUX_DIR)"
#	 LINUX_KARCH=rlx

DEFAULT_CFLAGS_mips=-Os -pipe


#DEFAULT_PACKAGES += hostapd-mini kmod-madwifi
define Target/Description
	Build firmware images rtl819x
endef

#define our own kernel/prepare
define Kernel/Prepare
	test x$(GIT_HOME) != x
	@echo "Plz git clone $(CONFIG_TARGET_LINUX_GIT_URL) $(CONFIG_TARGET_LINUX_GIT_REPOSITORY)"
	test -d $(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY)
	[ "x$(NETFILTER_TREEISH)" = x ] || test -d $(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY)
	(cd $(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY); git-cat-file -e $(LINUX_TREEISH))
	[ "x$(NETFILTER_TREEISH)" = x ] || (cd $(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY); git-cat-file -e $(NETFILTER_TREEISH))
	(cd $(KERNEL_BUILD_DIR); git-archive --format=tar --prefix=linux-$(LINUX_VERSION)/ --remote=$(GIT_HOME)/$(CONFIG_TARGET_LINUX_GIT_REPOSITORY) $(LINUX_TREEISH) | tar -xvf -)
	[ "x$(NETFILTER_TREEISH)" = x ] || (cd $(KERNEL_BUILD_DIR); git-archive --format=tar --prefix=linux-$(LINUX_VERSION)/ --remote=$(GIT_HOME)/$(CONFIG_TARGET_NETFILTER_GIT_REPOSITORY) $(NETFILTER_TREEISH) | tar -xvf - linux-$(LINUX_VERSION)/include/net/netfilter linux-$(LINUX_VERSION)/include/linux/netfilter linux-$(LINUX_VERSION)/include/linux/netfilter_ipv6 linux-$(LINUX_VERSION)/include/linux/netfilter_ipv4 linux-$(LINUX_VERSION)/net/netfilter linux-$(LINUX_VERSION)/net/ipv6/netfilter linux-$(LINUX_VERSION)/net/ipv4/netfilter)
	if [ -d $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/arch/i386 ]; then \
		mkdir -p $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/arch/x86; \
		ln -s $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/arch/i386/* $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/arch/x86/; \
		ln -s $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/include/asm-i386 $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/include/asm-x86; \
	fi
	$(Kernel/Patch)
endef

define Kernel/Configure
	echo "R6100 kernel configure"
	#Since rtl-sdk was included as patch ,the file exec flag will clean after patch
	#Need to chmod
	chmod +x $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/net/wireless/rtl8192cd/bin2c.pl
	chmod +x $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/rtkload/cvimg
	chmod +x $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/rtkload/lzma
	chmod +x $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/rtkload/lzma-24
	chmod +x $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/rtkload/lzma-26

	@echo "Cover data to drivers/net/wireless/rtl8192cd"
	rm -rf $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/net/wireless/rtl8192cd/data*
	tar -zxvf $(PLATFORM_DIR)/data.tgz -C $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/net/wireless/rtl8192cd/
	$(call Kernel/Configure/Default)
endef

$(eval $(call BuildTarget))
