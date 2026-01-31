#include "soft_tracer/renderer.hpp"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_render.h"
#include <cassert>

Renderer::Renderer(SDL_Window *window, uint32_t width, uint32_t height)
    : renderer_{SDL_CreateRenderer(window, NULL)}, width_{width},
      height_{height}, pixels_(width * height * g_channels),
      render_tex_{SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB24,
                                    SDL_TEXTUREACCESS_STREAMING, width,
                                    height)} {
  SDL_SetRenderLogicalPresentation(renderer_, width, height,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

Renderer::~Renderer() { SDL_DestroyRenderer(renderer_); }

SDL_Renderer *Renderer::Get() { return renderer_; }

void Renderer::BeginRendering() { SDL_RenderClear(renderer_); }

void Renderer::EndRendering() {
  void *texture_pixels;
  int32_t pitch = 0;

  if (SDL_LockTexture(render_tex_, nullptr, &texture_pixels, &pitch)) {
    size_t src_row_size = width_ * g_channels;

    uint8_t *src = pixels_.data();
    uint8_t *dst = static_cast<uint8_t*>(texture_pixels);

    for (int y = 0; y < height_; ++y) {
      std::memcpy(dst, src, src_row_size);
      src += src_row_size;
      dst += pitch;
    }

    SDL_UnlockTexture(render_tex_);
  }

  SDL_RenderTexture(renderer_, render_tex_, nullptr, nullptr);
  SDL_RenderPresent(renderer_);
}

void Renderer::SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g,
                        uint8_t b) {
  uint32_t idx = (y * width_ + x) * g_channels;
  assert(idx < pixels_.size() && "Pixel index out of range");

  pixels_[idx + 0] = r;
  pixels_[idx + 1] = g;
  pixels_[idx + 2] = b;
}
