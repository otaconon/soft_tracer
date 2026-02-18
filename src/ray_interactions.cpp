#include "soft_tracer/ray_interactions.hpp"

#include "glm/gtc/epsilon.hpp"

bool scatter_dielectric(Ray &ray, HitResult &hit_result, Material &material) {
  float refractive_index;
  glm::vec3 outward_normal;
  if (glm::dot(ray.direction, hit_result.normal) > 0) {
    refractive_index = material.ior;
    outward_normal = -hit_result.normal;
  } else {
    refractive_index = 1.f / material.ior;
    outward_normal = hit_result.normal;
  }

  ray.direction = ray.refract(outward_normal, refractive_index);
  ray.attenuation = {1.f, 1.f, 1.f};

  return true;
}

bool scatter_lambertian(Ray &ray, HitResult &hit_result, Material &material) {
  ray.direction = glm::normalize(hit_result.normal + utils::random_unit());
  ray.attenuation *= material.albedo;

  if (glm::all(glm::epsilonEqual(ray.direction, glm::vec3(0.0f), 1e-8f))) {
    ray.direction = hit_result.normal;
  }

  return true;
}

bool scatter_metallic(Ray &ray, HitResult &hit_result, Material &material) {
  ray.attenuation *= material.albedo;
  ray.direction = glm::normalize(ray.reflect(hit_result.normal) +
                  (material.roughness * utils::random_unit()));
  return glm::dot(ray.direction, hit_result.normal) > 0;
}
