// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include <ty/error.h>
#include <ty/logging.h>

#include "tybus/tybus.h"

#include "platform/scheduler.hpp"

static QueueHandle_t          mBacklogQueue = NULL;
static TyBusSchedulerNotifyFn mNotifyFn     = NULL;

static void schedulerTask(void *aParameters)
{
    static TyBusEvent event;
    for (;;)
    {
        while (xQueueReceive(mBacklogQueue, &event, portMAX_DELAY) != pdPASS)
        {
        }
        if (mNotifyFn != NULL)
        {
            mNotifyFn(&event);
        }
    }
}

void tyBusSchedulerEventPush(const TyBusEvent &aEvent)
{
    if (mBacklogQueue == NULL)
    {
        tyLogCrit("bus", "Scheduler not initialized");
        return;
    }
    xQueueSendToBack(mBacklogQueue, &aEvent, (TickType_t)10);
}

void tyBusOnSchedulerEvent(TyBusSchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tinyError tyBusSchedulerInit()
{
    mBacklogQueue = xQueueCreate(CONFIG_TYBUS_MAX_BACKLOG, sizeof(TyBusEvent));
    xTaskCreate(schedulerTask, "tScheduler", 1024, NULL, 0, NULL);

    return TY_ERROR_NONE;
}
