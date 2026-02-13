#include <camera.hpp>
#include <raymath.h>

constexpr Vector3 up = {0.0f, 1.0f, 0.0f};
constexpr Vector3 targetOffset = {0.0f, -10.0f, 25.0f};

void initCamera(Camera3D &camera, GameObject &player) {
  camera.position = player.pos - targetOffset;
  camera.target = player.pos;
  camera.up = up;
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
}

void updateCamera(Camera3D &camera, GameObject &player) {
  camera.position = player.pos - targetOffset;
  camera.target = player.pos;
  UpdateCamera(&camera, CAMERA_CUSTOM);
}
