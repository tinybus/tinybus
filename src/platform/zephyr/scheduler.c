// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <zephyr/kernel.h>

#include <tiny/error.h>
#include <tiny/logging.h>

#include "tinybus/platform/scheduler.h"
#include "tinybus/tinybus.h"

#define EVENT_QUEUE_LEN sm_EVENT_QUEUE_LEN
K_MSGQ_DEFINE(mBacklogQueue, sizeof(TinyEvent), CONFIG_TINYBUS_MAX_BACKLOG, 4);
struct k_thread m_task_sm_thread_data;
K_THREAD_STACK_DEFINE(sm_task_sm_stack_area, 1024);
static TinySchedulerNotifyFn mNotifyFn = NULL;
static void                  schedulerTask(void *p1, void *p2, void *p3)
{
    static TinyEvent event;
    for (;;)
    {
        k_msgq_get(&mBacklogQueue, &event, K_FOREVER);
        if (mNotifyFn != NULL)
        {
            mNotifyFn(&event);
        }
    }
}

void tinySchedulerEventPush(const TinyEvent *aEvent)
{
    if (k_msgq_put(&mBacklogQueue, aEvent, K_NO_WAIT) != 0)
    {
        tinyPlatLog(TINY_LOG_LEVEL_CRIT, "bus", "Backlog Queue full!");
    }
}

void tinyOnSchedulerEvent(TinySchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tinyError tinySchedulerInit()
{
    k_tid_t id =
        k_thread_create(&m_task_sm_thread_data, sm_task_sm_stack_area, K_THREAD_STACK_SIZEOF(sm_task_sm_stack_area),
                        schedulerTask, NULL, NULL, NULL, K_LOWEST_APPLICATION_THREAD_PRIO, 0, K_NO_WAIT);
    k_thread_name_set(id, "TINY_SCHEDULER");
    return TINY_ERROR_NONE;
}
