# Sensor Data Validator

A small C++ tool that reads synthetic turbofan engine sensor data from a CSV file and validates each reading against configured thresholds, reporting PASS/FAIL per cycle. Built as a System Test / Engineering portfolio project using TDD.

## What it does

- Parses CSV sensor readings (`unit_id`, `cycle`, plus 5 sensor values: turbine exit temperature, compressor outlet pressure, fan speed, fuel flow ratio, vibration)
- Checks each reading against min/max thresholds per sensor
- Reports which sensor(s) failed for each out-of-range reading
- Prints a summary (PASS/FAIL count) at the end

This mirrors a typical system-test pattern: input data → test criteria → pass/fail result, applied to industrial sensor telemetry.

## Data

The included `sample.csv` is synthetically generated (see `generate_data.py`), modeled after the structure of NASA's C-MAPSS turbofan degradation dataset, since the original dataset wasn't accessible at the time. Three simulated engines run for a number of cycles each; one engine is deliberately configured to drift past thresholds near the end of its run, giving known, reproducible FAIL cases for testing.

## Build & run

Requires CMake (3.14+) and a C++17 compiler.

```bash
mkdir build && cd build
cmake ..
cmake --build .
./Debug/sensor_validator.exe   # adjust path/extension for your platform
```

## Tests

Unit tests use Google Test (fetched automatically via CMake FetchContent), covering the CSV parser (valid lines, file parsing, malformed input) and the threshold checker (pass case, single/multiple sensor failures).

```bash
ctest
```

## Notes / future improvements

- `CsvParser` and `ThresholdChecker` are currently compiled separately into both the main executable and the test binary; a shared static library would be the cleaner long-term setup
- No CI pipeline yet (planned: GitHub Actions running build + ctest on push)
- Threshold boundaries are simple min/max checks; no hysteresis or multi-sample confirmation, so isolated sensor noise can trigger a single-cycle FAIL (visible in the sample output) — a real system would likely require consecutive failures before flagging

## AI assistance disclosure

The synthetic data generator and initial CMake/project scaffolding were put together with AI assistance to speed up setup. Test design, threshold decisions, and the TDD implementation of the parsing/validation logic are my own work.