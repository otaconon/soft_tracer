#include "soft_tracer/window.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

SDL_AppResult Window::init(const uint32_t width, const uint32_t height) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!((_window = SDL_CreateWindow("soft tracer", width, height, 0)))) {
    SDL_Log("Couldnt create SDL window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!((_renderer = SDL_CreateRenderer(_window, nullptr)))) {
    SDL_Log("Couldnt create SDL renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(_renderer, 1);
  SDL_SetRenderLogicalPresentation(
      _renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  if (!((_display_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA128_FLOAT,
            SDL_TEXTUREACCESS_STREAMING, width, height)))) {
    SDL_Log("Couldnt create SDL texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult Window::render(
    const std::vector<float>* pixel_buffer, const int32_t pitch) const {
  SDL_UpdateTexture(_display_texture, nullptr, pixel_buffer->data(), pitch);

  SDL_RenderClear(_renderer);
  SDL_RenderTexture(_renderer, _display_texture, nullptr, nullptr);
  SDL_RenderPresent(_renderer);

  return SDL_APP_CONTINUE;
}

void Window::shutdown() const {
  if (_renderer) {
    SDL_DestroyRenderer(_renderer);
  }
  if (_window) {
    SDL_DestroyWindow(_window);
  }
}
