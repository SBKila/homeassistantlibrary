#include "HADeviceClass.hpp"

namespace HALIB_NAMESPACE
{
    // --- Existing Shared Strings ---
    const char s_none[] PROGMEM = "";
    const char s_batt[] PROGMEM = "battery";
    const char s_cold[] PROGMEM = "cold";
    const char s_conn[] PROGMEM = "connectivity";
    const char s_door[] PROGMEM = "door";
    const char s_gar_door[] PROGMEM = "garage_door";
    const char s_gas[] PROGMEM = "gas";
    const char s_heat[] PROGMEM = "heat";
    const char s_light[] PROGMEM = "light";
    const char s_lock[] PROGMEM = "lock";
    const char s_moist[] PROGMEM = "moisture";
    const char s_motion[] PROGMEM = "motion";
    const char s_moving[] PROGMEM = "moving";
    const char s_occup[] PROGMEM = "occupancy";
    const char s_opening[] PROGMEM = "opening";
    const char s_plug[] PROGMEM = "plug";
    const char s_pow[] PROGMEM = "power";
    const char s_presnc[] PROGMEM = "presence";
    const char s_prob[] PROGMEM = "problem";
    const char s_safety[] PROGMEM = "safety";
    const char s_smoke[] PROGMEM = "smoke";
    const char s_sound[] PROGMEM = "sound";
    const char s_vibr[] PROGMEM = "vibration";
    const char s_window[] PROGMEM = "window";
    const char s_curr[] PROGMEM = "current";
    const char s_energy[] PROGMEM = "energy";
    const char s_hum[] PROGMEM = "humidity";
    const char s_illum[] PROGMEM = "illuminance";
    const char s_sig_str[] PROGMEM = "signal_strength";
    const char s_temp[] PROGMEM = "temperature";
    const char s_pfac[] PROGMEM = "power_factor";
    const char s_pres[] PROGMEM = "pressure";
    const char s_time[] PROGMEM = "timestamp";
    const char s_volt[] PROGMEM = "voltage";

    // --- New Category Specific Strings ---
    const char s_ident[] PROGMEM = "identify";
    const char s_restrt[] PROGMEM = "restart";
    const char s_updt[] PROGMEM = "update";
    const char s_btn[] PROGMEM = "button";
    const char s_dbell[] PROGMEM = "doorbell";
    const char s_awn[] PROGMEM = "awning";
    const char s_blind[] PROGMEM = "blind";
    const char s_curt[] PROGMEM = "curtain";
    const char s_damp[] PROGMEM = "damper";
    const char s_gate[] PROGMEM = "gate";
    const char s_shade[] PROGMEM = "shade";
    const char s_shutt[] PROGMEM = "shutter";
    const char s_humdr[] PROGMEM = "humidifier";
    const char s_dehum[] PROGMEM = "dehumidifier";
    const char s_tv[] PROGMEM = "tv";
    const char s_spkr[] PROGMEM = "speaker";
    const char s_recv[] PROGMEM = "receiver";
    const char s_outl[] PROGMEM = "outlet";
    const char s_swch[] PROGMEM = "switch";
    const char s_frmw[] PROGMEM = "firmware";
    const char s_watr[] PROGMEM = "water";

    // --- Sensor/Number Specific Strings ---
    const char s_abs_hum[] PROGMEM = "absolute_humidity";
    const char s_app_pow[] PROGMEM = "apparent_power";
    const char s_aqi[] PROGMEM = "aqi";
    const char s_area[] PROGMEM = "area";
    const char s_atm_pres[] PROGMEM = "atmospheric_pressure";
    const char s_bgc[] PROGMEM = "blood_glucose_concentration";
    const char s_co2[] PROGMEM = "carbon_dioxide";
    const char s_co[] PROGMEM = "carbon_monoxide";
    const char s_drate[] PROGMEM = "data_rate";
    const char s_dsize[] PROGMEM = "data_size";
    const char s_date[] PROGMEM = "date";
    const char s_dist[] PROGMEM = "distance";
    const char s_dur[] PROGMEM = "duration";
    const char s_edist[] PROGMEM = "energy_distance";
    const char s_estor[] PROGMEM = "energy_storage";
    const char s_enum[] PROGMEM = "enum";
    const char s_freq[] PROGMEM = "frequency";
    const char s_irrad[] PROGMEM = "irradiance";
    const char s_monet[] PROGMEM = "monetary";
    const char s_no2[] PROGMEM = "nitrogen_dioxide";
    const char s_no[] PROGMEM = "nitrogen_monoxide";
    const char s_n2o[] PROGMEM = "nitrous_oxide";
    const char s_o3[] PROGMEM = "ozone";
    const char s_ph[] PROGMEM = "ph";
    const char s_pm1[] PROGMEM = "pm1";
    const char s_pm25[] PROGMEM = "pm25";
    const char s_pm4[] PROGMEM = "pm4";
    const char s_pm10[] PROGMEM = "pm10";
    const char s_prec[] PROGMEM = "precipitation";
    const char s_preci[] PROGMEM = "precipitation_intensity";
    const char s_re_en[] PROGMEM = "reactive_energy";
    const char s_re_pow[] PROGMEM = "reactive_power";
    const char s_snd_pres[] PROGMEM = "sound_pressure";
    const char s_speed[] PROGMEM = "speed";
    const char s_so2[] PROGMEM = "sulphur_dioxide";
    const char s_temp_d[] PROGMEM = "temperature_delta";
    const char s_voc[] PROGMEM = "volatile_organic_compounds";
    const char s_vocp[] PROGMEM = "volatile_organic_compounds_parts";
    const char s_vol[] PROGMEM = "volume";
    const char s_vflow[] PROGMEM = "volume_flow_rate";
    const char s_vstor[] PROGMEM = "volume_storage";
    const char s_weight[] PROGMEM = "weight";
    const char s_wdir[] PROGMEM = "wind_direction";
    const char s_wspd[] PROGMEM = "wind_speed";

