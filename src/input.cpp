#include <input.hpp>
#include <raymath.h>

constexpr float mouseSensitivity = 0.01f;

void updateInput(Camera3D &camera) {
  auto mouseDelta = GetMouseDelta();

  camera.target.x += mouseDelta.x * mouseSensitivity;
  camera.target.y -= mouseDelta.y * mouseSensitivity;

  Vector3 velocity{};
  if (IsKeyDown(KEY_W)) velocity.x += 1;
  if (IsKeyDown(KEY_S)) velocity.x -= 1;
  if (IsKeyDown(KEY_D)) velocity.z += 1;
  if (IsKeyDown(KEY_A)) velocity.z -= 1;

  camera.position += velocity;
}
