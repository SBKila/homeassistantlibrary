# HomeAssistantLibrary (HALib)

**HALib** is a lightweight, asynchronous, and modular C++ library designed for **ESP8266** microcontrollers. It simplifies the creation of custom IoT devices that integrate seamlessly with **Home Assistant** via MQTT Discovery.

Unlike other libraries, HALib uses an **Adapter pattern** to separate hardware logic from Home Assistant protocol management and features a **Flash Spillover Buffer** to guarantee stability under low-memory conditions.

## 🚀 Features

* **Plug & Play Discovery:** Automatic Home Assistant MQTT Discovery (no YAML configuration required in HA).
* **Asynchronous & Non-blocking:** Built on top of `AsyncMqttClient`, relying on hardware interrupts and state machines without blocking the main loop.
* **🛡️ OOM Resilience (Spillover Buffer - v0.3.x):**
  * **Hybrid RAM + Flash Queue:** The RAM outbox is strictly capped at 5 messages. Excess messages during heavy Discovery bursts are automatically serialized to Flash storage (`LittleFS`).
  * **Peek & Pop Engine:** Messages remain queued until MQTT publish success is confirmed.
  * **Poison Pill Protection:** Automatically discards messages failing more than 250 consecutive times to prevent head-of-line deadlock.
* **Modular Architecture:**
  * **HADevice:** Represents the physical microcontroller node and manages WiFi/MQTT connectivity.
  * **HAAdapter:** Bridges specific hardware to standard HA components (e.g., energy meter pulse counter, buttons, LEDs).
  * **HAComponent:** Represents Home Assistant entities (`Sensor`, `Text Sensor`, `Binary Sensor`, `Switch`, `Light`, `Device Trigger`).
* **Built-in Adapters:**
  * ⚡ **DDS238:** S0 pulse counter for energy meters (EN 62053-31 compliant) with RTC persistence support.
  * 🔘 **PushBtn:** Button management with debouncing and event triggering.
  * 💡 **Led:** Simple LED control and state feedback.

## 📖 How It Works

1. **HADevice** is initialized with device details (Name, Manufacturer, Model, Firmware version).
2. **HAComponents** (such as `HAComponentSensor` or `HAComponentTextSensor`) are added to an **HAAdapter** or directly to the node.
3. In `loop()`, calling `myDevice->loop(WiFi.status())` monitors network state, triggers MQTT auto-reconnect, and flushes outgoing discovery/state messages.
4. Calling `setValue(...)` on any component immediately schedules an asynchronous state update to Home Assistant.

> ⚠️ **Important Architecture Notes:**
> * **1 Board = 1 HADevice:** HALib is strictly designed around a single physical device node per microcontroller. Never instantiate multiple `HADevice` instances on the same board.
> * **LittleFS Mandatory for Spillover Buffer:** You **must** call `LittleFS.begin()` in your `setup()` before executing any HALib operations. Without it, Flash swap cannot serialize or recover pending MQTT actions during discovery bursts.

## 💻 Usage Example

```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "HALib/HALib.hpp"

using namespace HALib;

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

HADevice *myDevice;
HAComponentTextSensor *myStatusText;

void setup() {
    Serial.begin(115200);

    // 0. Mount LittleFS (mandatory for the v0.3.x Flash Spillover Buffer)
    LittleFS.begin();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // 1. Initialize the Device node (DeviceName, Manufacturer, Model, Version)
    myDevice = new HADevice("LivingRoom_Node", "MyManufacturer", "NodeMCU_V2", "0.3.1");

    // 2. Configure MQTT broker connection (Broker IP/Host, Port, Optional Login, Optional Password)
    myDevice->setup("192.168.1.100", 1883, "mqtt_user", "mqtt_password");

    // 3. Create a Component (e.g. a Text Sensor)
    myStatusText = new HAComponentTextSensor("Current_Status", SC_NONE, true /* retain */);
    
    // 4. Attach component to the device
    myDevice->addComponent(myStatusText);
}

void loop() {
    // Keep WiFi and MQTT active, process queue
    myDevice->loop(WiFi.status());

    // Update state periodically
    static unsigned long lastMsg = 0;
    if (millis() - lastMsg > 10000) {
        lastMsg = millis();
        if (myDevice->isMqttconnected()) {
            myStatusText->setValue("System running normally");
        }
    }
}
```

## 📦 Installation

### PlatformIO

Add the library and `AsyncMqttClient` to your `platformio.ini`:

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
lib_deps =
    sbkila/HomeAssistantLibrary @ ^0.3.1
    marvinroger/AsyncMqttClient @ ^0.9.0
```
