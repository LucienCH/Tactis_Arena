#ifndef end_window_h
#define end_window_h

/* =============== DEPENDANCES =============== */


#include "struct.h"


/* =============== VARIABLES ================ */





/* =============== FONCTIONS ================ */


err_t loadEndTextures(SDL_Renderer * renderer);

err_t updateEndWindow(SDL_Renderer * renderer, winId win);

err_t displayEndWindow(winId win);

#endif