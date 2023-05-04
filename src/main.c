#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#define WINDOW_SIZE 1000

int main(void) {
  // init
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 2;
  }

  SDL_Window *window = SDL_CreateWindow("asymmetry", 0, 0, WINDOW_SIZE,
                                        WINDOW_SIZE, SDL_WINDOW_SHOWN);
  ;
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if (!window || !renderer) {
    fprintf(stderr, "Could not initialize window or renderer: %s\n",
            SDL_GetError());
    SDL_Quit();
    return 2;
  }

  for (;;) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

      switch (event.type) {
      case SDL_QUIT:
        goto quit;
      }
    }

    // clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

quit:
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return 0;
}
