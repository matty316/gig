#include <game.hpp>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <input.hpp>
#include <light.hpp>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

void Game::init() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gig");
 // ToggleFullscreen();
  SetTargetFPS(60);
  player.init("models/old_rusty_car.glb");
  camera.init(player);
  setupLighting();
  loadSkybox();
  DisableCursor();
}

void Game::update() {
  player.movement = updateInput();
  player.update();
  camera.update(player);

  float cameraPos[3] = { camera.pos().x, camera.pos().y, camera.pos().z };
  SetShaderValue(lightingShader, lightingShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

  for (auto &light : lights) light.updateLightValues(lightingShader);
}

void Game::draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  camera.begin();

  drawSkybox();

  BeginShaderMode(lightingShader);
  player.draw();
  EndShaderMode();

  // DrawPlane(Vector3Zero(), {10000, 10000}, WHITE);
  // DrawCube(Vector3Zero(), 2.0, 4.0, 2.0, WHITE);

  drawLightingSpheres();

  DrawGrid(10000, 10.0f);

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
  UnloadShader(lightingShader);
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

void Game::setupLighting() {
  lightingShader = LoadShader("shaders/lighting.vs", "shaders/lighting.fs");
  lightingShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightingShader, "viewPos");
  int ambientLoc = GetShaderLocation(lightingShader, "ambient");
  SetShaderValue(lightingShader, ambientLoc, (float[4]){0.1f, 0.1f, 0.1f, 1.0f}, SHADER_UNIFORM_VEC4);

  lights = {
    Light(LIGHT_POINT, (Vector3){ -2, 2, -2 }, Vector3Zero(), YELLOW, lightingShader),
    Light(LIGHT_POINT, (Vector3){ 2, 2, 2 }, Vector3Zero(), RED, lightingShader),
    Light(LIGHT_POINT, (Vector3){ -2, 2, 2 }, Vector3Zero(), GREEN, lightingShader),
    Light(LIGHT_POINT, (Vector3){ 2, 2, -2 }, Vector3Zero(), BLUE, lightingShader),
  };
}

void Game::drawLightingSpheres() {
  for (auto &light : lights) {
    if (light.enabled) DrawSphereEx(light.position, 0.2f, 8, 8, light.color);
    else DrawSphereWires(light.position, 0.2f, 8, 8, ColorAlpha(light.color, 0.3f));
  }
}
