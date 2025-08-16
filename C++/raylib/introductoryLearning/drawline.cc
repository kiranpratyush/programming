#include<raylib.h>


Color backGroundColor = {162,210,255};


int main()
{   const int windowHeight = 800;
    const int windowWidth = 1000;
    const char *spritePath = "./sprites/gun1.png";
    InitWindow(windowWidth,windowHeight,"Learning Window");
    Image gunImage = LoadImage(spritePath);
    ImageColorGrayscale(&gunImage);
    ImageRotate(&gunImage,180);
    Texture2D gunTexture = LoadTextureFromImage(gunImage);
    UnloadImage(gunImage);
    while(!WindowShouldClose())
    {   Vector2 lineStartPosition = {0,0};
        Vector2 lineEndPosition = {500,200};
        BeginDrawing();
        ClearBackground(backGroundColor);
        DrawTexture(gunTexture,20,20,WHITE);
        EndDrawing();
    }
    CloseWindow();
    
}