// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include "tinybus/error.h"
#include "tinybus/platform/scheduler.h"
#include "tinybus/tinybus.h"
#include "tinyplatform/logging.h"

static QueueHandle_t       mBacklogQueue = NULL;
static TbSchedulerNotifyFn mNotifyFn     = NULL;

static void schedulerTask(void *aParameters)
{
    static TbEvent event;
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

void tbSchedulerEventPush(const TbEvent *aEvent)
{
    if (mBacklogQueue == NULL)
    {
        tiLog(TI_LOG_LEVEL_ERROR, "bus", "Scheduler not initialized");
        return;
    }
    xQueueSendToBack(mBacklogQueue, aEvent, (TickType_t)10);
}

void tbOnSchedulerEvent(TbSchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tbError tiPlatformSchedulerInit()
{
    mBacklogQueue = xQueueCreate(CONFIG_TINYBUS_MAX_BACKLOG, sizeof(TbEvent));
    xTaskCreate(schedulerTask, "tb_main", 1024, NULL, 0, NULL);

    return TB_ERROR_NONE;
}
