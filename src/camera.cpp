#include "soft_tracer/camera.hpp"

#include "soft_tracer/hit_system.hpp"
#include "soft_tracer/ray.hpp"
#include "soft_tracer/ray_trace.hpp"
#include "soft_tracer/sphere.hpp"
#include "soft_tracer/utils.hpp"

#include <limits>

Camera::Camera(uint32_t image_width, uint32_t image_height) {
  _center = glm::vec3(0.f);

  constexpr float focal_length = 1.0f;
  constexpr float viewport_height = 2.0f;
  float viewport_width =
      viewport_height * (static_cast<float>(image_width) / image_height);

  glm::vec3 viewport_u = glm::vec3(viewport_width, 0, 0);
  glm::vec3 viewport_v = glm::vec3(0, -viewport_height, 0);

  _pixel_delta_u = viewport_u / static_cast<float>(image_width);
  _pixel_delta_v = viewport_v / static_cast<float>(image_height);

  glm::vec3 viewport_upper_left = _center - glm::vec3(0, 0, focal_length) -
                                  viewport_u / 2.f - viewport_v / 2.f;
  _pixel00_pos = viewport_upper_left + 0.5f * (_pixel_delta_u + _pixel_delta_v);
}

glm::vec3 Camera::ray_cast(uint32_t pixel_x, uint32_t pixel_y) const {
  glm::vec2 offset = utils::sample_square();
  glm::vec3 pixel_center = _pixel00_pos +
                           (pixel_x + offset.x) * _pixel_delta_u +
                           (pixel_y + offset.y) * _pixel_delta_v;
  glm::vec3 ray_direction = glm::normalize(pixel_center - _center);
  Ray ray(_center, ray_direction);

  std::vector<HitResult> entities_hit;
  constexpr uint32_t max_steps = 20;
  uint32_t steps = 0;
  HitResult entity_hit = hit_entities_with<Sphere>(
      ray, {0.001f, std::numeric_limits<float>::infinity()});
  while (entity_hit.entity.Valid() && steps < max_steps) {
    entities_hit.push_back(entity_hit);
    steps++;
    ray = {ray.point_at(entity_hit.t),
           utils::random_on_hemisphere(entity_hit.normal)};
    entity_hit = hit_entities_with<Sphere>(
        ray, {0.0001f, std::numeric_limits<float>::infinity()});
  }

  return ray_trace(ray, entities_hit);
}
