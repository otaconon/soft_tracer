#include "soft_tracer/renderer.hpp"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include <cassert>

Renderer::Renderer(SDL_Window *window, uint32_t image_width,
                   uint32_t image_height)
    : _renderer{SDL_CreateRenderer(window, nullptr)},
      _renderer_texture{SDL_CreateTexture(
          _renderer, SDL_PIXELFORMAT_RGB96_FLOAT, SDL_TEXTUREACCESS_STREAMING,
          image_width, image_height)},
      _image_width{image_width}, _image_height{image_height},
      _pixels(image_width * image_height * g_channels) {
  SDL_SetRenderLogicalPresentation(_renderer, image_width, image_height,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);


}

Renderer::~Renderer() { SDL_DestroyRenderer(_renderer); }

void Renderer::begin_rendering() const { SDL_RenderClear(_renderer); }

void Renderer::end_rendering() const {
  void *texture_pixels;
  int32_t pitch = 0;

  if (SDL_LockTexture(_renderer_texture, nullptr, &texture_pixels, &pitch)) {
    const size_t src_row_elements = _image_width * g_channels;
    const size_t src_row_size = src_row_elements * sizeof(float);

    const float *src = _pixels.data();
    auto dst = static_cast<uint8_t *>(texture_pixels);

    for (int y = 0; y < _image_height; ++y) {
      std::memcpy(dst, src, src_row_size);
      src += src_row_elements;
      dst += pitch;
    }

    SDL_UnlockTexture(_renderer_texture);
  }

  SDL_RenderTexture(_renderer, _renderer_texture, nullptr, nullptr);
  SDL_RenderPresent(_renderer);
}

void Renderer::set_pixel(uint32_t x, uint32_t y, float r, float g, float b) {
  const uint32_t idx = (y * _image_width + x) * g_channels;
  assert(idx < _pixels.size() && "Pixel index out of range");

  _pixels[idx + 0] = r;
  _pixels[idx + 1] = g;
  _pixels[idx + 2] = b;
}

void Renderer::set_pixel(uint32_t x, uint32_t y, glm::vec3 color) {
  const size_t idx = (y * _image_width + x) * g_channels;
  assert(idx < _pixels.size() && "Pixel index out of range");

  _pixels[idx + 0] = glm::clamp(color.r, 0.f, 1.f);
  _pixels[idx + 1] = glm::clamp(color.g, 0.f, 1.f);
  _pixels[idx + 2] = glm::clamp(color.b, 0.f, 1.f);
}
