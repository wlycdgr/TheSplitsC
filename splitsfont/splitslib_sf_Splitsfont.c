#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "splitslib_sf_Splitsfont.h"
#include "splitslib_util_Utilities.h"

#define WLY_DegsInACircle 360

#define WLY_SplitsfontSize 96
#define WLY_MaxVertices 16
#define WLY_MaxLines 16
#define WLY_Zero 0.0f
#define WLY_ZeroV (Vector2){0.0f, 0.0f}
#define WLY_DefaultKern 0.2

#define WLY_DefaultSizeAsPropOfScreenHeight 0.1
#define WLY_DefaultStrokeWeightAsPropOfScreenHeight 0.01
#define WLY_DefaultKernAsPropOfScreenHeight 0.02
#define WLY_DefaultStringColor BLACK
#define WLY_DefaultAngleInDegs 0.0f
#define WLY_DefaultRotationAlignment SPLITSLIB_SF_RotationAlignmentChorusLine
#define WLY_DefaultRotationDirection SPLITSLIB_SF_RotationDirectionCW
#define WLY_DefaultRotationSpeedInDegsPerSec 0.0
#define WLY_DefaultRenderStyle SPLITSLIB_SF_Vertical

// letters are arranged by English language frequency
// (as per https://en.wikipedia.org/wiki/Letter_frequency)
// to speed up strchr validation when building strings
// note that the leading space is not a typo but represents the space char
#define WLY_SplitsfontSupportedChars " ETAOINSHRDLCUMWFGYPBVKJXQZ0123456789:?"

#define WLY_SplitsfontMaxStrings 128
                                       
// immobile vertices simply have their starting position
// as their pivot position
// pivot points must be x-axis-aligned
typedef struct wly_SplitsfontVertex{
    bool isActive;
    Vector2 pivotPos;
    Vector2 pos;
    float offsetVectorMagnitude;
} wly_SplitsfontVertex;

typedef struct wly_SplitsfontLine{
    bool isActive;
    int startIndex;
    int endIndex;
    Vector2 *start;
    Vector2 *end;
} wly_SplitsfontLine;

typedef struct wly_SplitsfontGlyph{
    bool isActive;
    wly_SplitsfontVertex vertices[WLY_MaxVertices];
    wly_SplitsfontLine lines[WLY_MaxLines];
    int vertexCount;
    int lineCount;
    float width;
    float derivedPxWidth;
    float kernMultiplier; // TODO move this out of glyph struct since it never changes??
    float angleRad;
} wly_SplitsfontGlyph;

typedef struct wly_SplitsfontStringDisplayParams{
    float pxGlyphSize;
    float pxGlyphStrokeWeight;
    float pxStringKern;
    Color color;
    splitslib_sf_RotationDirection rotationDir;
    splitslib_sf_RotationAlignment rotationAlignment;
    float rotationSpeedInDegsPerSec;
    splitslib_sf_RenderStyle renderStyle;
}  wly_SplitsfontStringDisplayParams;

typedef struct wly_SplitsfontString{
    bool isActive;
    wly_SplitsfontGlyph **glyphPtrs;
    wly_SplitsfontStringDisplayParams params;
    int length;
    float currentAngleInDegs;
    float totalWidth;
} wly_SplitsfontString;

static wly_SplitsfontGlyph wly_splitsfontGlyphTemplates[WLY_SplitsfontSize];
static float wly_angle;
static float wly_isAngleIncreasing;
static Vector2 wly_penPos;
static wly_SplitsfontGlyph *ltrPtr;
static float wly_maxThickness = 10.0f;
static float wly_kern;
static wly_SplitsfontString *wly_stringPtrs[WLY_SplitsfontMaxStrings];
static bool wly_isInitialized;
static float wly_DEGTORAD;


static void wly_BuildGlyphTemplates();
static void wly_BeginGlyphTemplateInit(char ltr);
static void wly_InitGlyphTemplateVertex
(int index, Vector2 pivotPos, float xOffset);
static void wly_InitGlyphTemplateLine
(int index, int startVertexIndex, int endVertexIndex);
static void wly_FinishGlyphTemplateInit();
static wly_SplitsfontGlyph *wly_NewGlyph(char ltr);
static void wly_BuildGlyphTemplate_A();
static void wly_BuildGlyphTemplate_B();
static void wly_BuildGlyphTemplate_C();
static void wly_BuildGlyphTemplate_D();
static void wly_BuildGlyphTemplate_E();
static void wly_BuildGlyphTemplate_F();
static void wly_BuildGlyphTemplate_G();
static void wly_BuildGlyphTemplate_H();
static void wly_BuildGlyphTemplate_I();
static void wly_BuildGlyphTemplate_J();
static void wly_BuildGlyphTemplate_K();
static void wly_BuildGlyphTemplate_L();
static void wly_BuildGlyphTemplate_M();
static void wly_BuildGlyphTemplate_N();
static void wly_BuildGlyphTemplate_O();
static void wly_BuildGlyphTemplate_P();
static void wly_BuildGlyphTemplate_Q();
static void wly_BuildGlyphTemplate_R();
static void wly_BuildGlyphTemplate_S();
static void wly_BuildGlyphTemplate_T();
static void wly_BuildGlyphTemplate_U();
static void wly_BuildGlyphTemplate_V();
static void wly_BuildGlyphTemplate_W();
static void wly_BuildGlyphTemplate_X();
static void wly_BuildGlyphTemplate_Y();
static void wly_BuildGlyphTemplate_Z();
static void wly_BuildGlyphTemplate_0();
static void wly_BuildGlyphTemplate_1();
static void wly_BuildGlyphTemplate_2();
static void wly_BuildGlyphTemplate_3();
static void wly_BuildGlyphTemplate_4();
static void wly_BuildGlyphTemplate_5();
static void wly_BuildGlyphTemplate_6();
static void wly_BuildGlyphTemplate_7();
static void wly_BuildGlyphTemplate_8();
static void wly_BuildGlyphTemplate_9();
static void wly_BuildGlyphTemplate_colon();
static void wly_BuildGlyphTemplate_question();
static void wly_BuildGlyphTemplate_space();


