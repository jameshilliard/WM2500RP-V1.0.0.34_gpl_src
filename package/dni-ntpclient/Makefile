# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.4.3 2007/02/27 03:06:36 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=ntpclient
PKG_RELEASE:=4
PKG_BUILD_DEPENDS:=datalib
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

#Repeater has no WAN Interface
ifeq ($(BOARD),wm2500rp)
  CONFIG_WAN_IFNAME="br0"
endif

define Package/dni-ntpclient
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=The CGI interface
endef

define Package/dni-ntpclient/config
	menu "Configurations"
		depends on PACKAGE_dni-ntpclient
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Package/dni-ntpclient/description
	The CGI interface
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/ntpclient.git/* $(PKG_BUILD_DIR)/
endef

define Package/dni-ntpclient/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/ntpclient $(1)/usr/sbin/
	install -d -m0755 $(1)/etc/init.d
	install -m0755 ./files/ntpclient.init $(1)/etc/init.d/ntpclient
endef

$(eval $(call BuildPackage,dni-ntpclient))
