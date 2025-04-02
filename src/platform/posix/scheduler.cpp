// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <pthread.h>

#include "tybus-config.h"

#include <ty/error.h>
#include <ty/logging.h>

#include "tybus/tybus.h"

#include "etl/queue_mpmc_mutex.h"

#include "platform/scheduler.hpp"

static pthread_t                                            mThread;
static TyBusSchedulerNotifyFn                               mNotifyFn = NULL;
etl::queue_mpmc_mutex<TyBusEvent, CONFIG_TYBUS_MAX_BACKLOG> mBacklogQueue;

static void *schedulerTask(void *p1)
{
    static TyBusEvent event;
    for (;;)
    {
        if (mBacklogQueue.pop(event) && mNotifyFn != NULL)
        {
            mNotifyFn(&event);
        }
    }
}

void tyBusSchedulerEventPush(const TyBusEvent &aEvent)
{
    tyLogDebg("scheduler", "Scheduler event received %s", aEvent.event);
    while (!mBacklogQueue.push(aEvent))
    {
    }
}

void tyBusOnSchedulerEvent(TyBusSchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tinyError tyBusSchedulerInit()
{
    pthread_create(&mThread, NULL, schedulerTask, NULL);
    return TY_ERROR_NONE;
}
