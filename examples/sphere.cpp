#include "SDL3/SDL_init.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <soft_tracer/renderer.hpp>
#include <soft_tracer/tracer.hpp>

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

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      float r = static_cast<float>(i) / width;
      float g = static_cast<float>(j) / height;
      float b = 0.4;
      app->renderer.SetPixel(i, j, 255.99 * r, 255.99 * g, 255.99 * b);
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
  renderer->BeginRendering();
  renderer->EndRendering();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
