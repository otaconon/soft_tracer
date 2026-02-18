#pragma once

#include <glm/glm.hpp>

struct Material {
  glm::vec3 albedo;
  float roughness;
  float metallic;
  float transmission;
  float ior;
};

static Material make_dielectric(glm::vec3 albedo, float ior) {
  return Material{
    .albedo = albedo,
    .roughness = 0.f,
    .metallic = 0.f,
    .transmission = 1.f,
    .ior = ior
  };
}

static Material make_lambertian(glm::vec3 albedo) {
  return Material{
    .albedo = albedo,
    .roughness = 1.f,
    .metallic = 0.f,
    .transmission = 0.f,
    .ior = 0.f
  };
}

static Material make_metallic(glm::vec3 albedo, float roughness) {
  return Material{
    .albedo = albedo,
    .roughness = roughness,
    .metallic = 1.f,
    .transmission = 0.f,
    .ior = 0.f
  };
}
