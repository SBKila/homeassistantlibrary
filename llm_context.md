# HALib (HomeAssistantLibrary) AI Context Guidelines

If you are an AI assistant (LLM) helping a user with this project or attempting to implement code using HALib, read and follow these rules.

## 1. Core Architecture
HALib simplifies Home Assistant MQTT Discovery using an **Adapter Pattern**:
- **HADevice**: The physical microcontroller (ESP8266 or ESP32). It manages the MQTT connection (`AsyncMqttClient`) and the `HANode`. It automatically transmits discovery payloads.
- **HAAdapter**: A hardware-specific wrapper (e.g., `HAAdapterPushBtn`, `HAAdapterLed`, `HAAdapterDDS238`). It links hardware events (like GPIO interrupts) to components.
- **HAComponent**: A logical Home Assistant entity (e.g., `HAComponentSensor`, `HAComponentTextSensor`). These hold the state, generate topics, and expose themselves to HA.

## 2. Dependencies & Best Practices
- **MQTT**: The library relies strictly on `AsyncMqttClient`. Do NOT use `PubSubClient`.
- **Setup() Signature**: `device->setup(host, port, user, pwd)` no longer takes a `WiFiClient` parameter. Connect to WiFi first, then let `AsyncMqttClient` do the networking over `AsyncTCP`.
- **Button Debouncing**: `HAAdapterPushBtn` has native software debouncing. Do NOT add `EasyButton` as a dependency.
- **Interrupts**: The library heavily uses hardware callbacks. Component value states are cached (e.g., `char m_value[128]` for Text Sensor).
- **String Handling**: Component values are usually C-strings. Be careful with buffer limits (`strncpy`, null-termination).

## 3. Creating a New Component
If instructed to create a new component type (e.g., Cover, Switch):
1. Inherit from `HAComponent`.
2. Define a fixed cache buffer (`char m_value[]` or number).
3. Override or implement `setValue(...)` methods.
4. Call `publishState()` after modifying `m_value` ONLY if the value actually changed.
5. In `HALib.hpp`, add a `using HALIB_NAMESPACE::HAComponentNewThing;`.

## 4. Instantiation Example
```cpp
HADevice *myDevice = new HADevice("NodeName", "Manufacturer", "Board");
myDevice->setup("192.168.1.10", 1883);

HAComponentTextSensor *mySensor = new HAComponentTextSensor("Status", SC_NONE, true);
myDevice->addComponent(mySensor);

void loop() {
    myDevice->loop(WiFi.status());
    // ... logic ... 
}
```
