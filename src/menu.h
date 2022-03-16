#ifndef menu_h
#define menu_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"

void loadMenuTextures(SDL_Renderer *renderer);
void freeMenuTextures(void);
int displayMenu(int x, int y);

#endif
