#include "math.h"
#include "raylib.h"
#include "splitslib_am_AnalogMovement.h"
#include "splitslib_util_Utilities.h"

void splitslib_am_PilotAnalogShip(
splitslib_am_AnalogControls *controls, splitslib_am_Dashboard *dash, splitslib_am_BasicChassis *chassis
){
    // apply friction
    // TODO demagic
    dash->velocity.x *= 0.98;
    dash->velocity.y *= 0.98;

    float input_x = GetGamepadAxisMovement(controls->gamepadIndex, controls->xAxisIndex);
    float input_y = GetGamepadAxisMovement(controls->gamepadIndex, controls->yAxisIndex);
    
    dash->velocity.x += input_x;
    dash->velocity.y -= input_y;
    
    float velocityMagnitude = sqrt(pow(dash->velocity.x, 2) + pow(dash->velocity.y, 2));
    // TODO demagic / debug
    if (velocityMagnitude > 5.0){
        dash->velocity.x *= 5.0 / velocityMagnitude;
        dash->velocity.y *= 5.0 / velocityMagnitude;
    }
    
    chassis->currentPos.x += dash->velocity.x;
    chassis->currentPos.y += dash->velocity.y;
}