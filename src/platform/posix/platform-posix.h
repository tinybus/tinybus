// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#ifndef PLATFORM_POSIX_H_
#define PLATFORM_POSIX_H_

#include "tinybus-config.h"
#include <stdint.h>
#include "tinybus/error.h"
#include "tinybus/toolchain/toolchain.h"

/**
 * This function initializes the scheduler service.
 *
 */
tbError tiPlatformSchedulerInit(void);

#endif // PLATFORM_POSIX_H_
