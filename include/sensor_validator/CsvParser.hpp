#pragma once
#include "sensor_validator/SensorReading.hpp"
#include <string>
#include <vector>

class CsvParser {
public:
    static SensorReading parseLine(const std::string& line);
    static std::vector<SensorReading> parseFile(const std::string& filename);

};