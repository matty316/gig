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
  void init(const std::string modelPath);
  void setupModelMaterials(Shader shader);
  void update();
  void draw(Shader shader, int textureTilingLoc, int emissiveColorLoc, int emissiveIntensityLoc);
  void cleanup();

  Vector3 pos;
  float rotationAngle;
  Movement movement;

private:
  Vector3 scale, velocity, acceleration, forceAccum;
  float damping, inverseMass;
  Model model;
  Vector2 textureTiling;

  void setMass(float mass);
  void addForce(Vector3 force);
  void clearAccum();
};
