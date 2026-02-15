#include "raylib.h"
#include <input.hpp>
#include <print>
#include <raymath.h>

constexpr float leftStickDeadzoneX = 0.1f;
constexpr float leftStickDeadzoneY = 0.1f;
constexpr float rightStickDeadzoneX = 0.1f;
constexpr float rightStickDeadzoneY = 0.1f;
constexpr float leftTriggerDeadzone = -0.9f;
constexpr float rightTriggerDeadzone = -0.9f;

Movement updateInput() {
  Movement movement;

  int gamepad = 0;

  if (IsGamepadAvailable(gamepad)) {
    float leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
    float leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
    float rightStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X);
    float rightStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y);
    float leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);
    float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);

    if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
    if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f;
    if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) rightStickX = 0.0f;
    if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) rightStickY = 0.0f;
    if (leftTrigger < leftTriggerDeadzone) leftTrigger = -1.0f;
    if (rightTrigger < rightTriggerDeadzone) rightTrigger = -1.0f;

    movement.forward = IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    movement.backward = IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    movement.left = leftStickX < 0.0f || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    movement.right = leftStickX > 0.0f || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
  } else {
    movement.forward = IsKeyDown(KEY_SPACE);
    movement.backward = IsKeyDown(KEY_LEFT_SHIFT);
    movement.left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    movement.right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
  }

  return movement;
}
