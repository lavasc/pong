#include "raylib.h"

int playerScore = 0;
int AIScore = 0;

class Ball
{
public:
    Vector2 pos;
    int radius;
    int speedX, speedY;
    void Draw()
    {
        DrawCircleV(pos, radius, WHITE);
    }
    void Move()
    {
        pos.x += speedX;
        pos.y += speedY;
        if (pos.y + radius > GetScreenHeight() || pos.y - radius < 0)
        {
            speedY *= -1;
        }
        if (pos.x + radius > GetScreenWidth())
        {
            speedX *= -1;
            playerScore++;
        }
        if (pos.x - radius < 0)
        {
            speedX *= -1;
            AIScore++;
        }
    }
};

class Player
{
public:
    Vector2 pos;
    Vector2 size;
    int speed;
    void Draw()
    {
        DrawRectangleV(pos, size, WHITE);
    }
    void Move()
    {
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            pos.y -= speed;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            pos.y += speed;
        }
        if (pos.y <= 0)
        {
            pos.y = 0;
        }
        if (pos.y + size.y > GetScreenHeight())
        {
            pos.y = GetScreenHeight() - size.y;
        }
    }
};

class AI : public Player
{
public:
    void Move(int ballY)
    {
        if (pos.y + size.y / 2 > ballY)
        {
            pos.y -= speed;
        }
        else
        {
            pos.y += speed;
        }
        if (pos.y < 0)
        {
            pos.y = 0;
        }
        if (pos.y + size.y > GetScreenHeight())
        {
            pos.y = GetScreenHeight() - size.y;
        }
    }
};

Ball ball;
Player player;
AI ai;

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(60);

    ball.pos = {screenWidth / 2, screenHeight / 2};
    ball.radius = 20;
    ball.speedX = 15;
    ball.speedY = 15;
    player.pos = {-5, screenHeight / 2 - 60};
    player.size = {25, 120};
    player.speed = 14;
    ai.pos = {screenWidth - 20, screenHeight / 2 - 60};
    ai.size = {25, 120};
    ai.speed = 12;

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        ball.Move();
        player.Move();
        ai.Move(ball.pos.y);

        if (CheckCollisionCircleRec(ball.pos, ball.radius, Rectangle{player.pos.x, player.pos.y, player.size.x, player.size.y}))
        {
            ball.speedX *= -1;
        }

        if (CheckCollisionCircleRec(ball.pos, ball.radius, Rectangle{ai.pos.x, ai.pos.y, ai.size.x, ai.size.y}))
        {
            ball.speedX *= -1;
        }

        DrawLineV({screenWidth / 2, 0}, {screenWidth / 2, screenHeight}, WHITE);
        ball.Draw();
        player.Draw();
        ai.Draw();

        DrawText(TextFormat("%d", playerScore), screenWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%d", AIScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
