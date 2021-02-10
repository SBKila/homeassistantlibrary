#include <Arduino.h>
#define DEBUG
#ifdef DEBUG
#define DEBUG_INIT(x) Serial.begin(x)
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTLN_DEC(x) Serial.println(x, DEC)
#else
#define DEBUG_INIT(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLN_DEC(x)
#endif

#ifdef DEBUG_HALIB_DEVICE
#ifdef DEBUG_ESP_PORT
#define HALIB_DEVICE_DEBUG_MSG(...)   \
    DEBUG_ESP_PORT.print("[HALIB] "); \
    DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define HALIB_DEVICE_DEBUG_MSG(...)
#endif
#else
#define HALIB_DEVICE_DEBUG_MSG(...)
#endif