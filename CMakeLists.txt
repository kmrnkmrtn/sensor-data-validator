cmake_minimum_required(VERSION 3.14)
project(sensor_data_validator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sensor_validator 
    src/main.cpp
    src/CsvParser.cpp
    src/ThresholdChecker.cpp
)

target_include_directories(sensor_validator PRIVATE include)

# --- Testing setup ---
enable_testing()
add_subdirectory(tests)