
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_init.h>

#include <soft_tracer/material.hpp>
#include <soft_tracer/ecs/s_entity_manager.hpp>

#include <soft_tracer/camera.hpp>
#include <soft_tracer/ray_tracer.hpp>
#include <soft_tracer/sphere.hpp>

#include <thread>

struct AppState {
  int32_t width;
  int32_t height;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *display_texture;
  RayTracer ray_tracer;
  Camera camera;
  std::jthread ray_trace_thread;
};

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

  SDL_Renderer *renderer;
  if (!(renderer = SDL_CreateRenderer(window, nullptr))) {
    SDL_Log("Couldnt create SDL renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  SDL_SetRenderLogicalPresentation(renderer, image_width, image_height,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  SDL_Texture *texture;
  if (!(texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB96_FLOAT,
                                    SDL_TEXTUREACCESS_STREAMING, image_width,
                                    image_height))) {
    SDL_Log("Couldnt create SDL texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *app = new AppState{.width = image_width,
                               .height = image_height,
                               .window = window,
                               .renderer = renderer,
                               .display_texture = texture,
                               .ray_tracer{image_width, image_height},
                               .camera{image_width, image_height}};
  app->ray_trace_thread =
      std::jthread(&RayTracer::render, &app->ray_tracer, std::ref(app->camera));
  *app_state = app;

  auto &entity_manager = S_EntityManager::get_instance();
  entity_manager.add_components(entity_manager.create_entity(),
                                Sphere{{-1, 0, -1}, 0.5f},
                                make_metallic({0.4f, 0.2f, 0.5f}, 0.3f));
  entity_manager.add_components(entity_manager.create_entity(),
                                Sphere{{0, 0, -1}, 0.5f},
                                make_lambertian({0.2f, 0.5f, 0.4f}));
  entity_manager.add_components(entity_manager.create_entity(),
                                Sphere{{1, 0, -1}, 0.5f},
                                make_dielectric({0.f, 0.f, 0.f}, 1.5f));
  entity_manager.add_components(entity_manager.create_entity(),
                                Sphere{{0, -100.5f, -1.f}, 100.f},
                                make_lambertian({0.1f, 0.1f, 0.1f}));

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *app_state) {
  AppState *app = static_cast<AppState *>(app_state);
  RayTracer *ray_tracer = &(static_cast<AppState *>(app_state))->ray_tracer;

  void *texture_pixels;
  int32_t pitch = 0;

  if (SDL_LockTexture(app->display_texture, nullptr, &texture_pixels, &pitch)) {
    ray_tracer->write_image(static_cast<uint8_t *>(texture_pixels), pitch);
    SDL_UnlockTexture(app->display_texture);
  }

  SDL_RenderClear(app->renderer);
  SDL_RenderTexture(app->renderer, app->display_texture, nullptr, nullptr);
  SDL_RenderPresent(app->renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *app_state, SDL_AppResult result) {
  AppState *app = static_cast<AppState *>(app_state);
  app->ray_trace_thread.request_stop();
  SDL_DestroyRenderer(app->renderer);
}
