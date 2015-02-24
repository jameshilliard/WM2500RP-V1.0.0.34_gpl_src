# 
#  Copyright (C) 2007-2009 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=moca-util
PKG_VERSION:=0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
#PATCH_DIR:=./patches

include $(INCLUDE_DIR)/package.mk

define Package/moca-util
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=Support MoCA chip from Entropic Company.
endef

define Package/moca-util/config
	menu "Configurations"
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

MAKEOPTS=  \
		PATH="$(KERNEL_STAGING_DIR)/bin:$(TARGET_PATH)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE=$(TARGET_CROSS) \
		TOOL_CHAIN=$(TARGET_CROSS) \
		SDK_BUILD=1 \
		CUSTOM_KERNEL_SRC_PATH=$(LINUX_DIR) \
		TGT_INF=XMII

define Build/Compile
endef

define Package/moca-util/install
	$(INSTALL_DIR) $(1)/etc
	ln -sf /tmp/moca/examples $(1)/etc/mocaf
	$(CP) $(PKG_BUILD_DIR)/dni-scripts/moca.conf $(1)/etc/
	$(INSTALL_DIR) $(1)/etc/moca
	$(CP) $(PKG_BUILD_DIR)/examples $(1)/etc/moca/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/do/bin/* $(1)/etc/moca/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dni-scripts/dni-mtd $(1)/etc/moca/
	chmod a+x $(1)/etc/moca/*
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/dni-scripts/led-moca $(1)/sbin/
	install -m0755 $(PKG_BUILD_DIR)/dni-scripts/moca-control.sh $(1)/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/dni-scripts/net-moca $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/dni-scripts/bridge-vlan $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/sbin
	ln -sf /etc/moca/clnkcfg $(1)/usr/sbin/clnkcfg
	ln -sf /etc/moca/clnkhwtst $(1)/usr/sbin/clnkhwtst
	ln -sf /etc/moca/clnkmem $(1)/usr/sbin/clnkmem
	ln -sf /etc/moca/clnkpm $(1)/usr/sbin/clnkpm
	ln -sf /etc/moca/clnkrst $(1)/usr/sbin/clnkrst
	ln -sf /etc/moca/clinkd $(1)/usr/sbin/clinkd
	ln -sf /etc/moca/clnkfwupd $(1)/usr/sbin/clnkfwupd
	ln -sf /etc/moca/clnkmcast $(1)/usr/sbin/clnkmcast
	ln -sf /etc/moca/clnkmocamib $(1)/usr/sbin/clnkmocamib
	ln -sf /etc/moca/clnkqos $(1)/usr/sbin/clnkqos
	ln -sf /etc/moca/clnkstat $(1)/usr/sbin/clnkstat

	#DCAP related commands
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.02
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.03
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.08
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.16
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.18
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.19
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.35
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.37
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.38
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.40
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.41
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.42
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.44
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.45
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.46
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.101
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.102
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.103
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.104
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.107
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.109
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.110
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.111
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.112
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.115
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.116
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.122
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.123
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.125
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.126
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.132
	ln -sf /etc/moca/DCAP $(1)/sbin/DCAP.137
endef

$(eval $(call BuildPackage,moca-util))
