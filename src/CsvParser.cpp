#include "sensor_validator/CsvParser.hpp"
#include <sstream>
#include <fstream>
#include <vector>
#include <stdexcept>

SensorReading CsvParser::parseLine(const std::string& line) {
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;

    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    
    if (fields.size() != 7) {
        throw std::runtime_error("Malformed CSV line: expected 7 fields, got " + std::to_string(fields.size()));
    }
    SensorReading reading;
    reading.unit_id = std::stoi(fields[0]);
    reading.cycle = std::stoi(fields[1]);
    reading.t_turbine_exit = std::stod(fields[2]);
    reading.p_compressor_out = std::stod(fields[3]);
    reading.fan_speed_rpm = std::stod(fields[4]);
    reading.fuel_flow_ratio = std::stod(fields[5]);
    reading.vibration = std::stod(fields[6]);

    return reading;
}

std::vector<SensorReading> CsvParser::parseFile(const std::string& filename) {
    std::vector<SensorReading> sensorReadingVector;
    std::string line;
    std::ifstream file(filename);
    std::getline(file, line); 
    while(std::getline(file,line))
    {
        sensorReadingVector.push_back(CsvParser::parseLine(line));
    }
    return sensorReadingVector; 
    //close file?
}

