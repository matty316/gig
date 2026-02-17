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

  PlayerCamera camera;
  GameObject player;
  Model skybox;
  Shader lightingShader;
  std::vector<Light> lights;
  int metallicValueLoc;
  int roughnessValueLoc;
  int emissiveIntensityLoc;
  int emissiveColorLoc;
  int textureTilingLoc;
};
