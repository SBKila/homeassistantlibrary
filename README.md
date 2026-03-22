# HomeAssistantLibrary (HALib)

**HALib** is a lightweight, modular C++ library designed for **ESP8266** and **ESP32** microcontrollers. It simplifies the creation of custom IoT devices that integrate seamlessly with **Home Assistant** via MQTT Discovery.

Unlike other libraries, HALib uses an **Adapter pattern** to separate hardware logic from Home Assistant protocol management.

## 🚀 Features

* **Plug & Play:** Automatic Home Assistant MQTT Discovery (no YAML configuration required in HA).
* **Architecture Optimized:**
    * **ESP8266 (`ICACHE_RAM_ATTR`)** and **ESP32 (`IRAM_ATTR`)** compatible.
    * **Non-blocking:** heavily relies on interrupts and efficient state machines.
* **Modular Design:**
    * **HADevice:** Represents the microcontroller itself (the physical node publishing to MQTT).
    * **HAAdapter:** Handles specific hardware logic (e.g., Energy Meter, Button, LED). It bridges hardware behavior to standard components.
    * **HAComponent:** Represents the entity in Home Assistant (Sensor, Text Sensor, Switch, Binary Sensor). They hold the state, generate topics, and expose their behavior via Discovery.
* **Built-in Adapters:**
    * ⚡ **DDS238:** S0 Pulse counter for energy meters (EN 62053-31 compliant) with persistence support.
    * 🔘 **PushBtn:** Button management with debounce and event triggering (relies on `EasyButton`).
    * 💡 **Led:** Simple LED control and state feedback.

## 📖 How It Works

HALib relies on a hierarchy of objects to automate Home Assistant configuration:

1. **HADevice** is initialized with standard details (Model, Manufacturer).
2. **HAComponents** (like `HAComponentSensor` for numbers, `HAComponentTextSensor` for text) are added to an **HAAdapter** or directly to the node.
3. Once connected to MQTT, `HADevice` runs its internal loop and automatically sends the JSON config over the Discovery topics for each component.
4. Calling `setValue` on a component immediately updates the state in Home Assistant.

## 💻 Usage Example

This example demonstrates how to create a basic device node, add a text sensor to expose strings, and set up a standard loop:

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> // Or AsyncMqttClient
#include "HALib/HALib.hpp"

using namespace HALib;

// 1. Define MQTT Client and our Master Device
WiFiClient espClient;
HADevice *myDevice;
HAComponentTextSensor *myStatusText;

void setup() {
    Serial.begin(115200);

    // WiFi Setup...
    // [Connect to WiFi Network here]

    // 2. Initialize the Device node
    myDevice = new HADevice("LivingRoom_Node", "MyManufacturer", "Custom_Board", "1.0.0");
    myDevice->setup(espClient, "192.168.1.100", 1883); // MQTT broker IP/Port

    // 3. Create a Component. 
    // Example: A Text Sensor to send text states
    myStatusText = new HAComponentTextSensor("Current_Status", SC_NONE, true /* retain */);
    
    // 4. Attach component
    myDevice->addComponent(myStatusText);
}

void loop() {
    // Keep WiFi and MQTT active
    // If using PubSubClient:
    // myDevice->loop(WiFi.status() == WL_CONNECTED);

    // Update the state based on logic
    static unsigned long lastMsg = 0;
    if (millis() - lastMsg > 10000) {
        lastMsg = millis();
        myStatusText->setValue("System is running normally...");
    }
}
```

## 📦 Installation

### PlatformIO

Add the following to your `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    sbkila/HomeAssistantLibrary
    evert-arias/EasyButton @ ^2.0.1 ; Required only if using PushBtn Adapter
```
