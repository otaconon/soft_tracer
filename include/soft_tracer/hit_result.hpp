#pragma once

#include <glm/glm.hpp>
#include <limits>

#include "soft_tracer/entitiy.hpp"

struct HitResult {
  float t = std::numeric_limits<float>::infinity();
  glm::vec3 point;
  glm::vec3 normal;
  Entity entity;
};
