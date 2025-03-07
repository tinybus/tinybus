#  SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
#  SPDX-License-Identifier: Apache-2.0

## Select the target to build (native_sim / nrf52840dk_nrf52840)
TARGET ?= native_sim/native/64

# include common makefile
include ../../../make/makefile.mk

# import settings for the used Zeyphyr Platform
include ../../../make/zephyr-410.mk

ifndef ZEPHYR_BASE
$(error The ZEPHYR_BASE environment variable must be set)
endif

BUILD_DIR ?= build

.PHONY: build clean

# targets
# ---------------------------------------------------------------------------
## Build the firmware
build:
    west build -b ${TARGET} --build-dir ${BUILD_DIR} -p always


## Delete build directory
clean:
    $(RMDIR) $(BUILD_DIR)
