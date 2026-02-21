
#include "soft_tracer/app.hpp"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

#include <soft_tracer/ecs/s_entity_manager.hpp>
#include <soft_tracer/material.hpp>

#include <soft_tracer/camera.hpp>
#include <soft_tracer/ray_tracer.hpp>
#include <soft_tracer/sphere.hpp>

SDL_AppResult SDL_AppInit(void** app_state, int argc, char* argv[]) {
  constexpr float aspect_ratio = 16.f / 9.f;
  constexpr uint32_t render_width = 900;
  constexpr uint32_t render_height = render_width / aspect_ratio;

  auto& entity_manager = S_EntityManager::get_instance();
  entity_manager.add_components(entity_manager.create_entity(),
      Sphere{ { -1, 0, -1 }, 0.5f }, make_metallic({ 0.4f, 0.2f, 0.5f }, 0.3f));
  entity_manager.add_components(entity_manager.create_entity(),
      Sphere{ { 0, 0, -1 }, 0.5f }, make_lambertian({ 0.2f, 0.5f, 0.4f }));
  entity_manager.add_components(entity_manager.create_entity(),
      Sphere{ { 1, 0, -1 }, 0.5f },
      make_dielectric({ 0.f, 0.f, 0.f }, 1.f / 1.33f));
  entity_manager.add_components(entity_manager.create_entity(),
      Sphere{ { 0, -100.5f, -1.f }, 100.f },
      make_lambertian({ 0.1f, 0.1f, 0.1f }));

  *app_state = new App{render_width, render_height};

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* app_state, SDL_Event* event) {
  const auto app = static_cast<App*>(app_state);
  return app->HandleEvent(event);
}

SDL_AppResult SDL_AppIterate(void* app_state) {
  const auto app = static_cast<App*>(app_state);
  return app->HandleIterate();
}

void SDL_AppQuit(void* app_state, SDL_AppResult result) {
  const auto app = static_cast<App*>(app_state);
  delete app;
}
