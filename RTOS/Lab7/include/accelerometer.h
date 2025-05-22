#pragma once

// clang-format off
#define SENSITIVITY     0.00390625f // For normal mode: 4g (range) / 2^10 (discrete levels)
#define SCALING         64.0f
// clang-format on

void init_accelerometer();

void read_xyz(float *destination);