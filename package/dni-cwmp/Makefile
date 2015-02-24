# $Id: Makefile, 2012/11/20, Iceway $

include $(TOPDIR)/rules.mk

PKG_NAME:=dni-cwmp
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PLATFORM_PATCH_DIR:=./patches/$(CONFIG_TARGET_BOARD)-patches

include $(INCLUDE_DIR)/package.mk

config-y := --without-tr-143 \
	--with-tr-111 \
	--with-cwmpcpe \
	--enable-devmode \
	--enable-debug

define Package/dni-cwmp
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=CWMP (TR-069)
  DEPENDS:=+libopenssl
endef

define Package/dni-cwmp/config
	menu "Configurations"
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Package/dni-cwmp/description
  DNI TR069 implementation
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/cwmp.git/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/dni-cwmp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/src/.libs/libcwmp.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmpcpe/.libs/cwmpcpe $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmpcpe/device_info $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmpcpe/utility/cwmp_utility.sh $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmpcpe/utility/cwmprun.sh $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmpcpe/utility/IPPing_test $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmpcpe/utility/TRR_test $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmpcpe/utility/DHCP_opt_tool $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/cwmp
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/cwmpcpe/utility/cwmp_rsc.list $(1)/etc/cwmp
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/cwmpcpe/utility/cwmp_func.list $(1)/etc/cwmp
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/cwmpcpe/utility/IPPing_test $(1)/etc/cwmp
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/cwmpcpe/utility/TRR_test $(1)/etc/cwmp
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/cwmpcpe/utility/DHCP_opt_tool $(1)/etc/cwmp
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/cwmp.init $(1)/etc/init.d/cwmp
	$(INSTALL_BIN) ./files/telenet-certs.sh $(1)/usr/bin
	$(INSTALL_BIN) ./files/sync-tncrl.sh $(1)/usr/bin
endef

$(eval $(call BuildPackage,dni-cwmp))
