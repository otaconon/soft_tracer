#include "soft_tracer/ray_tracer.hpp"
#include <cassert>
#include <mutex>

RayTracer::RayTracer(uint32_t image_width, uint32_t image_height)
    : _image_width{image_width}, _image_height{image_height},
      _frame_ready{false},
      _render_buffer(image_width * image_height * g_channels) {
}

RayTracer::~RayTracer() {}

void RayTracer::render(const Camera &camera) {
  constexpr int samples = 8;
  constexpr float pixel_color_scale = 1.f / samples;
  for (uint32_t y = 0; y < _image_height; ++y) {
    for (uint32_t x = 0; x < _image_width; ++x) {
      glm::vec3 color(0.f);
      for (int sample = 0; sample < samples; sample++) {
        color += camera.ray_cast(x, y);
      }
      set_pixel(x, y, pixel_color_scale * color);
    }
  }

  _frame_ready = true;
}

void RayTracer::write_image(uint8_t *dst_image, int32_t pitch) {
  std::lock_guard<std::mutex> guard(_render_buffer_mutex);

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
