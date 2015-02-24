# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=net-snmp
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=datalib
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_TARGET_DIR:=$(PKG_BUILD_DIR)/target

PLATFORM_PATCH_DIR:=./patches/$(CONFIG_TARGET_BOARD)-patches

include $(INCLUDE_DIR)/package.mk

define Package/net-snmp
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=SNMP monitors the health and welfare of network equipment.
endef

define Package/net-snmp/config
	menu "Configurations"
		depends on PACKAGE_net-snmp
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Package/net-snmp/description
	Net-SNMP is a suite of applications used to implement SNMP v1, SNMP v2c and SNMP v3.
endef

define Build/Prepare
	 $(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/net-snmp/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) ./files/* $(1)/
	$(CP) $(PKG_BUILD_DIR)/agent/snmpd $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,net-snmp))
