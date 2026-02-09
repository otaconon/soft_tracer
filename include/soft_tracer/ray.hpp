#pragma once

#include "soft_tracer/entitiy.hpp"
#include <glm/glm.hpp>

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
  glm::vec3 attenuation;
  uint32_t image_x, image_y;

  glm::vec3 point_at(float t) const { return origin + t * direction; }
  glm::vec3 reflect(glm::vec3 n) {
    return direction - 2 * glm::dot(direction, n) * n;
  }
};
