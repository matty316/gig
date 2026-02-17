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

void GameObject::init(const std::string modelPath) {
  pos = {0.0f, 0.0f, 0.0f};
  velocity = Vector3Zero();
  acceleration = Vector3Zero();
  forceAccum = Vector3Zero();
  damping = 0.1f;
  rotationAngle = 0.0f;
  setMass(100.0f);
  float scaleVal = 0.25f;
  scale = {scaleVal, scaleVal, scaleVal};
  model = LoadModel(modelPath.c_str());
  textureTiling = (Vector2){ 0.5f, 0.5f };
}

void GameObject::setupModelMaterials(Shader shader) {
  model.materials[0].shader = shader;

  model.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
  model.materials[0].maps[MATERIAL_MAP_METALNESS].value = 1.0f;
  model.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
  model.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
  model.materials[0].maps[MATERIAL_MAP_EMISSION].color = (Color){ 255, 162, 0, 255 };

  model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = LoadTexture("textures/old_car_d.png");
  model.materials[0].maps[MATERIAL_MAP_METALNESS].texture = LoadTexture("textures/old_car_mra.png");
  model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = LoadTexture("textures/old_car_n.png");
  model.materials[0].maps[MATERIAL_MAP_EMISSION].texture = LoadTexture("textures/old_car_e.png");
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

void GameObject::draw(Shader shader, int textureTilingLoc, int emissiveColorLoc, int emissiveIntensityLoc) {
  SetShaderValue(shader, textureTilingLoc, &textureTiling, SHADER_UNIFORM_VEC2);
  Vector4 carEmissiveColor = ColorNormalize(model.materials[0].maps[MATERIAL_MAP_EMISSION].color);
  SetShaderValue(shader, emissiveColorLoc, &carEmissiveColor, SHADER_UNIFORM_VEC4);
  float emissiveIntensity = 0.01f;
  SetShaderValue(shader, emissiveIntensityLoc, &emissiveIntensity, SHADER_UNIFORM_FLOAT);
  DrawModelEx(model, pos, {0.0f, 1.0f, 0.0f}, -rotationAngle * RAD2DEG, scale, WHITE);
}

void GameObject::cleanup() {
  UnloadModel(model);
}

void GameObject::clearAccum() {
  forceAccum = Vector3Zero();
}
