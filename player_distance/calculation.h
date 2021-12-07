#pragma once

#include "public/player_distance.h"

typedef struct point_struct {
    float x;
    float y;
    float z;
    float __padding;
} point_t;

void InitCalculation();

void Calculate(point_t points[64]);
