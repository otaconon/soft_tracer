#pragma once

#include <glm/glm.hpp>
#include <random>

inline float random_uniform_float() {
  thread_local std::mt19937 generator(std::random_device{}());
  thread_local std::uniform_real_distribution<float> distribution(0.0, 1.0);
  return distribution(generator);
}

inline float random_uniform_float(float min, float max) {
  return min + (max - min) * random_uniform_float();
}

inline glm::vec2 sample_square() {
  return glm::vec2{random_uniform_float() - 0.5f, random_uniform_float() - 0.5f};
}
