# TyBus

TinyBus is a lightweight library that combines a **state machine** for managing system behavior with a **message broker** for inter-component communication. This approach enhances modularity and simplifies the development of complex embedded systems. Based on 20+ years of experience developing microcontroller projects (ESP32, STM32, nRF) with frameworks like ESP-IDF and Zephyr, TinyBus offers a proven and scalable architecture.  

This repository provides **ESP-IDF** and **Zephyr** implementations.  

## Installing
### ESP-IDF
You can use the component from the Component Registry, or clone the repository to your `components` folder
- Espressif Component Registry: [marcel-cd/tybus](https://components.espressif.com/components/marcel-cd/tybus/)

### Zephyr / Nordic Connect SDK
Clone the repository to your local `MODULE_EXT_ROOT` directory, have a look at [Zephyr Modules](https://docs.zephyrproject.org/latest/develop/modules.html)

## How It Works

TyBus is implemented as a singleton and internally uses a **FreeRTOS** queue and task for event handling.

### Example: Controlling an LED with a Button Press

Assume you have a development kit where you want to turn on an LED (`led.c`) when a button is pressed (`button.c`).

#### `led.h`
Define the **LED_EVENT_LED1_ON** event in the header file to allow `button.c` to trigger it:

```c
// Include TyBus
#include <tybus/tybus.h>

// Define the event
TYBUS_EVENT_NAME(LED, LED1_ON, "led1_on");
TYBUS_STATE_NAME(LED, START, "start");
```

#### `led.c`

```c
// Define the action
static void onLedOn(const Event *apEvent);

// Define the state table
static const StateTableRow stateTable[] = {
    {
        .state = LED_STATE_START,  // Matches any state
        .event = LED_EVENT_LED1_ON,
        .entryAction = onLedOn, // Function to call when the event occurs
        .nextState = NULL // Keep the current state
    }
};

// Handle the LED event action
static void onLedOn(const Event *apEvent) {
    gpio_set_level(GPIO_LED, 0); // Turn LED on
}

void LedModule() {
    // Subscribe to TyBus
    tyBusSubscribe(TY_SUBSCRIBER("led", stateTable, TY_TABLE_ROW_COUNT(stateTable), LED_STATE_START));
}
```

#### `button.c`

```c
// Include the LED event definition
#include "led.h"
#include <tybus/tybus.h>

// Define events and states
TYBUS_EVENT_NAME(BUTTON, INITIALIZE, "initialize");
TYBUS_EVENT_NAME(BUTTON, PRESSED, "button_short_pressed");
TYBUS_STATE_NAME(BUTTON, START, "start");
TYBUS_STATE_NAME(BUTTON, IDLE, "idle");

// Define actions
static void initModule(Event *apEvent);
static void onButtonPress(Event *apEvent);

// Define the state table
static const StateTableRow stateTable[] = {
    // Initialize the button module
    {
        .state = BUTTON_STATE_START,
        .event = BUTTON_EVENT_INITIALIZE,
        .entryAction = initModule,
        .nextState = BUTTON_STATE_POLLING,
    },
    // Handle button press event
    {
        .state = BUTTON_STATE_IDLE,
        .event = BUTTON_EVENT_PRESSED,
        .entryAction = onButtonPress,
        .nextState = NULL // Maintain the current state
    }
};

// Subscribe the button module to TyBus
void ButtonModule() {
    tyBusSubscribe(TY_SUBSCRIBER("button", stateTable, TY_TABLE_ROW_COUNT(stateTable), BUTTON_STATE_START));
}

// Handle the button press event and trigger the LED module
static void onButtonPress(Event *apEvent) {
    tyBusPublish(TYBUS_EVENT(LED_EVENT_LED1_ON, NULL, 0)); // Send event to LED module
}
```

## Contributing

We welcome contributions in the form of code, bug reports, and feedback.

- See [CONTRIBUTING.md](https://github.com/tybus/tybus/blob/main/CONTRIBUTING.md) for guidelines.

## Contact

Join the conversation on:
- **[GitHub Discussions](https://github.com/tybus/tybus/discussions)** – For general chat or questions.
- **[GitHub Issues](https://github.com/tybus/tybus/issues)** – To report bugs or suggest features.
