include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=debug_driver
PKG_VERSION:=
PKG_RELEASE:=

PKG_SOURCE:=
PKG_SOURCE_URL:=
PKG_MD5SUM:=
PKG_CAT:=zcat

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk

define KernelPackage/debug_driver
  SUBMENU:=
  TITLE:=Debug Driver for WM2500RP
  FILES:= \
	$(PKG_BUILD_DIR)/debug_driver.$(LINUX_KMOD_SUFFIX)
  #AUTOLOAD:=$(call AutoLoad,40,debug_driver)
endef

ENET_MAKEOPTS= \
                ENETDIR="$(PKG_BUILD_DIR)" \
                PATH="$(KERNEL_STAGING_DIR)/bin:$(TARGET_PATH)" \
                ARCH="$(LINUX_KARCH)" \
                CROSS_COMPILE="$(TARGET_CROSS)" \
                TOOLPREFIX="mips-linux-" \
                TOOLPATH="mips-linux-"

export CONFIG_TARGET_BOARD

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define KernelPackage/debug_driver/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/init-debug-driver $(1)/etc/init.d
endef

$(eval $(call KernelPackage,debug_driver))
