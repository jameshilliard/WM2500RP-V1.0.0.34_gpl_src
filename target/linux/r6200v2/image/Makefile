# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

modelname_val:=r6200v2
ifneq "x$(CONFIG_TARGET_MODELNAME)" "x"
  modelname_val:=$(call qstrip,$(CONFIG_TARGET_MODELNAME))
endif

MODULE_NAME=$(modelname_val)
HW_VERSION=$(CONFIG_TARGET_BOARD_HW_ID)
FW_VERSION=$(CONFIG_TARGET_FW_VERSION)
LG_VERSION=$(CONFIG_TARGET_LG_VERSION)
ifndef FW_REGION
  FW_REGION=""
endif
HW_ID=$(CONFIG_TARGET_BOARD_HW_ID)

define Build/Clean
	$(MAKE) -C lzma-loader clean
endef

define STRIP_DEBUG_SYMBOLS
	dbgsymf=$(basename $(1))_dbgsym$$(suffix $(1)); \
	if [ "$(1)" -nt "$$$${dbgsymf}" ]; then \
		echo "#- $0"; \
		ls -ln $1 | awk '{printf "Orig  size: %10d bytes, %s\n",$$$$5,$$$$NF}'; \
		cp -p -v $1 $$$$dbgsymf; $(TARGET_CROSS)strip -d $(1); touch $$$$dbgsymf; \
		ls -ln $1 | awk '{printf "Strip size: %10d bytes, %s\n",$$$$5,$$$$NF}'; \
	fi
endef

define Image/Prepare
	# Preserve the debug versions of these and strip for release
	# $(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/bcm57xx/bcm57xx.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/wl/wl.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/et/et.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/ctf/ctf.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/emf/emf.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/igs/igs.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/proxyarp/proxyarp.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/net/dpsta/dpsta.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/usb/host/ohci-hcd.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/usb/host/ehci-hcd.ko)
	-$(call STRIP_DEBUG_SYMBOLS,$(LINUX_DIR)/drivers/usb/host/xhci-hcd.ko)
	-mkdir -p $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)
	# $(CP) $(LINUX_DIR)/drivers/net/bcm57xx/bcm57xx.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/net/wl/wl.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/net/et/et.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/net/ctf/ctf.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/net/emf/emf.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/net/igs/igs.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/net/proxyarp/proxyarp.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/net/dpsta/dpsta.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/usb/host/ohci-hcd.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/usb/host/ehci-hcd.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/
	-$(CP) $(LINUX_DIR)/drivers/usb/host/xhci-hcd.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/

	echo "$(MODULE_NAME)" > $(TARGET_DIR)/module_name
	echo "$(MODULE_NAME)" > $(TARGET_DIR)/hardware_version
	echo "$(HW_ID)" > $(TARGET_DIR)/hardware_id
	echo "$(FW_VERSION)" > $(TARGET_DIR)/firmware_version
	echo "$(FW_REGION)" > $(TARGET_DIR)/firmware_region
	echo "$(LG_VERSION)" > $(TARGET_DIR)/default_language_version

endef

ifneq ($(KERNEL),2.4)
  define Image/Build/wgt634u
	dd if=$(KDIR)/loader.elf of=$(BIN_DIR)/openwrt-wgt634u-$(KERNEL)-$(2).bin bs=131072 conv=sync
	cat $(BIN_DIR)/openwrt-$(BOARD)-$(KERNEL)-$(1).trx >> $(BIN_DIR)/openwrt-wgt634u-$(KERNEL)-$(2).bin
  endef
endif

define Image/Build/CyberTAN
	$(STAGING_DIR)/bin/addpattern -4 -p $(3) -v v$(4) -i $(BIN_DIR)/openwrt-$(BOARD)-$(KERNEL)-$(1).trx -o $(BIN_DIR)/openwrt-$(2)-$(KERNEL)-$(5).bin
endef

