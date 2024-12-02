#include "login_view.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

void render_login(SDL_Renderer *renderer, TTF_Font *font, char *username, char *password)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255};
    SDL_Color blue = {50, 100, 200};

    SDL_Rect username_box = {200, 150, 200, 50};
    SDL_Rect password_box = {200, 250, 200, 50};
    SDL_Rect login_button = {200, 350, 200, 50};

    SDL_Surface *title_surface = TTF_RenderText_Solid(font, "User Login", white);
    SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = {200, 50, 200, 50};

    SDL_Surface *username_surface = TTF_RenderText_Solid(font, "Username:", white);
    SDL_Texture *username_texture = SDL_CreateTextureFromSurface(renderer, username_surface);

    SDL_Surface *password_surface = TTF_RenderText_Solid(font, "Password:", white);
    SDL_Texture *password_texture = SDL_CreateTextureFromSurface(renderer, password_surface);

    SDL_Surface *login_surface = TTF_RenderText_Solid(font, "Login", white);
    SDL_Texture *login_texture = SDL_CreateTextureFromSurface(renderer, login_surface);

    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

    SDL_RenderCopy(renderer, username_texture, NULL, &username_box);
    SDL_RenderCopy(renderer, password_texture, NULL, &password_box);

    SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, 255);
    SDL_RenderFillRect(renderer, &login_button);
    SDL_RenderCopy(renderer, login_texture, NULL, &login_button);

    SDL_DestroyTexture(title_texture);
    SDL_DestroyTexture(username_texture);
    SDL_DestroyTexture(password_texture);
    SDL_DestroyTexture(login_texture);
    SDL_FreeSurface(title_surface);
    SDL_FreeSurface(username_surface);
    SDL_FreeSurface(password_surface);
    SDL_FreeSurface(login_surface);

    SDL_RenderPresent(renderer);
}
