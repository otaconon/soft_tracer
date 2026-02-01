#pragma once

#include "SDL3/SDL_render.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

constexpr uint8_t g_channels = 3;

class Renderer {
public:
  Renderer(SDL_Window *window, uint32_t width, uint32_t height);
  ~Renderer();

  SDL_Renderer *get() const;

  void begin_rendering() const;
  void end_rendering() const;

  void set_pixel(uint32_t x, uint32_t y, float r, float g, float b);
  void set_pixel(uint32_t x, uint32_t y, glm::vec3 color);

private:
  SDL_Renderer *_renderer;
  SDL_Texture *_renderer_texture;
  uint32_t _width, _height;
  std::vector<float> _pixels;
};
