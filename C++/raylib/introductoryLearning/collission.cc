#include "raylib.h"
#include "raymath.h" // Required for vector math functions

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib Collision Force Example");

    // White Circle (the one that gets pushed)
    // It needs position, velocity, and radius.
    Vector2 whiteCirclePos = { (float)screenWidth / 2, (float)screenHeight / 2 };
    Vector2 whiteCircleVelocity = { 0.0f, 0.0f }; // Initially not moving
    float whiteCircleRadius = 20.0f;

    // Red Circle (the "pusher", controlled by the mouse)
    Vector2 redCirclePos = { 100.0f, 100.0f };
    float redCircleRadius = 30.0f;

    // Physics constants - you can tweak these!
    const float PUSH_FORCE = 700.0f; // How hard the red circle pushes the white one
    const float FRICTION = 0.18f;   // How quickly the white circle slows down (closer to 1.0 is less friction)

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // 1. Move the red circle to the mouse position
        redCirclePos = GetMousePosition();

        // 2. Check for collision between the two circles
        bool collision = CheckCollisionCircles(whiteCirclePos, whiteCircleRadius, redCirclePos, redCircleRadius);

        // 3. If a collision happens, resolve it and then apply the force
        if (collision)
        {
            // Calculate the vector pointing from the red circle's center to the white one's
            Vector2 direction = Vector2Subtract(whiteCirclePos, redCirclePos);
            
            // It's possible the circles are perfectly on top of each other.
            // In that case, the direction vector would be zero, which can't be normalized.
            // We'll add a small check to avoid a crash.
            if (direction.x != 0 || direction.y != 0)
            {
                // --- NEW: COLLISION RESOLUTION LOGIC ---
                float distance = Vector2Length(direction);
                float overlap = (whiteCircleRadius + redCircleRadius) - distance;
                
                // Normalize the direction vector to get a pure direction (length of 1)
                Vector2 normalizedDirection = Vector2Normalize(direction);

                // Move the white circle back by the amount it has overlapped
                // This ensures the circles are only just touching
                whiteCirclePos = Vector2Add(whiteCirclePos, Vector2Scale(normalizedDirection, overlap));
                // --- END OF NEW LOGIC ---

                // Calculate the impulse (the force to apply)
                // We scale the direction by our force constant and the frame time.
                Vector2 impulse = Vector2Scale(normalizedDirection, PUSH_FORCE * GetFrameTime());

                // Add the impulse to the white circle's velocity
                whiteCircleVelocity = Vector2Add(whiteCircleVelocity, impulse);
            }
        }

        // 4. Update the white circle's position based on its velocity
        // We scale the velocity by the frame time to make movement smooth and frame-rate independent.
        whiteCirclePos = Vector2Add(whiteCirclePos, Vector2Scale(whiteCircleVelocity, GetFrameTime()));

        // 5. Apply friction to the white circle's velocity so it slows down over time
        whiteCircleVelocity = Vector2Scale(whiteCircleVelocity, FRICTION);

        // (Optional) Add screen-edge collision to keep the white ball from flying off-screen
        if ((whiteCirclePos.x - whiteCircleRadius) < 0 || (whiteCirclePos.x + whiteCircleRadius) > screenWidth)
        {
            whiteCircleVelocity.x *= -1; // Reverse horizontal velocity
        }
        if ((whiteCirclePos.y - whiteCircleRadius) < 0 || (whiteCirclePos.y + whiteCircleRadius) > screenHeight)
        {
            whiteCircleVelocity.y *= -1; // Reverse vertical velocity
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            // Draw the circles
            DrawCircleV(whiteCirclePos, whiteCircleRadius, WHITE);
            DrawCircleV(redCirclePos, redCircleRadius, RED);

            DrawText("Move the red circle with your mouse to hit the white one!", 10, 10, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