static wly_SplitsfontString *wly_GetStrPtrAtId(int stringId);
static wly_SplitsfontGlyph *wly_NewGlyph(char ltr);
static void wly_DrawGlyph
(wly_SplitsfontGlyph *glyphPtr, wly_SplitsfontStringDisplayParams *params);
static void wly_RotateGlyphs(wly_SplitsfontString *strPtr);
static void wly_RotateGlyph(float wly_angle, wly_SplitsfontGlyph *glyphPtr);
static void wly_EraseString(wly_SplitsfontString *strPtr);


//static void wly_FreeString(wly_SplitsfontString *strPtr);
static void wly_FreeString(int ptrId);



// one time function that needs to be called
// before any of the others can be used
splitslib_sf_Result splitslib_sf_Init(){
    splitslib_sf_Result result;
    result.value = 0;
    
    if (wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleAlreadyInitialized;
        result.message = "sf_Init called when module is already initialized. No work was done.";
        return result;
    }
    
    ltrPtr = NULL;
    
    // The glyph templates use statically allocated memory,
    // so I don't think we need to make sure this works at run time
    wly_BuildGlyphTemplates();
    
    for (int i =0; i < WLY_SplitsfontMaxStrings; i++){
        wly_stringPtrs[i] = NULL;
    }
    
    wly_DEGTORAD = PI / 180;

    wly_isInitialized = true;
    
    result.status = SPLITSLIB_SF_OK;
    result.message = "sf_Init did its thing - module ready for use!";
    return result;
}



void splitslib_sf_InitStringbox(splitslib_sf_Stringbox *stringbox, const char *text, float size, float weight, float kern, float y){
    splitslib_sf_Result result;
    
    result = splitslib_sf_NewString();
    assert(SPLITSLIB_SF_OK == result.status);
    
    stringbox->id = result.value;
    int id = stringbox->id;
    
    splitslib_sf_SetText(id, text);
    splitslib_sf_SetSize(id, size);
    splitslib_sf_SetStrokeWeight(id, weight);
    splitslib_sf_SetKern(id, kern);
    
    result = splitslib_sf_GetWidth(id);
    float width = result.floatValue;
    
    stringbox->pos = (Vector2){
        GetScreenWidth() / 2.0 - width / 2.0,
        GetScreenHeight() * y
    };
}

void splitslib_sf_SetStringboxPosition(splitslib_sf_Stringbox *sbPtr, Vector2 pos){
    sbPtr->pos = pos;
}

// returns string id, not a pointer:
// we want client to be able to modify strings only through
// the public interface functions
splitslib_sf_Result splitslib_sf_NewString(){
    splitslib_sf_Result result;
    
    if (!wly_isInitialized){
        result.value = -1;
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "called sf_NewString before module was initialized.";
        goto fail;
    }
        
    
    int key = -1;
    for (int i = 0; i < WLY_SplitsfontMaxStrings; i++){
        if (NULL == wly_stringPtrs[i]){
            key = i;
            break;
        }
    }
    if (-1 == key) {
        result.value = -1;
        result.status = SPLITSLIB_SF_ModuleOutOfMemory;
        result.message = "called sf_NewString, but the module doesn't have\
            any unused string structs available. If you need more than\
            128 strings, edit and recompile.";
        goto fail;
    }
    
    wly_SplitsfontString *strPtr = NULL;
    if (NULL == (strPtr = malloc(sizeof(wly_SplitsfontString)))){
       result.value = -1;
       result.status = SPLITSLIB_SF_MallocAttemptFailed;
       result.message = "attempt to malloc memory for a new string struct failed in sf_NewString";
       goto fail;
    }
    
    strPtr->isActive = true;
    strPtr->glyphPtrs = NULL;
    strPtr->length = 0;
    strPtr->totalWidth = 0.0f;
    strPtr->currentAngleInDegs = WLY_DefaultAngleInDegs;
    float screenHeight = GetScreenHeight();
    strPtr->params.pxGlyphSize = 
        WLY_DefaultSizeAsPropOfScreenHeight * screenHeight;
    strPtr->params.pxGlyphStrokeWeight = 
        WLY_DefaultStrokeWeightAsPropOfScreenHeight * screenHeight;
    strPtr->params.pxStringKern =
        WLY_DefaultKernAsPropOfScreenHeight * screenHeight;
    strPtr->params.color = BLACK;
    strPtr->params.rotationAlignment = WLY_DefaultRotationAlignment; 
    strPtr->params.rotationDir = WLY_DefaultRotationDirection;
    strPtr->params.rotationSpeedInDegsPerSec= WLY_DefaultRotationSpeedInDegsPerSec;
    strPtr->params.renderStyle = WLY_DefaultRenderStyle;
    wly_stringPtrs[key] = strPtr;

    
    
    succeed:
        result.value = key;
        result.intValue = key;
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_NewString succeeded";
        return result;
    fail:
        return result;
}


splitslib_sf_Result  splitslib_sf_SetText(int stringId, const char *text){
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetText called before module initialized";
        goto fail;
    }
    
    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetText called with invalid string id";
        goto fail;
    }
    
    // validate text argument
    size_t strLength = strlen(text);
    for (int i = 0; i < strLength; i++){
        if (NULL == strchr(WLY_SplitsfontSupportedChars, text[i])){
            result.status = SPLITSLIB_SF_InvalidStringArgument;
            result.message = "string argument in call to sf_SetText\
                includes unsupported character: "+text[i];
            goto fail;
        }
    }
    
    
    
    wly_EraseString(strPtr);
    if (strLength > 0){
        if (
            NULL == (strPtr->glyphPtrs =
            (wly_SplitsfontGlyph**) 
            malloc(sizeof(wly_SplitsfontGlyph*) * strLength))
        ){
            result.status = SPLITSLIB_SF_MallocAttemptFailed;
            result.message = "attempt to malloc memory for the string\
                parameter glyph pointers in sf_SetText failed";
            goto fail;
        }
        
        for (int i = 0; i < strLength; i++){
            strPtr->glyphPtrs[i] = NULL;
        }
    }
    for (int i = 0; i < strLength; i++){
        if (
            NULL == 
            (strPtr->glyphPtrs[i] = wly_NewGlyph(text[i]))
        ){
            result.status = SPLITSLIB_SF_MallocAttemptFailed;
            result.message = "attempt to malloc memory for a string\
                parameter glyph in sf_SetText failed";
            goto fail;
        }
        strPtr->length++;
    }
    

    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "success!";
        return result;
    fail:
        if (NULL != strPtr) wly_EraseString(strPtr);
        return result;
}





