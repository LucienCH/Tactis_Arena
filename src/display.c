
/* =============== DEPENDANCES =============== */


#include <stdio.h>
#include <string.h>
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
#include "game_window.h"
#include "text.h"


/* =============== CONSTANTES ================ */


#define _NB_MAX_MAPS_ 50
#define _X_SIZE_ 30                         // | Size of the grid
#define _Y_SIZE_ 30                         // |
#define _FPS_ 30							// Define at which frequency the game has to refresh
#define _NB_MAX_LOGS_ 11					// Define how many logs the screen can display (-1)
#define _MAX_SIZE_LOGS_ STR_LONG			// Max length of a log message
#define _LOG_DURATION_ 20					// Duration a log is displayed (in seconds)
#define _NB_TEXT_CACHE_ 1000


/* =============== VARIABLES ================ */


unsigned long long frames = 0;				// Frames
const int camSpeed = 5;						// Speed of the camera movements

int xWinSize, yWinSize;						// x and y sizes of the window
Coord mouse_position;
Log logs[_NB_MAX_LOGS_];
TextCache text_cache[_NB_TEXT_CACHE_];

/* =============== FONCTIONS ================ */


err_t setRendererDriver(SDL_Renderer *renderer)
// Set the default renderer driver to OpenGL for MacOS compatibility
{
	SDL_RendererInfo *global_renderer_info = malloc(sizeof(SDL_RendererInfo));

	if (SDL_GetRendererInfo(renderer, global_renderer_info) != 0)
    {
        printf("\033[31;01m[DISPLAY ERROR]\033[00m : Erreur lors de l'obtention des informations du renderer : %s\n", SDL_GetError());
		return SDL_ERROR;
    } else {
		if (verbose >= 1) printf("\033[36;01m[DISPLAY]\033[00m : Driver utilisé par le renderer : %s\n", global_renderer_info->name);
		if (verbose >= 1) printf("\033[36;01m[DISPLAY]\033[00m : Résolution maximale des textures : %dpx / %dpx\n", global_renderer_info->max_texture_width, global_renderer_info->max_texture_height);
	}

	if (!strcmp(global_renderer_info->name, "opengl")){
		if (SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"))
		{
			if (verbose >= 1) printf("\033[36;01m[DISPLAY]\033[00m : Le driver utilisé est maintenant OpenGL\n");
		} else {
			printf("\033[31;01m[DISPLAY ERROR]\033[00m : Erreur lors du changement de driver : %s\n", SDL_GetError());
			return SDL_ERROR;
		}
	}
	
	free(global_renderer_info);

	return OK;
}



err_t displaySprite(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y)
// Display a sprite on the window
{
	SDL_Rect imgDestRect;

	if (texture == NULL)
		printf("\033[31;01m[DISPLAY ERROR]\033[00m : Aucune texture à afficher\n");
	
	imgDestRect.x = x;
	imgDestRect.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &(imgDestRect.w), &(imgDestRect.h));
	SDL_RenderCopy(renderer, texture, NULL, &imgDestRect);

	return OK;
}



err_t closeWindow(SDL_Window *pWindow)
// Kill and close the window
{
	SDL_DestroyWindow(pWindow);
	TTF_Quit();
	Mix_Quit();
  	SDL_Quit();

    return OK;
}



err_t addLog(char * message)
// Add a new message to display as log
{
	int logsSize;

	for (logsSize = 0; logs[logsSize].message; logsSize++);
	
	if (strlen(message) >= _MAX_SIZE_LOGS_)
	{
		printf("\033[31;01m[LOGS ERROR]\033[00m : La log %s ne peut être ajoutée car sa taille est trop importante (max : %d caracteres).\n", message, _MAX_SIZE_LOGS_);
		return STR_TOO_LONG;
	}
	else
	{
		if (logsSize > 0)
		{
			for (int i = logsSize-1; i >= 0; i--)
			{
				if (i == _NB_MAX_LOGS_-2)
				{
					free(logs[_NB_MAX_LOGS_-2].message);
					logs[_NB_MAX_LOGS_-2].message = NULL;
				}
				else
				{
					free(logs[i+1].message);
					logs[i+1].message = NULL;
					logs[i+1].message = malloc(sizeof(char)*(strlen(logs[i].message)+1));
					strcpy(logs[i+1].message, logs[i].message);
					logs[i+1].time = logs[i].time;
					free(logs[i].message);
					logs[i].message = NULL;
				}
			}
		}

		logs[0].message = malloc(sizeof(char) * (strlen(message)+1));
		logs[0].time = SDL_GetTicks();
		strcpy(logs[0].message, message);
	}

	if (verbose >= 1) printf("\033[36;01m[LOGS]\033[00m : %d messages dans les logs.\n", logsSize);

	return OK;
}



err_t displayLog(SDL_Renderer * renderer, Coord pos)
// Display logs
{
	int x = pos.x;
	int y = pos.y;
	int logsSize;

	for (logsSize = 0; logs[logsSize].message; logsSize++);

	if (logsSize > 0)
	{
		for (int i=0; logs[i].message; i++)
		{
			displayText(renderer, x, y, 20, logs[i].message, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
			y += 20;
		}
	}

	return OK;
}



err_t removeOldLogs(Uint32 time)
// Remove the logs to old to be still displayed
{
	int logsSize;

	for (logsSize = 0; logs[logsSize].message; logsSize++);
	
	if (logsSize > 0)
	{
		for (int i = logsSize-1; i >= 0; i--)
		{
			if (logs[i].time < (time - (_LOG_DURATION_*1000)) || logs[i].time > time)
			{
				free(logs[i].message);
				logs[i].message = NULL;
			}
		}
	}

	return OK;
}
