#ifndef RESULT_VIEW_H
#define RESULT_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

    void display_victory_screen(SDL_Renderer *renderer, const char *elo);
    void display_defeated_screen(SDL_Renderer *renderer, const char *elo);

#endif