splitslib_sf_Result splitslib_sf_SetSize(int stringId, float sizeAsPropOfScreenHeight){
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetSize called before module initialized";
        goto fail;
    }
    
    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetSize called with invalid string id";
        goto fail;
    }
    
    
    
    sizeAsPropOfScreenHeight = splitslib_util_Clamp(
        sizeAsPropOfScreenHeight, 0.0, 1.0);
    strPtr->params.pxGlyphSize = sizeAsPropOfScreenHeight * GetScreenHeight();
    
    
    
    succedd:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetSize succeeded";
        return result;
    fail:
        return result;
}





splitslib_sf_Result splitslib_sf_SetKern
(int stringId, float kernAsPropOfScreenHeight)
{
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetKern called before module initialized";
        goto fail;
    }
    
    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetKern called with invalid string id";
        goto fail;
    }
    
    
    
    kernAsPropOfScreenHeight = splitslib_util_Clamp(
        kernAsPropOfScreenHeight, 0.0, 1.0);
    strPtr->params.pxStringKern = kernAsPropOfScreenHeight * GetScreenHeight();
    
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetKern succeeded";
        return result;
    fail:
        return result;
}




splitslib_sf_Result splitslib_sf_SetStrokeWeight
(int stringId, float strokeWeightAsPropOfScreenHeight)
{
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetStrokeWeight called before module initialized";
        goto fail;
    }
    
    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetStrokeWeight called with invalid string id";
        goto fail;
    }
    
    
    
    strokeWeightAsPropOfScreenHeight = splitslib_util_Clamp(
        strokeWeightAsPropOfScreenHeight, 0.0, 1.0);
    strPtr->params.pxGlyphStrokeWeight = 
        strokeWeightAsPropOfScreenHeight * GetScreenHeight();
    
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetStrokeWeight succeeded";
        return result;
    fail:
        return result;
}





splitslib_sf_Result splitslib_sf_SetColor(int stringId, Color color)
{
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetColor called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetColor called with invalid string id";
        goto fail;
    }
    
    
    
    strPtr->params.color = color;
    
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetColor succeeded";
        return result;
    fail:
        return result;
}





splitslib_sf_Result splitslib_sf_SetAngle(int stringId, float angleDegrees)
{
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetAngle called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetAngle called with invalid string id";
        goto fail;
    }
    
    
    
    strPtr->currentAngleInDegs = angleDegrees; 
    wly_RotateGlyphs(strPtr);
    
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetAngle succeeded";
        return result;
    fail:
        return result;
}




splitslib_sf_Result splitslib_sf_SetRotationSpeed
(int stringId, float degreesPerSec)
{
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetRotationSpeed called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetRotationSpeed called with invalid string id";
        goto fail;
    }
    
    
    
    if (degreesPerSec < 0) degreesPerSec = 0;
    strPtr->params.rotationSpeedInDegsPerSec = degreesPerSec; 
    
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetRotationSpeed succeeded";
        return result;
    fail:
        return result;
}





splitslib_sf_Result splitslib_sf_SetRotationDirection
(int stringId, splitslib_sf_RotationDirection dir)
{    
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetRotationDirection called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetRotationDirection called with invalid string id";
        goto fail;
    }
    
    
    
    strPtr->params.rotationDir = dir;
    
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetRotationDirection succeeded";
        return result;
    fail:
        return result;
}





splitslib_sf_Result splitslib_sf_SetRotationAlignment
(int stringId, splitslib_sf_RotationAlignment alignment)
{
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetRotationAlignment called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetRotationAlignment called with invalid string id";
        goto fail;
    }
    
    
    
    strPtr->params.rotationAlignment = alignment;
    
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetRotationAlignment succeeded";
        return result;
    fail:
        return result;
}





splitslib_sf_Result splitslib_sf_SetRenderStyle
(int stringId, splitslib_sf_RenderStyle renderStyle)
{
    splitslib_sf_Result result;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_SetRenderStyle called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_SetRenderStyle called with invalid string id";
        goto fail;
    }
    
    
    
    strPtr->params.renderStyle = renderStyle;

    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_SetRenderStyle succeeded";
        return result;
    fail:
        return result;
}





splitslib_sf_Result splitslib_sf_AnimateString(int stringId)
{
    splitslib_sf_Result result;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_AnimateString called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_AnimateString called with invalid string id";
        goto fail;
    }
    
    
    
    strPtr->currentAngleInDegs += 
        strPtr->params.rotationDir *
        strPtr->params.rotationSpeedInDegsPerSec * GetFrameTime();        
    wly_RotateGlyphs(strPtr);

    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_AnimateString succeeded";
        return result;
    fail:
        return result;
}


splitslib_sf_Result splitslib_sf_GetWidth(int stringId)
{
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_GetWidth called before module initialized";
        goto fail;
    }
    
    
    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_GetWidth called with invalid string id";
        goto fail;
    }
    
    float width = 0;
    float size = strPtr->params.pxGlyphSize;
    for (int i = 0; i < strPtr->length; i++){
        width += strPtr->glyphPtrs[i]->width * size;
    }
    width += strPtr->params.pxStringKern * (strPtr->length - 1);
    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.floatValue = width;
        return result;
    fail:
        result.message = "sf_GetWidth failed :(";
        return result;
}


splitslib_sf_Result splitslib_sf_DrawString(int stringId, float pxX, float pxY){
    splitslib_sf_Result result;
    result.value = 0;
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_DrawString called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_DrawString called with invalid string id";
        goto fail;
    }

    
    // TODO validate & clamp this!
    wly_penPos.x = pxX;
    wly_penPos.y = pxY;
    wly_SplitsfontStringDisplayParams *params = &strPtr->params;
    for (int i = 0; i < strPtr->length; i++){
        wly_DrawGlyph(strPtr->glyphPtrs[i], params);
    }
    wly_penPos = WLY_ZeroV;


    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        result.message = "sf_DrawString succeeded";
        return result;
    fail:
        return result;
}

splitslib_sf_Result splitslib_sf_FreeString(int stringId)
{
    splitslib_sf_Result result;    
    wly_SplitsfontString *strPtr = NULL;
    
    if (!wly_isInitialized) {
        result.status = SPLITSLIB_SF_ModuleNotInitialized;
        result.message = "sf_DrawString called before module initialized";
        goto fail;
    }

    if (NULL == (strPtr = wly_GetStrPtrAtId(stringId))){
        result.status = SPLITSLIB_SF_InvalidIdArgument;
        result.message = "sf_DrawString called with invalid string id";
        goto fail;
    }
    
    

    wly_FreeString(stringId);

    
    
    succeed:
        result.status = SPLITSLIB_SF_OK;
        return result;
    fail:
        result.message = "sf_FreeString failed :(";
        return result;
}

