#include <gtest/gtest.h>
#include "sensor_validator/CsvParser.hpp"

TEST(CsvParserTest, ParsesSingleValidLine) {
    std::string line = "1,5,540.2,185.3,2388.0,0.521,0.12";

    SensorReading result = CsvParser::parseLine(line);

    EXPECT_EQ(result.unit_id, 1);
    EXPECT_EQ(result.cycle, 5);
    EXPECT_DOUBLE_EQ(result.t_turbine_exit, 540.2);
    EXPECT_DOUBLE_EQ(result.p_compressor_out, 185.3);
    EXPECT_DOUBLE_EQ(result.fan_speed_rpm, 2388.0);
    EXPECT_DOUBLE_EQ(result.fuel_flow_ratio, 0.521);
    EXPECT_DOUBLE_EQ(result.vibration, 0.12);
}

TEST(CsvParserTest, ParseesFileWithHeader) {
    std::string filename = TEST_DATA_DIR "sample_small.csv";
    std::vector<SensorReading> result = CsvParser::parseFile(filename);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].unit_id,1);
    EXPECT_EQ(result[0].unit_id,result[1].unit_id);
}

TEST(CsvParserTest, ThrowsOnMalformedLine) {
    std::string badLine = "1,5,540.2";  

    EXPECT_THROW(CsvParser::parseLine(badLine), std::runtime_error);
}