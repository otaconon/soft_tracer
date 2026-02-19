#include "soft_tracer/ray_interactions.hpp"

#include "glm/gtc/epsilon.hpp"

bool scatter_dielectric(
    Ray &ray, const HitResult &hit_result, const Material &material) {
  // Decide whether ray is entering or exiting from material, and calculate
  // appropriate refractive index based on that
  float refractive_index;
  glm::vec3 outward_normal;
  if (glm::dot(ray.direction, hit_result.normal) > 0) {
    refractive_index = material.ior;
    outward_normal = -hit_result.normal;
  } else {
    refractive_index = 1.f / material.ior;
    outward_normal = hit_result.normal;
  }

  const float cos_theta =
      std::fmin(dot(-glm::normalize(ray.direction), outward_normal), 1.f);
  const float sin_theta = std::sqrtf(1.f - cos_theta * cos_theta);

  // When ray cannot refract, reflect instead
  if (refractive_index * sin_theta > 1.f ||
      compute_reflectance(cos_theta, refractive_index) >
          utils::random<float>()) {
    ray.direction = ray.reflect(outward_normal);
  } else {
    ray.direction = ray.refract(outward_normal, refractive_index);
  }

  ray.attenuation *= 1.f;

  return true;
}

bool scatter_lambertian(
    Ray &ray, const HitResult &hit_result, const Material &material) {
  ray.direction = glm::normalize(hit_result.normal + utils::random_unit());
  ray.attenuation *= material.albedo;

  if (glm::all(glm::epsilonEqual(ray.direction, glm::vec3(0.0f), 1e-8f))) {
    ray.direction = hit_result.normal;
  }

  return true;
}

bool scatter_metallic(
    Ray &ray, const HitResult &hit_result, const Material &material) {
  ray.attenuation *= material.albedo;
  ray.direction = glm::normalize(ray.reflect(hit_result.normal) +
      material.roughness * utils::random_unit());
  return glm::dot(ray.direction, hit_result.normal) > 0;
}
