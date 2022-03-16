#ifndef multi_menu_h
#define multi_menu_h

/** \file multi_menu.h
 * \brief Menu for multiplayer mod
 * \author Lucien CHAUVIN
 */

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"

/** \fn displayMenuMulti(int x, int y)
 * \brief Create the main window for multiplayer mod 
 * \details Create a window and adjust it between client and server
 * \param x Size of the window
 * \param y Size of the window
 * \return 1 if the client or server have started the game or 0 if a player press quit button
*/

int displayMenuMulti(int x, int y);

/** \fn dispJoinMenu(SDL_Renderer *renderer, int x, int y)
 * \brief Adjust the window for the user that pressed Join button to join a hosted game 
 * \details Display all the necessary tools to connect the user to the server
 * \param renderer SDL renderer to render the window
 * \param x Size of the window
 * \param y Size of the window
 * \return Nothing
*/

void dispJoinMenu(SDL_Renderer *renderer, int x, int y);

/** \fn dispHostMenu(SDL_Renderer *renderer, int x, int y, int index)
 * \brief Adjust the window for the user that pressed Host button to host the game 
 * \details Display all the necessary tools to create the game server
 * \param renderer SDL renderer to render the window
 * \param x Size of the window
 * \param y Size of the window
 * \param index an index of the list of map that can be selected by the host
 * \return Nothing
*/

void dispHostMenu(SDL_Renderer *renderer, int x, int y, int index);

/** \fn freeMultiMenuTextures()
 * \brief Function to free all multiplayer menu textures 
 * \return Nothing
*/

void freeMultiMenuTextures();

/** \fn loadMultiMenuTextures(SDL_Renderer *renderer)
 * \brief Function to load all multiplayer menu textures
 * \param renderer SDL renderer where to load all textures 
 * \return Nothing
*/

void loadMultiMenuTextures(SDL_Renderer *renderer);

#endif