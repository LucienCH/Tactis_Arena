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
#include "characters.h"
#include "test.h"
#include "text.h"

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


SDL_Texture *bg = NULL,
			*quit = NULL;

// La musique est activée de base
int music = 1;

// x and y sizes of the window
int xWinSize;
int yWinSize;

int win_index = N;

err_t loadEndTextures(SDL_Renderer * renderer)
{
    bg = loadTexture(renderer, loadImage("../inc/img/wallpapers/menu2_720p.png"));
    quit = loadTexture(renderer, loadImage("../inc/img/buttons/quit_button_256.png"));

    return OK;
}

err_t updateEndWindow(SDL_Renderer * renderer, winId win)
// Update the end window
{
    displaySprite(renderer, bg, 0, 0);
    displaySprite(renderer, quit, xWinSize/2 - 128, yWinSize/3*2);
    if (win == WIN)
    {
        displayText(renderer, 400, yWinSize/3, 100, "YOU WON", "../inc/font/Pixels.ttf", 0, 255, 0, FALSE);
    }
    else
    {
        displayText(renderer, 400, yWinSize/3, 100, "YOU LOST", "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
    }
    win_index++;
    if (win_index > W) win_index = N;

    return OK;
}

err_t displayEndWindow(winId win)
// Create a window with with x*y size (in px)
{
    //Le pointeur vers la fenetre
	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

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
												  		1280,
												  		720,
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

    loadEndTextures(renderer);

	if( pWindow )
	{
        SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
		int running = 1;
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

                                SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
                                updateEndWindow(renderer, win);
								
							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						if(verbose>=2)printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term
						
						// Bouton "Quit"
						if (e.motion.x >= 585 && e.motion.x <= 705 && e.motion.y >= 577 && e.motion.y <= 623)
						{
							closeWindow(pWindow);
                            freeAllTextures();
							system("./TacticsArena -4");
						}

					break;
				}
			}
			SDL_Delay(16);
			SDL_RenderPresent(renderer);
		}
		closeWindow(pWindow);
	} else {
		printf("\033[31;01m[MENU ERROR]\033[00m : Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return OK;
}
