// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   Main include for C based applications
 */

#ifndef TINYBUS_TINYBUS_H
#define TINYBUS_TINYBUS_H

#include <stdbool.h>
#include <stddef.h>

#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

// Macro for genereting a const char* from a string literal
#define TB_EVENT_NAME(tag, name, value) const char *const tag##_EVENT_##name = value;
#define TB_STATE_NAME(tag, name, value) const char *const tag##_STATE_##name = value;
#define TB_EVENT(event, data, len) (&(TbEvent){event, data, len})
#define TB_SUBSCRIBER(module, table, tableRowCount, startState) \
    (&(TbSubscriber){module, table, tableRowCount, startState, NULL})

// ARRAY_SIZE ... (be careful, this is base on sizeof, only use on arrays)
#define TB_TABLE_ROW_COUNT(x) (sizeof(x) / sizeof((x)[0]))

typedef struct TbEvent
{
    const char *event;
    void       *data;
    size_t      dataLen;
} TbEvent;

typedef void (*TbStateActionFn)(const TbEvent *aEvent);

typedef bool (*TbStateConditionFn)();

typedef struct TbStateTableRow
{
    const char        *event;
    const char        *state;
    TbStateConditionFn conditionCheck;
    TbStateActionFn    entryAction;
    const char        *nextState;
    TbStateActionFn    exitAction;
    bool               stop;
} TbStateTableRow;

typedef struct TbSubscriber
{
    const char *const            name;
    const TbStateTableRow *const table;
    const size_t                 tableRowCount;
    const char                  *currentState;
    TbStateActionFn              exitAction;
} TbSubscriber;

tbError tbSubscribe(TbSubscriber *aSubscriber);

tbError tbPublish(const TbEvent *aEvent);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINYBUS_TINYBUS_H
