#pragma once
#include <pgmspace.h>

namespace HALIB_NAMESPACE
{
    // --- Enums ---
    enum BinarySensorDeviceClass : uint8_t
    {
        DC_NOTDEFINED = 0,
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

    enum SensorDeviceClass : uint8_t
    {
        SC_NONE = 0,
        SC_ABSOLUTE_HUMIDITY,
        SC_APPARENT_POWER,
        SC_AQI,
        SC_AREA,
        SC_ATMOSPHERIC_PRESSURE,
        SC_BATTERY,
        SC_BLOOD_GLUCOSE_CONCENTRATION,
        SC_CARBON_DIOXIDE,
        SC_CARBON_MONOXIDE,
        SC_CURRENT,
        SC_DATA_RATE,
        SC_DATA_SIZE,
        SC_DATE,
        SC_DISTANCE,
        SC_DURATION,
        SC_ENERGY,
        SC_ENERGY_DISTANCE,
        SC_ENERGY_STORAGE,
        SC_ENUM,
        SC_FREQUENCY,
        SC_GAS,
        SC_HUMIDITY,
        SC_ILLUMINANCE,
        SC_IRRADIANCE,
        SC_MOISTURE,
        SC_MONETARY,
        SC_NITROGEN_DIOXIDE,
        SC_NITROGEN_MONOXIDE,
        SC_NITROUS_OXIDE,
        SC_OZONE,
        SC_PH,
        SC_PM1,
        SC_PM25,
        SC_PM4,
        SC_PM10,
        SC_POWER_FACTOR,
        SC_POWER,
        SC_PRECIPITATION,
        SC_PRECIPITATION_INTENSITY,
        SC_PRESSURE,
        SC_REACTIVE_ENERGY,
        SC_REACTIVE_POWER,
        SC_SIGNAL_STRENGTH,
        SC_SOUND_PRESSURE,
        SC_SPEED,
        SC_SULPHUR_DIOXIDE,
        SC_TEMPERATURE,
        SC_TEMPERATURE_DELTA,
        SC_TIMESTAMP,
        SC_VOC,
        SC_VOC_PARTS,
        SC_VOLTAGE,
        SC_VOLUME,
        SC_VOLUME_FLOW_RATE,
        SC_VOLUME_STORAGE,
        SC_WATER,
        SC_WEIGHT,
        SC_WIND_DIRECTION,
        SC_WIND_SPEED
    };

    enum ButtonDeviceClass : uint8_t
    {
        BC_NONE = 0,
        BC_IDENTIFY,
        BC_RESTART,
        BC_UPDATE
    };

    enum EventDeviceClass : uint8_t
    {
        EC_NONE = 0,
        EC_BUTTON,
        EC_DOORBELL,
        EC_MOTION
    };

    enum CoverDeviceClass : uint8_t
    {
        CC_NONE = 0,
        CC_AWNING,
        CC_BLIND,
        CC_CURTAIN,
        CC_DAMPER,
        CC_DOOR,
        CC_GARAGE,
        CC_GATE,
        CC_SHADE,
        CC_SHUTTER,
        CC_WINDOW
    };
    enum NumberDeviceClass : uint8_t
    {
        NC_NONE = 0,
        NC_ABSOLUTE_HUMIDITY,
        NC_APPARENT_POWER,
        NC_AQI,
        NC_AREA,
        NC_ATMOSPHERIC_PRESSURE,
        NC_BATTERY,
        NC_BLOOD_GLUCOSE_CONCENTRATION,
        NC_CARBON_DIOXIDE,
        NC_CARBON_MONOXIDE,
        NC_CURRENT,
        NC_DATA_RATE,
        NC_DATA_SIZE,
        NC_DISTANCE,
        NC_DURATION,
        NC_ENERGY,
        NC_ENERGY_DISTANCE,
        NC_ENERGY_STORAGE,
        NC_FREQUENCY,
        NC_GAS,
        NC_HUMIDITY,
        NC_ILLUMINANCE,
        NC_IRRADIANCE,
        NC_MOISTURE,
        NC_MONETARY,
        NC_NITROGEN_DIOXIDE,
        NC_NITROGEN_MONOXIDE,
        NC_NITROUS_OXIDE,
        NC_OZONE,
        NC_PH,
        NC_PM1,
        NC_PM25,
        NC_PM4,
        NC_PM10,
        NC_POWER_FACTOR,
        NC_POWER,
        NC_PRECIPITATION,
        NC_PRECIPITATION_INTENSITY,
        NC_PRESSURE,
        NC_REACTIVE_ENERGY,
        NC_REACTIVE_POWER,
        NC_SIGNAL_STRENGTH,
        NC_SOUND_PRESSURE,
        NC_SPEED,
        NC_SULPHUR_DIOXIDE,
        NC_TEMPERATURE,
        NC_TEMPERATURE_DELTA,
        NC_VOC,
        NC_VOC_PARTS,
        NC_VOLTAGE,
        NC_VOLUME,
        NC_VOLUME_FLOW_RATE,
        NC_VOLUME_STORAGE,
        NC_WATER,
        NC_WEIGHT,
        NC_WIND_DIRECTION,
        NC_WIND_SPEED
    };

    enum HumidifierDeviceClass : uint8_t
    {
        HC_HUMIDIFIER = 0,
        HC_DEHUMIDIFIER
    };

    enum MediaPlayerDeviceClass : uint8_t
    {
        MC_TV = 0,
        MC_SPEAKER,
        MC_RECEIVER
    };

    enum SwitchDeviceClass : uint8_t
    {
        SCW_NONE = 0, // SCW pour Switch, SC déjà pris par Sensor
        SCW_OUTLET,
        SCW_SWITCH
    };

    enum UpdateDeviceClass : uint8_t
    {
        UC_NONE = 0,
        UC_FIRMWARE
    };

    enum ValveDeviceClass : uint8_t
    {
        VC_NONE = 0,
        VC_WATER,
        VC_GAS
    };

    extern PGM_P const BinarySensorDeviceClassValue[] PROGMEM; // Binary Sensor
    extern PGM_P const SensorDeviceClassValue[] PROGMEM;       // Sensor
    extern PGM_P const NumberDeviceClassValue[] PROGMEM;       // Number
    extern PGM_P const ButtonDeviceClassValue[] PROGMEM;
    extern PGM_P const EventDeviceClassValue[] PROGMEM;
    extern PGM_P const CoverDeviceClassValue[] PROGMEM;
    extern PGM_P const HumidifierDeviceClassValue[] PROGMEM;
    extern PGM_P const MediaPlayerDeviceClassValue[] PROGMEM;
    extern PGM_P const SwitchDeviceClassValue[] PROGMEM;
    extern PGM_P const UpdateDeviceClassValue[] PROGMEM;
    extern PGM_P const ValveDeviceClassValue[] PROGMEM;

} // namespace HALIB_NAMESPACE