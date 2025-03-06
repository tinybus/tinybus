// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   This file includes the platform abstraction for the scheduler service.
 */

#ifndef TINYBUS_PLATFORM_SCHEDULER_H_
#define TINYBUS_PLATFORM_SCHEDULER_H_

#include <stdarg.h>
#include <stdint.h>
#include "tinybus/tinybus.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*SchedulerNotifyFn)(const Event *aEvent);

/**
 * @addtogroup plat-scheduler
 *
 * @brief
 *   This module includes the platform abstraction for the debug log service.
 *
 * @{
 */

/**
 * add event to queue
 *
 * @param[in]  aLogLevel   The log level.
 * @param[in]  aFormat     A pointer to the format string.
 * @param[in]  ...         Arguments for the format specification.
 */
void tbSchedulerEventPush(const Event *aEvent);

void tbOnSchedulerEvent(SchedulerNotifyFn aNotifyFn);

/**
 * @}
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINYBUS_PLATFORM_SCHEDULER_H_
