# Nonsense Hello World Logger  

This is a **rather nonsense example** that logs `"Hello World"` to the console.  
Why? Because we can 🚀 —and because it demonstrates the basic usage of TyBus.  

## Running the Example  

To build this masterpiece on a Linux host, simply use:  

```sh
make build

# run on linux
./build/zephyr/zephyr.exe
```

## Code  

``` c
// 1. Define the events using in the state table
TYBUS_EVENT_NAME(MAIN, HELLO_WORLD, "HelloWorld");
TYBUS_STATE_NAME(MAIN, START, "Start");

// 2. Declare the actions using in the state table
static void onHelloWorld(const Event *aEvent);

// 3. Glue everything together
static const StateTableRow stateTable[] = {{
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

/*****************************************************************************/
// Implement the Actions
static void onHelloWorld(const Event *aEvent) { LOG_INF("Action onHelloWorld() called from TyBus"); }

int main(void)
{
    // Subscribe to the TyBus using the state table. This registers the module
    // to receive and process events according to the defined rules.
    LOG_WRN("Subscribe module '%s' to TyBus", TAG);
    tyBusSubscribe(TY_SUBSCRIBER(TAG, stateTable, TY_TABLE_ROW_COUNT(stateTable), MAIN_STATE_START));

    while (true)
    {
        LOG_INF("Publish HELLO_WORLD event");
        // Publish the HELLO_WORLD event.  The TyBus will then look up this
        // event in the state table and execute the corresponding action
        // (onHelloWorld in this case).  The NULL and 0 arguments indicate no data
        // is being sent with the event.    // publish an event to the TyBus
        tyBusPublish(TYBUS_EVENT(MAIN_EVENT_HELLO_WORLD, NULL, 0));

        // next event in 1 second
        k_sleep(K_SECONDS(1));
    }
    return 0;
}
```

## Running  

``` sh
*** Booting Zephyr OS build v4.0.0 ***
[00:00:00.000,000] <wrn> main: Subscribe module 'main' to TyBus
[00:00:00.000,000] <inf> main: Publish HELLO_WORLD event
[00:00:00.000,000] <inf> main: Action onHelloWorld() called from TyBus
[00:00:01.010,000] <inf> main: Publish HELLO_WORLD event
[00:00:01.010,000] <inf> main: Action onHelloWorld() called from TyBus
```

`
