#include <game.hpp>
#include <raylib.h>
#include <input.hpp>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

struct GameInstance {
  Camera3D camera = {};
} instance;

void init() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gig");
  SetTargetFPS(60);
  instance.camera.position = {10.0f, 5.0f, 10.0f};
  instance.camera.target = {0.0f, 0.0f, 0.0f};
  instance.camera.up = {0.0f, 1.0f, 0.0f};
  instance.camera.fovy = 45.0f;
  instance.camera.projection = CAMERA_PERSPECTIVE;
  DisableCursor();
}

void update() {
  updateInput(instance.camera);
  UpdateCamera(&instance.camera, CAMERA_CUSTOM);
}

void draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  BeginMode3D(instance.camera);

  DrawCube({0.0f, 1.0f, 0.0f}, 2.0f, 2.0f, 2.0f, RED);
  DrawGrid(100, 1.0f);

  EndMode3D();

  EndDrawing();
}

void run() {
  while (!WindowShouldClose()) {
    update();
    draw();
  }
}

void cleanup() {
  CloseWindow();
}
