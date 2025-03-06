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
#define TB_EVENT(event, data, len) (&(Event){event, data, len})
#define TB_SUBSCRIBER(module, table, tableRowCount, startState) \
    (&(Subscriber){module, table, tableRowCount, startState, NULL})

// ARRAY_SIZE ... (be careful, this is base on sizeof, only use on arrays)
#define TB_TABLE_ROW_COUNT(x) (sizeof(x) / sizeof((x)[0]))

typedef struct Event
{
    const char *event;
    void       *data;
    size_t      dataLen;
} Event;

typedef void (*StateActionFn)(const Event *aEvent);

typedef bool (*StateConditionFn)();

typedef struct StateTableRow
{
    const char      *event;
    const char      *state;
    StateConditionFn conditionCheck;
    StateActionFn    entryAction;
    const char      *nextState;
    StateActionFn    exitAction;
    bool             stop;
} StateTableRow;

typedef struct Subscriber
{
    const char *const          name;
    const StateTableRow *const table;
    const size_t               tableRowCount;
    const char                *currentState;
    StateActionFn              exitAction;
} Subscriber;

tbError tbSubscribe(Subscriber *aSubscriber);

tbError tbPublish(const Event *aEvent);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINYBUS_TINYBUS_H
