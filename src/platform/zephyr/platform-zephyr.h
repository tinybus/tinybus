// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
#ifndef PLATFORM_ZEPHYR_H_
#define PLATFORM_ZEPHYR_H_

#include "tinybus-config.h"
#include <stdint.h>
#include "tinybus/error.h"
#include "tinybus/toolchain/toolchain.h"

/**
 * This function initializes the scheduler service.
 *
 */
tbError tbPlatformSchedulerInit(void);

#endif // PLATFORM_ZEPHYR_H_
