#include "soft_tracer/camera.hpp"

#include "glm/trigonometric.hpp"
#include "soft_tracer/ray.hpp"
#include "soft_tracer/utils.hpp"

Camera::Camera(const uint32_t image_width, const uint32_t image_height) :
    _image_width{ image_width }, _image_height{ image_height } {
  recalculate();
}

void Camera::set_position(const glm::vec3 position) {
  _position = position;
  recalculate();
}

void Camera::look_at(const glm::vec3 target) {
  _target = target;
  recalculate();
}

void Camera::recalculate() {
  const float focal_length = glm::length(_target - _position);
  const float theta = glm::radians(_vfov);
  const float h = std::tan(theta / 2);
  const float viewport_height = 2.0f * h * focal_length;
  const float viewport_width =
      viewport_height * (static_cast<float>(_image_width) / _image_height);

  const glm::vec3 forward = glm::normalize(_target - _position);
  const glm::vec3 right =
      glm::normalize(glm::cross(forward, glm::vec3{ 0.f, 1.f, 0.f }));
  const glm::vec3 up = glm::normalize(glm::cross(right, forward));

  const glm::vec3 viewport_u = viewport_width * right;
  const glm::vec3 viewport_v = -viewport_height * up;

  _pixel_delta_u = viewport_u / static_cast<float>(_image_width);
  _pixel_delta_v = viewport_v / static_cast<float>(_image_height);

  const glm::vec3 viewport_upper_left =
      _position + focal_length * forward - viewport_u / 2.f - viewport_v / 2.f;
  _pixel00_pos = viewport_upper_left + 0.5f * (_pixel_delta_u + _pixel_delta_v);
}

Ray Camera::ray_cast(const uint32_t pixel_x, const uint32_t pixel_y) const {
  const glm::vec2 offset = utils::sample_square();
  const glm::vec3 pixel_center = _pixel00_pos +
      (pixel_x + offset.x) * _pixel_delta_u +
      (pixel_y + offset.y) * _pixel_delta_v;
  const glm::vec3 ray_direction = glm::normalize(pixel_center - _position);
  return Ray{ _position, ray_direction, { 1.f, 1.f, 1.f }, pixel_x, pixel_y };
}
