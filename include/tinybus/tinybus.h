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

#include <tiny/error.h>

#ifdef __cplusplus
extern "C" {
#endif

// Macro for genereting a const char* from a string literal
#define TINY_EVENT_NAME(tag, name, value) const char *const tag##_EVENT_##name = value;
#define TINY_STATE_NAME(tag, name, value) const char *const tag##_STATE_##name = value;
#define TINY_EVENT(event, data, len) (&(TinyEvent){event, data, len})
#define TINY_SUBSCRIBER(module, table, tableRowCount, startState) \
    (&(TinySubscriber){module, table, tableRowCount, startState, NULL})

// ARRAY_SIZE ... (be careful, this is base on sizeof, only use on arrays)
#define TINY_TABLE_ROW_COUNT(x) (sizeof(x) / sizeof((x)[0]))

typedef struct TinyEvent
{
    const char *event;
    void       *data;
    size_t      dataLen;
} TinyEvent;

typedef void (*TinyStateActionFn)(const TinyEvent *aEvent);

typedef bool (*TinyStateConditionFn)();

typedef struct TinyStateTableRow
{
    const char          *event;
    const char          *state;
    TinyStateConditionFn conditionCheck;
    TinyStateActionFn    entryAction;
    const char          *nextState;
    TinyStateActionFn    exitAction;
    bool                 stop;
} TinyStateTableRow;

typedef struct TinySubscriber
{
    const char *const              name;
    const TinyStateTableRow *const table;
    const size_t                   tableRowCount;
    const char                    *currentState;
    TinyStateActionFn              exitAction;
} TinySubscriber;

tinyError tinySubscribe(TinySubscriber *aSubscriber);

tinyError tinyPublish(const TinyEvent *aEvent);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINYBUS_TINYBUS_H
