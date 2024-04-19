SUMMARY = "This is a base image created as example for courses"

IMAGE_LINGUAS = " "

LICENSE = "MIT"

inherit core-image

export IMAGE_BASENAME = "Seneca-Custom-Image"
MACHINE_NAME ?= "${MACHINE}"
IMAGE_NAME = "${MACHINE_NAME}_${IMAGE_BASENAME}"

IMAGE_INSTALL:append = " kernel-devsrc"

IMAGE_INSTALL:append = " \
    usbutils \
    usbinit \
    i2c-tools \
    libgpiod \
    gamepad-app \
    "

IMAGE_FEATURES:append = " \
    ssh-server-openssh \
    splash \
    tools-debug \
    tools-sdk \
    "

DISTRO_FEATURES:append = " usbgadget"
MACHINE_FEATURES:append = " usbgadget usabhost"
