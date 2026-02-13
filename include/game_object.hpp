#pragma once

#include <raylib.h>

struct Movement {
  bool forward = false;
  bool backward = false;
  bool left = false;
  bool right = false;
};

struct GameObject {
  Vector3 pos, scale, velocity, acceleration, forceAccum;
  float rotationAngle, damping, inverseMass;
  Movement movement;
  Model model;

  void init();
  void setMass(float mass);
  void addForce(Vector3 force);
  void update();
  void clearAccum();
  void draw();
  void cleanup();
};
