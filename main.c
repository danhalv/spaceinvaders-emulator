#include <SDL2/SDL.h>

#include "arcade_machine/arcade_machine.h"

#define WINDOW_WIDTH MACHINE_SCREEN_WIDTH * 3
#define WINDOW_HEIGHT MACHINE_SCREEN_HEIGHT * 3

// SDL components
static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;

static machine_t* machine;
static uint8_t app_should_run = 1;

void init_sdl_components() {
  SDL_Init(SDL_INIT_EVERYTHING);

  window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                            WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    exit(0);
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    printf("Could not create renderer: %s\n", SDL_GetError());
    exit(0);
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                              SDL_TEXTUREACCESS_STREAMING, MACHINE_SCREEN_WIDTH,
                              MACHINE_SCREEN_HEIGHT);

  if (texture == NULL) {
    printf("Could not create texture: %s\n", SDL_GetError());
  }
}

void destroy_sdl_components() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void handle_input() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        app_should_run = 0;
        break;

      case SDL_KEYDOWN:

        if (event.key.keysym.sym == SDLK_q)
          app_should_run = 0;

        if (event.key.keysym.sym == SDLK_c)
          machine->in_port1 |= (1 << 0);  // coin deposit

        if (event.key.keysym.sym == SDLK_2)
          machine->in_port1 |= (1 << 1);  // 2P start

        if (event.key.keysym.sym == SDLK_RETURN)
          machine->in_port1 |= (1 << 2);  // 1P start

        if (event.key.keysym.sym == SDLK_SPACE) {
          machine->in_port1 |= (1 << 4);  // 1P shot
          machine->in_port2 |= (1 << 4);  // 2P shot
        }

        if (event.key.keysym.sym == SDLK_LEFT) {
          machine->in_port1 |= (1 << 5);  // 1P left
          machine->in_port2 |= (1 << 5);  // 2P left
        }

        if (event.key.keysym.sym == SDLK_RIGHT) {
          machine->in_port1 |= (1 << 6);  // 1P right
          machine->in_port2 |= (1 << 6);  // 2P right
        }
        break;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_c)
          machine->in_port1 &= ~(1 << 0);  // coin deposit

        if (event.key.keysym.sym == SDLK_2)
          machine->in_port1 &= ~(1 << 1);  // 2P start

        if (event.key.keysym.sym == SDLK_RETURN)
          machine->in_port1 &= ~(1 << 2);  // 1P start

        if (event.key.keysym.sym == SDLK_SPACE) {
          machine->in_port1 &= ~(1 << 4);  // 1P shot
          machine->in_port2 &= ~(1 << 4);  // 2P shot
        }

        if (event.key.keysym.sym == SDLK_LEFT) {
          machine->in_port1 &= ~(1 << 5);  // 1P left
          machine->in_port2 &= ~(1 << 5);  // 2P left
        }

        if (event.key.keysym.sym == SDLK_RIGHT) {
          machine->in_port1 &= ~(1 << 6);  // 1P right
          machine->in_port2 &= ~(1 << 6);  // 2P right
        }

        machine->in_port1 |= (1 << 3);  // reset bit3. should always be set.
        break;
    }
  }
}

void render() {
  const uint32_t pitch = sizeof(uint8_t) * 3 * MACHINE_SCREEN_WIDTH;
  SDL_UpdateTexture(texture, NULL, &machine->screen_buffer, pitch);

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

int main() {
  init_sdl_components();

  machine = create_machine();
  machine_load_invaders(machine);

  int timer = SDL_GetTicks();
  while (app_should_run) {
    handle_input();

    if ((SDL_GetTicks() - timer) >
        (1.0f / MACHINE_FPS) * 1000) {  // executed every 17 ms (60 fps)

      timer = SDL_GetTicks();

      machine_update_state(machine);

      machine_update_screen_buffer(machine);

      render();
    }
  }

  destroy_sdl_components();
  destroy_machine(machine);
  return 0;
}
