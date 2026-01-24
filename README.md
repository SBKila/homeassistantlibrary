# HomeAssistantLibrary (HALib)

**HALib** is a lightweight, modular C++ library designed for **ESP8266** and **ESP32** microcontrollers. It simplifies the creation of custom IoT devices that integrate seamlessly with **Home Assistant** via MQTT Discovery.

Unlike other libraries, HALib uses an **Adapter pattern** to separate hardware logic from Home Assistant protocol management.

## 🚀 Features

* **Plug & Play:** Automatic Home Assistant MQTT Discovery (no YAML configuration required in HA).
* **Architecture Optimized:**
    * **ESP8266 (`ICACHE_RAM_ATTR`)** and **ESP32 (`IRAM_ATTR`)** compatible.
    * **Non-blocking:** heavily relies on interrupts and efficient state machines.
* **Modular Design:**
    * **HADevice:** Represents the microcontroller itself.
    * **HAAdapter:** Handles specific hardware logic (e.g., Energy Meter, Button, LED).
    * **HAComponent:** Represents the entity in Home Assistant (Sensor, Switch, Binary Sensor).
* **Built-in Adapters:**
    * ⚡ **DDS238:** S0 Pulse counter for energy meters (EN 62053-31 compliant) with persistence support.
    * 🔘 **PushBtn:** Button management with debounce and event triggering (relies on `EasyButton`).
    * 💡 **Led:** Simple LED control and state feedback.

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
