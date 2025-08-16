#include<raylib.h>

Color backGroundColor = {162,210,255};

int main()
{
    const int screenHeight = GetScreenHeight();
    const int screenWidth = GetScreenWidth();
    InitWindow(screenWidth,screenWidth,"Load Images");
    Image cowBoyImage = LoadImage("./assets/animation/0.png");
    Texture2D cowBoyTexture = LoadTextureFromImage(cowBoyImage);
    Vector2 shipPos = {0,0};
    UnloadImage(cowBoyImage);
    SetTargetFPS(144);
    int x = 1;
    while(!WindowShouldClose())
    {   float deltaTime = GetFrameTime();
        shipPos.x+=100*deltaTime*x;
        shipPos.y+=50*deltaTime*x;
        x*=-1;
        BeginDrawing();
            ClearBackground(backGroundColor);
            DrawTextureV(cowBoyTexture,shipPos,WHITE);
            DrawFPS(0,0);
        EndDrawing();
    }

}