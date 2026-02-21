#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <vector>

#include "camera.hpp"
#include "hit_result.hpp"

struct Tile {
  size_t x, y, w, h;
};

constexpr uint8_t g_channels = 4;

class RayTracer {
public:
  RayTracer(uint32_t image_width, uint32_t image_height);
  ~RayTracer();

  void render(const Camera& camera);
  void render_thread_worker(const Camera& camera);
  void trace_ray(Ray& ray);
  static bool scatter_ray(Ray& ray, const HitResult& hit_result);

  void write_image(uint8_t* dst_image, int32_t pitch) const;

  [[nodiscard]] bool is_frame_ready() const noexcept {
    return _frame_ready;
  }

  std::vector<float>* get_render_buffer() noexcept {
    return &_render_buffer;
  }

  [[nodiscard]] constexpr uint32_t get_image_width() const noexcept {
    return _image_width;
  }
  [[nodiscard]] constexpr uint32_t get_image_height() const noexcept {
    return _image_height;
  }

  void set_pixel(uint32_t x, uint32_t y, glm::vec3 color);
  void add_to_pixel(uint32_t x, uint32_t y, glm::vec3 color);

private:
  uint32_t _image_width, _image_height;
  uint32_t _samples{1024};
  uint32_t _steps{10};

  std::atomic<bool> _frame_ready;

  std::vector<Tile> _tiles;
  std::atomic<size_t> _next_tile_idx{0};

  std::vector<float> _render_buffer;
};
