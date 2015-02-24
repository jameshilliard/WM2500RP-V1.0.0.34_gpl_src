# 
# Copyright (C) 2009 OpenWrt.org
#

include $(TOPDIR)/rules.mk

PKG_NAME:=net-cgi
PKG_RELEASE:=1
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

WEB_PATH:=usr/www
TMP_VAR1:=`echo $(CONFIG_LANGUAGE_PARTITION) | sed 's/.*\///'`
TMP_VAR2:=`echo $(CONFIG_POT_PARTITION) | sed 's/.*\///'`
TMP_VAR3:=`echo $(CONFIG_BOOTLOADER_PARTITION) | sed 's/.*\///'`
TMP_VAR4:=`echo $(CONFIG_FIRMWARE_PARTITION) | sed 's/.*\///'`

MODELNAME=$(call strip,$(call qstrip,$(CONFIG_TARGET_MODELNAME)))
B_MODELNAME=$(call strip, $(shell echo $(MODELNAME)|tr a-z A-Z))
L_MODELNAME=$(call strip, $(shell echo $(MODELNAME)|tr A-Z a-z))

LANGUAGE_TABLE=net-web-language_table

include $(INCLUDE_DIR)/package.mk

ifdef CONFIG_NET_CGI_PLATFORM_PATCH
  PLATFORM_PATCH_DIR:=./patches/$(BOARD)_patches
else
  PLATFORM_PATCH_DIR:=./patches/patches
endif

ifdef CONFIG_NET_CGI_PLATFORM_DEFAULT_LANGUAGE
  PLATFORM_DEFAULT_LANGUAGE:=www/www
else
  PLATFORM_DEFAULT_LANGUAGE:=www/$(L_MODELNAME)_www
endif

define Package/net-cgi
  SECTION:=utils
  CATEGORY:=Base system
  DEFAULT:=y
  TITLE:=The mini_httpd CGI interface
endef

define Package/net-cgi/config
	menu "Configurations"
		depends on PACKAGE_net-cgi
		source "$(SOURCE)/Config.in"
	endmenu
endef


define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/ 
endef

define Build/Compile
endef

define Package/net-cgi/install
	-[ ! -d $(PKG_BUILD_DIR)/install ] && install -d -m0755 $(PKG_BUILD_DIR)/install
	$(CP) ./Makefile.install  $(PKG_BUILD_DIR)/install/Makefile
	make -C $(PKG_BUILD_DIR)/install install PREFIX=$(1) WEB_PATH=$(WEB_PATH) PKG_BUILD_DIR=$(PKG_BUILD_DIR)
ifdef	CONFIG_NET_CGI_LANGUAGE_TABLE
	$(CP) $(PKG_BUILD_DIR)/$(LANGUAGE_TABLE)/result/*.tar.gz $(TOPDIR)/bin/
endif
endef

$(eval $(call BuildPackage,net-cgi))
