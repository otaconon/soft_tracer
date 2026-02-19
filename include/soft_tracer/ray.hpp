#pragma once

#include "utils.hpp"

#include <glm/glm.hpp>

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
  glm::vec3 attenuation;
  uint32_t image_x, image_y;

  [[nodiscard]] glm::vec3 point_at(const float t) const noexcept { return origin + t * direction; }
  [[nodiscard]] glm::vec3 reflect(const glm::vec3 surface_normal) const {
    return direction - 2 * glm::dot(direction, surface_normal) * surface_normal;
  }

  [[nodiscard]] glm::vec3 refract(
      const glm::vec3 surface_normal, const float refractive_index) const {
    const glm::vec3 normalized_direction = glm::normalize(direction);
    const float cos_theta = std::fmin(dot(-normalized_direction, surface_normal), 1.f);
    const glm::vec3 r_perpendicular =
        refractive_index * (normalized_direction + cos_theta * surface_normal);
    const glm::vec3 r_parallel =
        -std::sqrtf(std::fabs(1.0 - glm::length2(r_perpendicular))) * surface_normal;
    return r_perpendicular + r_parallel;
  }
};
