#pragma once
#include "sensor_validator/SensorReading.hpp"
#include <vector>
#include <string>

struct TestResult {
    bool passed;
    std::vector<std::string> failed_sensors;
};

class ThresholdChecker {
public:
    static TestResult check(const SensorReading& reading);
};