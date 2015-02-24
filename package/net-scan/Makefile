# 
#  Copyright (C) 2007-2009 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=net-scan
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/net-scan
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=Scanning the attached devices according to NETGEAR's spec.
endef

define Package/net-scan/config
	menu "Configurations"
		depends on PACKAGE_net-scan
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/net-scan/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/net-scan $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/dev-scan $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,net-scan))
