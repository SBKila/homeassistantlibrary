#pragma once
#include <pgmspace.h>

namespace HALIB_NAMESPACE
{
    const char class_00[] PROGMEM = "battery";
    const char class_01[] PROGMEM = "battery_charging";
    const char class_02[] PROGMEM = "cold";
    const char class_03[] PROGMEM = "connectivity";
    const char class_04[] PROGMEM = "door";
    const char class_05[] PROGMEM = "garage_door";
    const char class_06[] PROGMEM = "gas";
    const char class_07[] PROGMEM = "heat";
    const char class_08[] PROGMEM = "light";
    const char class_09[] PROGMEM = "lock";
    const char class_10[] PROGMEM = "moisture";
    const char class_11[] PROGMEM = "motion";
    const char class_12[] PROGMEM = "moving";
    const char class_13[] PROGMEM = "occupancy";
    const char class_14[] PROGMEM = "opening";
    const char class_15[] PROGMEM = "plug";
    const char class_16[] PROGMEM = "power";
    const char class_17[] PROGMEM = "presence";
    const char class_18[] PROGMEM = "problem";
    const char class_19[] PROGMEM = "safety";
    const char class_20[] PROGMEM = "smoke";
    const char class_21[] PROGMEM = "sound";
    const char class_22[] PROGMEM = "vibration";
    const char class_23[] PROGMEM = "window";

    PGM_P DeviceClassValue[] PROGMEM =
        {
            class_00,
            class_01,
            class_02,
            class_03,
            class_04,
            class_05,
            class_06,
            class_07,
            class_08,
            class_09,
            class_10,
            class_11,
            class_12,
            class_13,
            class_14,
            class_15,
            class_16,
            class_17,
            class_18,
            class_19,
            class_20,
            class_21,
            class_22,
            class_23};
    enum DeviceClass
    {
        DC_NOTDEFINED,
        DC_BATTERY,
        DC_BATTERY_CHARGING,
        DC_COLD,
        DC_CONNECTIVITY,
        DC_DOOR,
        DC_GARAGE_DOOR,
        DC_GAS,
        DC_HEAT,
        DC_LIGHT,
        DC_LOCK,
        DC_MOISTURE,
        DC_MOTION,
        DC_MOVING,
        DC_OCCUPANCY,
        DC_OPENING,
        DC_PLUG,
        DC_POWER,
        DC_PRESENCE,
        DC_PROBLEM,
        DC_SAFETY,
        DC_SMOKE,
        DC_SOUND,
        DC_VIBRATION,
        DC_WINDOW
    };
} // namespace HALIB_NAMESPACE