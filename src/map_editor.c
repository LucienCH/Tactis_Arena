#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "game_window.h"
#include "map.h"
#include "characters.h"
#include "common.h"
#include "textures.h"
#include "grid.h"
#include "display.h"
#include "text.h"

#define _NB_MAX_MAPS_ 20

int isInSaveMenu = 0;
int isInLoadMenu = 0;

int flagClick = 0;
Coord moveInit;

// Map list
char *mapList[_NB_MAX_MAPS_];
int mapListIndex = 0;

// Contains the name of the map the user wants to save
char mapName[50] = "map_";
char *composition;
int mapNameIndex;
Sint32 cursor;
Sint32 selection_len;

int loadEditorTextures(SDL_Renderer * renderer, TabTexture * textures)
// Load all the map related textures
{
	int index;

	/*if(verbose>=2)printf("[GRAPHICS] Effacement des textures pré-existantes...\n");

	freeTextures(textures);*/

	if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Chargement des textures du jeu...\n");

	// Loading interface texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/interface/block_interface.png")),
						NULL,
						"interface");

	// Loading eraser texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/interface/eraser_64.png")),
						NULL,
						"eraser");

	// Loading selection hover texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/interface/editor_selection.png")),
						NULL,
						"editor_selection");

	// Loading green button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/buttons/ok_button.png")),
						NULL,
						"ok_button");

	// Loading orange button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/buttons/load_button.png")),
						NULL,
						"load_button");

	// Loading red button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/buttons/cancel_button.png")),
						NULL,
						"cancel_button");

	// Loading blue button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/buttons/fill_button.png")),
						NULL,
						"fill_button");

	// Loading blur effect texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadOptImage("../inc/img/interface/blur.png")),
						NULL,
						"blur");

	// Loading save menu texture
	index = addTextureToTable(	textures,
								loadTexture(renderer, loadOptImage("../inc/img/interface/saveMenu.png")),
								NULL,
								"save_menu");

	if(verbose>=1)printf("\033[36;01m[MAP_EDITOR]\033[00m : %d texture(s) chargée(s) !\n", index+1);

	return index+1;
}

int listMaps(char *mapList[])
// List all the maps and save them in mapList
{
	int index = 0;
	DIR * d;
	struct dirent *dir;

	d = opendir("../maps");
	if (!d){
		printf("\033[31;01m[MAP_EDITOR ERROR]\033[00m : Aucune map sauvegardée\n");
		return -1;
	}
	else
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
			{
				mapList[index] = dir->d_name;
				index++;
			}
		}
		closedir(d);
	}

	return index;
}

