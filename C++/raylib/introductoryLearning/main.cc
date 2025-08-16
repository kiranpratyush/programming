#include<raylib.h>

const int screenWidth = 800;
const int screenHeight = 600;
const int radius = 20;

int main()
{   Vector2 ballPosition = {(float)screenWidth/2,(float)screenHeight/2};
    InitWindow(800,600,"Raylib Test");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {   if(IsKeyDown(KEY_RIGHT)) ballPosition.x+=2.0f;
        if(IsKeyDown(KEY_LEFT)) ballPosition.x-=2.0f;
        if(IsKeyDown(KEY_UP)) ballPosition.y-=2.0f;
        if(IsKeyDown(KEY_DOWN)) ballPosition.y+=2.0f;
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawText("HELLO WINDOW",10,10,300,BLUE);
        // Draw a red circle
        DrawCircleV(ballPosition,radius,RED);

        EndDrawing();
    }

    // Close window
    CloseWindow();
}