static wly_SplitsfontString *wly_GetStrPtrAtId(int stringId){
    if (stringId < 0 || stringId >= WLY_SplitsfontMaxStrings) goto fail;
    wly_SplitsfontString *strPtr = wly_stringPtrs[stringId];
    if (NULL == strPtr) goto fail;
    if (!strPtr->isActive) goto fail;
    
    succeed:
        return strPtr;
    fail:
        return NULL;
}



static wly_SplitsfontGlyph* wly_NewGlyph(char glyph)
{
    wly_SplitsfontGlyph *newGlyphPtr = NULL;
    if (
        NULL ==
        (newGlyphPtr = malloc(sizeof(wly_SplitsfontGlyph)))
    ) goto fail;
    
   
   
    *newGlyphPtr = wly_splitsfontGlyphTemplates[glyph];
    wly_SplitsfontLine *lPtr = NULL;
    for (int i = 0; i < WLY_MaxLines; i++){
        if (!newGlyphPtr->lines[i].isActive) break;
        
        lPtr = &(newGlyphPtr->lines[i]);
        lPtr->start = &(newGlyphPtr->vertices[lPtr->startIndex].pos);
        lPtr->end = &(newGlyphPtr->vertices[lPtr->endIndex].pos);
    }
    
   
   
    succeed:
        return newGlyphPtr;
    fail:
        return NULL;
}





static void wly_RotateGlyphs(wly_SplitsfontString *strPtr)
{
    if (
        WLY_DegsInACircle <= strPtr->currentAngleInDegs ||
        -WLY_DegsInACircle >= strPtr->currentAngleInDegs
    ){ 
        strPtr->currentAngleInDegs = 
            (float)((int)strPtr->currentAngleInDegs % WLY_DegsInACircle);
    }
    
    float angleInRads = strPtr->currentAngleInDegs * wly_DEGTORAD;
    float rotationAlignment = strPtr->params.rotationAlignment;
    for (int i = 0; i < strPtr->length; i++){
        wly_RotateGlyph(angleInRads, strPtr->glyphPtrs[i]);
        angleInRads *= rotationAlignment; 
    }
}





static void wly_RotateGlyph(float angleRad, wly_SplitsfontGlyph *glyphPtr)
{
    float cosValue = cos(angleRad);
    float sinValue = sin(angleRad);
    int vCount = glyphPtr->vertexCount;
    wly_SplitsfontVertex *v = NULL;
    for (int i = 0; i < vCount; i++){        
        v = &(glyphPtr->vertices[i]);
        v->pos.x = v->pivotPos.x + v->offsetVectorMagnitude * cosValue;
        v->pos.y = v->pivotPos.y - v->offsetVectorMagnitude * sinValue;
    }
}





static void wly_DrawGlyph
(wly_SplitsfontGlyph *glyphPtr, wly_SplitsfontStringDisplayParams *params)
{
    float strokeWeight = params->pxGlyphStrokeWeight;
    float size = params->pxGlyphSize;
    Color color = params->color;
    
    wly_SplitsfontLine *l = NULL;
    Vector2 *lStart = NULL;
    Vector2 *lEnd = NULL;
    Vector2 v1, v2, v3, v4, v5 = WLY_ZeroV;
    Vector2 left, right = WLY_ZeroV;
    splitslib_sf_RenderStyle renderStyle = params->renderStyle;
    
    
    // TODO parameterize this
    float posX, posY;
    Vector2 p1, p2, p3, p4, p5;
    int side = 2;
    switch(renderStyle){
    case (SPLITSLIB_SF_Vertices):
        for (int i = 0; i < glyphPtr->vertexCount; i++){
            posX = wly_penPos.x + glyphPtr->vertices[i].pos.x * size;
            posY = wly_penPos.y + glyphPtr->vertices[i].pos.y * size;
            p1 = (Vector2){posX - side, posY + side};
            p2 = (Vector2){posX + side, posY + side};
            p3 = (Vector2){posX + side, posY - side};
            p4 = (Vector2){posX - side, posY - side};
            p5 = p1;
            Vector2 points[5] = {p1, p2, p3, p4, p5};
            DrawPolyEx(points, 5, BLACK);
        }
        break;
    default:
        break;
    }
    
    
    
    for (int i=0; i < glyphPtr->lineCount; i++){
        l = &(glyphPtr->lines[i]);
        lStart = l->start;
        lEnd = l->end;
        
        switch(renderStyle){
        case (SPLITSLIB_SF_Vertices):           
            DrawLine(
                wly_penPos.x + l->start->x * size, 
                wly_penPos.y + l->start->y * size,
                wly_penPos.x + l->end->x * size, 
                wly_penPos.y + l->end->y * size,
                color);
            break;
        case (SPLITSLIB_SF_Vertical):
            for (int j = 0; j < strokeWeight; j++){
                DrawLine(
                    wly_penPos.x + j + l->start->x * size, 
                    wly_penPos.y + l->start->y * size,
                    wly_penPos.x + j + l->end->x * size, 
                    wly_penPos.y + l->end->y * size,
                    color);
            }
            break;
        case (SPLITSLIB_SF_Horizontal):
            for (int j = 0; j < strokeWeight; j++){
                DrawLine(
                    wly_penPos.x  + lStart->x * size, 
                    wly_penPos.y + j + lStart->y * size,
                    wly_penPos.x  + lEnd->x * size, 
                    wly_penPos.y + j + lEnd->y * size,
                    color);
            }
            break;
        default:
            break;
       }
        
    }
 
    wly_penPos.x += glyphPtr->width * size + params->pxStringKern;
}





static void wly_EraseString(wly_SplitsfontString *strPtr){
    if (NULL != strPtr->glyphPtrs){
        for (int i = 0; i < strPtr->length; i++){
            if (NULL != strPtr->glyphPtrs[i]) {
                free(strPtr->glyphPtrs[i]);
            }
        }
        free(strPtr->glyphPtrs);
        strPtr->glyphPtrs = NULL;
        strPtr->length = 0;
        strPtr->totalWidth = 0;
    }
}


