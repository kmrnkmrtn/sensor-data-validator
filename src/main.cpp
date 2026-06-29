#include "sensor_validator/CsvParser.hpp"
#include "sensor_validator/ThresholdChecker.hpp"
#include <iostream>

int main() {
    
    std::vector<SensorReading> readings = CsvParser::parseFile("sample.csv");

    int passCount = 0;
    int failCount = 0;

    for (const auto& reading : readings) {
        TestResult result = ThresholdChecker::check(reading);
        if (result.passed) {
            passCount++;
        } else {
            failCount++;
            std::cout << "FAIL unit " << reading.unit_id << " cycle " << reading.cycle << ": ";
            for (const auto& sensor : result.failed_sensors) {
                std::cout << sensor << " ";
            }
            std::cout << "\n";
        }
    }

    std::cout << "\nSummary: " << passCount << " PASS, " << failCount << " FAIL out of " << readings.size() << " readings\n";

    return 0;
}