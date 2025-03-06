#  SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
#  SPDX-License-Identifier: Apache-2.0

## Select the target to build (native_sim / nrf52840dk_nrf52840)
BOARD ?= native_sim/native/64

# include common makefile
include ../../../make/makefile.mk

# import settings for the used ESP-IDF
include ../../../make/zephyr-400.mk

ifndef ZEPHYR_BASE
$(error The ZEPHYR_BASE environment variable must be set)
endif

BUILDDIR ?= build

.PHONY: build debug-build flash recover clean index

# targets
# ---------------------------------------------------------------------------
## Build the firmware
build:
    west build -b ${BOARD} --build-dir ${BUILDDIR} -p always


## Delete build directory
clean:
    $(RMDIR) $(BUILDDIR)
