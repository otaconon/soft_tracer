#include "soft_tracer/ray.hpp"

Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction)
    : origin{origin}, direction{direction} {}

glm::vec3 Ray::point_at(float t) const { return origin + t * direction; }
