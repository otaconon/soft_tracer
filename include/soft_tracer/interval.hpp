#pragma once

#include <limits>

struct Interval {
  float min, max;

  [[nodiscard]] constexpr float get_size() const noexcept {
    return max - min;
  }
  [[nodiscard]] constexpr bool encloses(float val) const noexcept {
    return min <= val && val <= max;
  };

  static inline constexpr Interval empty() noexcept {
    return {std::numeric_limits<float>::infinity(),
            -std::numeric_limits<float>::infinity()};
  }

  static inline constexpr Interval universe() noexcept {
    return {std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::max()};
  }
};