    // --- Main Arrays Definition ---

    // 1. Binary Sensor (Manquait dans votre fichier)
    PGM_P const BinarySensorDeviceClassValue[] PROGMEM = {
        s_none, s_batt, s_cold, s_conn, s_door, s_gar_door, s_gas, s_heat,
        s_light, s_lock, s_moist, s_motion, s_moving, s_occup, s_opening, s_plug,
        s_pow, s_presnc, s_prob, s_safety, s_smoke, s_sound, s_vibr, s_window,
        s_curr, s_energy, s_hum, s_illum, s_sig_str, s_temp, s_pfac, s_pres,
        s_time, s_volt};

    PGM_P const SensorDeviceClassValue[] PROGMEM = {
        s_none, s_abs_hum, s_app_pow, s_aqi, s_area, s_atm_pres, s_batt, s_bgc, s_co2,
        s_co, s_curr, s_drate, s_dsize, s_date, s_dist, s_dur, s_energy, s_edist,
        s_estor, s_enum, s_freq, s_gas, s_hum, s_illum, s_irrad, s_moist, s_monet, s_no2,
        s_no, s_n2o, s_o3, s_ph, s_pm1, s_pm25, s_pm4, s_pm10, s_pfac, s_pow, s_prec, s_preci, s_pres};
    PGM_P const ButtonDeviceClassValue[] PROGMEM = {s_none, s_ident, s_restrt, s_updt};
    PGM_P const EventDeviceClassValue[] PROGMEM = {s_none, s_btn, s_dbell, s_motion};
    PGM_P const CoverDeviceClassValue[] PROGMEM = {
        s_none, s_awn, s_blind, s_curt, s_damp, s_door, s_gar_door, s_gate, s_shade, s_shutt, s_window};
    PGM_P const HumidifierDeviceClassValue[] PROGMEM = {s_humdr, s_dehum};
    PGM_P const MediaPlayerDeviceClassValue[] PROGMEM = {s_tv, s_spkr, s_recv};
    PGM_P const SwitchDeviceClassValue[] PROGMEM = {s_none, s_outl, s_swch};
    PGM_P const UpdateDeviceClassValue[] PROGMEM = {s_none, s_frmw};
    PGM_P const ValveDeviceClassValue[] PROGMEM = {s_none, s_watr, s_gas};

    // 4. Number (CORRIGÉ: suppression de s_date pour s'aligner avec l'Enum)
    PGM_P const NumberDeviceClassValue[] PROGMEM = {
        s_none, s_abs_hum, s_app_pow, s_aqi, s_area, s_atm_pres, s_batt, s_bgc, s_co2,
        s_co, s_curr, s_drate, s_dsize,
        s_dist, s_dur, s_energy, s_edist,
        s_estor, s_freq, s_gas, s_hum, s_illum, s_irrad, s_moist, s_monet, s_no2,
        s_no, s_n2o, s_o3, s_ph, s_pm1, s_pm25, s_pm4, s_pm10, s_pfac, s_pow,
        s_prec, s_preci, s_pres, s_re_en, s_re_pow, s_sig_str, s_snd_pres, s_speed,
        s_so2, s_temp, s_temp_d, s_voc, s_vocp, s_volt, s_vol, s_vflow, s_vstor,
        s_watr, s_weight, s_wdir, s_wspd};
} // namespace HALIB_NAMESPACE