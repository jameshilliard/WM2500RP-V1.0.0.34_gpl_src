# 
#  Copyright (C) 2007-2009 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=uhttpd
PKG_RELEASE:=31

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

ifdef CONFIG_UHTTPD_PLATFORM_PATCH
  PLATFORM_PATCH_DIR:=./patches/$(BOARD)_patches
else
  PLATFORM_PATCH_DIR:=./patches/patches
endif

include $(INCLUDE_DIR)/package.mk
WEB_PATH:=usr/www

define Package/uhttpd
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=uHTTPd - tiny, single threaded HTTP server.
  MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
  DEPENDS:=CONFIG_uhttpd-mod-tls_cyassl:libcyassl +CONFIG_uhttpd-mod-tls_openssl:libopenssl
endef

define Package/uhttpd/config
	menu "Configurations"
		depends on PACKAGE_uhttpd
		source "$(SOURCE)/Config.in"
	endmenu
endef

CGI_SUPPORT:= 1
LUA_SUPPORT:= 0
TLS_SUPPORTED:= 0
UHTTPD_TLS:=
TLS_CFLAGS:=
TLS_LDFLAGS:=
ifneq ($(CONFIG_uhttpd-mod-tls_cyassl),)
  TLS_SUPPORTED:=1
  UHTTPD_TLS:=cyassl
  TLS_CFLAGS:=-I$(STAGING_DIR)/usr/include/cyassl -DTLS_IS_CYASSL
  TLS_LDFLAGS:=-lcyassl -lm
endif
ifneq ($(CONFIG_uhttpd-mod-tls_openssl),)
  TLS_SUPPORTED:=1
  UHTTPD_TLS:=openssl
  TLS_CFLAGS:=-DTLS_IS_OPENSSL
  TLS_LDFLAGS:=-lssl
endif

TARGET_CFLAGS += $(TLS_CFLAGS)
TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib
MAKE_VARS += \
	FPIC="$(FPIC)" \
	LUA_SUPPORT="$(if $(CONFIG_uhttpd-mod-lua),1)" \
	CGI_SUPPORT="$(if $(CONFIG_uhttpd-mod-cgi),1)" \
	TLS_SUPPORT="$(TLS_SUPPORTED)" \
	UHTTPD_TLS="$(UHTTPD_TLS)" \
	TLS_CFLAGS="$(TLS_CFLAGS)" \
	TLS_LDFLAGS="$(TLS_LDFLAGS)"

define Build/Prepare
	$(CP) $(GIT_HOME)/uhttpd.git/* $(PKG_BUILD_DIR)/
endef

#define Build/Configure
#	(cd $(PKG_BUILD_DIR));
#endef

#define Build/Compile
#	(cd $(PKG_BUILD_DIR));
#endef

define Package/uhttpd/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/uhttpd.init $(1)/etc/init.d/uhttpd
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/files/uhttpd.config $(1)/etc/config/uhttpd
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/uhttpd $(1)/usr/sbin/uhttpd
	$(INSTALL_DIR) $(1)/$(WEB_PATH)/cgi-bin
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/files/uhttpd.sh $(1)/$(WEB_PATH)/cgi-bin
	chmod a+x $(1)/$(WEB_PATH)/cgi-bin/uhttpd.sh
	$(INSTALL_DIR) $(1)/usr/lib
	-$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,uhttpd))
