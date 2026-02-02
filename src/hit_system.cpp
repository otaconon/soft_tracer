#include "soft_tracer/hit_system.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/exponential.hpp"
#include "glm/gtx/norm.hpp"
#include "soft_tracer/hit_result.hpp"
#include "soft_tracer/s_entity_manager.hpp"
#include "soft_tracer/sphere.hpp"

template <>
std::vector<HitResult> hit_entities_with<Sphere>(const Ray &ray, const Interval& t_interval) {
  auto &entity_manager = S_EntityManager::get_instance();
  std::vector<HitResult> entities_hit;

  entity_manager.each<Sphere>([&](Entity e, const Sphere &sphere) {
    const glm::vec3 oc = ray.origin - sphere.center;
    const float a = glm::length2(ray.direction);
    const float h = dot(ray.direction, oc);
    const float c = glm::length2(oc) - sphere.radius * sphere.radius;
    const float discriminant = h*h - a * c;
    if (discriminant < 0) {
      return;
    }

    const float sqrt_discriminant = glm::sqrt(discriminant);
    float root = (-h - sqrt_discriminant) / a;
    if (!t_interval.encloses(root)) {
      root = (-h + sqrt_discriminant) / a;
      if (!t_interval.encloses(root)) {
        return;
      }
    }

    HitResult hit_result;
    hit_result.t = root;
    hit_result.point = ray.point_at(root);
    hit_result.normal = glm::normalize(hit_result.point - sphere.center);
    hit_result.entity = e;
    entities_hit.push_back(hit_result);
  });

  return entities_hit;
}
