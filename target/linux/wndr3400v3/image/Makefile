#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

modelname_val:=WNDR3400v3
ifneq "x$(CONFIG_TARGET_MODELNAME)" "x"
  modelname_val:=$(call qstrip,$(CONFIG_TARGET_MODELNAME))
endif

MODULE_NAME=$(modelname_val)
HW_VERSION=$(modelname_val)
FW_VERSION=$(CONFIG_TARGET_FW_VERSION)
LG_VERSION=$(CONFIG_TARGET_LG_VERSION)
HW_ID=\"$(CONFIG_TARGET_BOARD_HW_ID)\"

ifndef FW_REGION
	FW_REGION=""
endif

HEAD_LENGTH:=$(CONFIG_TARGET_HEAD_LENTGH)

KERNEL_SIZE=$(CONFIG_TARGET_KERNEL_SIZE)
MAX_KERNEL_SIZE=$(CONFIG_TARGET_MAX_KERNEL_SIZE)
MAX_IMAGE_SIZE=$(CONFIG_TARGET_MAX_IMAGE_SIZE)

define imgname
$(BIN_DIR)/$(IMG_PREFIX)-$(2)-$(patsubst jffs2-%,jffs2,$(patsubst squashfs-%,squashfs,$(1)))
endef

IMG_PREFIX:=openwrt-$(BOARD)$(if $(SUBTARGET),-$(SUBTARGET))
PROFILE:=WNDR3400v3

VMLINUX:=$(BIN_DIR)/$(IMG_PREFIX)-vmlinux
UIMAGE:=$(BIN_DIR)/$(IMG_PREFIX)-uImage
fs_squash:=squashfs-only
fs_all:=all
fs_4k:=4k
fs_64k:=64k
fs_128k:=128k
ENTRY=`readelf -a ${VMLINUX}.elf|grep "Entry"|cut -d":" -f 2`
ifeq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),y)
	fs_squash:=initramfs
	fs_all:=initramfs
	fs_4k:=initramfs
	fs_64k:=initramfs
	fs_128k:=initramfs
	VMLINUX:=$(BIN_DIR)/$(IMG_PREFIX)-vmlinux-initramfs
	UIMAGE:=$(BIN_DIR)/$(IMG_PREFIX)-uImage-initramfs
endif

define Image/Prepare
	echo "$(MODULE_NAME)" > $(TARGET_DIR)/module_name
	echo "$(HW_VERSION)" > $(TARGET_DIR)/hardware_version
	echo "$(FW_VERSION)" > $(TARGET_DIR)/firmware_version
	echo "$(FW_REGION)" > $(TARGET_DIR)/firmware_region
	echo "$(LG_VERSION)" > $(TARGET_DIR)/default_language_version
endef

define CompressLzma
  $(STAGING_DIR_HOST)/bin/lzma e $(1) -lc1 -lp2 -pb2 $(2)
endef

define PatchKernelLzma
	cp $(KDIR)/vmlinux $(KDIR)/vmlinux-$(1)
	$(STAGING_DIR_HOST)/bin/patch-cmdline $(KDIR)/vmlinux-$(1) '$(strip $(2))'
	$(call CompressLzma,$(KDIR)/vmlinux-$(1),$(KDIR)/vmlinux-$(1).bin.lzma)
endef

define PatchKernelGzip
	cp $(KDIR)/vmlinux $(KDIR)/vmlinux-$(1)
	$(STAGING_DIR_HOST)/bin/patch-cmdline $(KDIR)/vmlinux-$(1) '$(strip $(2))'
	gzip -9 -c $(KDIR)/vmlinux-$(1) > $(KDIR)/vmlinux-$(1).bin.gz
endef

define MkImageLzma
	mkimage -A mips -O linux -T kernel -a 0x80002000 -C lzma \
		-e $(ENTRY) -n 'MIPS OpenWrt Linux-$(LINUX_VERSION)' \
		-d $(1) $(2)
