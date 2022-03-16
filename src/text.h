#ifndef text_h
#define text_h

/* =============== DEPENDANCES =============== */


#include "struct.h"


/* =============== VARIABLES ================ */


extern TextCache text_cache[];


/* =============== FONCTIONS ================ */

bool isInCache(char *content, int size, char *police, SDL_Color color);

err_t addToCache(char *content, int size, char *police, SDL_Color color, SDL_Surface *surface);

err_t clearOldCache();

err_t freeTextCache();

SDL_Surface * getTextCache(char *content, int size, char *police, SDL_Color color);

err_t displayText(SDL_Renderer *renderer, int x, int y, int size, char *content, char *text_police, int r, int g, int b, bool caching);


#endif