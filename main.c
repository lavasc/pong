#include "raylib.h"
#include "raymath.h"

const int screen_width = 1280;
const int screen_height = 720;

struct paddle {
    Vector2 position;
    Vector2 size;
    float speed;
};

struct enemy {
    Vector2 position;
    Vector2 size;
    float speed;
};

struct ball {
    Vector2 position;
    float radius;
    Vector2 speed;
};

struct state {
    struct paddle paddle;
    struct enemy enemy;
    struct ball ball;
    float dt;
    int score;
};

void update(struct state *state);
void draw(const struct state *state);

int main(void) {
    InitWindow(screen_width, screen_height, "pong game");
    SetTargetFPS(60);

    Vector2 paddle_size = { 20.0f, 120.0f };

    struct state state = {
        .paddle = { .position = { 0.0f, (float)screen_height / 2.0f }, .size = paddle_size, .speed = 500.0f },
        .enemy = { .position = { (float)screen_width - paddle_size.x, (float)screen_height / 2.0f },
                   .size = paddle_size,
                   .speed = 600.0f },
        .ball = { .position = { (float)screen_width / 2.0f, (float)screen_height / 2.0f },
                  .radius = 20.0f,
                  .speed = { 700.0f, 700.0f } },
        .dt = 0.0f
    };

    while (!WindowShouldClose()) {
        state.dt = GetFrameTime();
        update(&state);
        draw(&state);
    }
    CloseWindow();
    return 0;
}

void update(struct state *state) {
    // 플레이어 키 감지 및 움직임
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        state->paddle.position.y -= state->paddle.speed * state->dt;
    } else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        state->paddle.position.y += state->paddle.speed * state->dt;
    }
    state->paddle.position.y = Clamp(state->paddle.position.y, 0.0f, (float)screen_height - state->paddle.size.y);

    // AI 움직임
    if (state->ball.position.y < state->enemy.position.y + state->enemy.size.y / 2.0f) {
        state->enemy.position.y -= state->enemy.speed * state->dt;
    } else if (state->ball.position.y > state->enemy.position.y + state->enemy.size.y / 2.0f) {
        state->enemy.position.y += state->enemy.speed * state->dt;
    }
    state->enemy.position.y = Clamp(state->enemy.position.y, 0.0f, (float)screen_height - state->enemy.size.y);

    // 공 움직임 및 튕기기
    state->ball.position.x += state->ball.speed.x * state->dt;
    state->ball.position.y += state->ball.speed.y * state->dt;
    // 벽 튕김
    // 플레이어 아웃
    if (state->ball.position.x - state->ball.radius <= 0.0f) {
        state->ball.speed.x *= -1;
    }
    // AI 아웃
    if (state->ball.position.x + state->ball.radius >= (float)screen_width) {
        state->ball.speed.x *= -1;
        state->score++;
    }
    // 위 아래
    if (state->ball.position.y + state->ball.radius >= (float)screen_height ||
        state->ball.position.y - state->ball.radius <= 0.0f) {
        state->ball.speed.y *= -1;
    }
    // 패들 튕김
    if (CheckCollisionCircleRec(state->ball.position, state->ball.radius,
                                (Rectangle){ state->paddle.position.x, state->paddle.position.y, state->paddle.size.x,
                                             state->paddle.size.y })) {
        state->ball.speed.x *= -1;
    }
    if (CheckCollisionCircleRec(state->ball.position, state->ball.radius,
                                (Rectangle){ state->enemy.position.x, state->enemy.position.y, state->enemy.size.x,
                                             state->enemy.size.y })) {
        state->ball.speed.x *= -1;
    }
}

void draw(const struct state *state) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleV(state->paddle.position, state->paddle.size, WHITE);
    DrawRectangleV(state->enemy.position, state->enemy.size, WHITE);
    DrawCircleV(state->ball.position, state->ball.radius, WHITE);
    DrawText(TextFormat("SCORE: %d", state->score),
             screen_width - MeasureText(TextFormat("SCORE: %d", state->score), 20) - 20, 20, 20, WHITE);

    EndDrawing();
}
