// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TyBus simplest example: Hello World
 */

#include <pthread.h>
#include <stdio.h>
#include <ty/logging.h>
#include <unistd.h>
#include "tybus/tybus.h"

const char *TAG = "main";
/*****************************************************************************
 * Tiny Bus Definitions for the main Module
 *****************************************************************************/

// 1. Define the events using in the state table
TY_EVENT_NAME(MAIN, HELLO_WORLD, "HelloWorld");
TY_STATE_NAME(MAIN, START, "Start");

// 2. Declare the actions using in the state table
static void onHelloWorld(const TinyEvent *aEvent);

// 3. Glue everything together
static const TinyStateTableRow stateTable[] = {{
    .state = MAIN_STATE_START,                // only process the event, if the current state
                                              // match. TY_STATE_INITIAL is the default state.
                                              // use TY_STATE_ANY, if this should be ignored
    .event          = MAIN_EVENT_HELLO_WORLD, // process if the event matches
    .conditionCheck = NULL,                   // No condition function.  The action is always executed.
    .entryAction    = onHelloWorld,           // The function to call when the event occurs.
    .nextState      = NULL,                   // Keep the current state after the action.
    .exitAction     = NULL,                   // No exit function.
    .stop           = true                    // Stop processing after this entry (this is the default case)
}};

static TinySubscriber mSubscriber = TY_SUBSCRIBER("app", stateTable, TY_TABLE_ROW_COUNT(stateTable), MAIN_STATE_START);
/*****************************************************************************/
// Implement the Actions
static void onHelloWorld(const TinyEvent *aEvent)
{
    tyLogCritPlat("Action onHelloWorld() called from TyBus");
}

int main(void)
{
    // Subscribe to the TyBus using the state table. This registers the module
    // to receive and process events according to the defined rules.
    tyLogInfoPlat("Subscribe module '%s' to TyBus", TAG);
    tinySubscribe(&mSubscriber);

    while (true)
    {
        tyLogInfoPlat("Publish HELLO_WORLD event");
        // Publish the HELLO_WORLD event.  The TyBus will then look up this
        // event in the state table and execute the corresponding action
        // (onHelloWorld in this case).  The NULL and 0 arguments indicate no data
        // is being sent with the event.    // publish an event to the TyBus
        tyPublish(MAIN_EVENT_HELLO_WORLD, NULL, 0);

        // next event in 1 second
        sleep(5);
    }
    return 0;
}
