#include <game.hpp>
#include <raylib.h>
#include <input.hpp>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

void Game::init() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gig");
  ToggleFullscreen();
  SetTargetFPS(60);
  player.init();
  camera.init(player);
  DisableCursor();
}

void Game::update() {
  player.movement = updateInput();
  player.update();
  camera.update(player);
}

void Game::draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  camera.begin();

  player.draw();
  DrawGrid(10000, 1.0f);
  DrawPlane({0.0f, 0.0f, 0.0f}, {10000, 10000}, PURPLE);

  camera.end();

  EndDrawing();
}

void Game::run() {
  while (!WindowShouldClose()) {
    update();
    draw();
  }
}

void Game::cleanup() {
  player.cleanup();
  CloseWindow();
}
