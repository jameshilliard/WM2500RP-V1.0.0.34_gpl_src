#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

modelname_val:=jndr2210
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

KERNEL_SIZE=1536k
MAX_KERNEL_SIZE=1572864
MAX_IMAGE_SIZE=14876672

ROOT_FLASH_OFFSET=1D0000

#the squashfs4 tool for jndr2210 has been patched to v4.1 , the command line also need to change.
SQUASHFS_OPTS:= -comp lzma -always-use-fragments

define imgname
$(BIN_DIR)/$(IMG_PREFIX)-$(2)-$(patsubst jffs2-%,jffs2,$(patsubst squashfs-%,squashfs,$(1)))
endef

IMG_PREFIX:=openwrt-$(BOARD)$(if $(SUBTARGET),-$(SUBTARGET))
PROFILE:=JNDR2210

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

define MkImageLzma
	mkimage -A mips -O linux -T kernel -a 0x80002000 -C lzma \
		-e $(ENTRY) -n 'MIPS OpenWrt Linux-$(LINUX_VERSION)' \
		-d $(1) $(2)
endef

define MkImageGzip
	mkimage -A mips -O linux -T kernel -a 0x80002000 -C gzip \
		-e $(ENTRY) -n 'MIPS OpenWrt Linux-$(LINUX_VERSION)' \
		-d $(1) $(2)
endef

define Image/BuildKernel
	echo "============Create linux.bin====================="
	make -C $(KDIR)/linux-$(LINUX_VERSION)/rtkload
	$(CP) $(KDIR)/linux-$(LINUX_VERSION)/rtkload/linux.bin  $(BIN_DIR)/
endef

define Image/Build/JNDR2210
	echo "Image/Build/JNDR2210"
	$(STAGING_DIR_HOST)/../../tools/checksize $(BIN_DIR)/linux.bin $(MAX_KERNEL_SIZE)
	$(CONFIG_TOOLCHAIN_BIN_PATH)/cvimg root $(BIN_DIR)/root.squashfs $(BIN_DIR)/root.bin $(ROOT_FLASH_OFFSET) $(ROOT_FLASH_OFFSET)
	$(CONFIG_TOOLCHAIN_BIN_PATH)/mgbin -c -o $(BIN_DIR)/fw.bin $(BIN_DIR)/root.bin $(BIN_DIR)/linux.bin
	$(STAGING_DIR_HOST)/../../tools/checksize $(BIN_DIR)/fw.bin $(MAX_IMAGE_SIZE)
	$(STAGING_DIR_HOST)/bin/mkdniimg \
		-B $(MODULE_NAME) -v $(FW_VERSION) -r $(FW_REGION) -H $(HW_ID) \
		-i $(BIN_DIR)/fw.bin \
		-o $(BIN_DIR)/$(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).img
endef

define Image/Build/JNDR2210/initramfs
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

define Image/Build/Profile/JNDR2210
	echo "Image/Build/Profile/JNDR2210"
	echo "1 = $(1)"
	echo "fs_64k = $(fs_64k)"
	$(call Image/Build/Template/$(fs_64k)/$(1),JNDR2210,jndr2210,board=JNDR2210)
endef

define Image/Build/Profile/Default
	$(call Image/Build/Profile/JNDR2210,$(1))
endef

define Image/Build/Profile/Madwifi
endef

define Image/Build/squashfs
	echo "Image/Build/squashfs"
	echo "IMG_PREFIX = $(IMG_PREFIX)"
	mv $(KDIR)/root.squashfs $(BIN_DIR)/
	$(call prepare_generic_squashfs,$(BIN_DIR)/root.squashfs)
	dd if=$(BIN_DIR)/root.squashfs of=$(BIN_DIR)/root.squashfs-4k.tmp0 bs=4k conv=sync
	$(call add_jffs2_mark,$(BIN_DIR)/root.squashfs-4k.tmp0)
	dd if=$(BIN_DIR)/root.squashfs-4k.tmp0 of=$(BIN_DIR)/$(IMG_PREFIX)-root.squashfs-4k bs=4k conv=sync
	$(call add_jffs2_mark,$(BIN_DIR)/$(IMG_PREFIX)-root.squashfs-4k)
	rm -f $(BIN_DIR)/root.squashfs-4k.tmp0
endef

define Image/Build/Initramfs
	echo "Image/Build/Initramfs"
	$(call Image/Build/Profile/$(PROFILE),initramfs)
endef

define Image/Build
	echo "Image/Build $(1)"
	echo "PROFILE $(PROFILE)"
	$(call Image/Build/$(1))
	dd if=$(BIN_DIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-root.$(1) bs=128k conv=sync

	$(call Image/Build/Profile/$(PROFILE),$(1))
endef

$(eval $(call BuildImage))
