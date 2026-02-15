#pragma once

#include <camera.hpp>
#include <game_object.hpp>

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

  PlayerCamera camera;
  GameObject player;
  Model skybox;
};
