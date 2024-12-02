#ifndef BASE_VIEW_H
#define BASE_VIEW_H

#include <SDL2/SDL.h>

int init_view(SDL_Window **window, SDL_Renderer **renderer);
void render_text(SDL_Renderer *renderer, const char *text);
void cleanup_view(SDL_Window *window, SDL_Renderer *renderer);

#endif // BASE_VIEW_H
