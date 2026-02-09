#include "soft_tracer/camera.hpp"

#include "soft_tracer/ray.hpp"
#include "soft_tracer/utils.hpp"

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

Ray Camera::ray_cast(uint32_t pixel_x, uint32_t pixel_y) const {
  glm::vec2 offset = utils::sample_square();
  glm::vec3 pixel_center = _pixel00_pos +
                           (pixel_x + offset.x) * _pixel_delta_u +
                           (pixel_y + offset.y) * _pixel_delta_v;
  glm::vec3 ray_direction = glm::normalize(pixel_center - _center);
  return Ray{_center, ray_direction, {1.f, 1.f, 1.f}, pixel_x, pixel_y};
}
