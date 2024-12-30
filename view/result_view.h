#ifndef RESULT_VIEW_H
#define RESULT_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
void history_view(SDL_Renderer *renderer, MYSQL *conn, TTF_Font *font, const char *username);

#endif