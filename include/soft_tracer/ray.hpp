#pragma once

#include <glm/glm.hpp>

class Ray {
public:
  glm::vec3 origin;
  glm::vec3 direction;

public:
  Ray(const glm::vec3 &origin, const glm::vec3 &direction);
  glm::vec3 point_at(float t) const;
};
