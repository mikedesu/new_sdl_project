#include "SDL_handler.h"
#include "mPrint.h"
#include <cstdlib>
#include <iostream>
// #include <vector>
//  #include <pair>

using namespace std;

static int game_start = 0;

SDL_Window *create_window() {
  // const int w = 1280;
  // const int h = 960;
  const int w = 320;
  const int h = 240;
  //  const int w = 400;
  //  const int h = 800;
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
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    cout << "Could not create renderer: " << SDL_GetError() << endl;
    exit(1);
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  return renderer;
}

SDL_Texture *create_target_texture(SDL_Renderer *renderer, int w, int h) {
  SDL_Texture *texture;
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_TARGET, w, h);
  if (texture == NULL) {
    cout << "Could not create texture: " << SDL_GetError() << endl;
    exit(1);
  }
  return texture;
}

#define DEBUG_TEXT_WRAP_LEN 320

int main() {
  srand(time(nullptr));
  SDL_Init(SDL_INIT_VIDEO);
  game_start = SDL_GetTicks64();
  TTF_Init();
  SDL_Window *window = create_window();
  SDL_Renderer *renderer = create_renderer(window);
  SDL_Texture *target_texture = create_target_texture(renderer, 320, 240);
  SDL_Surface *text_surface = nullptr;
  SDL_Texture *text_texture = nullptr;
  SDL_Color text_color = {255, 255, 255, 255};
  TTF_Font *gFont = TTF_OpenFont("ttf/hack.ttf", 20);
  char display_text[320] = "Hello, World!";
  SDL_Event e;
  bool quit = false;
  bool grid[240][320] = {{false}};
  long frame_count = 0;
  bool mouse_down = false;
  SDL_Rect text_rect = {0, 0, 320, 240};

  unsigned long milliseconds;
  unsigned long seconds;
  unsigned long minutes;

  milliseconds = SDL_GetTicks64() - game_start;
  seconds = milliseconds / 1000;
  minutes = seconds / 60;
  seconds = seconds % 60;
  snprintf(display_text, 320, "%02lu:%02lu", minutes, seconds);
  text_surface = TTF_RenderText_Blended_Wrapped(gFont, display_text, text_color,
                                                DEBUG_TEXT_WRAP_LEN);
  if (text_surface == nullptr) {
    cout << "Could not create text surface: " << TTF_GetError() << endl;
    exit(1);
  }
  text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  if (text_texture == nullptr) {
    cout << "Could not create text texture: " << SDL_GetError() << endl;
    exit(1);
  }
  text_rect.w = text_surface->w;
  text_rect.h = text_surface->h;
  SDL_FreeSurface(text_surface);

  while (!quit) {
    // input event handling
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN: {
        if (e.key.keysym.sym == SDLK_q) {
          mPrint("q pressed");
          quit = true;
        }
      } break;
      case SDL_MOUSEBUTTONDOWN: {
        mouse_down = true;
      } break;
      case SDL_MOUSEBUTTONUP: {
        mouse_down = false;
      } break;
      default:
        break;
      }
    }

    if (quit) {
      mPrint("Quitting...");
      break;
    }

    if (mouse_down) {
      int x, y;
      SDL_GetMouseState(&x, &y);
      // x = x / 4;
      // y = y / 4;
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          grid[y + i][x + j] = true;
        }
      }
    }

    if (frame_count % 60 == 0) {
      int x = 160;
      int y = 0;
      grid[y][x] = true;
      grid[y][x + 1] = true;
      grid[y + 1][x] = true;
      grid[y + 1][x + 1] = true;
    }

    for (int i = 239; i > 0; i--) {
      for (int j = 0; j < 320; j++) {
        if (grid[i][j]) {
          if (!grid[i + 1][j]) {
            grid[i][j] = false;
            grid[i + 1][j] = true;
          } else if (!grid[i + 1][j + 1]) {
            grid[i][j] = false;
            grid[i + 1][j + 1] = true;
          } else if (!grid[i + 1][j - 1]) {
            grid[i][j] = false;
            grid[i + 1][j - 1] = true;
          }
        }
      }
    }

    milliseconds = SDL_GetTicks64() - game_start;
    seconds = milliseconds / 1000;
    minutes = seconds / 60;
    seconds = seconds % 60;
    snprintf(display_text, 320, "%02lu:%02lu", minutes, seconds);
    text_surface = TTF_RenderText_Blended_Wrapped(
        gFont, display_text, text_color, DEBUG_TEXT_WRAP_LEN);
    if (text_surface == nullptr) {
      cout << "Could not create text surface: " << TTF_GetError() << endl;
      exit(1);
    }
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (text_texture == nullptr) {
      cout << "Could not create text texture: " << SDL_GetError() << endl;
      exit(1);
    }
    text_rect.w = text_surface->w;
    text_rect.h = text_surface->h;
    SDL_FreeSurface(text_surface);

    // render
    SDL_SetRenderTarget(renderer, target_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 194, 178, 128, 255);

    for (int i = 0; i < 240; i++) {
      for (int j = 0; j < 320; j++) {
        if (grid[i][j]) {
          SDL_RenderDrawPoint(renderer, j, i);
        }
      }
    }
    // SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, target_texture, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_RenderPresent(renderer);

    frame_count++;
  }
  // mPrint("Frame count: " + to_string(frame_count));

  SDL_DestroyTexture(target_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0;
}
