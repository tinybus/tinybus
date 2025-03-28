// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TyBus simplest example: Hello World
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "tybus/tybus.h"

#define LOG_MODULE_NAME main
LOG_MODULE_REGISTER(main);
const char *TAG = "main";
/*****************************************************************************
 * Ty Bus Definitions for the main Module
 *****************************************************************************/

// 1. Define the events using in the state table
TYBUS_EVENT_NAME(MAIN, HELLO_WORLD, "HelloWorld");
TYBUS_STATE_NAME(MAIN, START, "Start");

// 2. Declare the actions using in the state table
static void onHelloWorld(const TyBusEvent *aEvent);

// 3. Glue everything together
static const TyBusStateTableRow stateTable[] = {{
    .state = MAIN_STATE_START,                // only process the event, if the current state
                                              // match. TYBUS_STATE_INITIAL is the default state.
                                              // use TYBUS_STATE_ANY, if this should be ignored
    .event          = MAIN_EVENT_HELLO_WORLD, // process if the event matches
    .conditionCheck = NULL,                   // No condition function.  The action is always executed.
    .entryAction    = onHelloWorld,           // The function to call when the event occurs.
    .nextState      = NULL,                   // Keep the current state after the action.
    .exitAction     = NULL,                   // No exit function.
    .stop           = true                    // Stop processing after this entry (this is the default case)
}};
static TyBusSubscriber          mSubscriber =
    TYBUS_SUBSCRIBER("app", stateTable, TYBUS_TABLE_ROW_COUNT(stateTable), MAIN_STATE_START);

/*****************************************************************************/
// Implement the Actions
static void onHelloWorld(const TyBusEvent *aEvent)
{
    LOG_INF("Action onHelloWorld() called from TyBus");
}

int main(void)
{
    // Subscribe to the TyBus using the state table. This registers the module
    // to receive and process events according to the defined rules.
    LOG_WRN("Subscribe module '%s' to TyBus", TAG);
    tyBusSubscribe(&mSubscriber);

    while (true)
    {
        LOG_INF("Publish HELLO_WORLD event");
        // Publish the HELLO_WORLD event.  The TyBus will then look up this
        // event in the state table and execute the corresponding action
        // (onHelloWorld in this case).  The NULL and 0 arguments indicate no data
        // is being sent with the event.    // publish an event to the TyBus
        tyBusPublish(MAIN_EVENT_HELLO_WORLD, NULL, 0);

        // next event in 1 second
        k_sleep(K_SECONDS(1));
    }
    return 0;
}
