// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   Main include for C based applications
 */

#ifndef TYBUS_TYBUS_H
#define TYBUS_TYBUS_H

#include <stdbool.h>
#include <stddef.h>

#include <tiny/error.h>

#ifdef __cplusplus
extern "C" {
#endif

// Macro for genereting a const char* from a string literal
#define TY_EVENT_NAME(tag, name, value) const char *const tag##_EVENT_##name = value;
#define TY_STATE_NAME(tag, name, value) const char *const tag##_STATE_##name = value;
#define TY_EVENT(event, data, len) ((TinyEvent){event, data, len})
#define TY_SUBSCRIBER(module, table, tableRowCount, startState) \
    ((TinySubscriber){module, table, tableRowCount, startState, NULL})

// ARRAY_SIZE ... (be careful, this is base on sizeof, only use on arrays)
#define TY_TABLE_ROW_COUNT(x) (sizeof(x) / sizeof((x)[0]))

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
    const char          *state;
    const char          *event;
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

tinyError tyPublish(const char *aEventName, void *aData, size_t aDataLen);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TYBUS_TYBUS_H
