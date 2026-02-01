#pragma once

#include <glm/glm.hpp>

#include "soft_tracer/entitiy.hpp"

struct HitResult {
  float t;
  glm::vec3 point;
  glm::vec3 normal;
  Entity entity;
};
