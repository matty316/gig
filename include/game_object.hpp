#pragma once

#include <raylib.h>

struct Movement {
  bool forward = false;
  bool backward = false;
  bool left = false;
  bool right = false;
};

class GameObject {
public:
  void init();
  void update();
  void draw();
  void cleanup();

  Vector3 pos;
  float rotationAngle;
  Movement movement;

private:
  Vector3 scale, velocity, acceleration, forceAccum;
  float damping, inverseMass;
  Model model;

  void setMass(float mass);
  void addForce(Vector3 force);
  void clearAccum();
};
