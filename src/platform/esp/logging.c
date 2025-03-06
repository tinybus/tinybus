// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   This file includes the platform abstraction for the debug log service.
 */
#include <esp_log.h>
#include <stdarg.h>
#include <stdio.h>

#include "platform-esp.h"
#include "tinybus/platform/logging.h"

static const char *TAG = "tinybus";

/* Convert OT log level to zephyr log level. */
static inline int log_translate(tbLogLevel aLogLevel)
{
    switch (aLogLevel)
    {
    case TB_LOG_LEVEL_NONE:
    case TB_LOG_LEVEL_ERROR:
        return ESP_LOG_ERROR;
    case TB_LOG_LEVEL_WARN:
        return ESP_LOG_WARN;
    case TB_LOG_LEVEL_INFO:
        return ESP_LOG_INFO;
    case TB_LOG_LEVEL_DEBG:
        return ESP_LOG_DEBUG;
    default:
        break;
    }

    return -1;
}

void tbPlatLog(tbLogLevel aLogLevel, const char *aFormat, ...)
{
#if defined(CONFIG_TINYBUS_LOG)
    char    buffer[1024];
    int     level = log_translate(aLogLevel);
    va_list param_list;
    if (level < 0)
    {
        return;
    }

    va_start(param_list, aFormat);
    vsnprintf(buffer, sizeof(buffer), aFormat, param_list);
    ESP_LOG_LEVEL(level, TAG, "%s", buffer);
    va_end(param_list);
#else
    ARG_UNUSED(aLogLevel);
    ARG_UNUSED(aFormat);
#endif
}
