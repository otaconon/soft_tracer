#include "SDL3/SDL_init.h"
#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
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

struct AppState {
  int32_t width;
  int32_t height;
  SDL_Window *window;
  Renderer renderer;
};

glm::vec3 color(const Ray &ray) {
  auto entities_hit = hit_entities_with<Sphere>(ray, {-1000.f, 1000.f});
  std::ranges::sort(entities_hit, {}, &HitResult::t);
  for (const auto &hit : entities_hit) {
    return 0.5f * (hit.normal + 1.f);
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

  constexpr float aspect_ratio = 16.f / 9.f;
  constexpr int image_width = 900;
  constexpr int image_height = static_cast<int>(image_width / aspect_ratio);

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

  constexpr float focal_length = 1.0;
  constexpr float viewport_height = 2.0;
  constexpr float viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
  constexpr glm::vec3 camera_center = glm::vec3(0, 0, 0);

  constexpr glm::vec3 viewport_u = glm::vec3(viewport_width, 0, 0);
  constexpr glm::vec3 viewport_v = glm::vec3(0, -viewport_height, 0);

  constexpr glm::vec3 pixel_delta_u = viewport_u / static_cast<float>(image_width);
  constexpr glm::vec3 pixel_delta_v = viewport_v / static_cast<float>(image_height);

  constexpr glm::vec3 viewport_upper_left = camera_center -
                                  glm::vec3(0, 0, focal_length) -
                                  viewport_u / 2.f - viewport_v / 2.f;

  constexpr glm::vec3 origin =
      viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

  auto &entity_manager = S_EntityManager::get_instance();
  for (int i = 0; i < 3; i++) {
    Entity sphere = entity_manager.create_entity();
    entity_manager.add_components(sphere, Sphere{{i, 0, -1 - i}, 0.3f});
  }

  for (int y = 0; y < image_height; ++y) {
    for (int x = 0; x < image_width; ++x) {
      glm::vec3 pixel_center = origin +
                               static_cast<float>(x) * pixel_delta_u +
                               static_cast<float>(y) * pixel_delta_v;
      glm::vec3 ray_direction = pixel_center - camera_center;
      Ray ray(camera_center, ray_direction);
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
  Renderer *renderer = &(static_cast<AppState*>(app_state))->renderer;
  renderer->begin_rendering();
  renderer->end_rendering();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
