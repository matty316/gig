#pragma once

#include <raylib.h>
#include <game_object.hpp>

class PlayerCamera {
public:
  void init(GameObject &player);
  void update(GameObject &player);
  void begin();
  void end();
private:
  Camera3D camera;
};
