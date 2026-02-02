#include "SDL3/SDL_init.h"
#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "soft_tracer/ray_trace.hpp"
#include "soft_tracer/s_entity_manager.hpp"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glm/glm.hpp>
#include <memory_resource>
#include <vector>

#include <soft_tracer/hit_system.hpp>
#include <soft_tracer/ray.hpp>
#include <soft_tracer/renderer.hpp>
#include <soft_tracer/sphere.hpp>
#include <soft_tracer/tracer.hpp>
#include <soft_tracer/camera.hpp>

struct AppState {
  int32_t width;
  int32_t height;
  SDL_Window *window;
  Renderer renderer;
};

SDL_AppResult SDL_AppInit(void **app_state, int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  constexpr float aspect_ratio = 16.f / 9.f;
  constexpr int image_width = 900;
  constexpr int image_height = static_cast<int>(image_width / aspect_ratio);

  Camera camera(image_width, image_height);

  SDL_Window *window;
  if (!(window =
            SDL_CreateWindow("soft tracer", image_width, image_height, 0))) {
    SDL_Log("Couldnt create SDL window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *app =
      new AppState{.width = image_width,
                   .height = image_height,
                   .window = window,
                   .renderer = Renderer(window, image_width, image_height)};
  *app_state = app;

  auto &entity_manager = S_EntityManager::get_instance();
  for (int i = 0; i <= 4; i++) {
    Entity sphere = entity_manager.create_entity();
    entity_manager.add_components(sphere, Sphere{{i, 0, -1}, 0.2f});
  }

  constexpr int samples = 8;
  constexpr float pixel_color_scale = 1.f / samples;
  for (uint32_t y = 0; y < image_height; ++y) {
    for (uint32_t x = 0; x < image_width; ++x) {
      glm::vec3 color(0.f);
      for (int sample = 0; sample < samples; sample++) {
        auto entities_hit = camera.ray_cast(x, y);
        color += ray_trace(entities_hit);
      }
      app->renderer.set_pixel(x, y, pixel_color_scale * color);
    }
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *app_state) {
  Renderer *renderer = &(static_cast<AppState*>(app_state))->renderer;
  renderer->begin_rendering();
  renderer->end_rendering();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
