#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <math.h>
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

typedef struct {
  size_t state_count;
  Cell *case_defs;
} StateTransitions;

StateTransitions state_transitions_new(size_t state_count, Cell base) {
  StateTransitions rv = {
      .state_count = state_count,
      .case_defs = (Cell *)malloc(sizeof(Cell) * SDL_pow(8, state_count)),
  };
  memset(rv.case_defs, base, sizeof(Cell) * SDL_pow(8, state_count));

  return rv;
}

void state_transitions_free_defs(StateTransitions *self) {
  for (size_t i = 0; i < SDL_pow(8, self->state_count); i++)
    free(self->case_defs + i);
}

size_t state_transitions_index(StateTransitions *self, size_t *neighbors) {
  size_t idx = 0;
  size_t item_size = SDL_pow(8, self->state_count - 1);
  for (Cell state = 0; state < self->state_count; state++) {
    idx += neighbors[state] * item_size;
    item_size /= 8;
  }

  return idx;
}

void state_transitions_add_def(StateTransitions *self, size_t *neighbors,
                               Cell result) {
  size_t idx = state_transitions_index(self, neighbors);
  self->case_defs[idx] = result;
}

Cell state_transitions_get(StateTransitions *self, size_t *neighbors) {
  return self->case_defs[state_transitions_index(self, neighbors)];
}

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

  StateTransitions gol_dead = state_transitions_new(2, 0);
  size_t neighbors[] = {5, 3};
  state_transitions_add_def(&gol_dead, neighbors, 1);

  StateTransitions gol_alive = state_transitions_new(2, 0);
  neighbors[0] = 6;
  neighbors[1] = 2;
  state_transitions_add_def(&gol_alive, neighbors, 1);
  neighbors[0] = 5;
  neighbors[1] = 3;
  state_transitions_add_def(&gol_alive, neighbors, 1);

  neighbors[0] = 6;
  neighbors[1] = 2;

  printf("%d\n", state_transitions_get(&gol_dead, neighbors));

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
  state_transitions_free_defs(&gol_dead);
  state_transitions_free_defs(&gol_alive);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return 0;
}
