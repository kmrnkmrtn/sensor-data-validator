#include "sensor_validator/ThresholdChecker.hpp"

TestResult ThresholdChecker::check(const SensorReading& reading) {
    TestResult result;
    result.passed = true;

    if (reading.t_turbine_exit < 520.0 || reading.t_turbine_exit > 560.0) {
        result.failed_sensors.push_back("t_turbine_exit");
        result.passed = false;
    }
    if (reading.p_compressor_out < 170.0 || reading.p_compressor_out > 200.0) {
        result.failed_sensors.push_back("p_compressor_out");
        result.passed = false;
    }
    if (reading.fan_speed_rpm < 2370.0 || reading.fan_speed_rpm > 2410.0) {
        result.failed_sensors.push_back("fan_speed_rpm");
        result.passed = false;
    }
    if (reading.fuel_flow_ratio < 0.480 || reading.fuel_flow_ratio > 0.560) {
        result.failed_sensors.push_back("fuel_flow_ratio");
        result.passed = false;
    }
    if (reading.vibration < 0.0 || reading.vibration > 0.30) {
        result.failed_sensors.push_back("vibration");
        result.passed = false;
    }

    return result;
}