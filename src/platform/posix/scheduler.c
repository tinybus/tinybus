// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <pthread.h>

#include <tiny/error.h>
#include <tiny/logging.h>

#include "threadqueue.h"
#include <unistd.h>
#include "tiny/platform/logging.h"
#include "tinybus/platform/scheduler.h"
#include "tinybus/tinybus.h"

static pthread_t             mThread;
static TinySchedulerNotifyFn mNotifyFn = NULL;
static struct threadqueue    mBacklogQueue;

static void *schedulerTask(void *p1)
{
    static struct threadmsg msg;
    for (;;)
    {
        thread_queue_get(&mBacklogQueue, NULL, &msg);
        if (mNotifyFn != NULL)
        {
            if (msg.data != NULL)
            {
                mNotifyFn(msg.data);
            }
            else
            {
                tyLogCritPlat("Scheduler event received without data");
            }
        }
    }
}

void tinySchedulerEventPush(const TinyEvent *aEvent)
{
    tyLogDebgPlat("Scheduler event received %s", aEvent->event);
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
    return TY_ERROR_NONE;
}
