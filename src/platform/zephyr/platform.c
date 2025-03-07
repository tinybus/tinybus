// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   This file includes the platform-specific initializers.
 */

#include "platform-zephyr.h"

void tbPlatformInit(void)
{
    tbPlatformSchedulerInit();
}
