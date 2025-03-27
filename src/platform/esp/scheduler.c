// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include <tiny/error.h>
#include <tiny/logging.h>

#include "tiny/platform/logging.h"
#include "tinybus/platform/scheduler.h"
#include "tinybus/tinybus.h"

static QueueHandle_t         mBacklogQueue = NULL;
static TinySchedulerNotifyFn mNotifyFn     = NULL;

static void schedulerTask(void *aParameters)
{
    static TinyEvent event;
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

void tinySchedulerEventPush(const TinyEvent *aEvent)
{
    if (mBacklogQueue == NULL)
    {
        tinyPlatLog(TINY_LOG_LEVEL_CRIT, "bus", "Scheduler not initialized");
        return;
    }
    xQueueSendToBack(mBacklogQueue, aEvent, (TickType_t)10);
}

void tinyOnSchedulerEvent(TinySchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tinyError tinySchedulerInit()
{
    mBacklogQueue = xQueueCreate(CONFIG_TINYBUS_MAX_BACKLOG, sizeof(TinyEvent));
    xTaskCreate(schedulerTask, "tScheduler", 1024, NULL, 0, NULL);

    return TINY_ERROR_NONE;
}
