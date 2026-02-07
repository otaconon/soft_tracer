#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <random>

namespace utils {
  template<typename T>
  inline T random() {
    static thread_local std::mt19937 generator(std::random_device{}());
    static thread_local std::uniform_real_distribution<T> distribution(0.0, 1.0);
    return distribution(generator);
  }

  inline glm::vec3 random() {
    static thread_local std::mt19937 generator(std::random_device{}());
    static thread_local std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return {distribution(generator), distribution(generator), distribution(generator)};
  }

  template<typename T>
  inline T random(T min, T max) {
    return min + (max - min) * random<T>();
  }

  inline glm::vec3 random(float min, float max) {
    return {random<float>(min, max), random<float>(min, max), random<float>(min, max)};
  }

  inline glm::vec3 random_unit() {
    for (int i = 0; i < 100; ++i) {
      glm::vec3 p = random(-1.f, 1.f);
      float lensq = glm::length2(p);
      if (1e-15 < lensq && lensq <= 1) {
        return p / sqrt(lensq);
      }
    }
    return {};
  }

  inline glm::vec3 random_on_hemisphere(glm::vec3 normal) {
    glm::vec3 on_unit_sphere = random_unit();
    if (glm::dot(on_unit_sphere, normal) > 0.f) {
      return on_unit_sphere;
    }
    return -on_unit_sphere;
  }

  inline glm::vec2 sample_square() {
    return glm::vec2{random<float>() - 0.5f, random<float>() - 0.5f};
  }
}
