#include "soft_tracer/app.hpp"
#include "soft_tracer/utils.hpp"

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

  constexpr int N = 40;
  for (int i = 0; i < N; i++) {
    const float radius = utils::random<float>(0.05f, 0.2f);

    const glm::vec3 position{ utils::random<float>(-5.0f, 5.0f), radius,
      utils::random<float>(-7.0f, 1.f) };

    Entity e = entity_manager.create_entity();
    Sphere sphere{ position, radius };

    const int material_choice = utils::random<int>(0, 2);
    Material material;
    if (material_choice == 0) {
      material =
          make_metallic(utils::random(glm::vec3{0.0f}, glm::vec3{0.9f}), utils::random<float>(0.f, 1.f));
    } else if (material_choice == 1) {
      material =
          make_dielectric(utils::random(glm::vec3{0.0f}, glm::vec3{0.9f}), utils::random<float>(0.f, 1.f));
    } else {
      material = make_lambertian(utils::random(glm::vec3{0.0f}, glm::vec3{0.9f}));
    }
    entity_manager.add_components(e, std::move(sphere), std::move(material));
  }

  entity_manager.add_components(entity_manager.create_entity(),
      Sphere{ { 0.0f, -1000.0f, -1.0f }, 1000.f },
      make_lambertian({ 0.1f, 0.1f, 0.1f }));

  constexpr float aspect_ratio = 16.f / 9.f;
  constexpr uint32_t render_width = 1920;
  constexpr uint32_t render_height = render_width / aspect_ratio;
  *appstate = new App{ render_width, render_height };

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
