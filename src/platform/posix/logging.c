// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

#include "platform-posix.h"
#include "tinybus/platform/logging.h"

void tbPlatLog(tbLogLevel aLogLevel, const char *aFormat, ...)
{
#if defined(CONFIG_TINYBUS_LOG)
    va_list args;

    switch (aLogLevel)
    {
    case TB_LOG_LEVEL_NONE:
        aLogLevel = LOG_ALERT;
        break;
    case TB_LOG_LEVEL_ERROR:
        aLogLevel = LOG_CRIT;
        break;
    case TB_LOG_LEVEL_WARN:
        aLogLevel = LOG_WARNING;
        break;
    case TB_LOG_LEVEL_INFO:
        aLogLevel = LOG_INFO;
        break;
    case TB_LOG_LEVEL_DEBG:
        aLogLevel = LOG_DEBUG;
        break;
    default:
        aLogLevel = LOG_DEBUG;
        break;
    }

    va_start(args, aFormat);
#if defined(CONFIG_TINYBUS_SYSLOG)
    vsyslog(aLogLevel, aFormat, args);
#else
    vprintf(aFormat, args);
    printf("\n");
#endif
    va_end(args);
#else
    ARG_UNUSED(aLogLevel);
    ARG_UNUSED(aFormat);
#endif
}
