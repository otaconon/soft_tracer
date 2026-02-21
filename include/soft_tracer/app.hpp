#pragma once

#include "soft_tracer/ray_tracer.hpp"
#include "soft_tracer/window.hpp"
#include <thread>

class App {
public:
  App(uint32_t render_width, uint32_t render_height);
  ~App();

  SDL_AppResult HandleEvent(const SDL_Event* event);
  SDL_AppResult HandleIterate();
private:
  Window _window{};
  RayTracer _ray_tracer;
  Camera _camera;
  std::jthread _ray_tracing_thread;
};
