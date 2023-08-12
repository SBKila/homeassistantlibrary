#pragma once
#include <pgmspace.h>

namespace HALIB_NAMESPACE
{
    const char class_00[] PROGMEM = "";                                       // default
    const char binarysensorclass_00[] PROGMEM = "battery";                    // on means low, off means normal
    const char binarysensorclass_01[] PROGMEM = "battery_charging";           // on means charging, off means not charging
    const char binarysensorclass_02[] PROGMEM = "cold";                       // on means cold, off means normal
    const char binarysensorclass_03[] PROGMEM = "connectivity";               // on means connected, off means disconnected
    const char binarysensorclass_04[] PROGMEM = "door";                       // on means open, off means closed
    const char binarysensorclass_05[] PROGMEM = "garage_door";                // on means open, off means closed
    const char binarysensorclass_06[] PROGMEM = "gas";                        // on means gas detected, off means no gas (clear)
    const char binarysensorclass_07[] PROGMEM = "heat";                       // on means hot, off means normal
    const char binarysensorclass_08[] PROGMEM = "light";                      // on means light detected, off means no light
    const char binarysensorclass_09[] PROGMEM = "lock";                       // on means open (unlocked), off means closed (locked)
    const char binarysensorclass_10[] PROGMEM = "moisture";                   // on means moisture detected (wet), off means no moisture (dry)
    const char binarysensorclass_11[] PROGMEM = "motion";                     // on: on means motion detected, off means no motion (clear)
    const char binarysensorclass_12[] PROGMEM = "moving";                     // on means moving, off means not moving (stopped)
    const char binarysensorclass_13[] PROGMEM = "occupancy";                  // on means occupied (detected), off means not occupied (clear)
    const char binarysensorclass_14[] PROGMEM = "opening";                    // on means open, off means closed
    const char binarysensorclass_15[] PROGMEM = "plug";                       // on means device is plugged in, off means device is unplugged
    const char binarysensorclass_16[] PROGMEM = "power";                      // on means power detected, off means no power
    const char binarysensorclass_17[] PROGMEM = "presence";                   // on means home, off means away
    const char binarysensorclass_18[] PROGMEM = "problem";                    // on means problem detected, off means no problem (OK
    const char binarysensorclass_19[] PROGMEM = "safety";                     // on means unsafe, off means safe
    const char binarysensorclass_20[] PROGMEM = "smoke";                      // on means smoke detected, off means no smoke (clear)
    const char binarysensorclass_21[] PROGMEM = "sound";                      // on means sound detected, off means no sound (clear)
    const char binarysensorclass_22[] PROGMEM = "vibration";                  // on: on means vibration detected, off means no vibration (clear)
    const char binarysensorclass_23[] PROGMEM = "window";                     // on means open, off means closed
    const char binarysensorclass_24[] PROGMEM = "carbon_monoxide";            // on means carbon monoxide detected, off no carbon monoxide (clear)
    const char binarysensorclass_25[] PROGMEM = "tamper";                     // on means tampering detected, off means no tampering (clear)
    const char binarysensorclass_26[] PROGMEM = "update";                     // on means update available, off means up-to-date
    const char binarysensorclass_27[] PROGMEM = "running";                    // on means running, off means not running
    const char sensorclass_01[] PROGMEM = "apparent_power";                   // Apparent power in VA.
    const char sensorclass_02[] PROGMEM = "aqi";                              // Air Quality Index (unitless).
    const char sensorclass_03[] PROGMEM = "atmospheric_pressure";             // Atmospheric pressure in cbar, bar, hPa, inHg, kPa, mbar, Pa or psi
    const char sensorclass_04[] PROGMEM = "battery";                          // Percentage of battery that is left in %
    const char sensorclass_05[] PROGMEM = "carbon_dioxide";                   // Carbon Dioxide in CO2 (Smoke) in ppm
    const char sensorclass_06[] PROGMEM = "carbon_monoxide";                  // Carbon Monoxide in CO (Gas CNG/LPG) in ppm
    const char sensorclass_07[] PROGMEM = "current";                          // Current in A, mA
    const char sensorclass_08[] PROGMEM = "data_rate";                        // Data rate in bit/s, kbit/s, Mbit/s, Gbit/s, B/s, kB/s, MB/s, GB/s, KiB/s, MiB/s or GiB/s
    const char sensorclass_09[] PROGMEM = "data_size";                        // Data size in bit, kbit, Mbit, Gbit, B, kB, MB, GB, TB, PB, EB, ZB, YB, KiB, MiB, GiB, TiB, PiB, EiB, ZiB or YiB
    const char sensorclass_10[] PROGMEM = "date";                             // Date string (ISO 8601)
    const char sensorclass_11[] PROGMEM = "distance";                         // Generic distance in km, m, cm, mm, mi, yd, or in
    const char sensorclass_12[] PROGMEM = "duration";                         // Duration in d, h, min, or s
    const char sensorclass_13[] PROGMEM = "energy";                           // Energy in Wh, kWh, MWh, MJ, or GJ
    const char sensorclass_14[] PROGMEM = "energy_storage";                   // Stored energy in Wh, kWh, MWh, MJ, or GJ
    const char sensorclass_15[] PROGMEM = "enum";                             // Has a limited set of (non-numeric) states
    const char sensorclass_16[] PROGMEM = "frequency";                        // Frequency in Hz, kHz, MHz, or GHz
    const char sensorclass_17[] PROGMEM = "gas";                              // Gasvolume in m³, ft³ or CCF
    const char sensorclass_18[] PROGMEM = "humidity";                         // Percentage of humidity in the air in %
    const char sensorclass_19[] PROGMEM = "illuminance";                      // The current light level in lx
    const char sensorclass_20[] PROGMEM = "irradiance";                       // Irradiance in W/m² or BTU/(h⋅ft²)
    const char sensorclass_21[] PROGMEM = "moisture";                         // Percentage of water in a substance in %
    const char sensorclass_22[] PROGMEM = "monetary";                         // The monetary value (ISO 4217)
    const char sensorclass_23[] PROGMEM = "nitrogen_dioxide";                 // Concentration of Nitrogen Dioxide in µg/m³
    const char sensorclass_24[] PROGMEM = "nitrogen_monoxide";                // Concentration of Nitrogen Monoxide in µg/m³
    const char sensorclass_25[] PROGMEM = "nitrous_oxide";                    // Concentration of Nitrous Oxide in µg/m³
    const char sensorclass_26[] PROGMEM = "ozone";                            // Concentration of Ozone in µg/m³
    const char sensorclass_27[] PROGMEM = "pm1";                              // Concentration of particulate matter less than 1 micrometer in µg/m³
    const char sensorclass_28[] PROGMEM = "pm25";                             // Concentration of particulate matter less than 2.5 micrometers in µg/m³
    const char sensorclass_29[] PROGMEM = "pm10";                             // Concentration of particulate matter less than 10 micrometers in µg/m³
    const char sensorclass_30[] PROGMEM = "power_factor";                     // Power factor (unitless), unit may be None or %
    const char sensorclass_31[] PROGMEM = "power";                            // Power in W or kW
    const char sensorclass_32[] PROGMEM = "precipitation";                    // Accumulated precipitation in cm, in or mm
    const char sensorclass_33[] PROGMEM = "precipitation_intensity";          // Precipitation intensity in in/d, in/h, mm/d or mm/h
    const char sensorclass_34[] PROGMEM = "pressure";                         // Pressure in Pa, kPa, hPa, bar, cbar, mbar, mmHg, inHg or psi
    const char sensorclass_35[] PROGMEM = "reactive_power";                   // Reactive power in var
    const char sensorclass_36[] PROGMEM = "signal_strength";                  // Signal strength in dB or dBm
    const char sensorclass_37[] PROGMEM = "sound_pressure";                   // Sound pressure in dB or dBA
    const char sensorclass_38[] PROGMEM = "speed";                            // Generic speed in ft/s, in/d, in/h, km/h, kn, m/s, mph or mm/d
    const char sensorclass_39[] PROGMEM = "sulphur_dioxide";                  // Concentration of sulphur dioxide in µg/m³
    const char sensorclass_40[] PROGMEM = "temperature";                      // Temperature in °C, °F or K
    const char sensorclass_41[] PROGMEM = "timestamp";                        // Datetime object or timestamp string (ISO 8601)
    const char sensorclass_42[] PROGMEM = "volatile_organic_compounds";       // Concentration of volatile organic compounds in µg/m³
    const char sensorclass_43[] PROGMEM = "volatile_organic_compounds_parts"; // Ratio of volatile organic compounds in ppm or ppb
    const char sensorclass_44[] PROGMEM = "voltage";                          // Voltage in V, mV
    const char sensorclass_45[] PROGMEM = "volume";                           // Generic volume in L, mL, gal, fl. oz., m³, ft³, or CCF
    const char sensorclass_46[] PROGMEM = "volume_storage";                   // Generic stored volume in L, mL, gal, fl. oz., m³, ft³, or CCF
    const char sensorclass_47[] PROGMEM = "water";                            // Water consumption in L, gal, m³, ft³, or CCF
    const char sensorclass_48[] PROGMEM = "weight";                           // Generic mass in kg, g, mg, µg, oz, lb, or st
    const char sensorclass_49[] PROGMEM = "wind_speed";                       // Wind speed in ft/s, km/h, kn, m/s, or mph

