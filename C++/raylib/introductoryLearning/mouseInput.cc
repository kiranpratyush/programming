#include<iostream>
#include<cstdio>
#include<string>
#include<raylib.h>

struct MousePosition{
    int x{};
    int y{};
};


int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 800;
    char *leftMouseButtonDownText;
    MousePosition mousePosition;
    char mousePositionBuffer[100];
    InitWindow(screenWidth,screenHeight,"Mouse Input window");
    
    while(!WindowShouldClose())
    {   bool leftMouseButtonPressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        mousePosition.x = GetMouseX();
        mousePosition.y = GetMouseY();
        snprintf(mousePositionBuffer,sizeof(mousePositionBuffer),"POS X: %d POS Y: %d",mousePosition.x,mousePosition.y);
        if(leftMouseButtonPressed)
        {
            leftMouseButtonDownText = "Left Mouse Button Pressed";
        }
        else{
            leftMouseButtonDownText = "Left Mouse Button is not Pressed";
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(leftMouseButtonDownText,10,10,30,BLUE);
            DrawText(mousePositionBuffer,mousePosition.x,mousePosition.y,20,BLUE);
            
        EndDrawing();
    }

    CloseWindow();
}