#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_SIZE 1000
#define CELL_SIZE 15
#define CELL_BORDER_WIDTH 2
#define CELL_COUNT WINDOW_SIZE / CELL_SIZE

typedef unsigned int Cell;

typedef struct {
  unsigned char r, g, b, a;
} Rgba;

void cell_draw(Cell cell, size_t i, Rgba *colors, SDL_Renderer *renderer) {
  Rgba clr = colors[cell];
  SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, clr.a);
  SDL_Rect rect = {.x = i * CELL_SIZE + CELL_BORDER_WIDTH,
                   .y = i * CELL_SIZE + CELL_BORDER_WIDTH,
                   .w = CELL_SIZE - CELL_BORDER_WIDTH,
                   .h = CELL_SIZE - CELL_BORDER_WIDTH};
  SDL_RenderFillRect(renderer, &rect);
}

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

  // data
  Cell cells[CELL_COUNT];
  memset(cells, 0, sizeof(cells));

  Rgba colors[2];
  colors[0].r = 0;
  colors[0].g = 0;
  colors[0].b = 0;
  colors[0].a = 255;
  colors[1].r = 255;
  colors[1].g = 255;
  colors[1].b = 255;
  colors[1].a = 255;

  cells[0] = 1;

  // main loop
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

    // draw cells
    for (size_t i = 0; i < CELL_COUNT; i++) {
      cell_draw(cells[i], i, colors, renderer);
    }

    SDL_RenderPresent(renderer);
  }

quit:
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return 0;
}
