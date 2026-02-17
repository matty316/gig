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
  player.init("models/old_car_new.glb");
  camera.init(player);
  setupLighting();
  player.setupModelMaterials(lightingShader);
  loadSkybox();
  DisableCursor();
}

void Game::update() {
  player.movement = updateInput();
  player.update();
  camera.update(player);

  float cameraPos[3] = { camera.pos().x, camera.pos().y, camera.pos().z };
  SetShaderValue(lightingShader, lightingShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
}

void Game::draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  camera.begin();

  drawSkybox();

  BeginShaderMode(lightingShader);
  player.draw(lightingShader, textureTilingLoc, emissiveColorLoc, emissiveIntensityLoc);
  EndShaderMode();

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
  lightingShader = LoadShader("shaders/pbr.vs", "shaders/pbr.fs");
  lightingShader.locs[SHADER_LOC_MAP_ALBEDO] = GetShaderLocation(lightingShader, "albedoMap");
  lightingShader.locs[SHADER_LOC_MAP_METALNESS] = GetShaderLocation(lightingShader, "mraMap");
  lightingShader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(lightingShader, "normalMap");
  lightingShader.locs[SHADER_LOC_MAP_EMISSION] = GetShaderLocation(lightingShader, "emissiveMap");
  lightingShader.locs[SHADER_LOC_COLOR_DIFFUSE] = GetShaderLocation(lightingShader, "albedoColor");

  lightingShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightingShader, "viewPos");
  int lightCountLoc = GetShaderLocation(lightingShader, "numOfLights");
  int maxLightCount = MAX_LIGHTS;
  SetShaderValue(lightingShader, lightCountLoc, &maxLightCount, SHADER_UNIFORM_INT);

  float ambientIntensity = 0.02f;
  Color ambientColor = (Color){ 26, 32, 135, 255 };
  Vector3 ambientColorNormalized = (Vector3){ ambientColor.r/255.0f, ambientColor.g/255.0f, ambientColor.b/255.0f };
  SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "ambientColor"), &ambientColorNormalized, SHADER_UNIFORM_VEC3);
  SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "ambient"), &ambientIntensity, SHADER_UNIFORM_FLOAT);

  metallicValueLoc = GetShaderLocation(lightingShader, "metallicValue");
  roughnessValueLoc = GetShaderLocation(lightingShader, "roughnessValue");
  emissiveIntensityLoc = GetShaderLocation(lightingShader, "emissivePower");
  emissiveColorLoc = GetShaderLocation(lightingShader, "emissiveColor");
  textureTilingLoc = GetShaderLocation(lightingShader, "tiling");

  lights.emplace_back(CreateLight(LIGHT_DIRECTIONAL, (Vector3){-0.2f, -1.0f, -0.3f}, (Vector3){0.0f, 0.0f, 0.0f}, WHITE, 4.0f, lightingShader));

  lights.emplace_back(CreateLight(LIGHT_POINT, (Vector3){ -1.0f, 1.0f, -2.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, YELLOW, 4.0f, lightingShader));
  // lights.emplace_back(CreateLight(LIGHT_POINT, (Vector3){ 2.0f, 1.0f, 1.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, GREEN, 3.3f, lightingShader));
  // lights.emplace_back(CreateLight(LIGHT_POINT, (Vector3){ -2.0f, 1.0f, 1.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, RED, 8.3f, lightingShader));
  // lights.emplace_back(CreateLight(LIGHT_POINT, (Vector3){ 1.0f, 1.0f, -2.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, BLUE, 2.0f, lightingShader));

  int usage = 1;
  SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "useTexAlbedo"), &usage, SHADER_UNIFORM_INT);
  SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "useTexNormal"), &usage, SHADER_UNIFORM_INT);
  SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "useTexMRA"), &usage, SHADER_UNIFORM_INT);
  SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "useTexEmissive"), &usage, SHADER_UNIFORM_INT);
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
