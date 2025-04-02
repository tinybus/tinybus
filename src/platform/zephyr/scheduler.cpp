// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <zephyr/kernel.h>

#include <ty/error.h>
#include <ty/logging.h>

#include "platform/scheduler.hpp"
#include "tybus/tybus.h"

#define EVENT_QUEUE_LEN sm_EVENT_QUEUE_LEN
K_MSGQ_DEFINE(mBacklogQueue, sizeof(TyBusEvent), CONFIG_TYBUS_MAX_BACKLOG, 4);
struct k_thread m_task_sm_thread_data;
K_THREAD_STACK_DEFINE(sm_task_sm_stack_area, 1024);
static TyBusSchedulerNotifyFn mNotifyFn = NULL;
static void                   schedulerTask(void *p1, void *p2, void *p3)
{
    static TyBusEvent event;
    for (;;)
    {
        k_msgq_get(&mBacklogQueue, &event, K_FOREVER);
        if (mNotifyFn != NULL)
        {
            mNotifyFn(&event);
        }
    }
}

void tyBusSchedulerEventPush(const TyBusEvent &aEvent)
{
    if (k_msgq_put(&mBacklogQueue, &aEvent, K_NO_WAIT) != 0)
    {
        tyLogCrit("bus", "Backlog Queue full!");
    }
}

void tyBusOnSchedulerEvent(TyBusSchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tinyError tyBusSchedulerInit()
{
    k_tid_t id =
        k_thread_create(&m_task_sm_thread_data, sm_task_sm_stack_area, K_THREAD_STACK_SIZEOF(sm_task_sm_stack_area),
                        schedulerTask, NULL, NULL, NULL, K_LOWEST_APPLICATION_THREAD_PRIO, 0, K_NO_WAIT);
    k_thread_name_set(id, "TY_SCHEDULER");
    return TY_ERROR_NONE;
}
