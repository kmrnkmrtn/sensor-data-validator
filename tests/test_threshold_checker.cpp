#include <gtest/gtest.h>
#include "sensor_validator/ThresholdChecker.hpp"

TEST(ThresholdCheckerTest, AllValuesWithinRange_Passes) {
    SensorReading reading{1, 1, 540.0, 185.0, 2388.0, 0.520, 0.10};

    TestResult result = ThresholdChecker::check(reading);

    EXPECT_TRUE(result.passed);
    EXPECT_TRUE(result.failed_sensors.empty());
}

TEST(ThresholdCheckerTest, VibrationFails){
    SensorReading reading{1, 1, 540.0, 185.0, 2388.0, 0.520, 0.33};

    TestResult result = ThresholdChecker::check(reading);

    
    EXPECT_FALSE(result.passed);
    EXPECT_EQ(result.failed_sensors.size(),1);

}