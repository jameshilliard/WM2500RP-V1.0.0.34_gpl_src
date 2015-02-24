# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.1 2008-05-08 05:17:53 stoneliu Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=boxlogin
PKG_RELEASE:=4

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/boxlogin
  SECTION:=net
  CATEGORY:=Network
  DEFAULT:=y
  TITLE:=boxlogin
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/boxlogin/install
	install -d -m0755 $(1)/usr/sbin
	mkdir -p $(1)/etc/init.d
	install -m0755 ./files/boxlogin.init $(1)/etc/init.d/boxlogin
	install -m0755 $(PKG_BUILD_DIR)/boxlogin $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,boxlogin))
