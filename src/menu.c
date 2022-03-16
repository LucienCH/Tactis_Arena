#include <stdio.h>
#include <stdlib.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "map.h"
#include "struct.h"
#include "game_window.h"
#include "map_editor.h"
#include "common.h"
#include "display.h"
#include "textures.h"
#include "test.h"
#include "text.h"
#include "end_window.h"

#ifdef _WIN32
#  include <windows.h>
#  define psleep(sec) Sleep ((sec) * 1000)
#else
#  include <unistd.h>
#  define psleep(sec) sleep ((sec))
#endif

#define XPOS 500			// x position of the grid
#define YPOS 100			// y position of the grid
#define _NB_MAX_MAPS_ 20



SDL_Texture *background = NULL,
			*start_button = NULL,
			*quit_button = NULL,
			*music_on = NULL,
			*music_off = NULL,
			*map_editor_button = NULL,
			*multi_button = NULL;

// La musique est activée de base
int music_playing = 1;
int isMultiMenu = 0;

// Map list
char *mapList[_NB_MAX_MAPS_];
int mapIndex = 0;

void loadMenuTextures(SDL_Renderer *renderer)
// Load all the textures needed for the menu
{
	if (verbose >= 1) printf("\033[36;01m[MENU]\033[00m : Chargement des textures du menu...\n");

	// == Loading background ==
	background = loadTexture(renderer, loadOptImage("../inc/img/wallpapers/menu2_720p.png"));

	// == Loading start button ==
	start_button = loadTexture(renderer, loadOptImage("../inc/img/buttons/start_button_256.png"));

	// == Loading quit button ==
	quit_button = loadTexture(renderer, loadOptImage("../inc/img/buttons/quit_button_256.png"));

	// == Loading music ON switch ==
	music_on = loadTexture(renderer, loadOptImage("../inc/img/icons/music_on.png"));

	// == Loading music OFF switch ==
	music_off = loadTexture(renderer, loadOptImage("../inc/img/icons/music_off.png"));

	// == Loading map editor button ==
	map_editor_button = loadTexture(renderer, loadOptImage("../inc/img/buttons/map_editor_button_256.png"));
	
	// == Loading Multi switch ==
	multi_button = loadTexture(renderer, loadOptImage("../inc/img/buttons/multi_button_256.png"));
}

void freeMenuTextures()
// Free all the textures loaded
{
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(start_button);
	SDL_DestroyTexture(quit_button);
	SDL_DestroyTexture(music_on);
	SDL_DestroyTexture(music_off);
	SDL_DestroyTexture(map_editor_button);
}

void updateMenu(SDL_Renderer *renderer, int x, int y)
// Update the menu display
{	
	/* Background image */
	displaySprite(renderer, background, 0, 0);

	/* Start button */
	displaySprite(renderer, start_button, 500, 300);

	/* Editor button */
	displaySprite(renderer, map_editor_button, 515, 375);

	/* Multi button */
	displaySprite(renderer, multi_button, 515, 425);

	/* Quit button */
	displaySprite(renderer, quit_button, 515, 525);

	/* Bouton musique ON/OFF */
	if (music_playing){
		displaySprite(renderer, music_on, x-175, y-200);
	}else {
		displaySprite(renderer, music_off, x-175, y-200);
	}
	/* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 200, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, TRUE);

	/* Mentions de bas de menu */
	displayText(renderer, 5, y-20, 15, "Projet L2 Informatique - BUTEL CHAUVIN DOUCET LAFAY", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
}

int displayMenu(int x, int y)
// Create a window with with x*y size (in px)
{
    //Le pointeur vers la fenetre
	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

	// x and y sizes of the window
	int xWinSize;
	int yWinSize;

    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0 ) {
        printf("\033[31;01m[MENU ERROR]\033[00m : Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

	/* Initialisation TTF */
	if(TTF_Init() == -1) {
		printf("\033[31;01m[MENU ERROR]\033[00m : Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Tactics Arena - Menu", 	SDL_WINDOWPOS_UNDEFINED,
												  		SDL_WINDOWPOS_UNDEFINED,
												  		x,
												  		y,
												  		SDL_WINDOW_SHOWN);

	if(!pWindow){
		printf("\033[31;01m[MENU ERROR]\033[00m : Erreur à la création de la fenetre : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		printf("\033[31;01m[MENU ERROR]\033[00m : Erreur à la création du rendu\n");
		exit(EXIT_FAILURE);
	}

	setRendererDriver(renderer);

	// Launcher icon
    SDL_SetWindowIcon(pWindow, loadOptImage("../inc/sprites/goliath/sprite_indiv/64_64/front/Sprite_frontview_64.png"));

	if( pWindow )
	{
		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
		int running = 1;
		SDL_StartTextInput();
		while(running) {
			SDL_Event e;
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT: 
						running = 0;
						exit(EXIT_SUCCESS);
					break;
					case SDL_WINDOWEVENT:
						switch(e.window.event){
							case SDL_WINDOWEVENT_EXPOSED:
							case SDL_WINDOWEVENT_SIZE_CHANGED:
							case SDL_WINDOWEVENT_RESIZED:
							case SDL_WINDOWEVENT_HIDDEN:
							case SDL_WINDOWEVENT_SHOWN:

								loadMenuTextures(renderer);
								updateMenu(renderer, x, y);
								
							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						if(verbose>=2)printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term

						// Bouton "Start"
						if (e.motion.x >= 569 && e.motion.x <= 730 && e.motion.y >= 394 && e.motion.y <= 443)
						{
							closeWindow(pWindow);
							freeMenuTextures();
							return 2;
						}

						// Bouton test
						if (e.motion.x >= 0 && e.motion.x < 569 && e.motion.y >= 394 && e.motion.y <= 443)
						{
							closeWindow(pWindow);
							freeAllTextures();
							displayEndWindow(WIN);
						}

						// Bouton "Map editor"
						else if (e.motion.x >= 585 && e.motion.x <= 710 && e.motion.y >= 467 && e.motion.y <= 518)
						{
							closeWindow(pWindow);
							freeMenuTextures();
							return 3;
						}

						// Bouton "Multiplayer"
						else if (e.motion.x >= 585 && e.motion.x <= 710 && e.motion.y >= 540 && e.motion.y <= 600)
						{
							closeWindow(pWindow);
							freeMenuTextures();
							serverStatus = 0;
							return 4;
						}

						// Switch musique ON/OFF
						else if (e.motion.x >= 1202 && e.motion.x <= 1250 && e.motion.y >= 627 && e.motion.y <= 680)
						{
							if (music_playing)
							{
								music_playing = 0;
								pauseMenuMusic();
								updateMenu(renderer, x , y);
							}
							else
							{
								music_playing = 1;
								resumeMenuMusic();
								updateMenu(renderer, x , y);
							}
						}
						
						// Bouton "Quit"
						else if (e.motion.x >= 569 && e.motion.x <= 730 && e.motion.y >= 613 && e.motion.y <= 673)
						{
							closeWindow(pWindow);
							freeMenuTextures();
							freeAllTextures();
							exit(EXIT_SUCCESS);
						}
					break;
				}
			}
			SDL_Delay(16);
			SDL_RenderPresent(renderer);
		}
		closeWindow(pWindow);
		freeMenuTextures();
	} else {
		printf("\033[31;01m[MENU ERROR]\033[00m : Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}
	return 1;
}
