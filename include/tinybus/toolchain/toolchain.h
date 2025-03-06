// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 * @brief Macros to abstract toolchain specific capabilities
 *
 * This file contains various macros to abstract compiler capabilities that
 * utilize toolchain specific attributes and/or pragmas.
 */

#ifndef TINYBUS_INCLUDE_TOOLCHAIN_H_
#define TINYBUS_INCLUDE_TOOLCHAIN_H_

#if defined(__XCC__)
#include <tinybus/toolchain/xcc.h>
#elif defined(__CCAC__)
#include <tinybus/toolchain/mwdt.h>
#elif defined(__ARMCOMPILER_VERSION)
#include <tinybus/toolchain/armclang.h>
#elif defined(__IAR_SYSTEMS_ICC__)
#include <tinybus/toolchain/iar.h>
#elif defined(__llvm__) || (defined(_LINKER) && defined(__LLD_LINKER_CMD__))
#include <tinybus/toolchain/llvm.h>
#elif defined(__GNUC__) || (defined(_LINKER) && defined(__GCC_LINKER_CMD__))
#include <tinybus/toolchain/gcc.h>
#else
#error "Invalid/unknown toolchain configuration"
#endif

#endif /* TINYBUS_INCLUDE_TOOLCHAIN_H_ */
