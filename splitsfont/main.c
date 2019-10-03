#include "raylib.h"
#include "splitslib_sf_Splitsfont.h"

#define WLY_TargetFPS 60
#define WLY_WindowTitle "Splitsfont"
#define WLY_WindowWidth 1880

void UpdateDrawFrame(void);     // Update and Draw one frame

splitslib_sf_Result sf_result;
int string_id;

float string_width;
float string_x;

int tweak_index;
float text_size;
float text_kern;
float text_stroke_weight;
float text_angle;
int tweak_item_count;

int string_index = 0;
int string_item_count = 24;
const char *string_items[24];

float text_text;

int main(){
    SetTargetFPS(WLY_TargetFPS);
    
    InitWindow(WLY_WindowWidth, 720, WLY_WindowTitle);
   
    splitslib_sf_Init();
    
    sf_result = splitslib_sf_NewString();
    string_id = sf_result.value;
    
    splitslib_sf_SetText(string_id, "FUNNY FINGERS");
    splitslib_sf_SetRotationSpeed(string_id, 180);
    
    text_size = 0.02; // default;
    text_kern = 0.02;
    text_stroke_weight = 0.01;
    text_angle = 0;
    
    tweak_item_count = 4;
    tweak_index = 0;  // size
    
    
    
    
    
    
    
    
    
    // more menu items
    string_items[0] = "COOL";
    string_items[1] = "EASY";
    string_items[2] = "MEDIUM";
    string_items[3] = "HARD";
    string_items[4] = "DIFFICULT";
    string_items[5] = "ACHIEVEMENTS";
    string_items[6] = "HIGH SCORES";
    string_items[7] = "PLAY";
    string_items[8] = "CASHOUT";
    string_items[9] = "CONTINUE";
    string_items[10] = "TRY AGAIN";
    string_items[11] = "MAIN MENU";
    
    string_items[12] = "Menu items above, achievement names below";
    
    string_items[13] = "QWEIOP";
    string_items[14] = "THE DABBLER";
    string_items[15] = "THE QUALIFIER";
    string_items[16] = "THE UP AND COMER";
    string_items[17] = "FIGNERMUKCRE";
    string_items[18] = "TRIPLE UP";
    string_items[19] = "BEST FRIEND";
    string_items[20] = "NO PRESSURE";
    string_items[21] = "THE SCORE IS RIGHT";
    string_items[22] = "A LONGEST WORD";
    string_items[23] = "LEVEL 99";

    
    while (!WindowShouldClose()){
        UpdateDrawFrame();
    }

    CloseWindow();
    
    return 0;
}

void UpdateDrawFrame(){
    // Do updating....
    if (IsKeyPressed(KEY_D)){
        switch(tweak_index){
        case 0: text_size += 0.001; splitslib_sf_SetSize(string_id, text_size); break;
        case 1: text_kern += 0.001; splitslib_sf_SetKern(string_id, text_kern); break;
        case 2: text_stroke_weight += 0.001; splitslib_sf_SetStrokeWeight(string_id, text_stroke_weight); break;
        case 3: text_angle += 1; splitslib_sf_SetAngle(string_id, text_angle); break;
        default: break;
        }
    }
    else if (IsKeyPressed(KEY_A)){
        switch(tweak_index){
        case 0: text_size -= 0.001; splitslib_sf_SetSize(string_id, text_size); break;
        case 1: text_kern -= 0.001; splitslib_sf_SetKern(string_id, text_kern); break;
        case 2: text_stroke_weight -= 0.001; splitslib_sf_SetStrokeWeight(string_id, text_stroke_weight); break;
        case 3: text_angle -= 1; splitslib_sf_SetAngle(string_id, text_angle); break;
        default: break;
        }
    }
    else if (IsKeyPressed(KEY_S)){
        tweak_index += 1;
        if (tweak_index >= tweak_item_count) {
            tweak_index = 0;
        }
    }
    else if (IsKeyPressed(KEY_W)) {
        tweak_index -= 1;
        if (tweak_index < 0) {
            tweak_index = tweak_item_count - 1;
        }
    }
    
    
    
    else if (IsKeyPressed(KEY_DOWN)){
        string_index += 1;
        if (string_index >= string_item_count) {
            string_index = 0;
        }
        
        splitslib_sf_SetText(string_id, string_items[string_index]);
    }
    else if (IsKeyPressed(KEY_UP)) {
        string_index -= 1;
        if (string_index < 0) {
            string_index = string_item_count - 1;
        }
        
        splitslib_sf_SetText(string_id, string_items[string_index]);
    }
    
    sf_result = splitslib_sf_GetWidth(string_id);
    string_width = sf_result.floatValue;
    string_x = (WLY_WindowWidth / 2) - (string_width / 2);
    
    //splitslib_sf_AnimateString(string_id);
    
    BeginDrawing();
        ClearBackground(WHITE);
        
        DrawLine(300, 0, 300, 780, BLACK);
        DrawLine(1580, 0, 1580, 780, BLACK);
        
        DrawText(FormatText("Size: %1.3f", text_size), 5, 5, 24, 0 == tweak_index ? RED: BLACK);
        DrawText(FormatText("Kern: %1.3f", text_kern), 5, 30, 24, 1 == tweak_index ? RED: BLACK);
        DrawText(FormatText("Stroke weight: %1.3f", text_stroke_weight), 5, 55, 24, 2 == tweak_index ? RED: BLACK);
        DrawText(FormatText("Angle: %1.3f", text_angle), 5, 80, 24, 3 == tweak_index ? RED: BLACK);
        
        
        for (int i = 0; i < string_item_count; i++) {
            DrawText(string_items[i], 1585, 5 + 20 * i, 24, i == string_index ? RED : BLACK);
        }
        
        
        splitslib_sf_DrawString(string_id, string_x, 300);
    EndDrawing();
}