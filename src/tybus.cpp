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
#include "platform/scheduler.hpp"
#include "tybus/tybus.h"

static bool handleEvent(const TyBusSubscriber *aSubscriber, const TyBusEvent *aEvent)
{
    uint8_t i;
    bool    eventHandled = false;

    for (i = 0; i < aSubscriber->tableRowCount; i++)
    {
        const TyBusStateTableRow *row = &aSubscriber->table[i];

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
                *(TyBusStateActionFn *)aSubscriber->exitAction = row->exitAction;
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

static size_t           mSubscriberCount;
static TyBusSubscriber *mSubscriber[CONFIG_TYBUS_MAX_SUBSCRIBERS];
static bool             mInitialized;
static void             onSchedulerNotify(const TyBusEvent *aEvent)
{
    if (aEvent == NULL || aEvent->event == NULL)
    {
        tyLogCrit("bus", "Event is NULL");
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
        if (aEvent->data != NULL)
        {
            tyLogDebg("bus", "No match for event '%s' found", aEvent->event);
        }
        else
        {
            tyLogDebg("bus", "No match for event 'NULL' found");
        }
    }
}

static void init()
{
    // initialize event matrix
    mSubscriberCount = 0;
    tyBusSchedulerInit();
    // register callback to scheduler implementation
    tyBusOnSchedulerEvent(onSchedulerNotify);
    mInitialized = true;
}

tinyError tyBusPublish(const char *aEventName, void *aData, size_t aDataLen)
{
    // we need to copy the data within the event to the heap
    // it will be freed, after the event is processed
    /* void *dataCopy = tbMalloc(apEvent->dataLen); */
    /* memcpy(dataCopy, apEvent->data, apEvent->dataLen); */
    /* Event event = {.event = apEvent->event, .data = dataCopy, .dataLen = apEvent->dataLen}; */
    // FreeRTOS queue makes a copy of the data, it's save to use
    // a local variable (event)
    TyBusEvent event = {.event = aEventName, .data = aData, .dataLen = aDataLen};
    tyBusSchedulerEventPush(event);

    return TY_ERROR_NONE;
}

tinyError tyBusSubscribe(TyBusSubscriber *aSubscriber)
{
    // let the first subscription initialize the bus
    if (!mInitialized)
    {
        init();
    }
    if (aSubscriber == NULL || aSubscriber->tableRowCount == 0)
    {
        tyLogCrit("bus", "Subscriber NULL");
        return TY_ERROR_NONE;
    }
    if (aSubscriber->tableRowCount == 0)
    {
        tyLogDebg("bus", "tableRowCount == 0");
        return TY_ERROR_NONE;
    }
    // test if we have slots left
    if (mSubscriberCount + 1 >= CONFIG_TYBUS_MAX_SUBSCRIBERS)
    {
        tyLogWarn("bus", "Max count for subscribers reached");
        return TY_ERROR_SUBSCRIBER_COUNT_EXCEEDED;
    }
    mSubscriber[mSubscriberCount]             = aSubscriber;
    mSubscriber[mSubscriberCount]->exitAction = NULL;

    mSubscriberCount++;
    return TY_ERROR_NONE;
};
