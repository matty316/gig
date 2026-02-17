#include <camera.hpp>
#include <raymath.h>
#include <consts.hpp>

constexpr Vector3 up = {0.0f, 1.0f, 0.0f};
constexpr Vector3 targetOffset = {0.0f, 5.0f, 10.0f};

void PlayerCamera::init(GameObject &player) {
  camera.position = player.pos - targetOffset;
  camera.target = player.pos;
  camera.up = up;
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
}

void PlayerCamera::update(GameObject &player) {
  camera.position.x = player.pos.x - cos(player.rotationAngle + angleAdjustment) * targetOffset.z;
  camera.position.z = player.pos.z - sin(player.rotationAngle + angleAdjustment) * targetOffset.z;
  camera.position.y = player.pos.y + targetOffset.y;
  camera.target = player.pos;
  UpdateCamera(&camera, CAMERA_CUSTOM);
}

void PlayerCamera::begin() {
  BeginMode3D(camera);
}

void PlayerCamera::end() {
  EndMode3D();
}

Vector3 PlayerCamera::pos() {
  return camera.position;
}
