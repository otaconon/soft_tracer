#include "SDL3/SDL_init.h"
#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "soft_tracer/s_entity_manager.hpp"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glm/glm.hpp>
#include <vector>
#include <memory_resource>

#include <soft_tracer/ray.hpp>
#include <soft_tracer/renderer.hpp>
#include <soft_tracer/tracer.hpp>
#include <soft_tracer/hit_system.hpp>
#include <soft_tracer/sphere.hpp>

struct AppState {
  int32_t width;
  int32_t height;
  SDL_Window *window;
  Renderer renderer;
};

char buffer[1024];
std::pmr::monotonic_buffer_resource pool{buffer, sizeof(buffer)};

glm::vec3 color(const Ray &ray) {
  auto entities_hit = hit_entities_with<Sphere>(ray, -1000.f, 1000.f);
  if (entities_hit.size() > 0) {
    glm::vec3 normal = glm::normalize(entities_hit[0].point - glm::vec3{0, 0, -1});
    return 0.5f * (normal + 1.f);
  }

  const glm::vec3 unit_dir = glm::normalize(ray.direction_);
  const float t = 0.5f * (unit_dir.y + 1.0);
  return (1.0f - t) * glm::vec3(0.f) + t * glm::vec3(0.5, 0.7, 1.0);
}

SDL_AppResult SDL_AppInit(void **app_state, int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  int32_t width = 600;
  int32_t height = 600;

  SDL_Window *window;
  if (!(window = SDL_CreateWindow("soft tracer", width, height, 0))) {
    SDL_Log("Couldnt create SDL window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *app = new AppState{.width = width,
                               .height = height,
                               .window = window,
                               .renderer = Renderer(window, width, height)};
  *app_state = app;

  glm::vec3 lower_left_corner{-1.f, -1.f, -1.f};
  glm::vec3 horizontal{2.f, 0.f, 0.f};
  glm::vec3 vertical{0.f, 2.f, 0.f};
  glm::vec3 origin{0.f, 0.f, 0.f};

  auto& entity_manager = S_EntityManager::get_instance();
  Entity sphere = entity_manager.create_entity();
  entity_manager.add_components(sphere, Sphere{{0, 0, -1}, 0.5f});

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float u = static_cast<float>(x) / width;
      float v = static_cast<float>(y) / height;
      Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
      glm::vec3 col = color(ray);
      app->renderer.set_pixel(x, y, col);
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
  Renderer *renderer = &((AppState *)app_state)->renderer;
  renderer->begin_rendering();
  renderer->end_rendering();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