int displayLoadMenu(SDL_Renderer * renderer, char *mapList[], int xWinSize, int yWinSize, int index)
// Display a load map menu and load the selected one
{
	int xPos = (xWinSize/2)-300;
	int yPos = (yWinSize/2)-60;
	int xOpPos = xPos + 600;
	int yOpPos = yPos + 120;
	int nbMaps = listMaps(mapList);

	if (index <= 0) 		index = 0, 			mapListIndex = 0;
	if (index > nbMaps-1)	index = nbMaps-1,	mapListIndex = nbMaps-1;

	char mapName[50];
	sprintf(mapName, "%s", mapList[index]);

	displaySprite(renderer, getTexture(textures, "blur"), 0, 0);
	displaySprite(renderer, getTexture(textures, "save_menu"), xPos, yPos);
	displayText(renderer, xPos+10, yPos+5, 20, "Charger la map...", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displayText(renderer, xPos+10, yOpPos-25, 15, "[ENTER] pour valider, [ESC] pour annuler, [ARROWS] pour naviguer", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	if (mapList[index] != NULL) displayText(renderer, xPos+40, yPos+55, 20, mapName, "../inc/font/Pixels.ttf", 0, 0, 0, FALSE);
	if (index > 0) 		displayText(renderer, xPos+10, yPos+50, 30, "<", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	if (index < nbMaps-1) 	displayText(renderer, xOpPos-20, yPos+50, 30, ">", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);

	return 1;
}

int displaySaveMenu(SDL_Renderer * renderer, Tile * grid, int xWinSize, int yWinSize, char * mapName, int save)
// Display a save map menu and save the map
{
	int xPos = (xWinSize/2)-300;
	int yPos = (yWinSize/2)-60;
	int yOpPos = yPos + 120;

	displaySprite(renderer, getTexture(textures, "blur"), 0, 0);
	displaySprite(renderer, getTexture(textures, "save_menu"), xPos, yPos);
	displayText(renderer, xPos+10, yPos+5, 20, "Enregistrer la map...", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displayText(renderer, xPos+10, yOpPos-25, 20, "[ENTER] pour valider, [ESC] pour annuler", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	if (mapName != NULL) displayText(renderer, xPos+40, yPos+55, 20, mapName, "../inc/font/Pixels.ttf", 0, 0, 0, FALSE);

	return 1;
}

int saveMap(Tile * grid, const char * name)
// Save a map
{
	char mapName[40];

	sprintf(mapName, "../maps/%s", name);

	FILE * map;
	map = fopen(mapName, "wb");
	fwrite(grid, sizeof(Tile)*_X_SIZE_*_Y_SIZE_, 1, map);

	fclose(map);

	return 1;
}

int loadMap(Tile * grid, const char * name)
// Load a map
{
	char mapName[20];

	sprintf(mapName, "../maps/%s", name);
	if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Chargement de la map %s...\n", mapName);

	FILE * map;
	map = fopen(mapName, "rb");
	fread(grid, sizeof(Tile)*_X_SIZE_*_Y_SIZE_, 1, map);

	fclose(map);

	if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Map %s chargée avec succès !\n", mapName);

	return 1;
}

int changeTile(Tile * grid, int xpos, int ypos, int mx, int my, int pxBase, int toTile)
// Set the tile selected according to 2D iso from 2D coordinates
{
	int xIndex, yIndex, xIsoOrigin, yIsoOrigin, xTile, yTile;
	float cpAB, cpBC, cpDC, cpAD;

	// On déselectionne toutes les cases
	for (int i=0; i<_X_SIZE_; i++){
		for (int j=0; j<_Y_SIZE_; j++){
			(*(grid+i*_X_SIZE_+j)).selected = 0;
		}
	}

	// Position de l'origine de la map en 2D isométrique
	xIsoOrigin = xpos;
	yIsoOrigin = ypos+_Y_SIZE_*(pxBase/4);

	// Coordonnées 2D -> 2D iso
	xIndex = floor(((my-yIsoOrigin)/(pxBase/2) + ((mx-xIsoOrigin)/pxBase)))-1;
	yIndex = ceil((((mx-xIsoOrigin)/pxBase) - (my-yIsoOrigin)/(pxBase/2)))-1;

	if (my < yIsoOrigin){
		xIndex--;
		yIndex++;
	}

	xTile = xpos+((((xIndex+yIndex)/2)+1)*pxBase);
	yTile = ypos+((_Y_SIZE_-(yIndex-xIndex))*(pxBase/4)+(pxBase/4));

	// Calcul des coordonnées des 4 coins de la tile
	Coord A = { xTile, yTile };
	Coord B = { xTile + (pxBase / 2), yTile - (pxBase / 2) };
	Coord C = { xTile + pxBase, yTile };
	Coord D = { xTile + (pxBase / 2), yTile + (pxBase / 4) };

	// Calcul des coordonnées des vecteurs de la tile
	Vector AB = { B.x - A.x, B.y - A.y };
	Vector AM = { mx - A.x, my - A.y };
	Vector BC = { C.x - B.x, C.y - B.y };
	Vector BM = { mx - B.x, my - B.y };
	Vector DC = { C.x - D.x, C.y - D.y };
	Vector DM = { mx - D.x, my - D.y };
	Vector AD = { D.x - A.x, D.y - A.y };
	Vector A2M = { mx - A.x, my - A.y };
	cpAB = crossProduct(AB, AM);
	cpBC = crossProduct(BC, BM);
	cpDC = crossProduct(DC, DM);
	cpAD = crossProduct(AD, A2M);

	// Sélection de la case sélectionnée en fonction de la position relative du clic et des vecteurs
	if (cpAB > 0){
		xIndex--;
	} else if (cpBC > 0){
		yIndex++;
	} else if (cpDC < 0){
		xIndex++;
	} else if (cpAD < 0){
		yIndex--;
	}

	if (xIndex > _X_SIZE_-1 || yIndex > _Y_SIZE_-1 || xIndex < 0 || yIndex < 0) return 0;

	// DEBUG printf("[GRAPHICS] Case sélectionnée : %d, %d\n", xIndex, yIndex);
	(*(grid+xIndex*_X_SIZE_+yIndex)).tile_id = toTile;
	if (toTile == WATER || toTile == BLANK) (*(grid+xIndex*_X_SIZE_+yIndex)).walkable = 0;
	else (*(grid+xIndex*_X_SIZE_+yIndex)).walkable = 1;

	return 1;
}

void fillMap(Tile * grid, int block_id)
// Fill the map with the id_block block
{
	for (int i = 0; i < _X_SIZE_; i++)
	{
		for (int j = 0; j < _Y_SIZE_; j++)
		{
			(*(grid+i*_X_SIZE_+j)).tile_id = block_id;
			if (block_id == WATER) (*(grid+i*_X_SIZE_+j)).walkable = 0;
			else (*(grid+i*_X_SIZE_+j)).walkable = 1;
		}
	}
	if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Map remplie avec le bloc [%s] id %d\n", textures[block_id].texture_name, block_id);
}

Tile * build_map(MultiTile map[])
// Turns a multitile matrix to a tile matrix
{
	Tile map_result[_X_SIZE_][_Y_SIZE_];
	Trap_t temp_trap = {0, FALSE};

	for (int i=0; i < _X_SIZE_; i++)
	{
		for (int j=0; j < _Y_SIZE_; i++)
		{
			map_result[i][j].tile_id = map[i*_X_SIZE_+j].tile_id;
			map_result[i][j].trap = temp_trap;
			map_result[i][j].hovered = 0;
			map_result[i][j].selected = 0;
			map_result[i][j].entity = NULL;
			if (map_result[i][j].tile_id == WATER || map_result[i][j].tile_id == BLANK)
				map_result[i][j].walkable = FALSE;
			else
				map_result[i][j].walkable = TRUE;
		}
	}

	return &map_result[0][0];
}

int displayEditorMap(SDL_Renderer *renderer, int x, int y, int pxBase, int select, int xWinSize, int yWinSize)
// Display the map
{
	Coord selectionPos, blockPos, buttonPos;

	/* Le fond de la fenêtre sera blanc */
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

    for (int i=0; i < _X_SIZE_; i++){
        for (int j=(_Y_SIZE_-1); j >= 0; j--){

			blockPos.x = x+(j+1)*(pxBase/2)+(i+1)*(pxBase/2);
			blockPos.y = y+i*(pxBase/4)+(_Y_SIZE_-j)*(pxBase/4);

			if ((*(blankMatrix+i*_X_SIZE_+j)).tile_id == 1)
			{
				if ((*(blankMatrix+i*_X_SIZE_+j)).selected)
				{
					if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
					else				displaySprite(renderer, getBigTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
				} 
				else
				{
					if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "block"), blockPos.x, blockPos.y);
					else				displaySprite(renderer, getBigTexture(textures, "block"), blockPos.x, blockPos.y);
				}
			}
			else if ((*(blankMatrix+i*_X_SIZE_+j)).tile_id <= 7)
			{
				if (pxBase == 64)	displaySprite(renderer, textures[(*(blankMatrix+i*_X_SIZE_+j)).tile_id].texture, blockPos.x, blockPos.y);
				else 				displaySprite(renderer, textures[(*(blankMatrix+i*_X_SIZE_+j)).tile_id].big_texture, blockPos.x, blockPos.y);
			}

			if ((*(blankMatrix+i*_X_SIZE_+j)).entity != NULL)	displayCharacters(renderer, cSprites, (*(blankMatrix+i*_X_SIZE_+j)).entity, blockPos.x, blockPos.y-pxBase/1.6, pxBase);

			/*/ -- DEBUG Affichage des indices des tuiles --
			char pos[6];
			sprintf(pos, "%d,%d", i, j);
			displayText(renderer, imgDestRect.x+(pxBase/2)-10, imgDestRect.y+(pxBase/4), (pxBase/64)*10, pos, "../inc/font/Pixels.ttf", 255, 50, 50);
			// -- DEBUG --*/
        }
    }

	displaySprite(renderer, getTexture(textures, "interface"), 0, 0);
	displaySprite(renderer, getTexture(textures, "eraser"), 10, 50);

	for (int i = 1; i <= 7; i++)
	{
		if (i%2 == 0)		buttonPos.x = 10;
		else				buttonPos.x = 126;
		buttonPos.y = ceil(i/2)*74+50;
		displaySprite(renderer, textures[i].texture, buttonPos.x, buttonPos.y);
	}

	if (select%2 == 0)		selectionPos.x = 10;
	else					selectionPos.x = 126;
	selectionPos.y = ceil(select/2)*74+50;
	displaySprite(renderer, getTexture(textures, "editor_selection"), selectionPos.x, selectionPos.y);

	// Boutons
	displaySprite(renderer, getTexture(textures, "fill_button"), 0, yWinSize-160);
	displayText(renderer, 10, yWinSize-150, 20, "REMPLIR", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displaySprite(renderer, getTexture(textures, "cancel_button"), 0, yWinSize-120);
	displayText(renderer, 10, yWinSize-110, 20, "QUITTER", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displaySprite(renderer, getTexture(textures, "load_button"), 0, yWinSize-80);
	displayText(renderer, 10, yWinSize-70, 20, "OUVRIR UNE MAP", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	displaySprite(renderer, getTexture(textures, "ok_button"), 0, yWinSize-40);
	displayText(renderer, 10, yWinSize-30, 20, "ENREGISTRER", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);

	displayText(renderer, 20, 20, 20, "----- BLOCS -----", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);

	// -- DEBUG Affichage des coordonnées d'affichage de la map
	char str[12];
	sprintf(str, "%d, %d", x, y);
	displayText(renderer, 220, 20, 20, str, "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
	// -- DEBUG --

	if (isInSaveMenu == 1) displaySaveMenu(renderer, blankMatrix, xWinSize, yWinSize, mapName, 1);
	if (isInLoadMenu == 1) displayLoadMenu(renderer, mapList, xWinSize, yWinSize, mapListIndex);

	SDL_RenderPresent(renderer);

	return 1;
}

int createMapEditorWindow(int x, int y)
// Create a window with with x*y size (in px)
{
    // Le pointeur vers la fenetre
	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

	// Resolution of a bloc texture (can be 64 or 128)
	int PX = 64;

	// x and y pos where map is displayed
	int XPOS = 400;
	int YPOS = 170;

	moveInit.x = XPOS;
	moveInit.y = YPOS;

	// x and y sizes of the window
	int xWinSize;
	int yWinSize;

	// Block selected
	int SELECT = 1;

    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0 ) {
        printf("\033[31;01m[MAP_EDITOR ERROR]\033[00m : Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

	/* Initialisation TTF */
	if(TTF_Init() == -1) {
		printf("\033[31;01m[MAP_EDITOR ERROR]\033[00m : Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Tactics Arena - Editeur de map", 	SDL_WINDOWPOS_UNDEFINED,
												  					SDL_WINDOWPOS_UNDEFINED,
												  					x,
												  					y,
												  					SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if(!pWindow){
		printf("\033[31;01m[MAP_EDITOR ERROR]\033[00m : Erreur à la création de la fenetre : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		printf("\033[31;01m[MAP_EDITOR ERROR]\033[00m : Erreur à la création du rendu\n");
		exit(EXIT_FAILURE);
	}

	setRendererDriver(renderer);

	// Launcher icon
    SDL_SetWindowIcon(pWindow, loadOptImage("../inc/sprites/goliath/sprite_indiv/64_64/front/Sprite_frontview_64.png"));

	if( pWindow )
	{

		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

		// Chargement des textures
		loadMapTextures(renderer);
		if (loadEditorTextures(renderer, textures) == 0){
			printf("\033[31;01m[MAP_EDITOR ERROR]\033[00m : Erreur lors du chargement des textures (0 chargée)\n");
			return 0;
		}
		int start_seconds = SDL_GetTicks()/1000;
		int load_index = 0;
		while((SDL_GetTicks()/1000)-start_seconds < 3)
		{
			load_index++;
			SDL_SetRenderDrawColor(renderer, 21, 126, 172, 255);
			SDL_RenderClear(renderer);
			displayText(renderer, 200, yWinSize/2+120, 40, "Chargement des textures de l'editeur...", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
			displayText(renderer, 200, yWinSize/2, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, TRUE);
			if (load_index == 1)		displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "Ooo", "../inc/font/Aqua.ttf", 255, 255, 255, TRUE);
			else if (load_index == 2)	displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "oOo", "../inc/font/Aqua.ttf", 255, 255, 255, TRUE);
			else if (load_index == 3)	displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "ooO", "../inc/font/Aqua.ttf", 255, 255, 255, TRUE);
			SDL_Delay(100);
			SDL_RenderPresent(renderer);
			SDL_Delay(900);
		}

		/* Le fond de la fenêtre sera blanc */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Delay(1);
		
		displayEditorMap(renderer, XPOS, YPOS, PX, SELECT, xWinSize, yWinSize);

		SDL_RenderPresent(renderer);

		int running = 1;
		SDL_StartTextInput();
		while(running) {
			SDL_Event e;
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT: running = 0;
					break;
					case SDL_WINDOWEVENT:
						switch(e.window.event){
							case SDL_WINDOWEVENT_EXPOSED:
							case SDL_WINDOWEVENT_SIZE_CHANGED:
							case SDL_WINDOWEVENT_RESIZED:
							case SDL_WINDOWEVENT_HIDDEN:
							case SDL_WINDOWEVENT_SHOWN:

								SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

								/* Le fond de la fenêtre sera blanc */
                				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
								SDL_RenderClear(renderer);

								SDL_Delay(1);

								displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);

								SDL_RenderPresent(renderer);

							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:
						if (e.button.button == SDL_BUTTON_MIDDLE)
						{
							flagClick = 1;
							moveInit.x = e.motion.x;
							moveInit.y = e.motion.y;
						}

						//printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term
						if (e.motion.x <= 200 && isInSaveMenu == 0 && isInLoadMenu == 0)
						{
							if (e.motion.x >= 10 && e.motion.x <= 74 && e.motion.y >= 50 && e.motion.y <= 114)			SELECT = 0;
							else if (e.motion.x >= 126 && e.motion.x <= 190 && e.motion.y >= 50 && e.motion.y <= 114)	SELECT = 1;
							else if (e.motion.x >= 10 && e.motion.x <= 74 && e.motion.y >= 124 && e.motion.y <= 188)	SELECT = 2;
							else if (e.motion.x >= 126 && e.motion.x <= 190 && e.motion.y >= 124 && e.motion.y <= 188)	SELECT = 3;
							else if (e.motion.x >= 10 && e.motion.x <= 74 && e.motion.y >= 198 && e.motion.y <= 262)	SELECT = 4;
							else if (e.motion.x >= 126 && e.motion.x <= 190 && e.motion.y >= 198 && e.motion.y <= 262)	SELECT = 5;
							else if (e.motion.x >= 10 && e.motion.x <= 74 && e.motion.y >= 272 && e.motion.y <= 336)	SELECT = 6;
							else if (e.motion.x >= 126 && e.motion.x <= 190 && e.motion.y >= 272 && e.motion.y <= 336)	SELECT = 7;
							// Sauvegarder
							else if (e.motion.y >= yWinSize-40 && e.motion.y <= yWinSize) isInSaveMenu = 1;
							// Charger une map
							else if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-40) isInLoadMenu = 1;
							// Quitter
							else if (e.motion.y >= yWinSize-120 && e.motion.y <= yWinSize-80)
							{
								closeWindow(pWindow);
								freeTextures(textures);
								system("./TacticsArena");
							}
							// Remplir
							else if (e.motion.y >= yWinSize-160 && e.motion.y <= yWinSize-120)
							{
								fillMap(blankMatrix, SELECT);
							}
						}
						else
						{
							if (!isInSaveMenu) changeTile(blankMatrix, XPOS, YPOS, e.motion.x, e.motion.y, PX, SELECT);
						}

						displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);

					break;
					case SDL_MOUSEBUTTONUP:
						if (e.button.button == SDL_BUTTON_MIDDLE)
						{
							flagClick = 0;
						}
					break;
					case SDL_MOUSEWHEEL:
						if (e.wheel.y > 0)		// Scroll UP
						{
							if (PX == 64){
								PX = 128;
								if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Zoom In (Resolution : %dx%dpx)\n", pxBase, pxBase);
								XPOS *= 2;
								YPOS *= 2;
								displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
							}
						} else {				// Scroll DOWN
							if (PX == 128){
								PX = 64;
								if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Zoom Out (Resolution : %dx%dpx)\n", pxBase, pxBase);
								XPOS /= 2;
								YPOS /= 2;
								displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
							}
						}
					break;
					case SDL_KEYDOWN:
						if (isInSaveMenu == 0 && isInLoadMenu == 0){
							switch(e.key.keysym.sym)
							{
								case SDLK_KP_PLUS: 	// "+" key
									if (PX == 64){
										PX = 128;
										if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Zoom In (Resolution : %dx%dpx)\n", pxBase, pxBase);
										XPOS *= 2;
										YPOS *= 2;
										displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
									}
									break;
								case SDLK_KP_MINUS:	// "-" key
									if (PX == 128){
										PX = 64;
										if (verbose >= 1) printf("\033[36;01m[MAP_EDITOR]\033[00m : Zoom Out (Resolution : %dx%dpx)\n", pxBase, pxBase);
										XPOS /= 2;
										YPOS /= 2;
										displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
									}
									break;
								case SDLK_z:		// "z" key
									YPOS += (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
									break;
								case SDLK_q:		// "q" key
									XPOS += (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
									break;
								case SDLK_s:		// "s" key
									YPOS -= (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
									break;
								case SDLK_d:		// "d" key
									XPOS -= (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
									break;
							}
						}
						else
						{
							if (e.key.keysym.sym == SDLK_BACKSPACE)
							{
								if (strlen(mapName) > 4){
									mapName[strlen(mapName)-1] = '\0';
									displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
								}
							}
							else if (e.key.keysym.sym == SDLK_RETURN)
							{
								if (isInSaveMenu == 1) saveMap(blankMatrix, mapName);
								if (isInLoadMenu == 1)
								{
									listMaps(mapList);
									char mapSelected[50];
									sprintf(mapSelected, "%s", mapList[mapListIndex]);
									loadMap(blankMatrix, mapSelected);
								}
								isInSaveMenu = 0;
								isInLoadMenu = 0;
								displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
							}
							else if (e.key.keysym.sym == SDLK_ESCAPE)
							{
								isInSaveMenu = 0;
								isInLoadMenu = 0;
								displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
							}
							else if (e.key.keysym.sym == SDLK_LEFT)
							{
								if (isInLoadMenu == 1)
								{
									mapListIndex--;
									displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
								}
							}
							else if (e.key.keysym.sym == SDLK_RIGHT)
							{
								if (isInLoadMenu == 1)
								{
									mapListIndex++;
									displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
								}
							}
						}
						
					break;
					case SDL_TEXTINPUT:
						if (isInSaveMenu == 1)
						{
							strcat(mapName, e.text.text);
							displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
						}
					break;
					case SDL_TEXTEDITING:
						if (isInSaveMenu == 1)
						{
							composition = e.edit.text;
							cursor = e.edit.start;
							selection_len = e.edit.length;
							displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
						}
					case SDL_MOUSEMOTION:
						if (flagClick)
						{
							/*XPOS -= moveInit.x-e.motion.x;
							YPOS -= moveInit.y-e.motion.y;
							displayEditorMap(renderer, XPOS, YPOS, PX, grid, _X_SIZE_, _Y_SIZE_, SELECT,  xWinSize, yWinSize);*/
						}
					break;
				}
			}

			// Déplacement de la caméra grâce aux bords de l'écran
			if (e.motion.x <= xWinSize && e.motion.x >= xWinSize-20){
				XPOS -= (10*(PX/64));
				displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
			}
			else if (e.motion.x >= 200 && e.motion.x <= 220){
				XPOS += (10*(PX/64));
				displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
			}
			else if (e.motion.y < yWinSize && e.motion.y >= yWinSize-20){
				YPOS -= (10*(PX/64));
				displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
			}
			else if (e.motion.y <= 10){
				YPOS += (10*(PX/64));
				displayEditorMap(renderer, XPOS, YPOS, PX, SELECT,  xWinSize, yWinSize);
			}
			SDL_Delay(16);

		}
		closeWindow(pWindow);
		freeTextures(textures);
	} else {
		printf("\033[31;01m[MAP_EDITOR ERROR]\033[00m : Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return 1;
}