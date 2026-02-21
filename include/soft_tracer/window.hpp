#pragma once

#include <SDL3/SDL.h>
#include <vector>

class Window {
public:
  SDL_AppResult init(uint32_t width, uint32_t height);
  SDL_AppResult render(const std::vector<float>* pixel_buffer, int32_t pitch)const;
  void shutdown();

private:
  SDL_Window* _window{};
  SDL_Renderer* _renderer{};
  SDL_Texture* _display_texture{};
};
