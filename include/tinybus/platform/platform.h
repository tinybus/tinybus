// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 * @brief
 *   This file defines the platform-specific functions needed by TinyBus's example applications.
 */

#ifndef TINYBUS_PLATFORM_H_
#define TINYBUS_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Performs all platform-specific initialization of TinyBus's drivers.
 *
 * @note This function is not called by the TinyBus library. Instead, the system/RTOS should call this function
 *       when initialization of TinyBus's drivers is most appropriate.
 *
 */
void tbPlatformInit(void);

/**
 * Performs all platform-specific deinitialization for TinyBus's drivers.
 *
 * @note This function is not called by the TinyBus library. Instead, the system/RTOS should call this function
 *       when deinitialization of TinyBus's drivers is most appropriate.
 */
void tbPlatformDeinit(void);

#ifdef __cplusplus
} // end of extern "C"
#endif

#endif // TINYBUS_PLATFORM_H_
