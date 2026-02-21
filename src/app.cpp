#include "soft_tracer/app.hpp"
#include "SDL3/SDL_init.h"

App::App(const uint32_t render_width, const uint32_t render_height) :
    _camera{ render_width, render_height },
    _ray_tracer{ render_width, render_height } {
  _camera.set_position(glm::vec3{ 1.f });
  _camera.look_at(glm::vec3{ 0.f });
  _ray_tracing_thread =
      std::jthread(&RayTracer::render, &_ray_tracer, std::ref(_camera));

  _window.init(render_width, render_height);
}

App::~App() {
  if (_ray_tracing_thread.joinable()) {
    _ray_tracing_thread.request_stop();
  }

  _window.shutdown();
}

SDL_AppResult App::HandleEvent(const SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult App::HandleIterate() {
  _window.render(_ray_tracer.get_render_buffer(), _ray_tracer.get_image_width() * 16);
  return SDL_APP_CONTINUE;
}
