# gamepad-app_1.0.bb
SUMMARY = "User space application for Adafruit gamepad"
DESCRIPTION = "This user space application communicates with Adafruit gamepad via I2C."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

FILESEXTRAPATHS:prepend := "${THISDIR}/gamepad-app/files:"
SRC_URI = "file://gamepad_app.c"

S = "${WORKDIR}"

do_compile() {
    ${CC} gamepad_app.c -o gamepad-app ${LDFLAGS}
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 gamepad-app ${D}${bindir}
}

FILES_${PN} += "${bindir}/gamepad-app"
