#ifndef Splitslib_am_AnalogMovement_H
#define Splitslib_am_AnalogMovement_H

#include "raylib.h"

typedef struct splitslib_am_AnalogControls {
    int xAxisIndex;
    int yAxisIndex;
    int gamepadIndex;
} splitslib_am_AnalogControls;

typedef struct splitslib_am_Dashboard {
    Vector2 velocity;
} splitslib_am_Dashboard;

typedef struct splitslib_am_BasicChassis {
    Vector2 currentPos;
    Vector2 velocity;
    float radius;
    Color color;
} splitslib_am_BasicChassis;

void splitslib_am_PilotAnalogShip(
splitslib_am_AnalogControls *controls, splitslib_am_Dashboard *dash, splitslib_am_BasicChassis *chassis);

#endif