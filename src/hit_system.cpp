#include "soft_tracer/hit_system.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/exponential.hpp"
#include "glm/gtx/norm.hpp"
#include "soft_tracer/hit_result.hpp"
#include "soft_tracer/s_entity_manager.hpp"
#include "soft_tracer/sphere.hpp"
#include "soft_tracer/utils.hpp"

template <>
std::vector<HitResult> hit_entities_with<Sphere>(const Interval &t_interval) {
  auto &entity_manager = S_EntityManager::get_instance();
  std::vector<Sphere> spheres;
  entity_manager.each<Sphere>(
      [&](Entity e, Sphere &sphere) { spheres.push_back(sphere); });

  std::vector<HitResult> hits;
  entity_manager.each<Ray>([&](Entity e, Ray &ray) {
    HitResult hit_result{.entity = e, .ray = ray};
    for (Sphere sphere : spheres) {
      const glm::vec3 oc = ray.origin - sphere.center;
      const float a = glm::length2(ray.direction);
      const float h = dot(ray.direction, oc);
      const float c = glm::length2(oc) - sphere.radius * sphere.radius;
      const float discriminant = h * h - a * c;

      if (discriminant < 0) {
        continue;
      }

      const float sqrt_discriminant = glm::sqrt(discriminant);
      float root = (-h - sqrt_discriminant) / a;
      if (!t_interval.encloses(root)) {
        root = (-h + sqrt_discriminant) / a;
        if (!t_interval.encloses(root)) {
          continue;
        }
      }

      if (hit_result.t > root) {
        hit_result.t = root;
        hit_result.point = ray.point_at(root);
        hit_result.normal = glm::normalize(hit_result.point - sphere.center);
      }
    }

    hits.push_back(hit_result);
    if (hit_result.entity.Valid()) {
      ray.origin = hit_result.point;
      ray.direction = utils::random_on_hemisphere(hit_result.normal);
      ray.throughput *= 0.5f;
    }
  });

  return hits;
}
