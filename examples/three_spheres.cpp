#include "soft_tracer/app.hpp"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

#include <soft_tracer/ecs/s_entity_manager.hpp>
#include <soft_tracer/material.hpp>

#include <soft_tracer/sphere.hpp>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
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

  constexpr float aspect_ratio = 16.f / 9.f;
  constexpr uint32_t render_width = 900;
  constexpr uint32_t render_height = render_width / aspect_ratio;
  *appstate = new App{render_width, render_height};

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  const auto app = static_cast<App*>(appstate);
  return app->HandleEvent(event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  const auto app = static_cast<App*>(appstate);
  return app->HandleIterate();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  const auto app = static_cast<App*>(appstate);
  delete app;
}
