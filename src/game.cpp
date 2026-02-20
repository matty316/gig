#include <game.hpp>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <input.hpp>
#include <light.hpp>
#include <consts.hpp>

void Game::init() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gig");
  ToggleFullscreen();
  SetTargetFPS(60);
  setupLighting();
  player.init("models/car/Car2.obj", "models/car/car2_red.png", lightingShader, 0.5f);
  camera.init(player);
  setupFloor();
  loadSkybox();
  DisableCursor();
}

void Game::update() {
  player.movement = updateInput();
  player.update();
  camera.update(player);

  float cameraPos[3] = { camera.pos().x, camera.pos().y, camera.pos().z };
  SetShaderValue(lightingShader, lightingShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

  if (IsKeyPressed(KEY_EQUAL)) lights[0].intensity += 0.05f;
  if (IsKeyPressed(KEY_MINUS)) lights[0].intensity -= 0.05f;

  if (lights[0].intensity < 0.0f) lights[0].intensity = 0.0f;
  if (lights[0].intensity > 1.0f) lights[0].intensity = 1.0f;

  if (IsKeyPressed(KEY_D)) { lights[0].enabled = !lights[0].enabled; }
  if (IsKeyPressed(KEY_R)) { lights[1].enabled = !lights[1].enabled; }
  if (IsKeyPressed(KEY_G)) { lights[2].enabled = !lights[2].enabled; }
  if (IsKeyPressed(KEY_B)) { lights[3].enabled = !lights[3].enabled; }

  for (auto &light : lights) UpdateLight(lightingShader, light);
}

void Game::draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  camera.begin();

  drawSkybox();

  BeginShaderMode(lightingShader);
  drawFloor();
  EndShaderMode();

  player.draw(lightingShader, tilingLoc);
  drawLightingSpheres();

  //DrawGrid(1000, 10.0f);

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
  UnloadModel(floor);
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
  lightingShader.locs[SHADER_LOC_MAP_DIFFUSE] = GetShaderLocation(lightingShader, "texture0");

  tilingLoc = GetShaderLocation(lightingShader, "tiling");

  int ambientLoc = GetShaderLocation(lightingShader, "ambient");
  float ambientStrength = 0.05f;
  SetShaderValue(lightingShader, ambientLoc, (float[4]){ ambientStrength, ambientStrength, ambientStrength, 1.0f }, SHADER_UNIFORM_VEC4);

  lights.emplace_back(CreateLight(LIGHT_DIRECTIONAL, (Vector3){-0.2f, -1.0f, -0.3f}, (Vector3){0.0f, 0.0f, 0.0f}, WHITE, 0.5f, lightingShader));

  lights.emplace_back(CreateLight(LIGHT_POINT, (Vector3){ -2.0f, 1.0f, 1.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, RED, 1.0f, lightingShader));
  lights.emplace_back(CreateLight(LIGHT_POINT, (Vector3){ 2.0f, 1.0f, 1.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, GREEN, 1.0f, lightingShader));
  lights.emplace_back(CreateLight(LIGHT_POINT, (Vector3){ 1.0f, 1.0f, -2.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, BLUE, 1.0f, lightingShader));
}

void Game::drawLightingSpheres() {
  for (auto &light : lights) {
    if (light.type == LIGHT_POINT) {
      Color lightColor = (Color){
          (unsigned char)(light.color[0]*255),
          (unsigned char)(light.color[1]*255),
          (unsigned char)(light.color[2]*255),
          (unsigned char)(light.color[3]*255) };

      if (light.enabled) DrawSphereEx(light.position, 0.2f, 8, 8, lightColor);
      else DrawSphereWires(light.position, 0.2f, 8, 8, ColorAlpha(lightColor, 0.3f));
    }
  }
}

void Game::setupFloor() {
  auto texture = LoadTexture("textures/floor.png");
  auto mesh = GenMeshPlane(1000, 1000, 10, 10);
  floor = LoadModelFromMesh(mesh);
  floor.materials[0].shader = lightingShader;
  floor.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

void Game::drawFloor() {
  auto tiling = 256;
  SetShaderValue(lightingShader, tilingLoc, &tiling, SHADER_UNIFORM_INT);
  DrawModel(floor, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
}
