#include <SDL2/SDL.h>
#include "fire_result_view.h"

void drawFireResult(SDL_Renderer *renderer, int x, int y, int cellSize, bool isHit) {
    if (isHit) {
        // Vẽ vòng tròn màu xanh
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int w = 0; w < cellSize; w++) {
            for (int h = 0; h < cellSize; h++) {
                int dx = cellSize / 2 - w;
                int dy = cellSize / 2 - h;
                if ((dx * dx + dy * dy) <= (cellSize / 2) * (cellSize / 2)) {
                    SDL_RenderDrawPoint(renderer, x + w, y + h);
                }
            }
        }
    } else {
        // Vẽ dấu "X" màu đỏ
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x, y, x + cellSize, y + cellSize);
        SDL_RenderDrawLine(renderer, x + cellSize, y, x, y + cellSize);
    }
}
