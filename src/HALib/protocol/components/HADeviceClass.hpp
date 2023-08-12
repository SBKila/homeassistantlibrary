#pragma once
#include <pgmspace.h>

namespace HALIB_NAMESPACE
{
    const char class_00[] PROGMEM = "";                                       // default
    const char stringclass_00[] PROGMEM = "battery";                    // on means low, off means normal
    const char stringclass_01[] PROGMEM = "battery_charging";           // on means charging, off means not charging
    const char stringclass_02[] PROGMEM = "cold";                       // on means cold, off means normal
    const char stringclass_03[] PROGMEM = "connectivity";               // on means connected, off means disconnected
    const char stringclass_04[] PROGMEM = "door";                       // on means open, off means closed
    const char stringclass_05[] PROGMEM = "garage_door";                // on means open, off means closed
    const char stringclass_06[] PROGMEM = "gas";                        // on means gas detected, off means no gas (clear)
    const char stringclass_07[] PROGMEM = "heat";                       // on means hot, off means normal
    const char stringclass_08[] PROGMEM = "light";                      // on means light detected, off means no light
    const char stringclass_09[] PROGMEM = "lock";                       // on means open (unlocked), off means closed (locked)
    const char stringclass_10[] PROGMEM = "moisture";                   // on means moisture detected (wet), off means no moisture (dry)
    const char stringclass_11[] PROGMEM = "motion";                     // on: on means motion detected, off means no motion (clear)
    const char stringclass_12[] PROGMEM = "moving";                     // on means moving, off means not moving (stopped)
    const char stringclass_13[] PROGMEM = "occupancy";                  // on means occupied (detected), off means not occupied (clear)
    const char stringclass_14[] PROGMEM = "opening";                    // on means open, off means closed
    const char stringclass_15[] PROGMEM = "plug";                       // on means device is plugged in, off means device is unplugged
    const char stringclass_16[] PROGMEM = "power";                      // on means power detected, off means no power
    const char stringclass_17[] PROGMEM = "presence";                   // on means home, off means away
    const char stringclass_18[] PROGMEM = "problem";                    // on means problem detected, off means no problem (OK
    const char stringclass_19[] PROGMEM = "safety";                     // on means unsafe, off means safe
    const char stringclass_20[] PROGMEM = "smoke";                      // on means smoke detected, off means no smoke (clear)
    const char stringclass_21[] PROGMEM = "sound";                      // on means sound detected, off means no sound (clear)
    const char stringclass_22[] PROGMEM = "vibration";                  // on: on means vibration detected, off means no vibration (clear)
    const char stringclass_23[] PROGMEM = "window";                     // on means open, off means closed
    const char stringclass_24[] PROGMEM = "carbon_monoxide";            // on means carbon monoxide detected, off no carbon monoxide (clear)
    const char stringclass_25[] PROGMEM = "tamper";                     // on means tampering detected, off means no tampering (clear)
    const char stringclass_26[] PROGMEM = "update";                     // on means update available, off means up-to-date
    const char stringclass_27[] PROGMEM = "running";                    // on means running, off means not running
    const char stringclass_31[] PROGMEM = "apparent_power";                   // Apparent power in VA.
    const char stringclass_32[] PROGMEM = "aqi";                              // Air Quality Index (unitless).
    const char stringclass_33[] PROGMEM = "atmospheric_pressure";             // Atmospheric pressure in cbar, bar, hPa, inHg, kPa, mbar, Pa or psi
    // const char stringclass_34[] PROGMEM = "battery";                          // Percentage of battery that is left in %
    const char stringclass_35[] PROGMEM = "carbon_dioxide";                   // Carbon Dioxide in CO2 (Smoke) in ppm
    // const char stringclass_36[] PROGMEM = "carbon_monoxide";                  // Carbon Monoxide in CO (Gas CNG/LPG) in ppm
    const char stringclass_37[] PROGMEM = "current";                          // Current in A, mA
    const char stringclass_38[] PROGMEM = "data_rate";                        // Data rate in bit/s, kbit/s, Mbit/s, Gbit/s, B/s, kB/s, MB/s, GB/s, KiB/s, MiB/s or GiB/s
    const char stringclass_39[] PROGMEM = "data_size";                        // Data size in bit, kbit, Mbit, Gbit, B, kB, MB, GB, TB, PB, EB, ZB, YB, KiB, MiB, GiB, TiB, PiB, EiB, ZiB or YiB
    const char stringclass_40[] PROGMEM = "date";                             // Date string (ISO 8601)
    const char stringclass_41[] PROGMEM = "distance";                         // Generic distance in km, m, cm, mm, mi, yd, or in
    const char stringclass_42[] PROGMEM = "duration";                         // Duration in d, h, min, or s
    const char stringclass_43[] PROGMEM = "energy";                           // Energy in Wh, kWh, MWh, MJ, or GJ
    const char stringclass_44[] PROGMEM = "energy_storage";                   // Stored energy in Wh, kWh, MWh, MJ, or GJ
    const char stringclass_45[] PROGMEM = "enum";                             // Has a limited set of (non-numeric) states
    const char stringclass_46[] PROGMEM = "frequency";                        // Frequency in Hz, kHz, MHz, or GHz
    // const char stringclass_47[] PROGMEM = "gas";                              // Gasvolume in m³, ft³ or CCF
    const char stringclass_48[] PROGMEM = "humidity";                         // Percentage of humidity in the air in %
    const char stringclass_49[] PROGMEM = "illuminance";                      // The current light level in lx
    const char stringclass_50[] PROGMEM = "irradiance";                       // Irradiance in W/m² or BTU/(h⋅ft²)
    // const char stringclass_51[] PROGMEM = "moisture";                         // Percentage of water in a substance in %
    const char stringclass_52[] PROGMEM = "monetary";                         // The monetary value (ISO 4217)
    const char stringclass_53[] PROGMEM = "nitrogen_dioxide";                 // Concentration of Nitrogen Dioxide in µg/m³
    const char stringclass_54[] PROGMEM = "nitrogen_monoxide";                // Concentration of Nitrogen Monoxide in µg/m³
    const char stringclass_55[] PROGMEM = "nitrous_oxide";                    // Concentration of Nitrous Oxide in µg/m³
    const char stringclass_56[] PROGMEM = "ozone";                            // Concentration of Ozone in µg/m³
    const char stringclass_57[] PROGMEM = "pm1";                              // Concentration of particulate matter less than 1 micrometer in µg/m³
    const char stringclass_58[] PROGMEM = "pm25";                             // Concentration of particulate matter less than 2.5 micrometers in µg/m³
    const char stringclass_59[] PROGMEM = "pm10";                             // Concentration of particulate matter less than 10 micrometers in µg/m³
    const char stringclass_60[] PROGMEM = "power_factor";                     // Power factor (unitless), unit may be None or %
    // const char stringclass_61[] PROGMEM = "power";                            // Power in W or kW
    const char stringclass_62[] PROGMEM = "precipitation";                    // Accumulated precipitation in cm, in or mm
    const char stringclass_63[] PROGMEM = "precipitation_intensity";          // Precipitation intensity in in/d, in/h, mm/d or mm/h
    const char stringclass_64[] PROGMEM = "pressure";                         // Pressure in Pa, kPa, hPa, bar, cbar, mbar, mmHg, inHg or psi
    const char stringclass_65[] PROGMEM = "reactive_power";                   // Reactive power in var
    const char stringclass_66[] PROGMEM = "signal_strength";                  // Signal strength in dB or dBm
    const char stringclass_67[] PROGMEM = "sound_pressure";                   // Sound pressure in dB or dBA
    const char stringclass_68[] PROGMEM = "speed";                            // Generic speed in ft/s, in/d, in/h, km/h, kn, m/s, mph or mm/d
    const char stringclass_69[] PROGMEM = "sulphur_dioxide";                  // Concentration of sulphur dioxide in µg/m³
    const char stringclass_70[] PROGMEM = "temperature";                      // Temperature in °C, °F or K
    const char stringclass_71[] PROGMEM = "timestamp";                        // Datetime object or timestamp string (ISO 8601)
    const char stringclass_72[] PROGMEM = "volatile_organic_compounds";       // Concentration of volatile organic compounds in µg/m³
    const char stringclass_73[] PROGMEM = "volatile_organic_compounds_parts"; // Ratio of volatile organic compounds in ppm or ppb
    const char stringclass_74[] PROGMEM = "voltage";                          // Voltage in V, mV
    const char stringclass_75[] PROGMEM = "volume";                           // Generic volume in L, mL, gal, fl. oz., m³, ft³, or CCF
    const char stringclass_76[] PROGMEM = "volume_storage";                   // Generic stored volume in L, mL, gal, fl. oz., m³, ft³, or CCF
    const char stringclass_77[] PROGMEM = "water";                            // Water consumption in L, gal, m³, ft³, or CCF
    const char stringclass_78[] PROGMEM = "weight";                           // Generic mass in kg, g, mg, µg, oz, lb, or st
    const char stringclass_79[] PROGMEM = "wind_speed";                       // Wind speed in ft/s, km/h, kn, m/s, or mph

