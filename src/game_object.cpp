#include <game_object.hpp>
#include <raymath.h>

constexpr float angle = 90.0f * DEG2RAD;
constexpr float turningMin = 20.0f;
constexpr float accelMagnitude = 200.0f;
constexpr float angleAdjustment = 90.0f * DEG2RAD;

void GameObject::setMass(float mass) {
  inverseMass = 1.0f / mass;
}

void GameObject::addForce(Vector3 force) {
  forceAccum += force;
}

void GameObject::init() {
  pos = {0.0f, 0.5f, 0.0f};
  velocity = Vector3Zero();
  acceleration = Vector3Zero();
  forceAccum = Vector3Zero();
  damping = 0.1f;
  rotationAngle = 0.0f;
  setMass(100.0f);
  float scaleVal = .5f;
  scale = {scaleVal, scaleVal, scaleVal};
  model = LoadModel("models/cartoon_car/scene.gltf");
}

void GameObject::update() {
  float deltaTime = GetFrameTime();
  if (inverseMass <= 0.0f) return;

  float rotation = 0.0f;

  if (movement.forward) {
    Vector3 force = {accelMagnitude, 0.0f, accelMagnitude};
    addForce(force);
  }

  if (movement.backward) {
    float reverseFactor = 0.5f;
    Vector3 force = {-accelMagnitude, 0.0f, -accelMagnitude};
    force *= reverseFactor;
    addForce(force);
  }

  if (movement.left && (velocity.z > turningMin || velocity.z < -turningMin)) {
    rotation -= angle;
  }
  if (movement.right && (velocity.z > turningMin || velocity.z < -turningMin)) {
    rotation += angle;
  }

  Vector3 accum;
  accum = forceAccum * inverseMass;

  Vector3 resultingAccel;
  resultingAccel = acceleration + accum;

  velocity += resultingAccel;

  rotationAngle += rotation * deltaTime;

  velocity *= powf(damping, deltaTime);

  pos.x += cos(rotationAngle + angleAdjustment) * velocity.x * deltaTime;
  pos.z += sin(rotationAngle + angleAdjustment) * velocity.z * deltaTime;

  clearAccum();
}

void GameObject::draw() {
  DrawModelEx(model, pos, {0.0f, 1.0f, 0.0f}, -rotationAngle * RAD2DEG, scale, WHITE);
}

void GameObject::cleanup() {
  UnloadModel(model);
}

void GameObject::clearAccum() {
  forceAccum = Vector3Zero();
}
