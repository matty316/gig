#include <input.hpp>
#include <raymath.h>

Movement updateInput() {
  Movement movement;

  movement.forward = IsKeyDown(KEY_SPACE);
  movement.backward = IsKeyDown(KEY_LEFT_SHIFT);
  movement.left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
  movement.right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

  return movement;
}
