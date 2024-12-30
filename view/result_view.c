#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../model/match.h"
#include "../model/auth.h"
#include "../protocol.h"
#include "base_view.h"

void history_view(SDL_Renderer *renderer, MYSQL *conn, TTF_Font *font, const char *username) {
    // Xóa màn hình
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền đen
    SDL_RenderClear(renderer);

    // Lấy lịch sử trận đấu
    int match_count = 0;
    MatchData *matches = get_player_matches((char *)username, conn, &match_count);

    SDL_Color white = {255, 255, 255};

    if (!matches || match_count == 0) {
        draw_text(renderer, "No match history available.", 600, 300, white, font);
    } else {
        int y_offset = 70; 
        draw_text(renderer, "Lastest 20 matchs", 700, 20, white, font);
        for (int i = 0; i < match_count && i < 20; i++) { 
            char match_text[512];
            snprintf(match_text, sizeof(match_text), "Match %d: %s vs %s - Winner: %s",
                     matches[i].id, matches[i].player1, matches[i].player2, matches[i].winner);

            draw_text(renderer, match_text, 600, y_offset, white, font);
            y_offset += 40; 
        }
        free(matches); 
    }

    SDL_Color red = {255, 0, 0};
    SDL_Rect back_btn = {1400, 700, 100, 50}; 
    SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, 255);
    SDL_RenderFillRect(renderer, &back_btn);

    draw_text(renderer, "Back", 1420, 710, white, font);

    SDL_RenderPresent(renderer);

    // Xử lý sự kiện
    SDL_Event event;
    bool history_running = true;
    while (history_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                history_running = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= back_btn.x && x <= (back_btn.x + back_btn.w) &&
                    y >= back_btn.y && y <= (back_btn.y + back_btn.h)) {
                    printf("Back button clicked\n");
                    history_running = false; 
                }
            }
        }
    }
}
