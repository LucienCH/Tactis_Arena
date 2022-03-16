
/* =============== DEPENDENCES =============== */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"
#include "map.h"
#include "menu.h"
#include "characters.h"
#include "common.h"
#include "map_editor.h"
#include "grid.h"
#include "init.h"
#include "gameplay.h"
#include "display.h"
#include "textures.h"
#include "turn.h"
#include "gameplay.h"
#include "print.h"
#include "servFcnt.h"
#include "text.h"
#include "animations.h"
#include "end_window.h"

/** \file game_window.c
 * \brief Game window method functions
 * \details Contains all the events of the game window
 * \author Thibault DOUCET
 */

/* =============== CONSTANTES ================ */

#define _NB_MAX_MAPS_ 50 // Max number of maps being listed
#define _X_SIZE_ 30
#define _Y_SIZE_ 30
#define _FPS_ 60				 // Define at which frequency the game have to refresh
#define _TEXTURE_LOADING_TIME_ 4 // Loading screen duration for textures (in seconds)

/* =============== VARIABLES ================ */

SDL_Renderer *renderer = NULL;
SDL_Window *pWindow = NULL;
int XPOS = 50;				  // x and y pos where map is displayed
int YPOS = 50;				  // |
int selected_ability = -1;	  // Selected ability
int hover_ability = -1;		  // Hover ability button
bool hover_next_turn = FALSE; // Hover skip turn button
int hover_stats = 0;		  // Hover stats
int hover_passive_help = 0;	  // Hover passive help in ID card (with mouse position)
int end_of_turn = 0;		  // Fin de tour
char onLoadingScreen[_NB_CLASSES_][STR_SHORT] = {"Goliath", "Valkyrie", "Angel", "Ranger", "Mage", "Berserker"};
Direction camMove = -1;
int *exitThread;

int xWinSize, yWinSize; // x and y sizes of the window
Coord mouse_position;

/* =============== FONCTIONS ================ */

