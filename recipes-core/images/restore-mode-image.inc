DESCRIPTION ?= "Tiny initramfs image intended to run restore mode operations"
LICENSE = "MIT"

FACTORY_IMAGE ?= "minimal-nilrt-image"

PACKAGE_INSTALL = "${ROOTFS_BOOTSTRAP_INSTALL} \
                   packagegroup-ni-restoremode \
"

do_rootfs[depends] = "${FACTORY_IMAGE}:do_rootfs"

# Remove any kernel-image that the kernel-module-* packages may have pulled in.
PACKAGE_REMOVE = "kernel-image-*"

prune_unused_packages() {
	opkg -o ${IMAGE_ROOTFS} -f ${IPKGCONF_TARGET} --force-depends remove ${PACKAGE_REMOVE};
}

install_payload () {
	install -d ${IMAGE_ROOTFS}/payload
	install -m 0644 ${DEPLOY_DIR_IMAGE}/${FACTORY_IMAGE}-${MACHINE}.tar.bz2  ${IMAGE_ROOTFS}/payload/factory-image.tar.bz2
}

ROOTFS_POSTPROCESS_COMMAND += "prune_unused_packages; install_payload; "

IMAGE_PREPROCESS_COMMAND = "rootfs_update_timestamp;"

# Do not pollute the initrd image with rootfs features
IMAGE_FEATURES = ""

IMAGE_LINGUAS = ""

IMAGE_FSTYPES = "${INITRAMFS_FSTYPES}"
inherit core-image

BAD_RECOMMENDATIONS += "busybox-syslog"