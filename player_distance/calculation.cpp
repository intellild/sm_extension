#include <cmath>
#include <cassert>
#include "calculation.h"

static float g_Distance[64][64];

static inline float square(float value)
{
    return value * value;
}

void init_calculation()
{
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            g_Distance[i][j] = NAN;
        }
    }
}

void calculate(point_t points[64])
{
    for (int i = 0; i < 64; i++)
    {
        point_t p1 = points[i];
        for (int j = 0; j < 64; j++)
        {
            point_t p2 = points[j];
            // padding is always zero, to align SIMD length
            float distance = square(p1.x - p2.x) + square(p1.y - p2.y) + square(p1.z - p2.z) + square(p1.__padding - p2.__padding);
            g_Distance[i][j] = distance;
        }
    }
}

float get_distance(int client1, int client2) {
    assert(client1 > 0 && client1 <= 64);
    assert(client2 > 0 && client2 <= 64);
    return g_Distance[client1][client2];
}