    PGM_P DeviceClassValue[] PROGMEM =
        {
            class_00,             // default
            stringclass_00, // low, off means normal
            stringclass_01, // charging, off means not charging
            stringclass_02, // cold, off means normal
            stringclass_03, // connected, off means disconnected
            stringclass_04, // open, off means closed
            stringclass_05, // open, off means closed
            stringclass_06, // gas detected, off means no gas (clear)
            stringclass_07, // hot, off means normal
            stringclass_08, // light detected, off means no light
            stringclass_09, // open (unlocked), off means closed (locked)
            stringclass_10, // moisture detected (wet), off means no moisture (dry)
            stringclass_11, // ans motion detected, off means no motion (clear)
            stringclass_12, // moving, off means not moving (stopped)
            stringclass_13, // occupied (detected), off means not occupied (clear)
            stringclass_14, // open, off means closed
            stringclass_15, // device is plugged in, off means device is unplugged
            stringclass_16, // power detected, off means no power
            stringclass_17, // home, off means away
            stringclass_18, // problem detected, off means no problem (OK
            stringclass_19, // unsafe, off means safe
            stringclass_20, // smoke detected, off means no smoke (clear)
            stringclass_21, // sound detected, off means no sound (clear)
            stringclass_22, // ans vibration detected, off means no vibration (clear)
            stringclass_23, // open, off means closed
            stringclass_24, // carbon monoxide detected, off no carbon monoxide (clear)
            stringclass_25, // tampering detected, off means no tampering (clear)
            stringclass_26, // update available, off means up-to-date
            stringclass_27, // running, off means not running
            stringclass_31,       // Apparent power in VA.
            stringclass_32,       // Air Quality Index (unitless).
            stringclass_33,       // Atmospheric pressure in cbar, bar, hPa, inHg, kPa, mbar, Pa or psi
            // stringclass_34,       // Percentage of battery that is left in %
            stringclass_35,       // Carbon Dioxide in CO2 (Smoke) in ppm
            // stringclass_36,       // Carbon Monoxide in CO (Gas CNG/LPG) in ppm
            stringclass_37,       // Current in A, mA
            stringclass_38,       // Data rate in bit/s, kbit/s, Mbit/s, Gbit/s, B/s, kB/s, MB/s, GB/s, KiB/s, MiB/s or
            stringclass_39,       // Data size in bit, kbit, Mbit, Gbit, B, kB, MB, GB, TB, PB, EB, ZB, YB, KiB, MiB, Gi
            stringclass_40,       // Date string (ISO 8601)
            stringclass_41,       // Generic distance in km, m, cm, mm, mi, yd, or in
            stringclass_42,       // Duration in d, h, min, or s
            stringclass_43,       // Energy in Wh, kWh, MWh, MJ, or GJ
            stringclass_44,       // Stored energy in Wh, kWh, MWh, MJ, or GJ
            stringclass_45,       // Has a limited set of (non-numeric) states
            stringclass_46,       // Frequency in Hz, kHz, MHz, or GHz
            // stringclass_47,       // Gasvolume in m³, ft³ or CCF
            stringclass_48,       // Percentage of humidity in the air in %
            stringclass_49,       // The current light level in lx
            stringclass_50,       // Irradiance in W/m² or BTU/(h⋅ft²)
            // stringclass_51,       // Percentage of water in a substance in %
            stringclass_52,       // The monetary value (ISO 4217)
            stringclass_53,       // Concentration of Nitrogen Dioxide in µg/m³
            stringclass_54,       // Concentration of Nitrogen Monoxide in µg/m³
            stringclass_55,       // Concentration of Nitrous Oxide in µg/m³
            stringclass_56,       // Concentration of Ozone in µg/m³
            stringclass_57,       // Concentration of particulate matter less than 1 micrometer in µg/m³
            stringclass_58,       // Concentration of particulate matter less than 2.5 micrometers in µg/m³
            stringclass_59,       // Concentration of particulate matter less than 10 micrometers in µg/m³
            stringclass_60,       // Power factor (unitless), unit may be None or %
            // stringclass_61,       // Power in W or kW
            stringclass_62,       // Accumulated precipitation in cm, in or mm
            stringclass_63,       // Precipitation intensity in in/d, in/h, mm/d or mm/h
            stringclass_64,       // Pressure in Pa, kPa, hPa, bar, cbar, mbar, mmHg, inHg or psi
            stringclass_65,       // Reactive power in var
            stringclass_66,       // Signal strength in dB or dBm
            stringclass_67,       // Sound pressure in dB or dBA
            stringclass_68,       // Generic speed in ft/s, in/d, in/h, km/h, kn, m/s, mph or mm/d
            stringclass_69,       // Concentration of sulphur dioxide in µg/m³
            stringclass_70,       // Temperature in °C, °F or K
            stringclass_71,       // Datetime object or timestamp string (ISO 8601)
            stringclass_72,       // Concentration of volatile organic compounds in µg/m³
            stringclass_73,       // Ratio of volatile organic compounds in ppm or ppb
            stringclass_74,       // Voltage in V, mV
            stringclass_75,       // Generic volume in L, mL, gal, fl. oz., m³, ft³, or CCF
            stringclass_76,       // Generic stored volume in L, mL, gal, fl. oz., m³, ft³, or CCF
            stringclass_77,       // Water consumption in L, gal, m³, ft³, or CCF
            stringclass_78,       // Generic mass in kg, g, mg, µg, oz, lb, or st
            stringclass_79        // Wind speed in ft/s, km/h, kn, m/s, or mph
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
        // DC_BATTERY,                        // Percentage of battery that is left in %
        DC_CARBON_DIOXIDE,                 // Carbon Dioxide in CO2 (Smoke) in ppm
        // DC_CARBON_MONOXIDE,                // Carbon Monoxide in CO (Gas CNG/LPG) in ppm
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
        // DC_GAS,                            // Gasvolume in m³, ft³ or CCF
        DC_HUMIDITY,                       // Percentage of humidity in the air in %
        DC_ILLUMINANCE,                    // The current light level in lx
        DC_IRRADIANCE,                     // Irradiance in W/m² or BTU/(h⋅ft²)
        // DC_MOISTURE,                       // Percentage of water in a substance in %
        DC_MONETARY,                       // The monetary value (ISO 4217)
        DC_NITROGEN_DIOXIDE,               // Concentration of Nitrogen Dioxide in µg/m³
        DC_NITROGEN_MONOXIDE,              // Concentration of Nitrogen Monoxide in µg/m³
        DC_NITROUS_OXIDE,                  // Concentration of Nitrous Oxide in µg/m³
        DC_OZONE,                          // Concentration of Ozone in µg/m³
        DC_PM1,                            // Concentration of particulate matter less than 1 microm
        DC_PM25,                           // Concentration of particulate matter less than 2.5 micr
        DC_PM10,                           // Concentration of particulate matter less than 10 micro
        DC_POWER_FACTOR,                   // Power factor (unitless), unit may be None or %
        // DC_POWER,                          // Power in W or kW
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