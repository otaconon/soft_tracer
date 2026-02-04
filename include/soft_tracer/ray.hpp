#pragma once

#include <glm/glm.hpp>

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
  float throughput;
  uint32_t image_x, image_y;

  glm::vec3 point_at(float t) const { return origin + t * direction; }
};
