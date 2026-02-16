#pragma once

#include "soft_tracer/entitiy.hpp"
#include "soft_tracer/utils.hpp"
#include <glm/glm.hpp>

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
  glm::vec3 attenuation;
  uint32_t image_x, image_y;

  glm::vec3 point_at(float t) const { return origin + t * direction; }
  glm::vec3 reflect(glm::vec3 surface_normal) {
    return direction - 2 * glm::dot(direction, surface_normal) * surface_normal;
  }

  glm::vec3 refract(glm::vec3 surface_normal, float refractive_index) {
    glm::vec3 normalized_direction = glm::normalize(direction);
    float cos_theta = std::fmin(dot(-normalized_direction, surface_normal), 1.f);
    glm::vec3 r_perpendicular =
        refractive_index * (normalized_direction + cos_theta * surface_normal);
    glm::vec3 r_parallel =
        -std::sqrtf(std::fabs(1.0 - glm::length2(r_perpendicular))) * surface_normal;
    return r_perpendicular + r_parallel;
  }
};
