#ifndef FIRE_RESULT_VIEW_H
#define FIRE_RESULT_VIEW_H

#include <SDL2/SDL.h>
#include <stdbool.h>

void drawFireResult(SDL_Renderer *renderer, int x, int y, int cellSize, bool isHit);

#endif // FIRE_RESULT_VIEW_H
