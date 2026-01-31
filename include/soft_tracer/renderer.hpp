#include "SDL3/SDL_render.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <vector>

constexpr uint8_t g_channels = 3;

class Renderer {
public:
  Renderer(SDL_Window* window, uint32_t width, uint32_t height);
  ~Renderer();

  SDL_Renderer* Get();

  void BeginRendering();
  void EndRendering();

  void SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

private:
  SDL_Renderer *renderer_;
  SDL_Texture *render_tex_;
  uint32_t width_, height_;
  std::vector<uint8_t> pixels_;
};
