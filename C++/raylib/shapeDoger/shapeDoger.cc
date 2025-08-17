#include <raylib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <raymath.h>

class Obstacle
{
    Vector2 position;
    Vector2 initialPos;
    Vector2 velocity;
    int scale = 200;
    int screenWidth;
    int screenHeight;
    int radius = 10;

private:
    void respawn()
    {
        this->position = this->initialPos;
    }

public:
    Obstacle(int screenHeight, int screenWidth)
    {
        this->screenHeight = screenHeight;
        this->screenWidth = screenWidth;

        // FIX 3: Use screenWidth for the random position.
        // Also, spawn within a margin so the circle doesn't start inside a wall.
        int randomPosX = GetRandomValue(radius, screenWidth - radius);
        this->position = {(float)randomPosX, (float)radius}; // Start just inside the top
        this->initialPos = this->position;

        int velocityX = GetRandomValue(0, 1) == 0 ? 1 : -1;
        // FIX 2: Ensure initial Y velocity is always positive (downwards).
        int velocityY = 1;
        this->velocity = {(float)velocityX, (float)velocityY};
    }
    Vector2 getPosition()
    {
        return this->position;
    }
    int getRadius()
    {
        return this->radius;
    }
    void move()
    {
        float dt = GetFrameTime();
        Vector2 positionChange = Vector2Scale(velocity, dt * scale);
        position = Vector2Add(position, positionChange);

        // FIX 1: Check the EDGES (position +/- radius) for collision.
        // Also, clamp the position to prevent the circle from getting stuck in the wall.
        if ((position.x - radius) < 0 || (position.x + radius) > screenWidth)
        {
            velocity.x *= -1;
            // Clamp position to stay within bounds
            if ((position.x - radius) < 0)
                position.x = radius;
            if ((position.x + radius) > screenWidth)
                position.x = screenWidth - radius;
        }

        if ((position.y - radius) < 0 || (position.y + radius) > screenHeight)
        {
            velocity.y *= -1;
            // Clamp position to stay within bounds
            if ((position.y - radius) < 0)
                position.y = radius;
            if ((position.y + radius) > screenHeight)
                position.y = screenHeight - radius;
        }
    }

    void draw()
    {

        DrawCircle(position.x, position.y, radius, RED);
    }
    static std::vector<Obstacle> generateObstacles(int numObstacles, int screenWidth, int screenHeight)
    {
        std::vector<Obstacle> obstacles;
        for (int i = 1; i <= numObstacles; i++)
        {
            obstacles.emplace_back(screenHeight, screenWidth);
        }
        return obstacles;
    }
};

class Player
{
    Vector2 position;
    int screenWidth;
    int screenHeight;
    int radius = 5;
    Vector2 initialPosition;

public:
    Player(int screenHeight, int screenWidth)
    {
        this->screenHeight = screenHeight;
        this->screenWidth = screenWidth;
        this->position = {(float)screenWidth / 2, (float)screenHeight / 2};
        this->initialPosition = position;
    }
    void spawn()
    {
        this->position = GetMousePosition();
    }
    void update()
    {
        this->position = GetMousePosition();
    }
    void draw()
    {
        DrawCircle(position.x, position.y, radius, WHITE);
    }
    bool isCollided(std::vector<Obstacle> &obstacles)
    {
        for (auto &x : obstacles)
        {
            if (CheckCollisionCircles(x.getPosition(), x.getRadius(), this->position, this->radius))
            {
                return true;
            }
        }
        return false;
    }
};

class Game
{
    std::vector<Obstacle> obstacles;
    Player player;
    float collisionFlashTimer = 0.0f;

public:
    Game(int screenWidth, int screenHeight, int obstacleCount)
        : obstacles(Obstacle::generateObstacles(obstacleCount, screenWidth, screenHeight)),
          player(screenHeight, screenWidth)
    {
        // The constructor body can now be empty, or used for other setup logic.
    }
    void update()
    {
        for (auto& obstacle : obstacles)
        {
            obstacle.move();
        }
        player.update();
        if (player.isCollided(obstacles))
        {
            player.spawn();
            collisionFlashTimer = 0.25f;
        }
        if (collisionFlashTimer > 0.0f)
        {
            collisionFlashTimer -= GetFrameTime();
        }

    }
    void draw()
    {
        if (collisionFlashTimer > 0.0f) {
            ClearBackground(MAROON); // Flash color
        } else {
            ClearBackground(BLACK); // Normal color
        }
        for (auto& obstacle : obstacles)
        {
           obstacle.draw(); 
        }
        player.draw();
    }
};

int main()
{
    int screenWidth = 1000;
    int screenHeight = 800;
    char *basicCollisionWindow = "Basic collission window";
    InitWindow(screenWidth, screenHeight, basicCollisionWindow);
    Game game(screenWidth, screenHeight, 15);
    while (!WindowShouldClose())
    {   game.update();
        BeginDrawing();
        ClearBackground(BLACK);
            game.draw();
        EndDrawing();
    }

    CloseWindow();
}