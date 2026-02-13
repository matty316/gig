#include <game.hpp>
#include <raylib.h>
#include <input.hpp>
#include <camera.hpp>
#include <game_object.hpp>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
PlayerCamera camera;
GameObject player;

void init() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gig");
  ToggleFullscreen();
  SetTargetFPS(60);
  player.init();
  camera.init(player);
  DisableCursor();
}

void update() {
  player.movement = updateInput();
  player.update();
  camera.update(player);
}

void draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  camera.begin();

  player.draw();
  DrawGrid(10000, 1.0f);

  camera.end();

  EndDrawing();
}

void run() {
  while (!WindowShouldClose()) {
    update();
    draw();
  }
}

void cleanup() {
  player.cleanup();
  CloseWindow();
}
