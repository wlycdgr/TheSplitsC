// brought into compliance with style guide

#include "raylib.h"
#include "splitslib_pv_ParamValidation.h"

bool splitslib_pv_IsArrayIndexInBounds(int index, int arrayLength){
    if (
        0 > index ||
        arrayLength - 1 < index
    ){
        return false;
    }
    else {
        return true;
    }
}