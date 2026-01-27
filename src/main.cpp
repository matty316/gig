#include <raylib.h>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gig");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("gig window test", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();
}
