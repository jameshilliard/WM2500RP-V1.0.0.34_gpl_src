# 
#  Copyright (C) 2007 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dni-autoip
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/dni-autoip
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=Automatically set/get/consult IP address
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/dni-autoip/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/dniautoip $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/dniautoip-control.sh $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,dni-autoip))