    PGM_P DeviceClassValue[] PROGMEM =
        {
            class_00,             // default
            binarysensorclass_00, // on means low, off means normal
            binarysensorclass_01, // on means charging, off means not charging
            binarysensorclass_02, // on means cold, off means normal
            binarysensorclass_03, // on means connected, off means disconnected
            binarysensorclass_04, // on means open, off means closed
            binarysensorclass_05, // on means open, off means closed
            binarysensorclass_06, // on means gas detected, off means no gas (clear)
            binarysensorclass_07, // on means hot, off means normal
            binarysensorclass_08, // on means light detected, off means no light
            binarysensorclass_09, // on means open (unlocked), off means closed (locked)
            binarysensorclass_10, // on means moisture detected (wet), off means no moisture (dry)
            binarysensorclass_11, // on: on means motion detected, off means no motion (clear)
            binarysensorclass_12, // on means moving, off means not moving (stopped)
            binarysensorclass_13, // on means occupied (detected), off means not occupied (clear)
            binarysensorclass_14, // on means open, off means closed
            binarysensorclass_15, // on means device is plugged in, off means device is unplugged
            binarysensorclass_16, // on means power detected, off means no power
            binarysensorclass_17, // on means home, off means away
            binarysensorclass_18, // on means problem detected, off means no problem (OK
            binarysensorclass_19, // on means unsafe, off means safe
            binarysensorclass_20, // on means smoke detected, off means no smoke (clear)
            binarysensorclass_21, // on means sound detected, off means no sound (clear)
            binarysensorclass_22, // on: on means vibration detected, off means no vibration (clear)
            binarysensorclass_23, // on means open, off means closed
            binarysensorclass_24, // on means carbon monoxide detected, off no carbon monoxide (clear)
            binarysensorclass_25, // on means tampering detected, off means no tampering (clear)
            binarysensorclass_26, // on means update available, off means up-to-date
            binarysensorclass_27, // on means running, off means not running
            sensorclass_01,       // Apparent power in VA.
            sensorclass_02,       // Air Quality Index (unitless).
            sensorclass_03,       // Atmospheric pressure in cbar, bar, hPa, inHg, kPa, mbar, Pa or psi
            sensorclass_04,       // Percentage of battery that is left in %
            sensorclass_05,       // Carbon Dioxide in CO2 (Smoke) in ppm
            sensorclass_06,       // Carbon Monoxide in CO (Gas CNG/LPG) in ppm
            sensorclass_07,       // Current in A, mA
            sensorclass_08,       // Data rate in bit/s, kbit/s, Mbit/s, Gbit/s, B/s, kB/s, MB/s, GB/s, KiB/s, MiB/s or
            sensorclass_09,       // Data size in bit, kbit, Mbit, Gbit, B, kB, MB, GB, TB, PB, EB, ZB, YB, KiB, MiB, Gi
            sensorclass_10,       // Date string (ISO 8601)
            sensorclass_11,       // Generic distance in km, m, cm, mm, mi, yd, or in
            sensorclass_12,       // Duration in d, h, min, or s
            sensorclass_13,       // Energy in Wh, kWh, MWh, MJ, or GJ
            sensorclass_14,       // Stored energy in Wh, kWh, MWh, MJ, or GJ
            sensorclass_15,       // Has a limited set of (non-numeric) states
            sensorclass_16,       // Frequency in Hz, kHz, MHz, or GHz
            sensorclass_17,       // Gasvolume in m³, ft³ or CCF
            sensorclass_18,       // Percentage of humidity in the air in %
            sensorclass_19,       // The current light level in lx
            sensorclass_20,       // Irradiance in W/m² or BTU/(h⋅ft²)
            sensorclass_21,       // Percentage of water in a substance in %
            sensorclass_22,       // The monetary value (ISO 4217)
            sensorclass_23,       // Concentration of Nitrogen Dioxide in µg/m³
            sensorclass_24,       // Concentration of Nitrogen Monoxide in µg/m³
            sensorclass_25,       // Concentration of Nitrous Oxide in µg/m³
            sensorclass_26,       // Concentration of Ozone in µg/m³
            sensorclass_27,       // Concentration of particulate matter less than 1 micrometer in µg/m³
            sensorclass_28,       // Concentration of particulate matter less than 2.5 micrometers in µg/m³
            sensorclass_29,       // Concentration of particulate matter less than 10 micrometers in µg/m³
            sensorclass_30,       // Power factor (unitless), unit may be None or %
            sensorclass_31,       // Power in W or kW
            sensorclass_32,       // Accumulated precipitation in cm, in or mm
            sensorclass_33,       // Precipitation intensity in in/d, in/h, mm/d or mm/h
            sensorclass_34,       // Pressure in Pa, kPa, hPa, bar, cbar, mbar, mmHg, inHg or psi
            sensorclass_35,       // Reactive power in var
            sensorclass_36,       // Signal strength in dB or dBm
            sensorclass_37,       // Sound pressure in dB or dBA
            sensorclass_38,       // Generic speed in ft/s, in/d, in/h, km/h, kn, m/s, mph or mm/d
            sensorclass_39,       // Concentration of sulphur dioxide in µg/m³
            sensorclass_40,       // Temperature in °C, °F or K
            sensorclass_41,       // Datetime object or timestamp string (ISO 8601)
            sensorclass_42,       // Concentration of volatile organic compounds in µg/m³
            sensorclass_43,       // Ratio of volatile organic compounds in ppm or ppb
            sensorclass_44,       // Voltage in V, mV
            sensorclass_45,       // Generic volume in L, mL, gal, fl. oz., m³, ft³, or CCF
            sensorclass_46,       // Generic stored volume in L, mL, gal, fl. oz., m³, ft³, or CCF
            sensorclass_47,       // Water consumption in L, gal, m³, ft³, or CCF
            sensorclass_48,       // Generic mass in kg, g, mg, µg, oz, lb, or st
            sensorclass_49        // Wind speed in ft/s, km/h, kn, m/s, or mph
    };
    enum DeviceClass
    {
        DC_NOTDEFINED,
                                           // BINARY SENSOR
        DC_BATTERY,                        // on means low, off means normal
        DC_BATTERY_CHARGING,               // on means charging, off means not charging
        DC_COLD,                           // on means cold, off means normal
        DC_CONNECTIVITY,                   // on means connected, off means disconnected
        DC_DOOR,                           // on means open, off means closed
        DC_GARAGE_DOOR,                    // on means open, off means closed
        DC_GAS,                            // on means gas detected, off means no gas (clear)
        DC_HEAT,                           // on means hot, off means normal
        DC_LIGHT,                          // on means light detected, off means no light
        DC_LOCK,                           // on means open (unlocked), off means closed (locked)
        DC_MOISTURE,                       // on means moisture detected (wet), off means no moisture (dry)
        DC_MOTION,                         // on: on means motion detected, off means no motion (clear)
        DC_MOVING,                         // on means moving, off means not moving (stopped)
        DC_OCCUPANCY,                      // on means occupied (detected), off means not occupied (clear)
        DC_OPENING,                        // on means open, off means closed
        DC_PLUG,                           // on means device is plugged in, off means device is unplugged
        DC_POWER,                          // on means power detected, off means no power
        DC_PRESENCE,                       // on means home, off means away
        DC_PROBLEM,                        // on means problem detected, off means no problem (OK
        DC_SAFETY,                         // on means unsafe, off means safe
        DC_SMOKE,                          // on means smoke detected, off means no smoke (clear)
        DC_SOUND,                          // on means sound detected, off means no sound (clear)
        DC_VIBRATION,                      // on: on means vibration detected, off means no vibration (clear)
        DC_WINDOW,                         // on means open, off means closed
        DC_CARBON_MONOXIDE,                // on means carbon monoxide detected, off no carbon monoxide (clear)
        DC_TAMPER,                         // on means tampering detected, off means no tampering (clear)
        DC_UPDATE,                         // on means update available, off means up-to-date
        DC_RUNNING,                        // on means running, off means not running
                                           // SENSOR
        DC_APPARENT_POWER,                 // Apparent power in VA.
        DC_AQI,                            // Air Quality Index (unitless).
        DC_ATMOSPHERIC_PRESSURE,           // Atmospheric pressure in cbar, bar, hPa, inHg, kPa, mba
        DC_BATTERY,                        // Percentage of battery that is left in %
        DC_CARBON_DIOXIDE,                 // Carbon Dioxide in CO2 (Smoke) in ppm
        DC_CARBON_MONOXIDE,                // Carbon Monoxide in CO (Gas CNG/LPG) in ppm
        DC_CURRENT,                        // Current in A, mA
        DC_DATA_RATE,                      // Data rate in bit/s, kbit/s, Mbit/s, Gbit/s, B/s, kB/s,
        DC_DATA_SIZE,                      // Data size in bit, kbit, Mbit, Gbit, B, kB, MB, GB, TB,
        DC_DATE,                           // Date string (ISO 8601)
        DC_DISTANCE,                       // Generic distance in km, m, cm, mm, mi, yd, or in
        DC_DURATION,                       // Duration in d, h, min, or s
        DC_ENERGY,                         // Energy in Wh, kWh, MWh, MJ, or GJ
        DC_ENERGY_STORAGE,                 // Stored energy in Wh, kWh, MWh, MJ, or GJ
        DC_ENUM,                           // Has a limited set of (non-numeric) states
        DC_FREQUENCY,                      // Frequency in Hz, kHz, MHz, or GHz
        DC_GAS,                            // Gasvolume in m³, ft³ or CCF
        DC_HUMIDITY,                       // Percentage of humidity in the air in %
        DC_ILLUMINANCE,                    // The current light level in lx
        DC_IRRADIANCE,                     // Irradiance in W/m² or BTU/(h⋅ft²)
        DC_MOISTURE,                       // Percentage of water in a substance in %
        DC_MONETARY,                       // The monetary value (ISO 4217)
        DC_NITROGEN_DIOXIDE,               // Concentration of Nitrogen Dioxide in µg/m³
        DC_NITROGEN_MONOXIDE,              // Concentration of Nitrogen Monoxide in µg/m³
        DC_NITROUS_OXIDE,                  // Concentration of Nitrous Oxide in µg/m³
        DC_OZONE,                          // Concentration of Ozone in µg/m³
        DC_PM1,                            // Concentration of particulate matter less than 1 microm
        DC_PM25,                           // Concentration of particulate matter less than 2.5 micr
        DC_PM10,                           // Concentration of particulate matter less than 10 micro
        DC_POWER_FACTOR,                   // Power factor (unitless), unit may be None or %
        DC_POWER,                          // Power in W or kW
        DC_PRECIPITATION,                  // Accumulated precipitation in cm, in or mm
        DC_PRECIPITATION_INTENSITY,        // Precipitation intensity in in/d, in/h, mm/d or mm/h
        DC_PRESSURE,                       // Pressure in Pa, kPa, hPa, bar, cbar, mbar, mmHg, inHg
        DC_REACTIVE_POWER,                 // Reactive power in var
        DC_SIGNAL_STRENGTH,                // Signal strength in dB or dBm
        DC_SOUND_PRESSURE,                 // Sound pressure in dB or dBA
        DC_SPEED,                          // Generic speed in ft/s, in/d, in/h, km/h, kn, m/s, mph
        DC_SULPHUR_DIOXIDE,                // Concentration of sulphur dioxide in µg/m³
        DC_TEMPERATURE,                    // Temperature in °C, °F or K
        DC_TIMESTAMP,                      // Datetime object or timestamp string (ISO 8601)
        DC_VOLATILE_ORGANIC_COMPOUND,      // Concentration of volatile organic compounds in µg/m³
        DC_VOLATILE_ORGANIC_COMPOUND_PART, // Ratio of volatile organic compounds in ppm or ppb
        DC_VOLTAGE,                        // Voltage in V, mV
        DC_VOLUME,                         // Generic volume in L, mL, gal, fl. oz., m³, ft³, or CCF
        DC_VOLUME_STORAGE,                 // Generic stored volume in L, mL, gal, fl. oz., m³, ft³,
        DC_WATER,                          // Water consumption in L, gal, m³, ft³, or CCF
        DC_WEIGHT,                         // Generic mass in kg, g, mg, µg, oz, lb, or st
        DC_WIND_SPEED                      // Wind speed in ft/s, km/h, kn, m/s, or mph
    };
} // namespace HALIB_NAMESPACE