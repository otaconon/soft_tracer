#pragma once

#include <glm/glm.hpp>

class Ray {
public:
  glm::vec3 origin_;
  glm::vec3 direction_;

public:
  Ray(const glm::vec3 &origin, const glm::vec3 &direction);
  glm::vec3 point_at(float t) const;
};
