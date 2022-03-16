#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "map.h"
#include "struct.h"
#include "game_window.h"
#include "display.h"
#include "textures.h"
#include "multi_menu.h"
#include "servFcnt.h"
#include "common.h"
#include "text.h"
#include "turn.h"

/** \file multi_menu.c
 * \brief Menu for multiplayer mod
 * \author Lucien CHAUVIN
*/

#define _NB_MAX_MAPS_ 20

SDL_Texture *background_Multi = NULL,
			*quit_button_Multi = NULL,
			*music_on_Multi = NULL,
			*music_off_Multi = NULL,
			*join_button = NULL,
			*host_button = NULL,
			*ok_button_Multi = NULL;

int isHostMenu = 0;
int isJoinMenu = 0;
int inputPseudoBtn = 0;
int inputIpBtn = 0;
int isPseudoValid = 0;
int isIPValid = 0;
int isInfoJoinSet = 0;
int isClientCo = 0;
int music_Multi_playing = 1;
int mapListMultiIndex = 0;
int isMapMultiValid = 0;
int indexMapMulti = 0;
int serverStatus = 0;

char pseudoSrv[50] = "Pseudo : ";
char pseudoJoin[50] = "Pseudo : ";
char pseudoUser[50];
char pseudoClient[128];
char ipSrv[85];
char ipJoin[90] = "IP : ";
char *compo;
char *mapListMulti[_NB_MAX_MAPS_];
char mapNameMulti[50] = "map_";
char mapMultiSelected[50];

extern Sint32 cursor;
extern Sint32 selection_len;

char *tabLog[] = {
	"Création du serveur",
	"Le serveur est initialisé !",
	"En attente du client ...",
	"Une connexion a été établie !",
	" ",
};

// Initialisation du thread
typedef struct
{
	pthread_t thread_server;
	pthread_t thread_client;
} multiThread_t;

multiThread_t threadServ;
multiThread_t threadCli;

/*-- Fonction pour le thread du serveur. --*/
static void *fn_server(void *p_data)
{

	if (startTCPSocketServ() == OK)
	{
		if (verbose >= 1)
			printf("Fin de startTCPSocketServ \n");
		sleep(1);
		if (verbose >= 1)
			printf("Début de init_server \n");
		init_server();
	}
	return NULL;
}

/*-- Fonction pour le thread du client --*/
static void *fn_client(void *p_data)
{
	if (startTCPSocketCli() == OK)
	{
		if (verbose >= 1)
			printf("Fin de startTCPSOcketClient \n");
		sleep(1);
		if (verbose >= 1)
			printf("Début d'init_client \n");
		init_client();
	}
	return NULL;
}

void loadMultiMenuTextures(SDL_Renderer *renderer)
// Load all the textures needed for the menu
{
	if (verbose >= 2)
		printf("Chargement des textures du menu...\n");

	// == Loading background ==
	background_Multi = loadTexture(renderer, loadOptImage("../inc/img/wallpapers/menu2_720p.png"));

	// == Loading quit button ==
	quit_button_Multi = loadTexture(renderer, loadOptImage("../inc/img/buttons/quit_button_256.png"));

	// == Loading music ON switch ==
	music_on_Multi = loadTexture(renderer, loadOptImage("../inc/img/icons/music_on.png"));

	// == Loading music OFF switch ==
	music_off_Multi = loadTexture(renderer, loadOptImage("../inc/img/icons/music_off.png"));

	// == Loading Host switch ==
	host_button = loadTexture(renderer, loadOptImage("../inc/img/buttons/host_btn_256.png"));

	// == Loading Join switch ==
	join_button = loadTexture(renderer, loadOptImage("../inc/img/buttons/join_btn_256.png"));

	// == ok switch ==
	ok_button_Multi = loadTexture(renderer, loadOptImage("../inc/img/buttons/Okey_button.png"));
}

void freeMultiMenuTextures()
// Free all the textures loaded
{
	SDL_DestroyTexture(background_Multi);
	SDL_DestroyTexture(quit_button_Multi);
	SDL_DestroyTexture(music_on_Multi);
	SDL_DestroyTexture(music_off_Multi);
	SDL_DestroyTexture(host_button);
	SDL_DestroyTexture(join_button);
	SDL_DestroyTexture(ok_button_Multi);
}

