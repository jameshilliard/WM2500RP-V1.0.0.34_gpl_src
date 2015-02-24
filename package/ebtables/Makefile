#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ebtables
PKG_VERSION:=$(call qstrip,$(CONFIG_EBT_VERSION))
PKG_MD5SUM:=$(call qstrip,$(CONFIG_EBT_MD5SUM))
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-v$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/ebtables

PKG_BUILD_DIR:=$(BUILD_DIR)/ebtables-v$(PKG_VERSION)

ifdef CONFIG_EBT_PLATFORM_PATCH
  PLATFORM_PATCH_DIR:=./$(BOARD)_patches
else
  PLATFORM_PATCH_DIR:=./patches
endif

include $(INCLUDE_DIR)/package.mk

define Package/ebtables/config
	menu "Configurations"
		depends on PACKAGE_ebtables
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Package/ebtables
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+kmod-ebtables
  TITLE:=Ethernet bridge firewall administration utility
  URL:=http://ebtables.sourceforge.net/
endef

define Package/ebtables-utils
  $(call Package/ebtables)
  TITLE:=ebtables save/restore utilities
endef

define Package/ebtables/description
	The ebtables program is a filtering tool for a bridging firewall. The
	filtering is focussed on the Link Layer Ethernet frame fields. Apart
	from filtering, it also gives the ability to alter the Ethernet MAC
	addresses and implement a brouter.
endef

define Package/ebtables-utils/description
	$(call Package/ebtables/description)
endef

MAKE_VARS += EXT_LIBSI="$(LIBGCC_S)"

MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS)" \
	LIBDIR="/usr/lib"

define Build/Prepare
	$(call PKG_UNPACK)
	$(call PatchDir,$(PKG_BUILD_DIR),$(PLATFORM_PATCH_DIR))
endef

define Package/ebtables/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/ethertypes $(1)/etc/
	$(INSTALL_DIR) $(1)/usr/lib/ebtables
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lib*.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/extensions/*.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ebtables $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/cmd_ebtables $(1)/sbin
endef

define Package/ebtables-utils/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ebtables-save $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ebtables-restore $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,ebtables))
$(eval $(call BuildPackage,ebtables-utils))
