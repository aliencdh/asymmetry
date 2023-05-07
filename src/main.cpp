#include <SDL2/SDL.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr int WINDOW_SIZE = 1000;
constexpr int CELL_SIZE = 15;
constexpr int CELL_BORDER_WIDTH = 2;
constexpr int CELL_COUNT = WINDOW_SIZE / CELL_SIZE;

typedef size_t Cell;

struct Rgba {
  uint8_t r, g, b, a;

  Rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
};

template <size_t S> class StateTransitions {
  static constexpr size_t SIZE = sizeof(Cell) * std::pow(8, S);
  std::array<Cell, SIZE> cells;

public:
  StateTransitions(Cell base) { cells.fill(base); }

  template <size_t ARGS> Cell &operator[](const size_t (&neighbors)[ARGS]) {
    size_t idx = 0;
    size_t item_size = std::pow(8, S - 1);
    for (auto &state : neighbors) {
      idx += neighbors[state] * item_size;
      item_size /= 8;
    }

    return cells[idx];
  }
};

template <size_t C>
void cell_draw(Cell cell, int i, const std::array<Rgba, C> &colors,
               SDL_Renderer *renderer) {
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
  std::array<Cell, CELL_COUNT> cells({0});
  cells[0] = 1;

  StateTransitions<2> gol_dead(0);
  gol_dead[{5, 3}] = 1;

  StateTransitions<2> gol_alive(0);
  gol_alive[{6, 2}] = 1;
  gol_alive[{5, 3}] = 1;

  std::array<Rgba, 2> colors({
      Rgba(0, 0, 0, 255),
      Rgba(255, 255, 255, 255),
  });

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
    for (int i = 0; i < CELL_COUNT; i++) {
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
