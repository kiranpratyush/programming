/*******************************************************************************************
*
* raylib - Simple Physics and Collision Simulation
*
* This example demonstrates basic 2D physics and collision detection without using
* an external physics engine. It's a great way to understand the fundamentals.
*
* Concepts Covered:
* - A player object with position, velocity, and properties.
* - Basic physics: gravity and velocity (Euler integration).
* - User input for movement and jumping.
* - Collision detection between a circle (player) and rectangles (platforms).
* - Simple collision resolution to make the player stand on platforms.
*
* To Compile (on Windows with MinGW):
* gcc -o physics_sim.exe your_source_file_name.c -lraylib -lopengl32 -lgdi32 -lwinmm
*
* To Compile (on Linux):
* gcc -o physics_sim your_source_file_name.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
*
********************************************************************************************/

#include "raylib.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Player structure to hold all player-related data
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool canJump;
    Color color;
} Player;

// Platform structure (using Rectangle for simplicity)
// We can just use Raylib's Rectangle struct directly.

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib - Simple Physics & Collision Simulation");

    // --- Physics Constants ---
    const float gravity = 400.0f;    // Gravity force in pixels/second^2
    const float playerJumpSpeed = 250.0f; // Vertical speed on jump
    const float playerMoveSpeed = 200.0f; // Horizontal speed

    // --- Player Initialization ---
    Player player = { 0 };
    player.position = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    player.velocity = (Vector2){ 0, 0 };
    player.radius = 20.0f;
    player.canJump = false;
    player.color = MAROON;

    // --- Platforms Initialization ---
    // An array of rectangles to act as our platforms
    const int numPlatforms = 5;
    Rectangle platforms[numPlatforms] = {
        { 0, 500, screenWidth, 100 },      // Ground floor
        { 100, 400, 200, 20 },             // First platform
        { 400, 300, 250, 20 },             // Second platform
        { 50, 200, 150, 20 },              // Third platform
        { 600, 150, 100, 20 }              // Fourth platform
    };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime(); // Time since the last frame, crucial for frame-rate independent physics

        // --- 1. Handle User Input ---
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            player.velocity.x = -playerMoveSpeed;
        }
        else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            player.velocity.x = playerMoveSpeed;
        }
        else
        {
            // Apply friction/damping when no keys are pressed
            player.velocity.x = 0;
        }

        if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP)) && player.canJump)
        {
            player.velocity.y = -playerJumpSpeed; // Negative is up in screen coordinates
            player.canJump = false;
        }

        // --- 2. Apply Physics (Simple Euler Integration) ---

        // Apply gravity to vertical velocity.
        // We multiply by deltaTime to make the acceleration smooth and frame-rate independent.
        player.velocity.y += gravity * deltaTime;

        // Update player position based on velocity.
        player.position.x += player.velocity.x * deltaTime;
        player.position.y += player.velocity.y * deltaTime;


        // --- 3. Collision Detection and Resolution ---

        // Assume the player cannot jump until we detect a collision that allows it.
        player.canJump = false;

        for (int i = 0; i < numPlatforms; i++)
        {
            // Check for collision between the player circle and the platform rectangle
            if (CheckCollisionCircleRec(player.position, player.radius, platforms[i]))
            {
                // Collision detected! Now we need to resolve it.
                // A simple way is to check the player's previous position or velocity.
                // If the player was moving down, we assume they landed on top.

                // If the player's vertical velocity is positive (moving down)
                // and the bottom of the player is near the top of the platform...
                if (player.velocity.y > 0)
                {
                    // Snap the player's position to be exactly on top of the platform.
                    // This prevents them from sinking into it.
                    player.position.y = platforms[i].y - player.radius;

                    // Stop their downward movement.
                    player.velocity.y = 0;

                    // Since they are on a platform, they can jump again.
                    player.canJump = true;
                }
                // NOTE: This is a very simple resolution. A more robust system would
                // calculate the penetration depth and push the player out along the
                // shortest axis, handling side and bottom collisions as well.
            }
        }

        // --- 4. Screen Boundary Checks ---
        // Prevent player from going off the left/right sides of the screen
        if (player.position.x - player.radius < 0)
        {
            player.position.x = player.radius;
        }
        if (player.position.x + player.radius > screenWidth)
        {
            player.position.x = screenWidth - player.radius;
        }
        // A simple "reset" if the player falls off the bottom
        if (player.position.y > screenHeight + player.radius) {
            player.position = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
            player.velocity = (Vector2){ 0, 0 };
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            // Draw all the platforms
            for (int i = 0; i < numPlatforms; i++)
            {
                DrawRectangleRec(platforms[i], DARKGRAY);
            }

            // Draw the player
            DrawCircleV(player.position, player.radius, player.color);

            // Draw some helper text
            DrawText("Use Arrow Keys/A-D to Move, Space to Jump", 10, 10, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
