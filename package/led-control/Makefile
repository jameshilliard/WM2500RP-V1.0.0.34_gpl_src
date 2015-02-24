# 
# Copyright (C) 2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=led-control
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

ifdef CONFIG_LED_CONTROL_PLATFORM_SRC
	PLATFORM_SRC_DIR:=./src/$(BOARD)_src
else
	PLATFORM_SRC_DIR:=./src/src
endif

define Package/led-control
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Update utility for LED control
endef

define Package/led-control/config
	menu "Configurations"
		depends on PACKAGE_led-control
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/led-control/install
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/ledcontrol $(1)/sbin/
endef

$(eval $(call BuildPackage,led-control))
