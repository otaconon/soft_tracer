#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <random>

namespace utils {
inline std::mt19937& get_random_generator() {
  thread_local std::mt19937 generator(std::random_device{}());
  return generator;
}

template <typename T>
T random() {
  if constexpr (std::is_same_v<T, bool>) {
    std::bernoulli_distribution distribution(0.5);
    return distribution(get_random_generator());
  } else if constexpr (std::is_floating_point_v<T>) {
    std::uniform_real_distribution<T> distribution(0.0, 1.0);
    return distribution(get_random_generator());
  } else if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> distribution;
    return distribution(get_random_generator());
  }
  return T{};
}

template <>
inline glm::vec3 random<glm::vec3>() {
  return { random<float>(), random<float>(), random<float>() };
}

template <typename T>
 T random(T min, T max) {
  if constexpr (std::is_floating_point_v<T>) {
    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(get_random_generator());
  } else if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> distribution(min, max);
    return distribution(get_random_generator());
  }
  return T{};
}

inline glm::vec3 random(const float min, const float max) {
  return { random<float>(min, max), random<float>(min, max),
    random<float>(min, max) };
}

inline glm::vec3 random(const glm::vec3& min, const glm::vec3& max) {
  return { random<float>(min.x, max.x), random<float>(min.y, max.y),
    random<float>(min.z, max.z) };
}

inline glm::vec3 random_unit() {
  for (int i = 0; i < 100; ++i) {
    glm::vec3 p = random(-1.f, 1.f);
    const float lensq = glm::length2(p);
    if (1e-15 < lensq && lensq <= 1) {
      return p / sqrt(lensq);
    }
  }
  return {};
}

inline glm::vec3 random_on_hemisphere(const glm::vec3 normal) {
  const glm::vec3 on_unit_sphere = random_unit();
  if (glm::dot(on_unit_sphere, normal) > 0.f) {
    return on_unit_sphere;
  }
  return -on_unit_sphere;
}

inline glm::vec2 sample_square() {
  return glm::vec2{ random<float>() - 0.5f, random<float>() - 0.5f };
}
} //namespace utils