int listMapsMulti(char *mapListMulti[])
// List all the maps and save them in mapList
{
	int index = 0;
	DIR *d;
	struct dirent *dir;

	d = opendir("../maps");
	if (!d)
	{
		printf("Aucune map sauvegardée\n");
		return -1;
	}
	else
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
			{
				mapListMulti[index] = dir->d_name;
				index++;
			}
		}
		closedir(d);
	}

	return index;
}

void dispLog(SDL_Renderer *renderer, int consoleX, int consoleY)
{

	for (int i = 0; i < logFlag; i++)
	{
		displayText(renderer, consoleX + 20, consoleY + (25 * i), 22, tabLog[i], "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
		if (i == 4)
		{
			displayText(renderer, consoleX + 20, consoleY + (25 * i), 22, pseudoClient, "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
			displaySprite(renderer, ok_button_Multi, 530, 450);
			displayText(renderer, 605, 540, 45, "Start", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
			isClientCo = 1;
		}
	}
}

void dispHostMenu(SDL_Renderer *renderer, int x, int y, int index)
{

	SDL_Rect console;
	console.x = x - (40 + 450);
	console.y = 350;
	console.w = 450;
	console.h = 250;

	SDL_Rect infoHost;
	infoHost.x = 500;
	infoHost.y = 350;
	infoHost.w = 250;
	infoHost.h = 75;

	SDL_Rect inputSrv;
	inputSrv.x = 40;
	inputSrv.y = 350;
	inputSrv.w = 400;
	inputSrv.h = 350;

	SDL_Rect pseudoHostBox;
	pseudoHostBox.x = 150;
	pseudoHostBox.y = 400;
	pseudoHostBox.w = 250;
	pseudoHostBox.h = 28;

	SDL_Rect mapListBox;
	mapListBox.x = 75;
	mapListBox.y = 490;
	mapListBox.w = 325;
	mapListBox.h = 30;

	int nbMaps = listMapsMulti(mapListMulti);

	if (index <= 0)
		index = 0, indexMapMulti = 0;
	if (index > nbMaps - 1)
		index = nbMaps - 1, indexMapMulti = nbMaps - 1;

	sprintf(mapNameMulti, "%s", mapListMulti[index]);

	/* Background image */
	displaySprite(renderer, background_Multi, 0, 0);
	/* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 100, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, TRUE);
	/* Quit button */
	displaySprite(renderer, quit_button_Multi, x - 300, y - 190);
	/* initialisation de l'opacitée */
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	/* initialise la couleur sur noir */
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
	/*remplis le rectange*/
	SDL_RenderFillRect(renderer, &console);
	/* ré atribution des valeurs par défault */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	// displayText(renderer, 50, 400, 15, "Host est séléctioné", "../inc/font/PixelOperator.ttf", 255, 255, 255);

	/*-------------------input box Host menu -------------------------*/

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
	SDL_RenderFillRect(renderer, &inputSrv);
	if (inputPseudoBtn == 1)
	{
		SDL_SetRenderDrawColor(renderer, 153, 153, 153, 185);
		SDL_RenderFillRect(renderer, &pseudoHostBox);
	}
	else if (isPseudoValid == 1)
	{
		SDL_SetRenderDrawColor(renderer, 126, 190, 135, 185);
		SDL_RenderFillRect(renderer, &pseudoHostBox);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 185);
		SDL_RenderFillRect(renderer, &pseudoHostBox);
	}
	displayText(renderer, inputSrv.x + 10, inputSrv.y + 15, 22, "Saisir votre Pseudo : ", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displayText(renderer, inputSrv.x + 15, inputSrv.y + 50, 22, pseudoSrv, "../inc/font/PixelOperator.ttf", 255, 255, 255, FALSE);

	displayText(renderer, mapListBox.x - 30, mapListBox.y - 30, 22, "Chosir une map : ", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displayText(renderer, mapListBox.x - 15, mapListBox.y + 3, 22, "<", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displayText(renderer, mapListBox.x + mapListBox.w + 5, mapListBox.y + 4, 22, ">", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);

	if (isPseudoValid == 1)
	{
		SDL_SetRenderDrawColor(renderer, 126, 190, 135, 185);
		SDL_RenderFillRect(renderer, &mapListBox);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 185);
		SDL_RenderFillRect(renderer, &mapListBox);
	}
	if (mapListMulti[index] != NULL)
		displayText(renderer, mapListBox.x + 10, mapListBox.y + 4, 20, mapNameMulti, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

	displaySprite(renderer, ok_button_Multi, 50, mapListBox.y + mapListBox.h + 20);
	displayText(renderer, 150, mapListBox.y + mapListBox.h + 110, 55, "OK", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);

	/*-------------------input box Host menu -------------------------*/

	if (logFlag >= 3)
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
		SDL_RenderFillRect(renderer, &infoHost);
		displayText(renderer, infoHost.x + 10, infoHost.y + 15, 22, "L'ip du serveur est :", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
		displayText(renderer, infoHost.x + 15, infoHost.y + 50, 22, monIP, "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
	}

	dispLog(renderer, console.x, console.y);

	if (music_Multi_playing)
	{
		displaySprite(renderer, music_on_Multi, x - 175, y - 200);
	}
	else
	{
		displaySprite(renderer, music_off_Multi, x - 175, y - 200);
	}
}

void dispJoinMenu(SDL_Renderer *renderer, int x, int y)
{
	SDL_Rect infoJoin;
	infoJoin.x = 450;
	infoJoin.y = 350;
	infoJoin.w = 475;
	infoJoin.h = 170;

	SDL_Rect ipJoinBox;
	ipJoinBox.x = 515;
	ipJoinBox.y = 397;
	ipJoinBox.w = 350;
	ipJoinBox.h = 28;

	SDL_Rect inputJoin;
	inputJoin.x = 40;
	inputJoin.y = 350;
	inputJoin.w = 300;
	inputJoin.h = 150;

	SDL_Rect pseudoJoinBox;
	pseudoJoinBox.x = 150;
	pseudoJoinBox.y = 400;
	pseudoJoinBox.w = 150;
	pseudoJoinBox.h = 28;

	/* Background image */
	displaySprite(renderer, background_Multi, 0, 0);
	/* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 100, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, TRUE);
	/* Quit button */
	displaySprite(renderer, quit_button_Multi, x - 300, y - 190);

	/*-----------input box for JoinMenu--------*/
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
	SDL_RenderFillRect(renderer, &inputJoin);
	if (inputPseudoBtn == 1)
	{
		SDL_SetRenderDrawColor(renderer, 153, 153, 153, 185);
		SDL_RenderFillRect(renderer, &pseudoJoinBox);
	}
	else if (isPseudoValid == 1)
	{
		SDL_SetRenderDrawColor(renderer, 126, 190, 135, 185);
		SDL_RenderFillRect(renderer, &pseudoJoinBox);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 185);
		SDL_RenderFillRect(renderer, &pseudoJoinBox);
	}

	displayText(renderer, inputJoin.x + 10, inputJoin.y + 15, 22, "Saisir votre Pseudo : ", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displayText(renderer, inputJoin.x + 15, inputJoin.y + 50, 22, pseudoJoin, "../inc/font/PixelOperator.ttf", 255, 255, 255, FALSE);
	displaySprite(renderer, ok_button_Multi, 50, 340);
	displayText(renderer, 140, 430, 55, "OK", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
	/*-----------------------------------------*/

	/*-----------info box for JoinMenu---------*/
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
	SDL_RenderFillRect(renderer, &infoJoin);
	if (inputIpBtn == 1)
	{
		SDL_SetRenderDrawColor(renderer, 153, 153, 153, 185);
		SDL_RenderFillRect(renderer, &ipJoinBox);
	}
	else if (isIPValid == 1)
	{
		SDL_SetRenderDrawColor(renderer, 126, 190, 135, 185);
		SDL_RenderFillRect(renderer, &ipJoinBox);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 185);
		SDL_RenderFillRect(renderer, &ipJoinBox);
	}

	displayText(renderer, infoJoin.x + 10, infoJoin.y + 15, 22, "Saisir l'ip du serveur : ", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displayText(renderer, infoJoin.x + 15, infoJoin.y + 50, 22, ipJoin, "../inc/font/PixelOperator.ttf", 255, 255, 255, FALSE);
	displaySprite(renderer, ok_button_Multi, 480, 360);
	displayText(renderer, 590, 450, 55, "OK", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
	if ((isPseudoValid == 1) && (isIPValid == 1))
	{
		displaySprite(renderer, ok_button_Multi, 530, 450);
		displayText(renderer, 605, 540, 55, "JOIN", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
	}
	/*-----------------------------------------*/

	if (music_Multi_playing)
	{
		displaySprite(renderer, music_on_Multi, x - 175, y - 200);
	}
	else
	{
		displaySprite(renderer, music_off_Multi, x - 175, y - 200);
	}
}

int displayMenuMulti(int x, int y)
// Create a window with with x*y size (in px)
{

	SDL_Window *pWindow = NULL;
	SDL_Renderer *renderer = NULL;

	int xWinSize;
	int yWinSize;

	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		fprintf(stdout, "[MENU] Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}

	/* Initialisation TTF */
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Tactics Arena - Menu", SDL_WINDOWPOS_UNDEFINED,
							   SDL_WINDOWPOS_UNDEFINED,
							   x,
							   y,
							   SDL_WINDOW_SHOWN);

	if (!pWindow)
	{
		fprintf(stderr, "Erreur à la création de la fenetre : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		fprintf(stderr, "Erreur à la création du renderer\n");
		exit(EXIT_FAILURE);
	}

	setRendererDriver(renderer);

	// Launcher icon
	SDL_SetWindowIcon(pWindow, loadOptImage("../inc/sprites/goliath/sprite_indiv/64_64/front/Sprite_frontview_64.png"));

	if (pWindow)
	{
		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);
		int running = 1;

		while (running)
		{
			SDL_Event u;
			while (SDL_PollEvent(&u))
			{

				switch (u.type)
				{
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_WINDOWEVENT:
					switch (u.window.event)
					{
					case SDL_WINDOWEVENT_EXPOSED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_SHOWN:

						loadMultiMenuTextures(renderer);
						if (isHostMenu == 0 && isJoinMenu == 0)
						{

							/* Background image */
							displaySprite(renderer, background_Multi, 0, 0);
							/* Host button */
							displaySprite(renderer, host_button, 500, 350);
							/* Join button */
							displaySprite(renderer, join_button, 500, 415);
							/* Quit button */
							displaySprite(renderer, quit_button_Multi, x - 300, y - 190);
							/* Affiche en gros Tactics Arena */
							displayText(renderer, 300, 200, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, TRUE);
							/* Mentions de bas de menu */
							displayText(renderer, 5, y - 20, 15, "Projet L2 Informatique - BUTEL CHAUVIN DOUCET LAFAY", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
							if (music_Multi_playing)
							{
								displaySprite(renderer, music_on_Multi, x - 175, y - 200);
							}
							else
							{
								displaySprite(renderer, music_off_Multi, x - 175, y - 200);
							}
						}

						// SDL_RenderPresent(renderer);

						break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:

					if (verbose >= 2)
						printf("\nX: %d | Y: %d\n", u.motion.x, u.motion.y); // Debug console pos x & y on term

					// Bouton "Host"
					if (u.motion.x >= 576 && u.motion.x <= 723 && u.motion.y >= 449 && u.motion.y <= 488 && isHostMenu == 0 && isJoinMenu == 0)
					{
						isHostMenu = 1;
						if (verbose >= 2)
							printf("Host cliqué :) \n");

						dispHostMenu(renderer, x, y, indexMapMulti);
					}

					// Bouton "Join"
					else if (u.motion.x >= 575 && u.motion.x <= 724 && u.motion.y >= 513 && u.motion.y <= 554 && isHostMenu == 0 && isJoinMenu == 0)
					{
						isJoinMenu = 1;
						if (verbose >= 2)
							printf("Join cliqué :) \n");

						dispJoinMenu(renderer, x, y);
					}

					// PseudoBox Host
					else if (u.motion.x >= 150 && u.motion.x <= 401 && u.motion.y >= 404 && u.motion.y <= 432 && isHostMenu == 1 && isPseudoValid == 0)
					{
						inputPseudoBtn = 1;
					}

					//Input Host box
					else if (u.motion.x >= 126 && u.motion.x <= 242 && u.motion.y >= 640 && u.motion.y <= 683 && isHostMenu == 1 && serverStatus == 0)
					{
						serverStatus = 1;
						isPseudoValid = 1;
						char pseuTemp[50];
						strcpy(pseuTemp, pseudoSrv);
						//printf("\npseudo temp : %s\n",pseuTemp);
						for (int i = 0; i < (strlen(pseuTemp)); i++)
						{
							pseudoUser[i - 9] = pseuTemp[i];
							//printf("\nps | i : %d | char %c \n", i, pseuTemp[i]);
						}
						sprintf(mapMultiSelected, "%s", mapNameMulti);
						if (verbose >= 1)
							printf("\nTest User pseudo : %s\n", pseudoUser);
						if (verbose >= 1)
							printf("\n Test mapMultiSelected : %s \n", mapMultiSelected);

						pthread_create(&threadServ.thread_server, NULL, fn_server, NULL);
					}

					// MapHost ++
					else if (u.motion.x >= 400 && u.motion.x <= 426 && u.motion.y >= 493 && u.motion.y <= 520 && isHostMenu == 1)
					{
						indexMapMulti++;
						if (verbose >= 2)
							printf("index map multi %d\n", indexMapMulti);
						dispHostMenu(renderer, x, y, indexMapMulti);
					}

					// MapHost --
					else if (u.motion.x >= 50 && u.motion.x <= 74 && u.motion.y >= 496 && u.motion.y <= 520 && isHostMenu == 1)
					{
						indexMapMulti--;
						if (verbose >= 2)
							printf("index map multi %d\n", indexMapMulti);
						dispHostMenu(renderer, x, y, indexMapMulti);
					}

					// PseudoBox Join
					else if (u.motion.x >= 150 && u.motion.x <= 305 && u.motion.y >= 399 && u.motion.y <= 432 && isJoinMenu == 1)
					{
						inputPseudoBtn = 1;
						inputIpBtn = 0;
					}

					//Ok Btn Pseudo Join
					else if (u.motion.x >= 127 && u.motion.x <= 241 && u.motion.y >= 441 && u.motion.y <= 484 && isJoinMenu == 1 && isPseudoValid == 0)
					{
						isPseudoValid = 1;
						inputPseudoBtn = 0;

						char pseuTemp[50];
						strcpy(pseuTemp, pseudoJoin);

						for (int i = 0; i < (strlen(pseuTemp)); i++)
						{
							pseudoUser[i - 9] = pseuTemp[i];
						}
						if (verbose >= 1)
							printf("\nTest User pseudo : %s\n", pseudoUser);
					}

					// IPBox
					else if (u.motion.x >= 520 && u.motion.x <= 868 && u.motion.y >= 399 && u.motion.y <= 429 && isJoinMenu == 1)
					{
						inputIpBtn = 1;
						inputPseudoBtn = 0;
					}

					// OK Btn join
					else if (u.motion.x >= 556 && u.motion.x <= 668 && u.motion.y >= 461 && u.motion.y <= 500 && isJoinMenu == 1 && isIPValid == 0)
					{
						inputIpBtn = 0;
						isIPValid = 1;
						char ipTemp[85];

						strcpy(ipTemp, ipJoin);

						//printf("\nip temp : %s\n",ipTemp);
						for (int i = 5; i < (strlen(ipTemp)); i++)
						{
							ipSrv[i - 5] = ipTemp[i];
							//printf("\nipTemp[%d] | char %c \n", i, ipTemp[i]);
						}
						if (verbose >= 2)
							printf("\nTest IP Join : %s\n", ipSrv);
					}

					// Info Join set
					else if (u.motion.x >= 608 && u.motion.x <= 721 && u.motion.y >= 550 && u.motion.y <= 590 && isJoinMenu == 1 && isIPValid == 1 && isPseudoValid == 1 && isInfoJoinSet == 0)
					{
						isInfoJoinSet = 1;
						pthread_create(&threadCli.thread_client, NULL, fn_client, NULL);
					}

					// Start for host Btn after infoJoinSet
					else if (u.motion.x >= 606 && u.motion.x <= 722 && u.motion.y >= 550 && u.motion.y <= 594 && isHostMenu == 1 && isClientCo == 1)
					{
						serverStatus = 2;
					}

					// Nouveau boutton "QUIT"
					else if (u.motion.x >= 1000 && u.motion.x <= 1190 && u.motion.y >= 627 && u.motion.y <= 680)
					{
						if (isHostMenu == 1)
						{
							isHostMenu = 0;
						}
						if (isJoinMenu == 1)
						{
							isJoinMenu = 0;
						}

						closeWindow(pWindow);
						freeMultiMenuTextures();
						system("./TacticsArena");
					}

					// Switch musique ON/OFF
					else if (u.motion.x >= 1202 && u.motion.x <= 1250 && u.motion.y >= 627 && u.motion.y <= 680)
					{
						if (music_Multi_playing)
						{
							music_Multi_playing = 0;
							pauseMenuMusic();
							displaySprite(renderer, music_off_Multi, x - 175, y - 200);
						}
						else
						{
							music_Multi_playing = 1;
							resumeMenuMusic();
							displaySprite(renderer, music_on_Multi, x - 175, y - 200);
						}
					}
					else
					{
						inputIpBtn = 0;
						inputPseudoBtn = 0;
					}
					break;
				case SDL_KEYDOWN:
					if (u.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (inputPseudoBtn == 1 && isHostMenu == 1)
						{
							if (strlen(pseudoSrv) > 9)
							{
								pseudoSrv[strlen(pseudoSrv) - 1] = '\0';
								dispHostMenu(renderer, x, y, indexMapMulti);
							}
						}
						else if (inputPseudoBtn == 1 && isJoinMenu == 1)
						{
							if (strlen(pseudoJoin) > 9)
							{
								pseudoJoin[strlen(pseudoJoin) - 1] = '\0';
								dispJoinMenu(renderer, x, y);
							}
						}
						else if (inputIpBtn == 1 && isJoinMenu == 1)
						{
							if (strlen(ipJoin) > 5)
							{
								ipJoin[strlen(ipJoin) - 1] = '\0';
								dispJoinMenu(renderer, x, y);
							}
						}
					}
					break;
				case SDL_TEXTINPUT:
					if ((isHostMenu == 1) && (inputPseudoBtn == 1))
					{
						strcat(pseudoSrv, u.text.text);
						dispHostMenu(renderer, x, y, indexMapMulti);
					}
					else if ((isJoinMenu == 1) && (inputPseudoBtn == 1))
					{
						strcat(pseudoJoin, u.text.text);
						dispJoinMenu(renderer, x, y);
					}
					else if ((isJoinMenu == 1) && (inputIpBtn == 1))
					{
						strcat(ipJoin, u.text.text);
						dispJoinMenu(renderer, x, y);
					}
					break;
				case SDL_TEXTEDITING:
					if (isHostMenu == 1)
					{
						compo = u.edit.text;
						cursor = u.edit.start;
						selection_len = u.edit.length;
						dispHostMenu(renderer, x, y, indexMapMulti);
					}
					else if (isJoinMenu == 1)
					{
						compo = u.edit.text;
						cursor = u.edit.start;
						selection_len = u.edit.length;
						dispJoinMenu(renderer, x, y);
					}
					break;
				}
			}

			if (serverStatus == 4 && isJoinMenu == 1)
			{
				closeWindow(pWindow);
				freeMultiMenuTextures();
				stopMenuMusic(2);
				return 1;
			}
			else if (serverStatus != 3 && isHostMenu == 1)
			{
				if (verbose >= 3)
				{
					printf("Waiting status to be 3 \n");
				}
			}
			else if (serverStatus == 3 && isHostMenu == 1)
			{
				closeWindow(pWindow);
				freeMultiMenuTextures();
				stopMenuMusic(2);
				return 1;
			}

			if (isHostMenu == 1 || isJoinMenu == 1)
			{
				if (isJoinMenu == 1)
				{
					dispJoinMenu(renderer, x, y);
				}
				else
				{
					dispHostMenu(renderer, x, y, indexMapMulti);
				}
			}

			if (serverStatus != 4 && isJoinMenu == 1)
			{
				if (verbose >= 3)
				{
					printf("Waiting status to be 4 \n");
				}

				if (verbose >= 3)
					printf("Server status = %d \n", serverStatus);

				if (serverStatus >= 2)
				{
					displayText(renderer, 460, 630, 40, "Vous etes connecté !", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
				}
				else if (serverStatus == -2)
				{
					displayText(renderer, 450, 630, 40, "Impossible de se connecter", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
					displayText(renderer, 360, 673, 40, "Veuillez relancer le menu multi-joueur", "../inc/font/PixelOperator.ttf", 255, 255, 255, TRUE);
				}
			}

			SDL_Delay(16);
			SDL_RenderPresent(renderer);
		}
		closeWindow(pWindow);
		freeMultiMenuTextures();
		SDL_StopTextInput();
	}
	else
	{
		fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
	}
	return 0;
}