static void wly_FreeString(int ptrId)
{
    if (NULL != wly_stringPtrs[ptrId]){
        for (int i =0; i < wly_stringPtrs[ptrId]->length; i++){
            if (NULL != wly_stringPtrs[ptrId]->glyphPtrs[i]){
                free(wly_stringPtrs[ptrId]->glyphPtrs[i]);
            }
        }
        free(wly_stringPtrs[ptrId]->glyphPtrs);
        free(wly_stringPtrs[ptrId]);
        wly_stringPtrs[ptrId] = NULL;
    }
}




static void wly_BuildGlyphTemplates(){
    wly_BuildGlyphTemplate_A();
    wly_BuildGlyphTemplate_B();
    wly_BuildGlyphTemplate_C();
    wly_BuildGlyphTemplate_D();
    wly_BuildGlyphTemplate_E();
    wly_BuildGlyphTemplate_F();
    wly_BuildGlyphTemplate_G();
    wly_BuildGlyphTemplate_H();
    wly_BuildGlyphTemplate_I();
    wly_BuildGlyphTemplate_J();
    wly_BuildGlyphTemplate_K();
    wly_BuildGlyphTemplate_L();
    wly_BuildGlyphTemplate_M();
    wly_BuildGlyphTemplate_N();
    wly_BuildGlyphTemplate_O();
    wly_BuildGlyphTemplate_P();
    wly_BuildGlyphTemplate_Q();
    wly_BuildGlyphTemplate_R();
    wly_BuildGlyphTemplate_S();
    wly_BuildGlyphTemplate_T();
    wly_BuildGlyphTemplate_U();
    wly_BuildGlyphTemplate_V();
    wly_BuildGlyphTemplate_W();
    wly_BuildGlyphTemplate_X();
    wly_BuildGlyphTemplate_Y();
    wly_BuildGlyphTemplate_Z();
    
    wly_BuildGlyphTemplate_0();
    wly_BuildGlyphTemplate_1();
    wly_BuildGlyphTemplate_2();
    wly_BuildGlyphTemplate_3();
    wly_BuildGlyphTemplate_4();
    wly_BuildGlyphTemplate_5();
    wly_BuildGlyphTemplate_6();
    wly_BuildGlyphTemplate_7();
    wly_BuildGlyphTemplate_8();
    wly_BuildGlyphTemplate_9();
    
    wly_BuildGlyphTemplate_colon();
    wly_BuildGlyphTemplate_question();
    wly_BuildGlyphTemplate_space();
}



static void wly_BuildGlyphTemplate_A(){
    wly_BeginGlyphTemplateInit('A');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.35f, 0.6f}, -0.19);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.35f, 0.6f}, 0.19);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.35f, 1.0f}, -0.35);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.35f, 1.0f}, 0.35);

        wly_InitGlyphTemplateLine(0, 3, 0);
        wly_InitGlyphTemplateLine(1, 0, 4);
        wly_InitGlyphTemplateLine(2, 1, 2);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_B(){
    wly_BeginGlyphTemplateInit('B');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, -0.3);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, 0.05);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.82f}, 0.3);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.58f}, 0.3);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 0.48f}, 0.12);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.3f, 0.4f}, 0.3);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.3f, 0.15f}, 0.3);
        wly_InitGlyphTemplateVertex(8, (Vector2){0.3f, 0.0f}, 0.05);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 4);
        wly_InitGlyphTemplateLine(4, 5, 4);
        wly_InitGlyphTemplateLine(5, 5, 6);
        wly_InitGlyphTemplateLine(6, 6, 7);
        wly_InitGlyphTemplateLine(7, 8, 7);
        wly_InitGlyphTemplateLine(8, 0, 8);
    wly_FinishGlyphTemplateInit();
}


