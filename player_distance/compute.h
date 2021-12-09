#pragma once

typedef struct point_struct {
    float x;
    float y;
    float z;
    float __padding;
} point_t;

void InitCompute();

void Compute(point_t points[64]);

float GetDistance(int client1, int client2);
