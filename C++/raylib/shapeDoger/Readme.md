This first game is all about the fundamentals: movement, drawing basic shapes, and handling collisions.

Objective: Control a player circle with the mouse or keyboard and dodge randomly falling rectangles. If you collide with a rectangle, the game resets.

Skills You'll Practice:

1-5: Window initialization, game loop, colors, and drawing basic shapes (circles, rectangles).

8: Collision detection between a circle (player) and rectangles (obstacles). You'll use CheckCollisionCircleRec().

Step-by-Step Plan:

Setup: Create a window.

Player: Draw a circle that represents the player. Make its position follow the mouse cursor (GetMousePosition()) or move with arrow keys.

Enemies: Create an array of rectangles. In each frame, make them fall from the top of the screen to the bottom.

Spawning: If a rectangle goes off-screen, reset its position to the top at a new random horizontal location.

Collision: In the game loop, check for a collision between the player's circle and each of the falling rectangles.

Game Over: If a collision is detected, you can either end the game or simply change the background color to red for a moment before resetting the player's position.


## Approach 
1. Player
2. Obstacle
3. Obstacles
3. Collision detection of the player with any of the obstacle 
4. Game : Keep tracks of the score and resets


### Player 
properties:
1. Pos x,y
2. update
3. CheckCollide
4. screenWidth,screenHeight 


### Game:
properites and methods:
1. score
2. play : update the player , update the obstacles,keep track of score 
