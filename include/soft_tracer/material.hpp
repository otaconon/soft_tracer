#pragma once

#include <glm/glm.hpp>

struct Material {
  glm::vec3 albedo;
  float roughness;
  float metallic;
  float transmission;
  float ior;
};

static Material make_dielectric(const glm::vec3 albedo, const float ior) {
  return Material{ .albedo = albedo,
    .roughness = 0.f,
    .metallic = 0.f,
    .transmission = 1.f,
    .ior = ior };
}

static Material make_lambertian(const glm::vec3 albedo) {
  return Material{ .albedo = albedo,
    .roughness = 1.f,
    .metallic = 0.f,
    .transmission = 0.f,
    .ior = 0.f };
}

static Material make_metallic(const glm::vec3 albedo, const float roughness) {
  return Material{ .albedo = albedo,
    .roughness = roughness,
    .metallic = 1.f,
    .transmission = 0.f,
    .ior = 0.f };
}

static float compute_reflectance(float cosine, float refraction_index) {
  // Use Schlick's approximation for reflectance
  auto r0 = (1 - refraction_index) / (1 + refraction_index);
  r0 = r0 * r0;
  return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}
