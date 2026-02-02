#pragma once

#include <glm/glm.hpp>

#include "soft_tracer/hit_result.hpp"

class Camera {
public:
  Camera(uint32_t image_width, uint32_t image_height);

  [[nodiscard]] constexpr glm::vec3 get_center() noexcept { return _center; }
  [[nodiscard]] constexpr glm::vec3 get_origin() noexcept { return _pixel00_pos; }

  std::vector<HitResult> ray_cast(uint32_t pixel_x, uint32_t pixel_y) const;

private:
  glm::vec3 _pixel_delta_u, _pixel_delta_v;
  glm::vec3 _center;
  glm::vec3 _pixel00_pos;
};
