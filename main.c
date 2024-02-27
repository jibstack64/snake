#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define BACK_COLOUR      BLACK
#define SNAKE_COLOUR     RAYWHITE
#define APPLE_COLOUR     RED

#define TICK_SPEED       0.1f 
#define VNULL            (Vector2) { -1, -1 }

#define VERSION          "1.1"


Vector2 rrandom(int, int);
bool adeq(Vector2, Vector2, Vector2);
int cfind(Vector2, Vector2*, int);
void fill(Vector2*, Vector2, int);
bool vcomp(Vector2, Vector2);
Vector2 scale(Vector2, int);

int main(void) {

    const char* title = "snake " VERSION " - Joseph";
    const int width = 400, height = 400;
    const int segment_side = (
            ((width + height) / 2) / 10);
    const int target_fps = 60;

    InitWindow(width, height, title);
    SetTargetFPS(target_fps);
    srand(time(NULL));
    
new_game:;

    Vector2 snake[
            ((width + height) / 2) / segment_side
        ]; fill(snake, (Vector2) { 1, 1 }, 3);
    Vector2 next = (Vector2) { 1, 0 };
    int length = 3;

    Vector2 apple = {
        (width / 2) / segment_side,
        (height / 2) / segment_side
    };
    
    // track tick
    float tick = 0.0f;

    while (!WindowShouldClose()) {

        /* get user input */
        if (IsKeyDown(KEY_W)) {
            next = adeq(snake[0], (Vector2) { 0, -1 }, snake[1]) ? next :
                        (Vector2) { 0, -1 };
        }
        if (IsKeyDown(KEY_A)) {
            next = adeq(snake[0], (Vector2) { -1, 0 }, snake[1]) ? next :
                        (Vector2) { -1, 0 };
        }
        if (IsKeyDown(KEY_S)) {
            next = adeq(snake[0], (Vector2) { 0, 1 }, snake[1]) ? next :
                        (Vector2) { 0, 1 };
        }
        if (IsKeyDown(KEY_D)) {
            next = adeq(snake[0], (Vector2) { 1, 0 }, snake[1]) ? next :
                        (Vector2) { 1, 0 };
        }
        if (IsKeyDown(KEY_Q)) {
            break;
        }


        /* ~~~~~~ GAME ~~~~~~ */

        /* if not tick, no reason drawing */
        if (tick >= 1.0f) {
            tick = 0.0f;
        } else {
            tick += TICK_SPEED;
            goto draw;
        }

        /* move the snake */
        for (int i = length; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = (Vector2) { snake[0].x + next.x, snake[0].y + next.y };

        /* collisions */
        if (snake[0].x >= width / segment_side ||
                snake[0].y >= height / segment_side ||
                snake[0].x < 0 || snake[0].y < 0) {
            goto new_game;
        }
        for (int i = 0; i < length; i++) {
            if (cfind(snake[i], snake, length) > 1) {
                goto new_game;
            }
        }

        /* score apple */
        if (vcomp(snake[0], apple)) {
            while (cfind(apple, snake, length) > -1) {
                apple = rrandom(((width + height) / 2 / segment_side)-1, 0);
            }

            length++;

            /* increase snake length */
            snake[length] = (Vector2) {
                snake[length-1].x + (snake[length-1].x - snake[length-2].x),
                snake[length-1].y + (snake[length-1].y - snake[length-2].y)
            };
        }

        draw:

        BeginDrawing();

            ClearBackground(BACK_COLOUR);

            /* draw the apple */
            DrawRectangle(apple.x * segment_side, apple.y * segment_side,
                        segment_side, segment_side,
                        APPLE_COLOUR);

            /* draw the snake */
            for (int i = 0; i < length; i++) {
                Vector2 pos = scale(snake[i], segment_side);
                DrawRectangle(pos.x, pos.y, segment_side, segment_side,
                        SNAKE_COLOUR);
                DrawRectangleLines(pos.x, pos.y, segment_side, segment_side,
                        ORANGE);
            }

            /* draw score */
            char buf[4]; buf[3] = '\0';
            sprintf(buf, "%d", length-3);
            DrawText(buf, width - 50, 20, 30, RAYWHITE);

        EndDrawing();
    }

    return 0;
}

Vector2 rrandom(int max, int min) {
    return (Vector2) {
        (int)(ceil((rand() % (max - min + 1)) + min)),
        (int)(ceil((rand() % (max - min + 1)) + min))
    };
}

bool adeq(Vector2 a, Vector2 b, Vector2 c) {
    return vcomp((Vector2){ a.x + b.x, a.y + b.y }, c);
}

int cfind(Vector2 item, Vector2* arr, int arrl) {
    int l = -1;
    for (int i = 0; i < arrl; i++) {
        if (vcomp(arr[i], item)) {
            l = l == -1 ? 1 : l + 1;
        }
    }
    return l;
}

void fill(Vector2* snake, Vector2 from, int count) {
    for (int i = 0; i < count; i++) {
        snake[i] = (Vector2) { from.x - i, from.y };
    }
}

bool vcomp(Vector2 a, Vector2 b) {
    return a.x == b.x && a.y == b.y;
}

Vector2 scale(Vector2 v, int s) {
    return (Vector2) { v.x * s, v.y * s };
}


