#pragma once

struct SensorReading {
    int unit_id;
    int cycle;
    double t_turbine_exit;
    double p_compressor_out;
    double fan_speed_rpm;
    double fuel_flow_ratio;
    double vibration;
};