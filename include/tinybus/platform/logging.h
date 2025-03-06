// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   This file includes the platform abstraction for the debug log service.
 */

#ifndef TINYBUS_PLATFORM_LOGGING_H_
#define TINYBUS_PLATFORM_LOGGING_H_

#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup plat-logging
 *
 * @brief
 *   This module includes the platform abstraction for the debug log service.
 *
 * @{
 */

/**
 * Log level None.
 *
 * @note Log Levels are defines so that embedded implementations can eliminate code at compile time via
 * #if/#else/#endif.
 */
#define TB_LOG_LEVEL_NONE 0

/**
 * Log level Critical.
 *
 * @note Log Levels are defines so that embedded implementations can eliminate code at compile time via
 * #if/#else/#endif.
 */
#define TB_LOG_LEVEL_ERROR 1

/**
 * Log level Warning.
 *
 * @note Log Levels are defines so that embedded implementations can eliminate code at compile time via
 * #if/#else/#endif.
 */
#define TB_LOG_LEVEL_WARN 2

/**
 * Log level Informational.
 *
 * @note Log Levels are defines so that embedded implementations can eliminate code at compile time via
 * #if/#else/#endif.
 */
#define TB_LOG_LEVEL_INFO 3

/**
 * Log level Debug.
 *
 * @note Log Levels are defines so that embedded implementations can eliminate code at compile time via
 * #if/#else/#endif.
 */
#define TB_LOG_LEVEL_DEBG 4

/**
 * Represents the log level.
 */
typedef int tbLogLevel;

/**
 * Outputs logs.
 *
 * @param[in]  aLogLevel   The log level.
 * @param[in]  aFormat     A pointer to the format string.
 * @param[in]  ...         Arguments for the format specification.
 */
void tbPlatLog(tbLogLevel aLogLevel, const char *aFormat, ...);

/**
 * @}
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINYBUS_PLATFORM_LOGGING_H_
