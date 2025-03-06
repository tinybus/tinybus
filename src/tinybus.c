// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TinyBus implementation
 */
#include <stdbool.h>
#include <stdint.h>
#include "tinybus/platform/logging.h"

#include "tinybus-config.h"
#include "tinybus/platform/platform.h"
#include "tinybus/platform/scheduler.h"
#include "tinybus/tinybus.h"

static bool handleEvent(const Subscriber *aSubscriber, const Event *aEvent)
{
    uint8_t i;
    bool    eventHandled = false;

    for (i = 0; i < aSubscriber->tableRowCount; i++)
    {
        const StateTableRow *row = &aSubscriber->table[i];

        if ((aEvent->event == row->event) && ((aSubscriber->currentState == row->state) || (row->state == NULL)) &&
            ((row->conditionCheck == NULL || (row->conditionCheck)())))
        {
            // call exit action from last state (only, if the next state is new)
            if (aSubscriber->exitAction != NULL && row->nextState != NULL)
            {
                (aSubscriber->exitAction)(aEvent);
            }

            // call entry action from new state
            if (row->entryAction != NULL)
            {
                (row->entryAction)(aEvent);
            }

            // set current state
            if (row->nextState != NULL)
            {
                *(const char **)aSubscriber->currentState = row->nextState;
            }

            // set exit action for next state (only, if the next state is new)
            if (row->nextState != NULL)
            {
                *(StateActionFn *)aSubscriber->exitAction = row->exitAction;
            }
            eventHandled = true;
            // break if needed
            if (row->stop)
            {
                break;
            }
        }
    }

    return eventHandled;
}

static size_t      mSubscriberCount;
static Subscriber *mSubscriber[CONFIG_TINYBUS_MAX_SUBSCRIBERS];
static bool        mInitialized;

static void onSchedulerNotify(const Event *aEvent)
{
    bool eventHandled = false;
    for (size_t i = 0; i < mSubscriberCount; i++)
    {
        eventHandled |= handleEvent(mSubscriber[i], aEvent);
    }
    /* // Free event slot */
    /* if (event.data != NULL) */
    /* { */
    /*     tb_free((uint8_t *)event.data); */
    /*     event.data = NULL; */
    /* } */
    if (!eventHandled)
    {
        /* tbPlatLog(TB_LOG_LEVEL_DEBG, "No match for event '%s' found", aEvent->event); */
        tbPlatLog(TB_LOG_LEVEL_DEBG, "No match for event found");
    }
}

static void init()
{
    // initialize event matrix
    mSubscriberCount = 0;
    tbPlatformInit();
    // register callback to scheduler implementation
    tbOnSchedulerEvent(onSchedulerNotify);
    mInitialized = true;
}

tbError tbPublish(const Event *aEvent)
{
    // we need to copy the data within the event to the heap
    // it will be freed, after the event is processed
    /* void *dataCopy = tbMalloc(apEvent->dataLen); */
    /* memcpy(dataCopy, apEvent->data, apEvent->dataLen); */
    /* Event event = {.event = apEvent->event, .data = dataCopy, .dataLen = apEvent->dataLen}; */
    // FreeRTOS queue makes a copy of the data, it's save to use
    // a local variable (event)
    tbSchedulerEventPush(aEvent);

    return TB_ERROR_NONE;
}

tbError tbSubscribe(Subscriber *aSubscriber)
{
    // let the first subscription initialize the bus
    if (!mInitialized)
    {
        init();
    }
    if (aSubscriber == NULL || aSubscriber->tableRowCount == 0)
    {
        tbPlatLog(TB_LOG_LEVEL_ERROR, "Subscriber NULL");
        return TB_ERROR_NONE;
    }
    if (aSubscriber->tableRowCount == 0)
    {
        tbPlatLog(TB_LOG_LEVEL_DEBG, "tableRowCount == 0");
        return TB_ERROR_NONE;
    }
    // test if we have slots left
    if (mSubscriberCount + 1 >= CONFIG_TINYBUS_MAX_SUBSCRIBERS)
    {
        tbPlatLog(TB_LOG_LEVEL_WARN, "Max count for subscribers reached");
        return TB_ERROR_SUBSCRIBER_COUNT_EXCEEDED;
    }
    mSubscriber[mSubscriberCount]             = aSubscriber;
    mSubscriber[mSubscriberCount]->exitAction = NULL;

    mSubscriberCount++;
    return TB_ERROR_NONE;
};