static void wly_BuildGlyphTemplate_C(){
    wly_BeginGlyphTemplateInit('C');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 0.0f}, 0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, -0.275);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 1.0f}, 0.275);

        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 2, 0);
        wly_InitGlyphTemplateLine(2, 2, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_D()
{
    wly_BeginGlyphTemplateInit('D');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, -0.3);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.85f}, 0.15);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.7f}, 0.3);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 0.3f}, 0.3);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.3f, 0.15f}, 0.15);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.3f, 0.0f}, WLY_Zero);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 4);
        wly_InitGlyphTemplateLine(4, 4, 5);
        wly_InitGlyphTemplateLine(5, 6, 5);
        wly_InitGlyphTemplateLine(6, 7, 6);
        wly_InitGlyphTemplateLine(7, 0, 7);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_E(){
    wly_BeginGlyphTemplateInit('E');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 0.55f}, -0.3);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, -0.3);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 1.0f}, 0.3);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.55f}, 0.2);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 0.0f}, 0.3);
        
        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 2, 3);
        wly_InitGlyphTemplateLine(2, 1, 4);
        wly_InitGlyphTemplateLine(3, 0, 5);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_F()
{
    wly_BeginGlyphTemplateInit('F');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 0.5f}, -0.3f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.5f}, 0.1f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.0f}, 0.3f);
        
        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 0, 4);
        wly_InitGlyphTemplateLine(2, 1, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_G()
{
    wly_BeginGlyphTemplateInit('G');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 1.0f}, -0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, 0.275);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 0.55f}, 0.275);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.55f}, 0.025);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 0.0f}, 0.275);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 4, 3);
        wly_InitGlyphTemplateLine(4, 0, 5);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_H()
{
    wly_BeginGlyphTemplateInit('H');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 0.55f}, -0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, -0.275);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 1.0f}, 0.275);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.55f}, 0.275);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 0.0f}, 0.275);
        
        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 3, 5);
        wly_InitGlyphTemplateLine(2, 1, 4);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_I()
{
    wly_BeginGlyphTemplateInit('I');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 1.0f}, -0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 1.0f}, 0.275);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.0f}, 0.275);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 0.0f}, WLY_Zero);
        
        wly_InitGlyphTemplateLine(0, 0, 4);
        wly_InitGlyphTemplateLine(1, 1, 3);
        wly_InitGlyphTemplateLine(2, 5, 2);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_J()
{
    wly_BeginGlyphTemplateInit('J');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.6f}, -0.025);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 0.6f}, -0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 0.8f}, -0.275);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 0.95f}, -0.175);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 1.0f}, -0.075);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 1.0f}, 0.125);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.275f, 0.9f}, 0.225);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.275f, 0.8f}, 0.275);
        wly_InitGlyphTemplateVertex(8, (Vector2){0.275f, 0.0f}, 0.275);
        wly_InitGlyphTemplateVertex(9, (Vector2){0.275f, 0.0f}, -0.275);
        
        wly_InitGlyphTemplateLine(0, 1, 0);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 4);
        wly_InitGlyphTemplateLine(4, 4, 5);
        wly_InitGlyphTemplateLine(5, 5, 6);
        wly_InitGlyphTemplateLine(6, 6, 7);
        wly_InitGlyphTemplateLine(7, 7, 8);
        wly_InitGlyphTemplateLine(8, 9, 8);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_K()
{
    wly_BeginGlyphTemplateInit('K');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 0.55f}, -0.3);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, -0.3);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 1.0f}, 0.3);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.45f}, -0.2);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 0.0f}, 0.3);
        
        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 1, 5);
        wly_InitGlyphTemplateLine(2, 4, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_L()
{
    wly_BeginGlyphTemplateInit('L');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 1.0f}, -0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, 0.275);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_M()
{
    wly_BeginGlyphTemplateInit('M');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, -0.3);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, 0.3);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3, 0.0f}, 0.3);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3, 0.45}, WLY_Zero);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 2, 3);
        wly_InitGlyphTemplateLine(2, 0, 4);
        wly_InitGlyphTemplateLine(3, 4, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_N()
{
    wly_BeginGlyphTemplateInit('N');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 1.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.0f}, 0.3f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 2, 4);
        wly_InitGlyphTemplateLine(2, 0, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_O()
{
    
    wly_BeginGlyphTemplateInit('O');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.15);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 0.1f}, -0.25);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 0.2f}, -0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.8f}, -0.3f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.9f}, -0.25f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 1.0f}, -0.15f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.3f, 1.0f}, 0.15f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.3f, 0.9f}, 0.25f);
        wly_InitGlyphTemplateVertex(8, (Vector2){0.3f, 0.8f}, 0.3f);
        wly_InitGlyphTemplateVertex(9, (Vector2){0.3f, 0.2f}, 0.3f);
        wly_InitGlyphTemplateVertex(10, (Vector2){0.3f, 0.1f}, 0.25f);
        wly_InitGlyphTemplateVertex(11, (Vector2){0.3f, 0.0f}, 0.15f);
        
        wly_InitGlyphTemplateLine(0, 1, 0);
        wly_InitGlyphTemplateLine(1, 2, 1);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 4);
        wly_InitGlyphTemplateLine(4, 4, 5);
        wly_InitGlyphTemplateLine(5, 5, 6);
        wly_InitGlyphTemplateLine(6, 6, 7);
        wly_InitGlyphTemplateLine(7, 7, 8);
        wly_InitGlyphTemplateLine(8, 8, 9);
        wly_InitGlyphTemplateLine(9, 10, 9);
        wly_InitGlyphTemplateLine(10, 11, 10);
        wly_InitGlyphTemplateLine(11, 0, 11);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_P()
{
    wly_BeginGlyphTemplateInit('P');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 0.5f}, -0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, -0.275);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 0.5f}, 0.275f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.0f}, 0.275f);

        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 1, 3);
        wly_InitGlyphTemplateLine(2, 3, 4);
        wly_InitGlyphTemplateLine(3, 0, 4);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_Q()
{
    wly_BeginGlyphTemplateInit('Q');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.15);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 0.1f}, -0.25);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 0.2f}, -0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.8f}, -0.3f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.9f}, -0.25f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 1.0f}, -0.15f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.3f, 1.0f}, 0.15f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.3f, 0.9f}, 0.25f);
        wly_InitGlyphTemplateVertex(8, (Vector2){0.3f, 0.8f}, 0.3f);
        wly_InitGlyphTemplateVertex(9, (Vector2){0.3f, 0.2f}, 0.3f);
        wly_InitGlyphTemplateVertex(10, (Vector2){0.3f, 0.1f}, 0.25f);
        wly_InitGlyphTemplateVertex(11, (Vector2){0.3f, 0.0f}, 0.15f);
        wly_InitGlyphTemplateVertex(12, (Vector2){0.3f, 1.05f}, 0.35f);
        wly_InitGlyphTemplateVertex(13, (Vector2){0.3f, 0.85f}, 0.15f);
        
        wly_InitGlyphTemplateLine(0, 1, 0);
        wly_InitGlyphTemplateLine(1, 2, 1);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 4);
        wly_InitGlyphTemplateLine(4, 4, 5);
        wly_InitGlyphTemplateLine(5, 5, 6);
        wly_InitGlyphTemplateLine(6, 6, 7);
        wly_InitGlyphTemplateLine(7, 7, 8);
        wly_InitGlyphTemplateLine(8, 8, 9);
        wly_InitGlyphTemplateLine(9, 10, 9);
        wly_InitGlyphTemplateLine(10, 11, 10);
        wly_InitGlyphTemplateLine(11, 0, 11);
        wly_InitGlyphTemplateLine(12, 13, 12);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_R()
{
    wly_BeginGlyphTemplateInit('R');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 0.55f}, -0.275);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, -0.275);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 0.55f}, 0.275f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.0f}, 0.275f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 0.55f}, -0.175f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.275f, 1.0f}, 0.275f);

        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 1, 3);
        wly_InitGlyphTemplateLine(2, 3, 4);
        wly_InitGlyphTemplateLine(3, 0, 4);
        wly_InitGlyphTemplateLine(4, 5, 6);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_S()
{
    wly_BeginGlyphTemplateInit('S');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 1.0f}, -0.275f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 1.0f}, 0.175f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 0.9f}, 0.275f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 0.55f}, 0.275f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.45f}, 0.175f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 0.45f}, -0.175f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.275f, 0.35f}, -0.275f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.275f, 0.1f}, -0.275f);
        wly_InitGlyphTemplateVertex(8, (Vector2){0.275f, 0.0f}, -0.175f);
        wly_InitGlyphTemplateVertex(9, (Vector2){0.275f, 0.0f}, 0.275);

        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 4, 3);
        wly_InitGlyphTemplateLine(4, 5, 4);
        wly_InitGlyphTemplateLine(5, 6, 5);
        wly_InitGlyphTemplateLine(6, 7, 6);
        wly_InitGlyphTemplateLine(7, 7, 8);
        wly_InitGlyphTemplateLine(8, 8, 9);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_T()
{
    wly_BeginGlyphTemplateInit('T');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 0.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.0f}, WLY_Zero);

        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 1, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_U()
{
    wly_BeginGlyphTemplateInit('U');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 0.85f}, -0.275f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, -0.175f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 1.0f}, 0.175f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.85f}, 0.275);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 0.0f}, 0.275);

        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 4);
        wly_InitGlyphTemplateLine(4, 4, 5);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_V()
{
    wly_BeginGlyphTemplateInit('V');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 0.0f}, 0.3f);

        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_W()
{
    wly_BeginGlyphTemplateInit('W');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 0.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.33f}, -0.3f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 1.0f}, -0.1f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 0.66f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.3f, 1.0f}, 0.1f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.3f, 0.33f}, 0.3f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 3, 4);
        wly_InitGlyphTemplateLine(3, 4, 5);
        wly_InitGlyphTemplateLine(4, 5, 6);
        wly_InitGlyphTemplateLine(5, 6, 7);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_X()
{
    wly_BeginGlyphTemplateInit('X');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.0f}, 0.3f);
        
        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 1, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_Y()
{
    wly_BeginGlyphTemplateInit('Y');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 0.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.5f}, WLY_Zero);
        
        wly_InitGlyphTemplateLine(0, 0, 3);
        wly_InitGlyphTemplateLine(1, 3, 2);
        wly_InitGlyphTemplateLine(2, 3, 1);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_Z()
{
    wly_BeginGlyphTemplateInit('Z');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.3f, 0.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.3f, 1.0f}, -0.3f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.3f, 1.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.3f, 0.0f}, 0.3f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.3f, 0.5f}, -0.15f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.3f, 0.5f}, 0.15f);
        
        wly_InitGlyphTemplateLine(0, 0, 3);
        wly_InitGlyphTemplateLine(1, 1, 3);
        wly_InitGlyphTemplateLine(2, 1, 2);
        wly_InitGlyphTemplateLine(3, 4, 5);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_0()
{    
    wly_BeginGlyphTemplateInit('0');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 0.0f}, -0.275f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 1.0f}, -0.275f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 1.0f}, 0.275f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 0.0f}, 0.275f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 0, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_1()
{    
    wly_BeginGlyphTemplateInit('1');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.325f, 1.0f}, -0.325f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.325f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.325f, 1.0f}, 0.225f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.325f, 0.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.325f, 0.2f}, -0.2f);
        
        wly_InitGlyphTemplateLine(0, 0, 2);
        wly_InitGlyphTemplateLine(1, 1, 3);
        wly_InitGlyphTemplateLine(2, 4, 3);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_2()
{    
    wly_BeginGlyphTemplateInit('2');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.275f, 1.0f}, -0.275f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.275f, 1.0f}, 0.275f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.275f, 0.2f}, 0.175f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.275f, 0.0f}, 0.025f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.275f, 0.0f}, -0.175f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.275f, 0.15f}, -0.225f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 0, 2);
        wly_InitGlyphTemplateLine(2, 3, 2);
        wly_InitGlyphTemplateLine(3, 4, 3);
        wly_InitGlyphTemplateLine(4, 5, 4);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_3()
{    
    wly_BeginGlyphTemplateInit('3');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.225f, 1.0f}, -0.225f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.225f, 1.0f}, 0.225f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.225f, 0.55f}, 0.225f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.225f, 0.55f}, -0.1f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.225f, 0.0f}, 0.225f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.225f, 0.0f}, -0.225f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 3, 2);
        wly_InitGlyphTemplateLine(2, 5, 4);
        wly_InitGlyphTemplateLine(3, 1, 4);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_4()
{    
    wly_BeginGlyphTemplateInit('4');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.25f, 0.0f}, -0.25f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.25f, 0.55f}, -0.25f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.25f, 1.0f}, 0.25f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.25f, 0.55f}, 0.25f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.25f, 0.0f}, 0.25f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 3);
        wly_InitGlyphTemplateLine(2, 4, 2);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_5()
{    
    wly_BeginGlyphTemplateInit('5');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.25f, 1.0f}, -0.25f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.25f, 1.0f}, 0.15f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.25f, 0.9f}, 0.25f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.25f, 0.55f}, 0.25f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.25f, 0.45f}, 0.15f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.25f, 0.45f}, -0.25f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.25f, 0.0f}, -0.25f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.25f, 0.0f}, 0.25f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 4, 3);
        wly_InitGlyphTemplateLine(4, 5, 4);
        wly_InitGlyphTemplateLine(5, 6, 5);
        wly_InitGlyphTemplateLine(6, 6, 7);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_6()
{    
    wly_BeginGlyphTemplateInit('6');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.25f, 1.0f}, -0.25f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.25f, 1.0f}, 0.15f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.25f, 0.9f}, 0.25f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.25f, 0.55f}, 0.25f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.25f, 0.45f}, 0.15f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.25f, 0.45f}, -0.25f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.25f, 0.0f}, -0.25f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.25f, 0.0f}, 0.25f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 4, 3);
        wly_InitGlyphTemplateLine(4, 5, 4);
        wly_InitGlyphTemplateLine(5, 6, 0);
        wly_InitGlyphTemplateLine(6, 6, 7);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_7()
{    
    wly_BeginGlyphTemplateInit('7');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.225f, 0.0f}, -0.225f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.225f, 0.55f}, -0.025f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.225f, 1.0f}, 0.225);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.225f, 0.55f}, 0.325f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.225f, 0.0f}, 0.225);
        
        wly_InitGlyphTemplateLine(0, 0, 4);
        wly_InitGlyphTemplateLine(1, 1, 3);
        wly_InitGlyphTemplateLine(2, 2, 4);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_8()
{    
    wly_BeginGlyphTemplateInit('8');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.25f, 0.0f}, -0.15f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.25f, 0.1f}, -0.25f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.25f, 0.35f}, -0.25f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.25f, 0.45f}, -0.15f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.25f, 0.55f}, -0.25);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.25f, 0.9f}, -0.25f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.25f, 1.0f}, -0.15f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.25f, 1.0f}, 0.15f);
        wly_InitGlyphTemplateVertex(8, (Vector2){0.25f, 0.9f}, 0.25f);
        wly_InitGlyphTemplateVertex(9, (Vector2){0.25f, 0.55f}, 0.25f);
        wly_InitGlyphTemplateVertex(10, (Vector2){0.25f, 0.45f}, 0.15f);
        wly_InitGlyphTemplateVertex(11, (Vector2){0.25f, 0.35f}, 0.25f);
        wly_InitGlyphTemplateVertex(12, (Vector2){0.25f, 0.1f}, 0.25f);
        wly_InitGlyphTemplateVertex(13, (Vector2){0.25f, 0.0f}, 0.15f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 4);
        wly_InitGlyphTemplateLine(4, 4, 5);
        wly_InitGlyphTemplateLine(5, 5, 6);
        wly_InitGlyphTemplateLine(6, 6, 7);
        wly_InitGlyphTemplateLine(7, 7, 8);
        wly_InitGlyphTemplateLine(8, 8, 9);
        wly_InitGlyphTemplateLine(9, 9, 10);
        wly_InitGlyphTemplateLine(10, 10, 3);
        wly_InitGlyphTemplateLine(11, 10, 11);
        wly_InitGlyphTemplateLine(12, 11, 12);
        wly_InitGlyphTemplateLine(13, 12, 13);
        wly_InitGlyphTemplateLine(14, 13, 0);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_9()
{    
    wly_BeginGlyphTemplateInit('9');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.25f, 0.0f}, -0.15f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.25f, 0.1f}, -0.25f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.25f, 0.35f}, -0.25f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.25f, 0.45f}, -0.15f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.25f, 0.9f}, -0.25);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.25f, 1.0f}, -0.15f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.25f, 1.0f}, 0.15f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.25f, 0.9f}, 0.25f);
        wly_InitGlyphTemplateVertex(8, (Vector2){0.25f, 0.45f}, 0.25f);
        wly_InitGlyphTemplateVertex(9, (Vector2){0.25f, 0.1f}, 0.25f);
        wly_InitGlyphTemplateVertex(10, (Vector2){0.25f, 0.0f}, 0.15f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 4, 5);
        wly_InitGlyphTemplateLine(4, 5, 6);
        wly_InitGlyphTemplateLine(5, 6, 7);
        wly_InitGlyphTemplateLine(6, 7, 9);
        wly_InitGlyphTemplateLine(7, 8, 3);
        wly_InitGlyphTemplateLine(8, 9, 10);
        wly_InitGlyphTemplateLine(9, 10, 0);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_colon()
{    
    wly_BeginGlyphTemplateInit(':');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.05f, 0.3f}, -0.05f);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.05f, 0.4f}, -0.05f);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.05f, 0.4f}, 0.05f);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.05f, 0.3f}, 0.05f);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.05f, 0.6f}, -0.05f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.05f, 0.7f}, -0.05f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.05f, 0.7f}, 0.05f);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.05f, 0.6f}, 0.05f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 1, 2);
        wly_InitGlyphTemplateLine(2, 2, 3);
        wly_InitGlyphTemplateLine(3, 3, 0);
        wly_InitGlyphTemplateLine(4, 4, 5);
        wly_InitGlyphTemplateLine(5, 5, 6);
        wly_InitGlyphTemplateLine(6, 6, 7);
        wly_InitGlyphTemplateLine(7, 7, 4);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_question()
{    
    wly_BeginGlyphTemplateInit('?');
        wly_InitGlyphTemplateVertex(0, (Vector2){0.2f, 1.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(1, (Vector2){0.2f, 0.9f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(2, (Vector2){0.2f, 0.8f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(3, (Vector2){0.2f, 0.65f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(4, (Vector2){0.2f, 0.45f}, 0.2f);
        wly_InitGlyphTemplateVertex(5, (Vector2){0.2f, 0.2f}, 0.2f);
        wly_InitGlyphTemplateVertex(6, (Vector2){0.2f, 0.0f}, WLY_Zero);
        wly_InitGlyphTemplateVertex(7, (Vector2){0.2f, 0.2f}, -0.2f);
        
        wly_InitGlyphTemplateLine(0, 0, 1);
        wly_InitGlyphTemplateLine(1, 2, 3);
        wly_InitGlyphTemplateLine(2, 3, 4);
        wly_InitGlyphTemplateLine(3, 4, 5);
        wly_InitGlyphTemplateLine(4, 5, 6);
        wly_InitGlyphTemplateLine(5, 6, 7);
    wly_FinishGlyphTemplateInit();
}

static void wly_BuildGlyphTemplate_space()
{
    wly_BeginGlyphTemplateInit(' ');
    
    ltrPtr->width = 0.5f;
    ltrPtr->kernMultiplier = 1;
    
    ltrPtr = NULL;
}

static void wly_BeginGlyphTemplateInit(char ltr){
    ltrPtr = &(wly_splitsfontGlyphTemplates[ltr]);
    ltrPtr->isActive = true;
    ltrPtr->lineCount = 0;
}

static void wly_InitGlyphTemplateVertex(int index, Vector2 pivotPos, float xOffset)
{
    wly_SplitsfontVertex *vPtr = &(ltrPtr->vertices[index]);
    vPtr->isActive = true;
    vPtr->pivotPos = pivotPos;
    vPtr->offsetVectorMagnitude = xOffset;
    vPtr->pos.x = vPtr->pivotPos.x + xOffset;
    vPtr->pos.y = vPtr->pivotPos.y;
    ltrPtr->vertexCount++;
}

static void wly_InitGlyphTemplateLine(int index, int startVertexIndex, int endVertexIndex)
{
    wly_SplitsfontLine *lPtr = &(ltrPtr->lines[index]);
    lPtr->isActive = true;
    lPtr->startIndex = startVertexIndex;
    lPtr->endIndex = endVertexIndex;
    ltrPtr->lineCount++;
}

static void wly_FinishGlyphTemplateInit(){
    float width = 0.0f;
    for (int i = 0; i < WLY_MaxVertices; i++){
        if (!ltrPtr->vertices[i].isActive) break;
        
        if (ltrPtr->vertices[i].pos.x > width)
            width = ltrPtr->vertices[i].pos.x;
    }
    ltrPtr->width = width;
    
    // TODO replace this stub with actual implementation
    // that sets different kern multipliers for different letters
    ltrPtr->kernMultiplier = 1;
    
    ltrPtr = NULL;
}
    