define Image/Build/Motorola
	$(STAGING_DIR)/bin/motorola-bin -$(3) $(BIN_DIR)/openwrt-$(BOARD)-$(KERNEL)-$(1).trx $(BIN_DIR)/openwrt-$(2)-$(4).bin
endef

define Image/Build/USR
	$(STAGING_DIR)/bin/trx2usr $(BIN_DIR)/openwrt-$(BOARD)-$(KERNEL)-$(1).trx $(BIN_DIR)/openwrt-$(2)-$(3).bin
endef

define trxalign/jffs2-128k
-a 0x20000 -f $(KDIR)/root.$(1)
endef
define trxalign/jffs2-64k
-a 0x10000 -f $(KDIR)/root.$(1)
endef
define trxalign/squashfs
-a 1024 -f $(KDIR)/root.$(1) -a 0x10000 -A $(KDIR)/fs_mark
endef


define Image/Build/jffs2-128k
	$(call Image/Build/CyberTAN,$(1),wrt54gs,W54S,4.80.1,$(patsubst jffs2-%,jffs2,$(1)))
	$(call Image/Build/CyberTAN,$(1),wrtsl54gs,W54U,2.08.1,$(patsubst jffs2-%,jffs2,$(1)))
  ifeq ($(KERNEL),2.6)
	$(call Image/Build/wgt634u,$(1),$(patsubst jffs2-%,jffs2,$(1)))
  endif
endef

define Image/Build/jffs2-64k
	$(call Image/Build/CyberTAN,$(1),wrt54g3g,W54F,2.01.1,$(patsubst jffs2-%,jffs2,$(1)))
	$(call Image/Build/CyberTAN,$(1),wrt54g,W54G,4.60.1,$(patsubst jffs2-%,jffs2,$(1)))
	$(call Image/Build/CyberTAN,$(1),wrt54gs_v4,W54s,1.09.1,$(patsubst jffs2-%,jffs2,$(1)))
	$(call Image/Build/CyberTAN,$(1),wrt150n,N150,1.00.5,$(patsubst jffs2-%,jffs2,$(1)))
	$(call Image/Build/CyberTAN,$(1),wrt300n_v1,EWCB,0.93.10,$(patsubst jffs2-%,jffs2,$(1)))
	$(call Image/Build/Motorola,$(1),wa840g,2,$(patsubst jffs2-%,jffs2,$(1)))
	$(call Image/Build/Motorola,$(1),we800g,3,$(patsubst jffs2-%,jffs2,$(1)))
endef

define Image/Build/squashfs
	$(call Image/Build/jffs2-64k,$(1))
	$(call Image/Build/jffs2-128k,$(1))
endef

define Image/Build
	$(STAGING_DIR_HOST)/bin/mksquashfs-brcm $(TARGET_DIR) $(KDIR)/target.squashfs -noappend -all-root
	$(MAKE) CROSS_COMPILE=$(TARGET_CROSS) LD=$(TARGET_CROSS)ld CC=$(TARGET_CROSS)gcc LINUXDIR=$(LINUX_DIR) \
		CONFIG_SQUASHFS=y ARCH=arm SRCBASE=$(BUILD_DIR_BASE) PATH="$(BUILD_DIR)/tools":$(PATH) \
		-C $(BUILD_DIR_BASE)/router/compressed

	$(BUILD_DIR)/tools/trx -o $(BIN_DIR)/$(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).trx $(BUILD_DIR_BASE)/router/compressed/vmlinuz $(KDIR)/target.squashfs

	$(STAGING_DIR_HOST)/bin/mkdniimg -B $(MODULE_NAME) -v $(FW_VERSION) -r $(FW_REGION) -H $(HW_ID) \
		-i $(BIN_DIR)/$(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).trx \
		-o $(BIN_DIR)/$(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).img
	-cd $(BIN_DIR) && md5sum $(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).img > $(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).md5.txt
	-cd $(BIN_DIR) && zip $(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).zip $(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).img $(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).md5.txt
endef

$(eval $(call BuildImage))
