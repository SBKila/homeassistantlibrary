#pragma once

#include <pgmspace.h>

namespace HALIB_NAMESPACE
{
    const char type_0[] PROGMEM = "device";
    const char type_1[] PROGMEM = "alarm_control_panel";
    const char type_2[] PROGMEM = "binary_sensor";
    const char type_3[] PROGMEM = "camera";
    const char type_4[] PROGMEM = "cover";
    const char type_5[] PROGMEM = "fan";
    const char type_6[] PROGMEM = "climate";
    const char type_7[] PROGMEM = "light";
    const char type_8[] PROGMEM = "lock";
    const char type_9[] PROGMEM = "sensor";
    const char type_10[] PROGMEM = "switch";
    const char type_11[] PROGMEM = "vacuum";
    const char type_12[] PROGMEM = "device_automation";
    const char type_13[] PROGMEM = "number";
    

    // extern PGM_P componentTypeTag[] PROGMEM;
    PGM_P componentTypeTag[] PROGMEM =
        {
            type_0,
            type_1,
            type_2,
            type_3,
            type_4,
            type_5,
            type_6,
            type_7,
            type_8,
            type_9,
            type_10,
            type_11,
            type_12,
            type_13
        };

    enum ComponentType
    {
        DEVICE,
        ALARM_CONTROL_PANEL,
        BINARY_SENSOR,
        CAMERA,
        COVER,
        FAN,
        CLIMATE,
        LIGHT,
        LOCK,
        SENSOR,
        SWITCH,
        VACUUM,
        DEVICE_TRIGGER,
        NUMBER
    };
} // namespace HALIB_NAMESPACE
