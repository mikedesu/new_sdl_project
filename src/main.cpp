#include "SDL_handler.h"
#include <cstdlib>
#include <iostream>

using namespace std;

SDL_Window *create_window() {
  const int w = 320;
  const int h = 240;
  SDL_Window *window;
  window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, w, h, 0);
  if (window == NULL) {
    cout << "Could not create window: " << SDL_GetError() << endl;
    exit(1);
  }
  return window;
}

SDL_Renderer *create_renderer(SDL_Window *window) {
  SDL_Renderer *renderer;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    cout << "Could not create renderer: " << SDL_GetError() << endl;
    exit(1);
  }
  return renderer;
}

int main() {
  srand(time(nullptr));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = create_window();
  SDL_Renderer *renderer = create_renderer(window);

  SDL_Event e;
  bool quit = false;

  int r = 0;
  int g = 0;
  int b = 0;
  int a = 255;

  while (!quit) {

    // input event handling
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        cout << "Key pressed" << endl;
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
        break;
      }
    }
    // game world update

    // render
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  return 0;
}