endef

define MkImageGzip
	mkimage -A mips -O linux -T kernel -a 0x80060000 -C gzip \
		-e 0x80060000 -n 'MIPS OpenWrt Linux-$(LINUX_VERSION)' \
		-d $(1) $(2)
endef

define Image/BuildKernel
	cp $(KDIR)/vmlinux.elf $(VMLINUX).elf
	cp $(KDIR)/vmlinux $(VMLINUX).bin
	gzip -9 -c $(KDIR)/vmlinux > $(KDIR)/vmlinux.bin.gz
	$(call CompressLzma,$(KDIR)/vmlinux,$(KDIR)/vmlinux.bin.lzma)
	dd if=$(KDIR)/vmlinux.bin.lzma of=$(VMLINUX).lzma bs=65536 conv=sync
	dd if=$(KDIR)/vmlinux.bin.gz of=$(VMLINUX).gz bs=65536 conv=sync
	$(call MkImageGzip,$(KDIR)/vmlinux.bin.gz,$(UIMAGE)-gzip.bin)
	$(call MkImageLzma,$(KDIR)/vmlinux.bin.lzma,$(UIMAGE)-lzma.bin)
	$(call Image/Build/Initramfs)
endef

wndr3400v3_mtdlayout=mtdparts=ath-nor0:256k(u-boot)ro,64k(u-boot-env)ro,960k(kernel),6144k(rootfs),512k(language),64k(pot),64k(traffic_meter),64k(config),64k(art),7104k@0x50000(firmware)
define Image/Build/WNDR3400v3
	echo "Image/Build/WNDR3400v3"
	$(call PatchKernelLzma,$(2),$(3) $(wndr3400v3_mtdlayout))
	$(call MkImageLzma,$(KDIR)/vmlinux-$(2).bin.lzma,$(KDIR)/vmlinux-$(2).uImage)
	$(STAGING_DIR_HOST)/bin/board \
		$(KDIR)/vmlinux-$(2).uImage \
		$(KDIR)/vmlinux-$(2).uImage.normal \
		$(CONFIG_TARGET_MAGIC_NUMBER)
	$(call MkImageLzma,$(KDIR)/root.$(1),$(KDIR)/squashfs-$(2).uImage)
	$(STAGING_DIR_HOST)/bin/board \
		$(KDIR)/squashfs-$(2).uImage \
		$(KDIR)/squashfs-$(2).uImage.tmp \
		$(CONFIG_TARGET_MAGIC_NUMBER)
	cat $(KDIR)/vmlinux-$(2).uImage.normal > $(KDIR)/vmlinux-$(2).uImage.tmp
	$(STAGING_DIR_HOST)/../../tools/checksize $(KDIR)/vmlinux-$(2).uImage.tmp $(shell expr $(MAX_KERNEL_SIZE) - 64)
	dd if=$(KDIR)/vmlinux-$(2).uImage.tmp bs=$(shell expr $(MAX_KERNEL_SIZE) - 64) conv=sync of=$(KDIR)/vmlinux-$(2).uImage.final
	dd if=$(KDIR)/squashfs-$(2).uImage.tmp bs=64 count=1 >> $(KDIR)/vmlinux-$(2).uImage.final
	( \
	    dd if=$(KDIR)/vmlinux-$(2).uImage.final bs=$(KERNEL_SIZE) conv=sync; \
	    dd if=$(KDIR)/root.$(1) bs=64k; \
	) > $(call imgname,$(1),$(2))-sysupgrade.bin
	$(STAGING_DIR_HOST)/../../tools/checksize $(call imgname,$(1),$(2))-sysupgrade.bin $(MAX_IMAGE_SIZE)
	$(STAGING_DIR_HOST)/bin/mkdniimg \
		-B $(MODULE_NAME) -v $(FW_VERSION) -r $(FW_REGION) -H $(HW_ID) \
		-i $(call imgname,$(1),$(2))-sysupgrade.bin \
		-o $(BIN_DIR)/$(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).img
