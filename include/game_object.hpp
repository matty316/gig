#pragma once

#include <raylib.h>
#include <string>

struct Movement {
  bool forward = false;
  bool backward = false;
  bool left = false;
  bool right = false;
};

class GameObject {
public:
  void init(const std::string &modelPath, const std::string &texturePath, Shader shader, float scale = 1.0f);
  void update();
  void draw(Shader shader, int tilingLoc);
  void cleanup();
  void setShader(Shader shader);

  Vector3 pos;
  float rotationAngle;
  Movement movement;

private:
  Vector3 scaleVec, velocity, acceleration, forceAccum;
  float damping, inverseMass;
  Model model, floor;

  Texture2D texture;

  void setMass(float mass);
  void addForce(Vector3 force);
  void clearAccum();
};
