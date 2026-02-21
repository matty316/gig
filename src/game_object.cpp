#include <game_object.hpp>
#include <raymath.h>
#include <consts.hpp>

constexpr float angle = 45.0f * DEG2RAD;
constexpr float turningMin = 20.0f;
constexpr float accelMagnitude = 200.0f;

void GameObject::setMass(float mass) {
  inverseMass = 1.0f / mass;
}

void GameObject::addForce(Vector3 force) {
  forceAccum += force;
}

void GameObject::init(const std::string &modelPath, const std::string &texturePath, Shader shader, float scale, Vector3 position, float mass) {
  pos = position;
  velocity = Vector3Zero();
  acceleration = Vector3Zero();
  forceAccum = Vector3Zero();
  damping = 0.1f;
  rotationAngle = 0.0f;
  setMass(mass);
  scaleVec = {scale, scale, scale};
  model = LoadModel(modelPath.c_str());
  setShader(shader);
  texture = LoadTexture(texturePath.c_str());
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

void GameObject::setShader(Shader shader) {
  model.materials[0].shader = shader;
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

void GameObject::draw(Shader shader, int tilingLoc) {
  auto tiling = 1;
  SetShaderValue(shader, tilingLoc, &tiling, SHADER_UNIFORM_INT);
  DrawModelEx(model, pos, {0.0f, 1.0f, 0.0f}, -rotationAngle * RAD2DEG, scaleVec, WHITE);
}

void GameObject::cleanup() {
  UnloadModel(model);
}

void GameObject::clearAccum() {
  forceAccum = Vector3Zero();
}
