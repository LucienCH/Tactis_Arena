
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


/* =============== CONSTANTES ================ */


#define _NB_TEXT_CACHE_ 1000    // Max number of cached texts
#define _CACHE_AGE_ 5           // Max age of a cached text before being removed


/* =============== VARIABLES ================ */


TextCache text_cache[_NB_TEXT_CACHE_];
int text_cache_size = 0;


/* =============== FONCTIONS ================ */


bool isInCache(char *content, int size, char *police, SDL_Color color)
// Returns true if the given text is in cache
{
    bool result = TRUE;

    for (int i = 0; i < text_cache_size; i++)
    {
        result = TRUE;
        // Check content
        result = result && (strcmp(content, text_cache[i].content) == 0);
        // Check police
        result = result && (strcmp(police, text_cache[i].police) == 0);
        // Check size
        result = result && (size == text_cache[i].size);
        // Check R color
        result = result && (color.r == text_cache[i].color.r);
        // Check G color
        result = result && (color.g == text_cache[i].color.g);
        // Check B color
        result = result && (color.b == text_cache[i].color.b);

        if (result == TRUE) break;
    }

    if (text_cache_size == 0) result = FALSE;

    return result;
}


err_t addToCache(char *content, int size, char *police, SDL_Color color, SDL_Surface *surface)
// Add the given texture (and its properties) to cache
{
    if (isInCache(content, size, police, color)) return ALREADY_IN_CACHE;

    if (verbose >= 1) printf("\033[36;01m[TEXT CACHE]\033[00m : Ajout du texte '%s' au cache à l'ID %d ...\n", content, text_cache_size);

    text_cache[text_cache_size].content = content;
    text_cache[text_cache_size].police = police;
    text_cache[text_cache_size].size = size;
    text_cache[text_cache_size].color = color;
    text_cache[text_cache_size].surface = surface;
    text_cache[text_cache_size].time = SDL_GetTicks();

    text_cache_size++;

    if (verbose >= 1) printf("\033[36;01m[TEXT CACHE]\033[00m : Taille du cache : %d\n", text_cache_size);

    return OK;
}


err_t destroyCacheEntry(int index)
// Destroy a cache entry
{
    if (verbose >= 1) printf("\033[36;01m[TEXT CACHE]\033[00m : Suppression du cache : %s (Age : %d)\n", text_cache[index].content, SDL_GetTicks()-text_cache[index].time);
    SDL_FreeSurface(text_cache[index].surface);
    text_cache[index].surface = NULL;
    //free(text_cache[index].content);
    //free(text_cache[index].police);
    text_cache[index].content = NULL;
    text_cache[index].police = NULL;
    if (verbose >= 1) printf("\033[36;01m[TEXT CACHE]\033[00m : Entree supprimee avec succes\n");
    text_cache_size--;

    return OK;
}


err_t slideCache(int index)
// Slide all the caches entries if an entry is destroyed
{
    for (int i = index; i < text_cache_size && text_cache[i].content; i++)
    {
        text_cache[i].content = malloc(sizeof(char) * strlen(text_cache[i+1].content));
        strcpy(text_cache[i].content, text_cache[i+1].content);
        text_cache[i].police = malloc(sizeof(char) * strlen(text_cache[i+1].police));
        strcpy(text_cache[i].police, text_cache[i+1].police);
        text_cache[i].color = text_cache[i+1].color;
        text_cache[i].size = text_cache[i+1].size;
        text_cache[i].time = text_cache[i+1].time;
        text_cache[i].surface = malloc(sizeof(SDL_Surface*));
        text_cache[i].surface = text_cache[i+1].surface;
    }

    return OK;
}


err_t clearOldCache()
// Clear old cache entries
{
    if (text_cache_size > 0)
    {
        for (int i = 0; i < text_cache_size; i++)
        {
            if (text_cache[i].time > (SDL_GetTicks()-(_CACHE_AGE_*1000)) || text_cache[i].time > SDL_GetTicks())
            {
                destroyCacheEntry(i);
                slideCache(i);
            }
        }
    }

    return OK;
}


