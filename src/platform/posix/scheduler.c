// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <pthread.h>

#include <tiny/error.h>
#include <tiny/logging.h>

#include "threadqueue.h"
#include <unistd.h>
#include "tinybus/platform/scheduler.h"
#include "tinybus/tinybus.h"

static pthread_t             mThread;
static TinySchedulerNotifyFn mNotifyFn = NULL;
static struct threadqueue    mBacklogQueue;

static void *schedulerTask(void *p1)
{
    static TinyEvent event;
    for (;;)
    {
        thread_queue_get(&mBacklogQueue, NULL, (void *)&event);
        if (mNotifyFn != NULL)
        {
            mNotifyFn(&event);
        }
    }
}

void tinySchedulerEventPush(const TinyEvent *aEvent)
{
    tinyPlatLog(TINY_LOG_LEVEL_INFO, "Scheduler event received %s", aEvent->event);
    thread_queue_add(&mBacklogQueue, (void *)aEvent, 0);
}

void tinyOnSchedulerEvent(TinySchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tinyError tinySchedulerInit()
{
    thread_queue_init(&mBacklogQueue);
    pthread_create(&mThread, NULL, schedulerTask, NULL);
    return TINY_ERROR_NONE;
}
