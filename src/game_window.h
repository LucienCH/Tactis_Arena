#ifndef game_window_h
#define game_window_h


/* =============== DEPENDENCES =============== */


#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"

/** \file game_window.h
 * \brief Game window method functions
 * \author Thibault DOUCET
 */

/* =============== VARIABLES =============== */


extern SDL_Renderer *renderer;
extern SDL_Window *pWindow;
extern int XPOS;
extern int YPOS;
extern int selected_ability;
extern int hover_ability;
extern bool hover_next_turn;
extern int hover_stats;
extern int hover_passive_help;
extern Coord mouse_position;
extern char description[100];
extern Direction camMove;
extern TTF_Font *policePixels;


/* =============== FONCTIONS =============== */

/** \fn createGameWindow(int x, int y)
 * \brief Create the window where the game is played
 * \details This function manages the different events of the window
 * \return int
 */
int createGameWindow(int x, int y);

#endif
