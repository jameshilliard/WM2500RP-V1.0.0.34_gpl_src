# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=brcm-squashfs-4.2
#PKG_VERSION:=4.2
PKG_VERSION:=

#PKG_SOURCE:=$(PKG_NAME)$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=@SF/squashfs
#PKG_CAT:=zcat

PKG_BUILD_DIR:=$(TOOL_BUILD_DIR)/$(PKG_NAME)$(PKG_VERSION)
HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/$(PKG_NAME)$(PKG_VERSION)

LSDK_TREEISH:=$(CONFIG_LSDK_TREEISH)

include $(INCLUDE_DIR)/host-build.mk

define Host/Prepare
	mkdir -p $(BUILD_DIR_BASE)/src
	mkdir -p $(HOST_BUILD_DIR)/src/router
	(cd $(BUILD_DIR_BASE); rm -rf tools; git-archive --format=tar --remote=$(GIT_HOME)/brcm_sdk.git $(LSDK_TREEISH) | tar -xvf - src/tools; mv src/tools ./; rm -rf src)
	(cd $(HOST_BUILD_DIR); rm -rf squashfs; git-archive --format=tar --remote=$(GIT_HOME)/brcm_sdk.git $(LSDK_TREEISH) | tar -xvf - src/router/squashfs-4.2; mv src/router/squashfs-4.2/* ./; rm -rf src)
endef

define Host/Compile
	$(MAKE) -C $(HOST_BUILD_DIR) SRCBASE=$(BUILD_DIR_BASE) LINUXDIR=$(LINUX_DIR) mksquashfs
endef

define Host/Install
	mkdir -p $(STAGING_DIR)/bin
	$(CP) $(HOST_BUILD_DIR)/mksquashfs $(STAGING_DIR_HOST)/bin/mksquashfs-brcm
endef

define Host/Clean
	rm -f $(STAGING_DIR_HOST)/bin/mksquashfs-brcm
endef

$(eval $(call HostBuild))
