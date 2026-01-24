#pragma once

#include <pgmspace.h>

namespace HALIB_NAMESPACE
{
    // Enum defining the different Home Assistant component types
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

    // Extern declaration of the array containing component type strings in Flash
    // The definition is in the .cpp file
    extern PGM_P componentTypeTag[];
} // namespace HALIB_NAMESPACE