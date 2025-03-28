// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TyBus implementation
 */
#include <stdbool.h>
#include <stdint.h>

#include "tybus-config.h"
#include <ty/logging.h>
#include <ty/platform.h>
#include "tybus/platform/scheduler.h"
#include "tybus/tybus.h"

static bool handleEvent(const TinySubscriber *aSubscriber, const TinyEvent *aEvent)
{
    uint8_t i;
    bool    eventHandled = false;

    for (i = 0; i < aSubscriber->tableRowCount; i++)
    {
        const TinyStateTableRow *row = &aSubscriber->table[i];

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
                *(TinyStateActionFn *)aSubscriber->exitAction = row->exitAction;
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

static size_t          mSubscriberCount;
static TinySubscriber *mSubscriber[CONFIG_TYBUS_MAX_SUBSCRIBERS];
static bool            mInitialized;
static void            onSchedulerNotify(const TinyEvent *aEvent)
{
    if (aEvent == NULL || aEvent->event == NULL)
    {
        tyPlatLog(TY_LOG_LEVEL_CRIT, "bus", "Event is NULL");
        return;
    }
    bool eventHandled = false;
    for (size_t i = 0; i < mSubscriberCount; i++)
    {
        eventHandled |= handleEvent(mSubscriber[i], aEvent);
    }
    /* // Free event slot */
    /* if (event.data != NULL) */
    /* { */
    /*     tinyFree((uint8_t *)event.data); */
    /*     event.data = NULL; */
    /* } */
    if (!eventHandled)
    {
        tyPlatLog(TY_LOG_LEVEL_DEBG, "bus", "No match for event '%s' found", aEvent->event);
        /* tyPlatLog(TY_LOG_LEVEL_DEBG, "bus", "No match for event found"); */
    }
}

static void init()
{
    // initialize event matrix
    mSubscriberCount = 0;
    tinySchedulerInit();
    // register callback to scheduler implementation
    tinyOnSchedulerEvent(onSchedulerNotify);
    mInitialized = true;
}

tinyError tyPublish(const char *aEventName, void *aData, size_t aDataLen)
{
    // we need to copy the data within the event to the heap
    // it will be freed, after the event is processed
    /* void *dataCopy = tbMalloc(apEvent->dataLen); */
    /* memcpy(dataCopy, apEvent->data, apEvent->dataLen); */
    /* Event event = {.event = apEvent->event, .data = dataCopy, .dataLen = apEvent->dataLen}; */
    // FreeRTOS queue makes a copy of the data, it's save to use
    // a local variable (event)
    TinyEvent event = {.event = aEventName, .data = aData, .dataLen = aDataLen};
    tinySchedulerEventPush(&event);

    return TY_ERROR_NONE;
}

tinyError tinySubscribe(TinySubscriber *aSubscriber)
{
    // let the first subscription initialize the bus
    if (!mInitialized)
    {
        init();
    }
    if (aSubscriber == NULL || aSubscriber->tableRowCount == 0)
    {
        tyPlatLog(TY_LOG_LEVEL_CRIT, "bus", "Subscriber NULL");
        return TY_ERROR_NONE;
    }
    if (aSubscriber->tableRowCount == 0)
    {
        tyPlatLog(TY_LOG_LEVEL_DEBG, "bus", "tableRowCount == 0");
        return TY_ERROR_NONE;
    }
    // test if we have slots left
    if (mSubscriberCount + 1 >= CONFIG_TYBUS_MAX_SUBSCRIBERS)
    {
        tyPlatLog(TY_LOG_LEVEL_WARN, "bus", "Max count for subscribers reached");
        return TY_ERROR_SUBSCRIBER_COUNT_EXCEEDED;
    }
    mSubscriber[mSubscriberCount]             = aSubscriber;
    mSubscriber[mSubscriberCount]->exitAction = NULL;

    mSubscriberCount++;
    return TY_ERROR_NONE;
};
