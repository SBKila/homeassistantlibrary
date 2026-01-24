#pragma once
#include <pgmspace.h>

namespace HALIB_NAMESPACE
{
    // L'Enum reste ici
    enum DeviceClass
    {
        DC_NOTDEFINED,
        DC_BATTERY,
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
        DC_WINDOW,
        DC_CURRENT,
        DC_ENERGY,
        DC_HUMIDITY,
        DC_ILLUMINANCE,
        DC_SIGNAL_STRENGTH,
        DC_TEMPERATURE,
        DC_POWER_FACTOR,
        DC_PRESSURE,
        DC_TIMESTAMP,
        DC_VOLTAGE
    };

    // Seulement la déclaration extern (PAS de = { ... })
    extern PGM_P DeviceClassValue[] PROGMEM;

} // namespace HALIB_NAMESPACE