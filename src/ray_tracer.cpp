#include "soft_tracer/ray_tracer.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/epsilon.hpp"
#include "soft_tracer/dielectric_material.hpp"
#include "soft_tracer/hit_system.hpp"
#include "soft_tracer/lambertian_material.hpp"
#include "soft_tracer/metal_material.hpp"
#include "soft_tracer/s_entity_manager.hpp"
#include "soft_tracer/sphere.hpp"
#include "soft_tracer/utils.hpp"
#include <cassert>
#include <limits>

RayTracer::RayTracer(uint32_t image_width, uint32_t image_height)
    : _image_width{image_width}, _image_height{image_height},
      _frame_ready{false},
      _render_buffer(image_width * image_height * g_channels) {}

RayTracer::~RayTracer() {}

void RayTracer::render(const Camera &camera) {
  _tiles.clear();
  uint32_t TILE_SIZE = 32;
  for (uint32_t y = 0; y < _image_height; y += TILE_SIZE) {
    for (uint32_t x = 0; x < _image_width; x += TILE_SIZE) {
      uint32_t w = std::min(TILE_SIZE, _image_width - x);
      uint32_t h = std::min(TILE_SIZE, _image_height - y);
      _tiles.push_back({x, y, w, h});
    }
  }

  _next_tile_idx = 0;

  unsigned int core_count = std::thread::hardware_concurrency()/2;
  std::vector<std::thread> workers;

  for (unsigned int i = 0; i < core_count; ++i) {
    workers.emplace_back(&RayTracer::render_thread_worker, this,
                         std::ref(camera));
  }

  for (auto &t : workers) {
    t.join();
  }
}

void RayTracer::render_thread_worker(const Camera &camera) {
  for (size_t tile_idx = _next_tile_idx.fetch_add(1); tile_idx < _tiles.size();
       tile_idx = _next_tile_idx.fetch_add(1)) {
    Tile t = _tiles[tile_idx];

    for (size_t y = t.y; y < t.y + t.h; ++y) {
      for (size_t x = t.x; x < t.x + t.w; ++x) {
        for (int sample = 0; sample < _samples; ++sample) {
          Ray ray = camera.ray_cast(x, y);
          trace_ray(ray);
        }
      }
    }
  }
}

void RayTracer::trace_ray(Ray &ray) {
  EntityManager &entity_manager = S_EntityManager::get_instance();
  for (uint32_t step = 0; step < _steps; ++step) {
    HitResult hit_result = hit_entities_with<Sphere>(
        ray, {0.001f, std::numeric_limits<float>::infinity()});

    const float color_per_sample = 1.f / _samples;
    if (hit_result.t != std::numeric_limits<float>::infinity()) {
      if (!scatter_ray(ray, hit_result)) {
        return;
      }
    } else {
      glm::vec3 unit_direction = glm::normalize(ray.direction);
      float a = 0.5 * (unit_direction.y + 1.0);
      glm::vec3 color =
          (1.0f - a) * glm::vec3{1.0, 1.0, 1.0} + a * glm::vec3{0.5, 0.7, 1.0};

      add_to_pixel(ray.image_x, ray.image_y,
                   ray.attenuation * color_per_sample * color);
      return;
    }
  }
}

bool RayTracer::scatter_ray(Ray &ray, HitResult &hit_result) {
  EntityManager &entity_manager = S_EntityManager::get_instance();
  ray.origin = hit_result.point;

  if (auto material = entity_manager.get_component<LambertianMaterial>(
          hit_result.entity_hit)) {
    ray.direction = glm::normalize(hit_result.normal + utils::random_unit());
    ray.attenuation *= material->albedo;
  } else if (auto material = entity_manager.get_component<MetalMaterial>(
                 hit_result.entity_hit)) {
    ray.direction = ray.reflect(hit_result.normal);
    ray.direction =
        glm::normalize(ray.direction) + (material->fuzz * utils::random_unit());
    ray.attenuation *= material->albedo;
    if (!(glm::dot(ray.direction, hit_result.normal) > 0)) {
      return false;
    }
  } else if (auto material = entity_manager.get_component<DielectricMaterial>(
                 hit_result.entity_hit)) {
    float refractive_index;
    glm::vec3 outward_normal;
    if (glm::dot(ray.direction, hit_result.normal) > 0) {
      refractive_index = material->refractive_index;
      outward_normal = -hit_result.normal;
    } else {
      refractive_index = 1.f / material->refractive_index;
      outward_normal = -hit_result.normal;
    }

    ray.direction = ray.refract(outward_normal, refractive_index);
    ray.attenuation = {1.f, 1.f, 1.f};
  }

  if (glm::all(glm::epsilonEqual(ray.direction, glm::vec3(0.0f), 1e-8f))) {
    ray.direction = hit_result.normal;
  }

  return true;
}

void RayTracer::write_image(uint8_t *dst_image, int32_t pitch) {
  const size_t src_row_elements = _image_width * g_channels;
  const size_t src_row_size = src_row_elements * sizeof(float);
  const float *src = _render_buffer.data();

  for (int y = 0; y < _image_height; ++y) {
    std::memcpy(dst_image, src, src_row_size);
    src += src_row_elements;
    dst_image += pitch;
  }
}

void RayTracer::set_pixel(uint32_t x, uint32_t y, float r, float g, float b) {
  const uint32_t idx = (y * _image_width + x) * g_channels;
  assert(idx < _render_buffer.size() && "Pixel index out of range");

  _render_buffer[idx + 0] = r;
  _render_buffer[idx + 1] = g;
  _render_buffer[idx + 2] = b;
}

void RayTracer::set_pixel(uint32_t x, uint32_t y, glm::vec3 color) {
  const size_t idx = (y * _image_width + x) * g_channels;
  assert(idx < _render_buffer.size() && "Pixel index out of range");

  _render_buffer[idx + 0] = glm::clamp(color.r, 0.f, 1.f);
  _render_buffer[idx + 1] = glm::clamp(color.g, 0.f, 1.f);
  _render_buffer[idx + 2] = glm::clamp(color.b, 0.f, 1.f);
}

void RayTracer::add_to_pixel(uint32_t x, uint32_t y, glm::vec3 color) {
  const size_t idx = (y * _image_width + x) * g_channels;
  assert(idx < _render_buffer.size() && "Pixel index out of range");

  _render_buffer[idx + 0] += glm::clamp(color.r, 0.f, 1.f);
  _render_buffer[idx + 1] += glm::clamp(color.g, 0.f, 1.f);
  _render_buffer[idx + 2] += glm::clamp(color.b, 0.f, 1.f);
}
