#pragma once

#include <array>
#include <cinttypes>

typedef struct point_struct {
    float x;
    float y;
    float z;
    float __padding;
} point_t;

using distance_matrix_t = std::array<std::array<float, 64>, 64>;

void InitCompute();

void Compute(const std::array<point_t, 64> &points);

float GetDistance(int32_t client1, int32_t client2);

const distance_matrix_t & GetAllDistance();
