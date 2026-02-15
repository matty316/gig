#include <game.hpp>
#include <raylib.h>
#include <rlgl.h>
#include <input.hpp>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

void Game::init() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gig");
 // ToggleFullscreen();
  SetTargetFPS(60);
  player.init();
  camera.init(player);
  loadSkybox();
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

  drawSkybox();

  player.draw();
  DrawGrid(10000, 1.0f);

  camera.end();

  DrawFPS(10, 10);

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
  UnloadShader(skybox.materials[0].shader);
  UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
  UnloadModel(skybox);
  CloseWindow();
}

void Game::loadSkybox() {
  Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
  skybox = LoadModelFromMesh(cube);

  skybox.materials[0].shader = LoadShader("shaders/skybox.vs", "shaders/skybox.fs");

  SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP}, SHADER_UNIFORM_INT);
  SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){ 0 }, SHADER_UNIFORM_INT);
  SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

  Shader shdrCubemap = LoadShader("shaders/cubemap.vs", "shaders/cubemap.fs");

  SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

  Image image = LoadImage("textures/skybox2.png");
  skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(image, CUBEMAP_LAYOUT_AUTO_DETECT);
  UnloadImage(image);
}

void Game::drawSkybox() {
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}
