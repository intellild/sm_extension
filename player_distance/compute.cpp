#include <array>
#include <cmath>
#include <cassert>
#include "compute.h"
#include "ext_math.h"

using std::array;

static distance_matrix_t g_Distance;

void InitCompute()
{
    for (auto i = 0; i < 64; i++)
    {
        for (auto j = 0; j < 64; j++)
        {
            g_Distance[i][j] = NAN;
        }
    }
}

void Compute(const array<point_t, 64> &points)
{
    for (auto i = 0; i < 64; i++)
    {
        const point_t &p1 = points[i];
        for (auto j = 0; j < 64; j++)
        {
            const point_t &p2 = points[j];
            // padding is always zero, to align SIMD length
            float distance = square(p1.x - p2.x) + square(p1.y - p2.y) + square(p1.z - p2.z) + square(p1.__padding - p2.__padding);
            g_Distance[i][j] = distance;
        }
    }
}

float GetDistance(int32_t client1, int32_t client2) {
    assert(client1 > 0 && client1 <= 64);
    assert(client2 > 0 && client2 <= 64);
    return g_Distance[client1][client2];
}

const distance_matrix_t & GetAllDistance()
{
    return g_Distance;
}