int createGameWindow(int x, int y)
// Create a window with with x*y size (in px)
{
	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
	{
		printf("\033[31;01m[GAME_WINDOW ERROR]\033[00m : Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}

	/* Initialisation TTF */
	if (TTF_Init() == -1)
	{
		printf("\033[31;01m[GAME_WINDOW ERROR]\033[00m : Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Tactics Arena", SDL_WINDOWPOS_UNDEFINED,
							   SDL_WINDOWPOS_UNDEFINED,
							   x,
							   y,
							   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (!pWindow)
	{
		printf("\033[31;01m[GAME_WINDOW ERROR]\033[00m : Erreur à la création de la fenetre : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("\033[31;01m[GAME_WINDOW ERROR]\033[00m : Erreur à la création du rendu\n");
		exit(EXIT_FAILURE);
	}

	/* *************** INITIALISATION AUDIO *************** */
	if (verbose >= 2)
		printf("\033[36;01m[AUDIO]\033[00m : Audio driver: %s\n", SDL_GetCurrentAudioDriver());

	int i, count = SDL_GetNumAudioDevices(0);

	/*-- Boucle debug audio --*/
	for (i = 0; i < count; ++i)
	{
		if (verbose >= 2)
			printf("\033[36;01m[AUDIO]\033[00m : Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
	}

	/* On ouvre le device audio */
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		printf("\033[31;01m[AUDIO ERROR]\033[00m : %s\n", Mix_GetError());
	}

	Mix_Chunk *nopeSound = Mix_LoadWAV("../inc/sound_effects/cant_click.wav");
	/* ************************************************* */

	printf("%s", error_message[setRendererDriver(renderer)]);

	// Launcher icon
	SDL_SetWindowIcon(pWindow, loadOptImage("../inc/sprites/goliath/sprite_indiv/64_64/front/Sprite_frontview_64.png"));
	if (pWindow)
	{
		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

		// Textures loading screen
		int start_seconds = SDL_GetTicks() / 1000;
		int loadingAnim = 0;
		int loadingChar = rand() % 6;
		int xCharPos;
		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
		SDL_SetRenderDrawColor(renderer, 21, 126, 172, 255);
		SDL_RenderClear(renderer);
		displayText(renderer, 200, yWinSize / 2 + 120, 40, "Chargement des textures du jeu...", "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
		displayText(renderer, 200, yWinSize / 2, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, FALSE);
		SDL_RenderPresent(renderer);

		// Chargement des textures
		if (textures_loaded == FALSE)
			loadMapTextures(renderer);
		if (cSprites_loaded == FALSE)
			loadSprites(renderer, cSprites);
		if (animTextures_loaded == FALSE)
			loadAnimationTextures();

		while ((SDL_GetTicks() / 1000) - start_seconds < _TEXTURE_LOADING_TIME_)
		{
			xCharPos = xWinSize - (loadingAnim * 20);
			if (xCharPos < -100)
			{
				loadingAnim = 0;
				loadingChar++;
				if (loadingChar > 5)
					loadingChar = 0;
			}
			SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
			SDL_SetRenderDrawColor(renderer, 21, 126, 172, 255);
			SDL_RenderClear(renderer);
			displayText(renderer, 200, yWinSize / 2 + 120, 40, "Chargement des textures du jeu...", "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
			displayText(renderer, 200, yWinSize / 2, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, FALSE);
			displaySprite(renderer, getBigCharTexture(onLoadingScreen[loadingChar], W, loadingAnim % 6), xCharPos, yWinSize - 112);
			SDL_RenderPresent(renderer);
			loadingAnim++;
			SDL_Delay(100);
		}

		// Loading screen for connection
		if (is_online)
		{
			loadingAnim = 0;
			loadingChar = 0;
			while (!game_setup)
			{
				xCharPos = xWinSize - (loadingAnim * 20);
				if (xCharPos < -100)
				{
					loadingAnim = 0;
					loadingChar++;
					if (loadingChar > 5)
						loadingChar = 0;
				}
				SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
				SDL_SetRenderDrawColor(renderer, 21, 126, 172, 255);
				SDL_RenderClear(renderer);
				displayText(renderer, 200, yWinSize / 2 + 120, 40, "Communication des informations avec le serveur...", "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
				displayText(renderer, 200, yWinSize / 2, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, FALSE);
				displaySprite(renderer, getBigCharTexture(onLoadingScreen[0], W, loadingAnim % 6), xCharPos, yWinSize - 112);
				SDL_RenderPresent(renderer);
				loadingAnim++;
				SDL_Delay(100);
			}
		}

		/* Le fond de la fenêtre sera blanc */
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		if (verbose == 2)
		{
			printf("\033[36;01m[GAME_WINDOW]\033[00m : Structure actuelle de la map ([ID_TILE]|[WALKABLE]) : \n");
			debugGrid(matrix, _X_SIZE_, _Y_SIZE_);
		}

		if (!is_online)
		{
			ent_init_test();
		}

		SDL_Delay(1);

		displayMap(renderer, XPOS, YPOS);

		SDL_RenderPresent(renderer);

		game_setup = FALSE;

		Entity *tempEntity = NULL;

		int running = 1;

		playMenuMusic(3);
		while (running && !game_over_global)
		{
			tempEntity = getEntity(getSelectedPos());
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{

				case SDL_QUIT:
					running = 0;
					closeWindow(pWindow);
					displayEndWindow(LOSE);
					break;

				case SDL_WINDOWEVENT:
					switch (e.window.event)
					{
					case SDL_WINDOWEVENT_EXPOSED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_SHOWN:

						SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

						break;
					}
					break;

				/* ********** CLICS SOURIS ************ */
				case SDL_MOUSEBUTTONDOWN:

					if (verbose >= 2)
						printf("X: %d | Y: %d\n", e.motion.x, e.motion.y); // Debug console pos x & y on term

					// Compétences et actions
					if (tempEntity != NULL && is_ally(tempEntity))
					{
						if (e.motion.y >= yWinSize - 80 && e.motion.y <= yWinSize - 16)
						{
							if (your_turn())
							{
								// Sélection des compétences
								if (selected_ability != Last_Sacrifice)
								{
									// Mouvement
									if (e.motion.x >= 16 && e.motion.x <= 80)
									{
										if (!able_ability(tempEntity, Mvt, TRUE))
											selected_ability = Mvt;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 1
									else if (e.motion.x >= 96 && e.motion.x <= 160)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[0].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 2
									else if (e.motion.x >= 176 && e.motion.x <= 240)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[1].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 3
									else if (e.motion.x >= 256 && e.motion.x <= 320)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[2].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 4
									else if (e.motion.x >= 336 && e.motion.x <= 400)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[3].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Tourner personnage vers la droite
									else if (e.motion.x >= 416 && e.motion.x <= 480)
									{
										turnRight(tempEntity);
									}
									// Tourner personnage vers la gauche
									else if (e.motion.x >= 496 && e.motion.x <= 560)
									{
										turnLeft(tempEntity);
									}
									else
										selectTile(XPOS, YPOS, e.motion.x, e.motion.y);
								}
								else
								// Sélection de l'allié à ressuciter grâce à Last Sacrifice
								{
									int nbDeadAllies = count_dead_allies(tempEntity);
									action revive = {tempEntity->cha_id, {0, 0}, Last_Sacrifice};
									Entity *deadAllies[nbDeadAllies];
									get_dead_allies(tempEntity, deadAllies);

									// Allié 1
									if (e.motion.x >= 16 && e.motion.x <= 80 && nbDeadAllies == 1)
									{
										revive.c = deadAllies[0]->coords;
										action_set(revive);
									}
									// Allié 2
									else if (e.motion.x >= 96 && e.motion.x <= 160 && nbDeadAllies == 2)
									{
										revive.c = deadAllies[1]->coords;
										action_set(revive);
									}
									// Allié 3
									else if (e.motion.x >= 176 && e.motion.x <= 240 && nbDeadAllies == 3)
									{
										revive.c = deadAllies[2]->coords;
										action_set(revive);
									}
									// Allié 4
									else if (e.motion.x >= 256 && e.motion.x <= 320 && nbDeadAllies == 4)
									{
										revive.c = deadAllies[3]->coords;
										action_set(revive);
									}
									// Allié 5
									else if (e.motion.x >= 336 && e.motion.x <= 400 && nbDeadAllies == 5)
									{
										revive.c = deadAllies[4]->coords;
										action_set(revive);
									}
									selected_ability = -1;
									unselect();
								}

								// Exception Last Sacrifice
								if (selected_ability == Last_Sacrifice && count_dead_allies(tempEntity) < 1)
								{
									selected_ability = -1;
									addLog("No dead allies yet");
									Mix_PlayChannel(-1, nopeSound, 0);
								}

								if (verbose >= 1)
									printf("\033[36;01m[GAME_WINDOW]\033[00m : Selected ability : %d\n", selected_ability);
							}
							else
							{
								addLog("Not available during opponent's turn");
								Mix_PlayChannel(-1, nopeSound, 0);
							}
						}
						else
						{
							selectTile(XPOS, YPOS, e.motion.x, e.motion.y);
						}
					}
					else
					{
						selected_ability = -1;
						selectTile(XPOS, YPOS, e.motion.x, e.motion.y);
					}

					// Fin de tour
					if (e.motion.x >= xWinSize - 280 && e.motion.x <= xWinSize - 24 && e.motion.y >= yWinSize - 80 && e.motion.y <= yWinSize - 16 && your_turn())
					{
						hover_next_turn = FALSE;
						set_endturn();
					}
					break;

				/* ********** SCROLL SOURIS ************ */
				case SDL_MOUSEWHEEL:
					if (e.wheel.y > 0) // Scroll UP
					{
						if (pxBase == 64)
						{
							pxBase = 128;
							if (verbose >= 1)
								printf("\033[36;01m[GAME_WINDOW]\033[00m : Zoom In (Resolution : %dx%dpx)\n", pxBase, pxBase);
							XPOS *= 2;
							YPOS *= 2;
						}
					}
					else
					{ // Scroll DOWN
						if (pxBase == 128)
						{
							pxBase = 64;
							if (verbose >= 1)
								printf("\033[36;01m[GAME_WINDOW]\033[00m : Zoom Out (Resolution : %dx%dpx)\n", pxBase, pxBase);
							XPOS /= 2;
							YPOS /= 2;
						}
					}
					break;

				/* ********** APPUI TOUCHE CLAVIER ************ */
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
					case SDLK_KP_PLUS: // "+" key
						if (pxBase == 64)
						{
							pxBase = 128;
							addLog("PX 128");
							if (verbose >= 1)
								printf("\033[36;01m[GAME_WINDOW]\033[00m : Zoom In (Resolution : %dx%dpx)\n", pxBase, pxBase);
							XPOS *= 2;
							YPOS *= 2;
						}
						break;
					case SDLK_KP_MINUS: // "-" key
						if (pxBase == 128)
						{
							pxBase = 64;
							addLog("PX 64");
							if (verbose >= 1)
								printf("\033[36;01m[GAME_WINDOW]\033[00m : Zoom Out (Resolution : %dx%dpx)\n", pxBase, pxBase);
							XPOS /= 2;
							YPOS /= 2;
						}
						break;
					case SDLK_ESCAPE:
						if (selected_ability != -1)
							selected_ability = -1;
						else
						{
							unselect();
							unhover();
						}
						break;
					/* ***** DEPLACEMENTS CAMERA (RACCOURCIS CLAVIER) ***** */
					case SDLK_z: // "z" key
						YPOS += (10 * (pxBase / 64));
						break;
					case SDLK_q: // "q" key

						XPOS += (10 * (pxBase / 64));
						break;
					case SDLK_s: // "s" key

						YPOS -= (10 * (pxBase / 64));
						break;
					case SDLK_d: // "d" key

						XPOS -= (10 * (pxBase / 64));
						break;
					case SDLK_UP: // "ARROW UP" key

						YPOS += (10 * (pxBase / 64));
						break;
					case SDLK_LEFT: // "ARROW LEFT" key

						XPOS += (10 * (pxBase / 64));
						break;
					case SDLK_DOWN: // "ARROW DOWN" key

						YPOS -= (10 * (pxBase / 64));
						break;
					case SDLK_RIGHT: // "ARROW RIGHT" key

						XPOS -= (10 * (pxBase / 64));
						break;
					/* ***** SELECTION CAPACITES (RACCOURCIS CLAVIER) ***** */
					default:
						if (your_turn())
						{
							// Sélection des compétences
							if (selected_ability != Last_Sacrifice && tempEntity != NULL)
							{
								if (is_ally(tempEntity))
								{
									// Mouvement
									if (e.key.keysym.sym == SDLK_1)
									{
										if (!able_ability(tempEntity, Mvt, TRUE))
											selected_ability = Mvt;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 1
									else if (e.key.keysym.sym == SDLK_2)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[0].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 2
									else if (e.key.keysym.sym == SDLK_3)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[1].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 3
									else if (e.key.keysym.sym == SDLK_4)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[2].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Compétence 4
									else if (e.key.keysym.sym == SDLK_5)
									{
										if (!able_ability(tempEntity, tempEntity->cha_class->cla_abilities[3].ab_id, TRUE))
											selected_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
										else
											Mix_PlayChannel(-1, nopeSound, 0);
									}
									// Tourner personnage vers la droite
									else if (e.key.keysym.sym == SDLK_6)
									{
										turnRight(tempEntity);
									}
									// Tourner personnage vers la gauche
									else if (e.key.keysym.sym == SDLK_7)
									{
										turnLeft(tempEntity);
									}
								}
								else
								{
									Mix_PlayChannel(-1, nopeSound, 0);
								}
							}
							else
							{

								Mix_PlayChannel(-1, nopeSound, 0);
							}
						}
						else
						{

							Mix_PlayChannel(-1, nopeSound, 0);
						}
					}
					break;

				/* ********** MOUVEMENTS SOURIS ************ */
				case SDL_MOUSEMOTION:
					hover_ability = -1;
					hover_next_turn = FALSE;
					mouse_position.x = e.motion.x;
					mouse_position.y = e.motion.y;

					hoverTile(XPOS, YPOS, e.motion.x, e.motion.y);

					// Hover skip turn button
					if (e.motion.x >= xWinSize - 280 && e.motion.x <= xWinSize - 24 && e.motion.y >= yWinSize - 80 && e.motion.y <= yWinSize - 16)
						hover_next_turn = TRUE;

					// Hover compétences et actions
					tempEntity = getEntity(getSelectedPos());
					if (tempEntity != NULL)
					{
						if (e.motion.y >= yWinSize - 80 && e.motion.y <= yWinSize - 16)
						{
							if (e.motion.x >= 16 && e.motion.x <= 80)
								hover_ability = Mvt;
							else if (e.motion.x >= 96 && e.motion.x <= 160)
								hover_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
							else if (e.motion.x >= 176 && e.motion.x <= 240)
								hover_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
							else if (e.motion.x >= 256 && e.motion.x <= 320)
								hover_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
							else if (e.motion.x >= 336 && e.motion.x <= 400)
								hover_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
						}
						// Hover stats
						else if (e.motion.x >= 25 && e.motion.x < 110 && e.motion.y >= 142 && e.motion.y <= 174)
						{
							hover_stats = 1;
						}
						else if (e.motion.x >= 110 && e.motion.x < 195 && e.motion.y >= 142 && e.motion.y <= 174)
						{
							hover_stats = 2;
						}
						else if (e.motion.x >= 195 && e.motion.x < 280 && e.motion.y >= 142 && e.motion.y <= 174)
						{
							hover_stats = 3;
						}
						else if (e.motion.x >= 280 && e.motion.x < 365 && e.motion.y >= 142 && e.motion.y <= 174)
						{
							hover_stats = 4;
						}
						else if (e.motion.x >= 165 && e.motion.x < 240 && e.motion.y >= 102 && e.motion.y <= 134)
						{
							hover_stats = 5;
						}
						else if (e.motion.x >= 240 && e.motion.x < 315 && e.motion.y >= 102 && e.motion.y <= 134)
						{
							hover_stats = 6;
						}
						else if (e.motion.x >= 315 && e.motion.x < 390 && e.motion.y >= 102 && e.motion.y <= 134)
						{
							hover_stats = 7;
						}
						// Hover passive
						else if (e.motion.x >= 377 && e.motion.x <= 396 && e.motion.y >= 156 && e.motion.y <= 174)
						{
							hover_passive_help = 1;
							mouse_position.x = e.motion.x;
							mouse_position.y = e.motion.y;
						}
						else
						{
							hover_passive_help = 0;
							hover_stats = 0;
						}
					}
					break;
				}
			}

			/* ********** MOUVEMENTS CAMERA ************ */
			if (SDL_GetMouseFocus() == pWindow)
			{
				camMove = -1;
				// Déplacement de la caméra grâce aux bords de l'écran
				if (mouse_position.x <= xWinSize && mouse_position.x >= xWinSize - 20)
				{
					XPOS -= (camSpeed * (pxBase / 64));
					camMove = E;
				}
				if (mouse_position.x <= 20 && mouse_position.x >= 0)
				{
					XPOS += (camSpeed * (pxBase / 64));
					camMove = W;
				}
				if (mouse_position.y <= yWinSize && mouse_position.y >= yWinSize - 20)
				{
					YPOS -= (camSpeed * (pxBase / 64));
					camMove = S;
				}
				if (mouse_position.y <= 20 && mouse_position.y >= 0)
				{
					YPOS += (camSpeed * (pxBase / 64));
					camMove = N;
				}
				// Vérification pour ne pas dépasser des "border" avec la caméra
				if (XPOS > 500 * (pxBase / 64))
					XPOS = 500 * (pxBase / 64);
				if (XPOS < -1000 * (pxBase / 64))
					XPOS = -1000 * (pxBase / 64);
				if (YPOS > 300 * (pxBase / 64))
					YPOS = 300 * (pxBase / 64);
				if (YPOS < -500 * (pxBase / 64))
					YPOS = -500 * (pxBase / 64);
			}

			if (opponent_set)
			{
				opponent_action();
			}

			displayMap(renderer, XPOS, YPOS);

			SDL_Delay(1000 / _FPS_);
		}
		closeWindow(pWindow);
		freeTextures(textures);
		freeTextures(cSprites);
		freeTextCache();
		displayEndWindow(game_over_global);
		exit(EXIT_SUCCESS);
	}
	else
	{
		printf("\033[31;01m[GAME_WINDOW ERROR]\033[00m : Erreur de création de la fenêtre: %s\n", SDL_GetError());
	}

	return 1;
}