SDL_Surface * getTextCache(char *content, int size, char *police, SDL_Color color)
// Returns the texture of a text if it exists in cache
{
    if (!isInCache(content, size, police, color)) exit(EXIT_FAILURE);

    SDL_Surface * surface = NULL;
    bool result = TRUE;

    for (int i = 0; i < text_cache_size; i++)
    {
        result = TRUE;
        // Check content
        result = result && (strcmp(content, text_cache[i].content) == 0);
        // Check police
        result = result && (strcmp(police, text_cache[i].police) == 0);
        // Check size
        result = result && (size == text_cache[i].size);
        // Check R color
        result = result && (color.r == text_cache[i].color.r);
        // Check G color
        result = result && (color.g == text_cache[i].color.g);
        // Check B color
        result = result && (color.b == text_cache[i].color.b);

        if (result == TRUE)
        {
            surface = text_cache[i].surface;
            text_cache[i].time = SDL_GetTicks();
            break;
        }
    }

    return surface;
}



err_t freeTextCache()
// Clear the entire cache
{
    for (int i = 0; i < text_cache_size; i++)
    {
        SDL_FreeSurface(text_cache[i].surface);
    }

    return OK;
}



err_t displayText(SDL_Renderer *renderer, int x, int y, int size, char *content, char *text_police, int r, int g, int b, bool caching)
// Displays text on the window
{
    SDL_Color color = {r, g, b};
    SDL_Rect txtDestRect;

    if (isInCache(content, size, text_police, color) && caching == TRUE)
    {

        // Création de la texture à partir de la surface
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, getTextCache(content, size, text_police, color));
        if(!text_tex){
            printf("\033[31;01m[TEXT CACHE ERROR]\033[00m : Erreur à la création du rendu du texte : %s\n", SDL_GetError());
            return SDL_ERROR;
        }

        txtDestRect.x = x;
        txtDestRect.y = y;

        SDL_QueryTexture(text_tex, NULL, NULL, &(txtDestRect.w), &(txtDestRect.h));

        /* Ajout du texte en noir */
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderCopy(renderer, text_tex, NULL, &txtDestRect);

        SDL_DestroyTexture(text_tex);
    }
    else
    {
        if (verbose >= 1 && caching) printf("\033[36;01m[TEXT CACHE]\033[00m : Texte non disponible dans le cache...\n");

        SDL_Surface *text = NULL;
        TTF_Font *police = NULL;

        // Chargement de la police
        if( (police = TTF_OpenFont(text_police, size)) == NULL){
            printf("\033[31;01m[TEXT CACHE ERROR]\033[00m : Erreur chargement initial font : %s\n", TTF_GetError());
            return SDL_ERROR;
        }

        // Création de la surface à partir du texte
        text = TTF_RenderUTF8_Blended(police, content, color);
        if(!text){
            printf("\033[31;01m[TEXT CACHE ERROR]\033[00m : Erreur à la création du texte : %s\n", SDL_GetError());
            return SDL_ERROR;
        }

        // Création de la texture à partir de la surface
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text);
        if(!text_tex){
            printf("\033[31;01m[TEXT CACHE ERROR]\033[00m : Erreur à la création du rendu du texte : %s\n", SDL_GetError());
            return SDL_ERROR;
        }

        if (caching == TRUE) addToCache(content, size, text_police, color, text);

        SDL_QueryTexture(text_tex, NULL, NULL, &(txtDestRect.w), &(txtDestRect.h));

        txtDestRect.x = x;
        txtDestRect.y = y;

        /* Ajout du texte en noir */
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderCopy(renderer, text_tex, NULL, &txtDestRect);

        TTF_CloseFont(police);
        if (caching == FALSE) SDL_FreeSurface(text);
        SDL_DestroyTexture(text_tex);
    }

	return OK;
}