endef

define Image/Build/WNDR3400v3/initramfs
	echo "Image/Build/WNDR3400v3/initramfs"
	$(call PatchKernelLzma,$(2),$(3) $(wndr3400v3_mtdlayout))
	$(call MkImageLzma,$(KDIR)/vmlinux-$(2).bin.lzma,$(KDIR)/vmlinux-$(2).uImage)
	$(STAGING_DIR_HOST)/bin/board \
		$(KDIR)/vmlinux-$(2).uImage \
		$(call imgname,$(1),$(2))-uImage.bin \
		$(CONFIG_TARGET_MAGIC_NUMBER)
endef

define Image/Build/Template/initramfs/initramfs
	echo "Image/Build/Template/initramfs/initramfs"
	echo "$(1),$(2),$(3),$(4),$(5),$(6),$(7)"
	$(call Image/Build/$(1)/initramfs,initramfs,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/all/squashfs
	echo "Image/Build/Template/all/squashfs"
	echo "$(1),$(2),$(3),$(4),$(5),$(6),$(7)"
	$(call Image/Build/$(1),squashfs,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/all/jffs2-64k
	$(call Image/Build/$(1),jffs2-64k,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/all/jffs2-128k
	$(call Image/Build/$(1),jffs2-128k,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/squashfs-only/squashfs
	$(call Image/Build/$(1),squashfs,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/4k/squashfs
	$(call Image/Build/$(1),squashfs-4k,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/64k/squashfs
	echo "Image/Build/Template/64k/squashfs"
	echo "$(1),$(2),$(3),$(4),$(5),$(6),$(7)"
	$(call Image/Build/$(1),squashfs,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/64k/jffs2-64k
	echo "Image/Build/Template/64k/jffs2-64k"
	echo "$(1),$(2),$(3),$(4),$(5),$(6),$(7)"
	$(call Image/Build/$(1),jffs2-64k,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/128k/squashfs
	$(call Image/Build/$(1),squashfs,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Template/128k/jffs2-128k
	$(call Image/Build/$(1),jffs2-128k,$(2),$(3),$(4),$(5),$(6),$(7))
endef

define Image/Build/Profile/WNDR3400v3
	echo "Image/Build/Profile/WNDR3400v3"
	echo "1 = $(1)"
	echo "fs_64k = $(fs_64k)"
	$(call Image/Build/Template/$(fs_64k)/$(1),WNDR3400v3,wndr3400v3,board=WNDR3400v3)
endef

define Image/Build/Profile/Default
	$(call Image/Build/Profile/WNDR3400v3,$(1))
endef

define Image/Build/Profile/Madwifi
endef

define Image/Build/squashfs
	echo "Image/Build/squashfs"
	echo "IMG_PREFIX = $(IMG_PREFIX)"
	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
	dd if=$(KDIR)/root.squashfs of=$(KDIR)/root.squashfs-4k.tmp0 bs=4k conv=sync
	$(call add_jffs2_mark,$(KDIR)/root.squashfs-4k.tmp0)
	dd if=$(KDIR)/root.squashfs-4k.tmp0 of=$(BIN_DIR)/$(IMG_PREFIX)-root.squashfs-4k bs=4k conv=sync
	$(call add_jffs2_mark,$(BIN_DIR)/$(IMG_PREFIX)-root.squashfs-4k)
	rm -f $(KDIR)/root.squashfs-4k.tmp0
endef

define Image/Build/Initramfs
	echo "Image/Build/Initramfs"
	$(call Image/Build/Profile/$(PROFILE),initramfs)
endef

define Image/Build
	echo "Image/Build $(1)"
	echo "PROFILE $(PROFILE)"
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-root.$(1) bs=128k conv=sync

	$(call Image/Build/Profile/$(PROFILE),$(1))
endef

$(eval $(call BuildImage))
