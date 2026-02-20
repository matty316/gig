#pragma once

#include <camera.hpp>
#include <game_object.hpp>
#include <light.hpp>
#include <vector>

class Game {
public:
  void init();
  void run();
  void cleanup();
private:
  void update();
  void draw();
  void loadSkybox();
  void drawSkybox();
  void setupLighting();
  void drawLightingSpheres();
  void setupFloor();
  void drawFloor();

  PlayerCamera camera;
  GameObject player;
  Model skybox, floor;
  Shader lightingShader;
  int tilingLoc;
  float dirLightIntensity = 0.5f;
  std::vector<Light> lights;
};
