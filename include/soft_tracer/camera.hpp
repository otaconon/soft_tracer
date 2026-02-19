#pragma once

#include "soft_tracer/ray.hpp"
#include <glm/glm.hpp>

class Camera {
public:
  Camera(uint32_t image_width, uint32_t image_height);

  [[nodiscard]] glm::vec3 get_center() const noexcept { return _center; }
  [[nodiscard]] glm::vec3 get_origin() const noexcept { return _pixel00_pos; }

  Ray ray_cast(uint32_t pixel_x, uint32_t pixel_y) const;

private:
  glm::vec3 _pixel_delta_u{}, _pixel_delta_v{};
  glm::vec3 _center{0.f};
  glm::vec3 _pixel00_pos{};
};
