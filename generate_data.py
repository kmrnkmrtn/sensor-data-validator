#!/usr/bin/env python3
"""
Synthetic turbofan sensor data generator.

Generates a CSV with columns:
unit_id, cycle, t_turbine_exit, p_compressor_out, fan_speed_rpm, fuel_flow_ratio, vibration

Each "unit" (engine) runs for a number of cycles. Most units degrade mildly
and never cross failure thresholds. One or more units are configured to
deliberately exceed thresholds near the end of their run, giving known,
reproducible FAIL cases for testing the C++ ThresholdChecker.
"""

import csv
import random

random.seed(42)  # reproducible output

# Nominal (healthy, cycle-0) sensor values
NOMINAL = {
    "t_turbine_exit": 540.0,    # deg R, rises with degradation
    "p_compressor_out": 185.0,  # psi, falls with degradation
    "fan_speed_rpm": 2388.0,    # roughly stable
    "fuel_flow_ratio": 0.520,   # rises with degradation
    "vibration": 0.10,          # mm/s, rises with degradation, spikes near failure
}

# Thresholds the C++ ThresholdChecker will use (keep in sync with C++ config!)
THRESHOLDS = {
    "t_turbine_exit": (520.0, 560.0),
    "p_compressor_out": (170.0, 200.0),
    "fan_speed_rpm": (2370.0, 2410.0),
    "fuel_flow_ratio": (0.480, 0.560),
    "vibration": (0.0, 0.30),
}

# Per-unit configuration: (num_cycles, degrade_strength, force_failure)
# degrade_strength: how much drift accumulates by the last cycle (0 = none)
# force_failure: if True, last N cycles deliberately pushed past threshold
UNIT_CONFIGS = [
    {"unit_id": 1, "cycles": 80, "degrade_strength": 0.3, "force_failure": False},
    {"unit_id": 2, "cycles": 90, "degrade_strength": 0.5, "force_failure": False},
    {"unit_id": 3, "cycles": 70, "degrade_strength": 1.0, "force_failure": True},
]

FORCE_FAILURE_WINDOW = 10  # last N cycles where failure is forced


def gen_value(sensor, cycle, total_cycles, degrade_strength, direction, force_failure):
    nominal = NOMINAL[sensor]
    noise = random.gauss(0, nominal * 0.003)  # small measurement noise

    progress = cycle / total_cycles  # 0..1
    drift = direction * degrade_strength * progress * (nominal * 0.05)

    value = nominal + drift + noise

    if force_failure and cycle > total_cycles - FORCE_FAILURE_WINDOW:
        low, high = THRESHOLDS[sensor]
        # push value just past the relevant boundary, scaled by how deep
        # into the forced-failure window we are
        depth = (cycle - (total_cycles - FORCE_FAILURE_WINDOW)) / FORCE_FAILURE_WINDOW
        if direction >= 0:
            value = high + depth * (nominal * 0.05) + abs(noise)
        else:
            value = low - depth * (nominal * 0.05) - abs(noise)

    return round(value, 3)


# direction: +1 means sensor rises with degradation, -1 means it falls
DIRECTIONS = {
    "t_turbine_exit": +1,
    "p_compressor_out": -1,
    "fan_speed_rpm": 0,   # stays roughly flat, only noise
    "fuel_flow_ratio": +1,
    "vibration": +1,
}


def generate_rows():
    rows = []
    for cfg in UNIT_CONFIGS:
        unit_id = cfg["unit_id"]
        total_cycles = cfg["cycles"]
        degrade_strength = cfg["degrade_strength"]
        force_failure = cfg["force_failure"]

        for cycle in range(1, total_cycles + 1):
            row = {"unit_id": unit_id, "cycle": cycle}
            for sensor in NOMINAL:
                direction = DIRECTIONS[sensor]
                row[sensor] = gen_value(
                    sensor, cycle, total_cycles, degrade_strength,
                    direction, force_failure
                )
            rows.append(row)
    return rows


def write_csv(path, rows):
    fieldnames = ["unit_id", "cycle", "t_turbine_exit", "p_compressor_out",
                  "fan_speed_rpm", "fuel_flow_ratio", "vibration"]
    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for row in rows:
            writer.writerow(row)


if __name__ == "__main__":
    rows = generate_rows()
    write_csv("sample.csv", rows)
    print(f"Wrote {len(rows)} rows to sample.csv")
    print("Unit summary:")
    for cfg in UNIT_CONFIGS:
        tag = "FORCED FAILURE near end" if cfg["force_failure"] else "healthy/mild degradation"
        print(f"  unit {cfg['unit_id']}: {cfg['cycles']} cycles, {tag}")
