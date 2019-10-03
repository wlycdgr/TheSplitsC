#ifndef SPLITSLIB_SF_Splitsfont_H
#define SPLITSLIB_SF_Splitsfont_H

#include "raylib.h"

/*
The Splitsfont module provides a rough
but flexible and animation-friendly text rendering solution
that does not rely on 3rd party fonts
*/

typedef enum splitslib_sf_Status{
    SPLITSLIB_SF_OK = 0,
    SPLITSLIB_SF_InvalidArgument,
    SPLITSLIB_SF_InvalidIdArgument,
    SPLITSLIB_SF_InvalidStringArgument,
    SPLITSLIB_SF_MallocAttemptFailed,
    SPLITSLIB_SF_ModuleNotInitialized,
    SPLITSLIB_SF_ModuleAlreadyInitialized,
    SPLITSLIB_SF_ModuleOutOfMemory
} splitslib_sf_Status;
    
typedef struct splitslib_sf_Result{
    int value;
    float floatValue;
    int intValue;
    char *sValue;
    splitslib_sf_Status status;
    char *message;
} splitslib_sf_Result;

typedef enum splitslib_sf_RotationAlignment{
    SPLITSLIB_SF_RotationAlignmentChorusLine = 1,
    SPLITSLIB_SF_RotationAlignmentArgyle = -1
} splitslib_sf_RotationAlignment;

typedef enum splitslib_sf_RotationDirection{
    SPLITSLIB_SF_RotationDirectionCW = -1,
    SPLITSLIB_SF_RotationDirectionCounterCW = 1
} splitslib_sf_RotationDirection;

typedef enum splitslib_sf_RenderStyle{
    SPLITSLIB_SF_Horizontal = 0,
    SPLITSLIB_SF_Vertical,
    SPLITSLIB_SF_Vertices
} splitslib_sf_RenderStyle;

typedef struct splitslib_sf_Stringbox{
    int id;
    Vector2 pos;
} splitslib_sf_Stringbox;


// build the glyph templates
// and perform other initialization
splitslib_sf_Result splitslib_sf_Init();


// initializes a string box with some basic custom settings 
// use if you don't need the super fine grained control provided by the rest of the API
// centers the string horizontally
void splitslib_sf_InitStringbox(splitslib_sf_Stringbox *stringbox, const char *text, float size, float weight, float kern, float y);

void splitslib_sf_SetStringboxPosition(splitslib_sf_Stringbox *sbPtr, Vector2 pos);

// creates a new splitsfont string
// and returns unique positive int id handle
// as value property on result struct
// sets value to -1 on failure
//
// initializes the string object to use
// these defaults display and animation settings:
// size: 0.02 of screen height
// color: BLACK
// kern: 0.02 of screen height
// stroke weight: 0.01 of screen height
// style: ?
// angle: 0
// rotation speed: 0
// rotation direction: SPLITSLIB_SF_RotationDirectionCW
// rotation alignment: SPLITSLIB_SF_RotationAlignmentChorusLine
splitslib_sf_Result splitslib_sf_NewString();


// sets the text value for our splitsfont string object
// returns 0 in value field
// on success, returns new string value in message field
// retains old text value if any of the arguments are invalid,
// and sets string text to empty string if anything else goes wrong
splitslib_sf_Result splitslib_sf_SetText(int stringId, const char *text);
// splitsfont supports the following characters:
// A-Z [note: uppercase only!]
// 0-9
// : ? space
// the presence of any other character in the argument
// will cause the call to fail
// empty strings are fine


// proportional to height of display area
// values outside of [0-1] are clamped to this range
// in case of failure, old size is retained
splitslib_sf_Result splitslib_sf_SetSize
(int stringId, float sizeAsPropOfScreenHeight);


// proportional to height of display area
// values outside of [0-1] are clamped to this range
// in case of failure, old kern is retained
splitslib_sf_Result splitslib_sf_SetKern
(int stringId, float kernAsPropOfScreenHeight);


// proportional to height of display area
// values outside of [0-1] are clamped to this range
// in case of failure, old stroke weight is retained
splitslib_sf_Result splitslib_sf_SetStrokeWeight
(int stringId, float strokeWeightAsPropOfScreenHeight);


// in case of failure, old color is retained
splitslib_sf_Result splitslib_sf_SetColor(int stringId, Color color);


// in case of failure, old angle is retained
// does not affect animation settings
splitslib_sf_Result splitslib_sf_SetAngle(int stringId, float angleDegrees);


// trying to use this to change rotation direction won't work -
// negative values are set to 0
// use SetRotationDirection for that instead
// in case of failure, old rotation speed is retained
splitslib_sf_Result splitslib_sf_SetRotationSpeed
(int stringId, float degreesPerSec);


// in case of failure, old rotation direction is retained
splitslib_sf_Result splitslib_sf_SetRotationDirection
(int stringId, splitslib_sf_RotationDirection dir);


// in case of failure, old rotation alignment is retained
splitslib_sf_Result splitslib_sf_SetRotationAlignment
(int stringId, splitslib_sf_RotationAlignment alignment);


// in case of failure, retains old drawing status
splitslib_sf_Result splitslib_sf_SetRenderStyle
(int stringId, splitslib_sf_RenderStyle renderStyle);


// updates the string's glyph vertex position data
// based on current rotation speed, direction, and alignment settings
// returns 0 in value field
// check status and message fields for success? & details
splitslib_sf_Result splitslib_sf_AnimateString(int stringId);


// returns total width of string as floatValue
splitslib_sf_Result splitslib_sf_GetWidth(int stringId);


// draw the string using its current display settings
// takes absolute pixel coordinates of top left corner of string
// does NOT check to see whether they are in display area
// returns 0 on success, -1 on failure
splitslib_sf_Result splitslib_sf_DrawString(int stringId, float pxX, float pxY);


// release the resource when done using it and/or before changing game state
splitslib_sf_Result splitslib_sf_FreeString(int stringId);
// TODO
// implement a _FreeStrings function that frees all the strings in one go

#endif