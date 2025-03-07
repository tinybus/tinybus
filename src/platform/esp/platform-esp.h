// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#ifndef TINYBUS_PLATFORM_ESP_H_
#define TINYBUS_PLATFORM_ESP_H_

#include "tinybus-esp-config.h"
#include <stdint.h>
#include "tinybus/error.h"
#include "tinybus/toolchain/toolchain.h"

/**
 * This function initializes the scheduler service.
 *
 */
tbError tbPlatformSchedulerInit(void);

#endif // TINYBUS_PLATFORM_ESP_H_
