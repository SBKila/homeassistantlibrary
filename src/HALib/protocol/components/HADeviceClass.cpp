#include "HADeviceClass.hpp"

namespace HALIB_NAMESPACE
{
    // Définition des chaînes
    const char dc_0[] PROGMEM = "None";
    const char dc_1[] PROGMEM = "battery";
    const char dc_2[] PROGMEM = "cold";
    const char dc_3[] PROGMEM = "connectivity";
    const char dc_4[] PROGMEM = "door";
    const char dc_5[] PROGMEM = "garage_door";
    const char dc_6[] PROGMEM = "gas";
    const char dc_7[] PROGMEM = "heat";
    const char dc_8[] PROGMEM = "light";
    const char dc_9[] PROGMEM = "lock";
    const char dc_10[] PROGMEM = "moisture";
    const char dc_11[] PROGMEM = "motion";
    const char dc_12[] PROGMEM = "moving";
    const char dc_13[] PROGMEM = "occupancy";
    const char dc_14[] PROGMEM = "opening";
    const char dc_15[] PROGMEM = "plug";
    const char dc_16[] PROGMEM = "power";
    const char dc_17[] PROGMEM = "presence";
    const char dc_18[] PROGMEM = "problem";
    const char dc_19[] PROGMEM = "safety";
    const char dc_20[] PROGMEM = "smoke";
    const char dc_21[] PROGMEM = "sound";
    const char dc_22[] PROGMEM = "vibration";
    const char dc_23[] PROGMEM = "window";
    const char dc_24[] PROGMEM = "current";
    const char dc_25[] PROGMEM = "energy";
    const char dc_26[] PROGMEM = "humidity";
    const char dc_27[] PROGMEM = "illuminance";
    const char dc_28[] PROGMEM = "signal_strength";
    const char dc_29[] PROGMEM = "temperature";
    const char dc_30[] PROGMEM = "power_factor";
    const char dc_31[] PROGMEM = "pressure";
    const char dc_32[] PROGMEM = "timestamp";
    const char dc_33[] PROGMEM = "voltage";

    // Définition du tableau (Le code réel)
    PGM_P DeviceClassValue[] PROGMEM = {
        dc_0, dc_1, dc_2, dc_3, dc_4, dc_5, dc_6, dc_7, dc_8, dc_9,
        dc_10, dc_11, dc_12, dc_13, dc_14, dc_15, dc_16, dc_17, dc_18, dc_19,
        dc_20, dc_21, dc_22, dc_23, dc_24, dc_25, dc_26, dc_27, dc_28, dc_29,
        dc_30, dc_31, dc_32, dc_33};

} // namespace HALIB_NAMESPACE