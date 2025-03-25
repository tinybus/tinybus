// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <pthread.h>

#include "threadqueue.h"
#include <unistd.h>
#include "tinybus/error.h"
#include "tinybus/platform/logging.h"
#include "tinybus/platform/scheduler.h"
#include "tinybus/tinybus.h"

static pthread_t           mThread;
static TbSchedulerNotifyFn mNotifyFn = NULL;
static struct threadqueue  mBacklogQueue;

static void *schedulerTask(void *p1)
{
    static TbEvent event;
    for (;;)
    {
        thread_queue_get(&mBacklogQueue, NULL, (void *)&event);
        if (mNotifyFn != NULL)
        {
            mNotifyFn(&event);
        }
    }
}

void tbSchedulerEventPush(const TbEvent *aEvent)
{
    tiLog(TI_LOG_LEVEL_INFO, "Scheduler event received %s", aEvent->event);
    thread_queue_add(&mBacklogQueue, (void *)aEvent, 0);
}

void tbOnSchedulerEvent(TbSchedulerNotifyFn aNotifyFn)
{
    mNotifyFn = aNotifyFn;
}

tbError tiPlatformSchedulerInit()
{
    thread_queue_init(&mBacklogQueue);
    pthread_create(&mThread, NULL, schedulerTask, NULL);
    return TB_ERROR_NONE;
}
