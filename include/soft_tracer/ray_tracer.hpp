#pragma once

#include "SDL3/SDL_render.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include <thread>

#include "soft_tracer/camera.hpp"

constexpr uint8_t g_channels = 3;

class RayTracer {
public:
  RayTracer(uint32_t image_width, uint32_t image_height);
  ~RayTracer();

  void render(const Camera& camera);

  void write_image(uint8_t* dst_image, int32_t pitch);

  [[nodiscard]] bool is_frame_ready() const noexcept {
    return _frame_ready;
  }

  [[nodiscard]] constexpr uint32_t get_image_width() const noexcept {
    return _image_width;
  }
  [[nodiscard]] constexpr uint32_t get_image_height() const noexcept {
    return _image_height;
  }

  void set_pixel(uint32_t x, uint32_t y, float r, float g, float b);
  void set_pixel(uint32_t x, uint32_t y, glm::vec3 color);

private:
  uint32_t _image_width, _image_height;

  std::atomic<bool> _frame_ready;

  std::vector<float> _render_buffer;
  std::mutex _render_buffer_mutex;
};
