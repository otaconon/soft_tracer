#pragma once

#include <glm/glm.hpp>

#include "soft_tracer/hit_result.hpp"
#include "soft_tracer/utils.hpp"
#include "soft_tracer/ray.hpp"

#include <algorithm>

inline glm::vec3 ray_trace(const Ray& ray, std::vector<HitResult> entities_hit) {
  glm::vec3 unit_direction = glm::normalize(ray.direction);
  float a = 0.5*(unit_direction.y + 1.0);
  glm::vec3 color = (1.0f-a)*glm::vec3{1.0, 1.0, 1.0} + a*glm::vec3{0.5, 0.7, 1.0};

  float color_multiplier = 0.5f;
  for (const auto &hit : entities_hit) {
    color = color_multiplier * color;
    color_multiplier *= 0.5f;
  }

  return color;
}
