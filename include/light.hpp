#pragma once

#include <raylib.h>

enum LightType {
  LIGHT_DIRECTIONAL = 0,
  LIGHT_POINT
};

static int lightsCount = 0;

struct Light {
  LightType type;
  bool enabled;
  Vector3 position;
  Vector3 target;
  Color color;
  float attenuation;

  int enabledLoc;
  int typeLoc;
  int positionLoc;
  int targetLoc;
  int colorLoc;
  int attenuationLoc;

  Light(LightType type, Vector3 position, Vector3 target, Color color, Shader shader) : type(type), position(position), target(target), color(color) {
    enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightsCount));
    typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightsCount));
    positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightsCount));
    targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightsCount));
    colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightsCount));

    enabled = true;

    updateLightValues(shader);
    lightsCount++;
  }

  void updateLightValues(Shader shader) {
    SetShaderValue(shader, enabledLoc, &enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, typeLoc, &type, SHADER_UNIFORM_INT);

    float pos[3] = { position.x, position.y, position.z };
    SetShaderValue(shader, positionLoc, pos, SHADER_UNIFORM_VEC3);

    float targ[3] = { target.x, target.y, target.z };
    SetShaderValue(shader, targetLoc, targ, SHADER_UNIFORM_VEC3);

    float col[4] = { (float)color.r/(float)255, (float)color.g/(float)255,
                       (float)color.b/(float)255, (float)color.a/(float)255 };
    SetShaderValue(shader, colorLoc, col, SHADER_UNIFORM_VEC4);
  }
};
