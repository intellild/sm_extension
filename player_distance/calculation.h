#pragma once

typedef struct point_struct {
    float x;
    float y;
    float z;
    float __padding;
} point_t;

void init_calculation();

void calculate(point_t points[64]);

float get_distance(int client1, int client2);
