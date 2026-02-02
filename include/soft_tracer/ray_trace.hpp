#pragma once

#include <glm/glm.hpp>

#include "soft_tracer/hit_result.hpp"

#include <algorithm>

inline glm::vec3 ray_trace(std::vector<HitResult> entities_hit) {
  std::ranges::sort(entities_hit, {}, &HitResult::t);
  for (const auto &hit : entities_hit) {
    return 0.5f * (hit.normal + 1.f);
  }

  return glm::vec3{0.05f, 0.05f, 0.1f};
}
