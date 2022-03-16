
/* =============== DEPENDENCES =============== */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "game_window.h"
#include "substruct.h"
#include "common.h"
#include "textures.h"
#include "display.h"
#include "grid.h"
#include "text.h"
#include "print.h"


/* =============== CONSTANTES =============== */


#define _X_SIZE_ 30
#define _Y_SIZE_ 30
#define _NB_CLASSES_ 6
#define _NB_ANIM_ 6


/* =============== VARIABLES =============== */


int indexCharTable = 0;


/* =============== FONCTIONS =============== */


int addCharacterTexture(SDL_Renderer *renderer, char * name)
// Add to the CharTexture table all the animation of a class and its given direction
{
    int dirIndex = N;
    int animIndex = 1;
    char path[STR_LONG];
    char temp[STR_LONG];
    char *dirFolder;

    sprintf(path, "../inc/sprites/%s/sprite_indiv/64_64/", name);
    if(verbose == 3) printf("\033[36;01m[CHARACTERS]\033[00m : Chargement des textures du dossier : %s\n", path);

    // Textures face (64x64)
	charTextures[indexCharTable].texture_name = name;
    if(verbose == 3) printf("\033[36;01m[CHARACTERS]\033[00m : Chargement de la texture : %s\n", strcat(path, "front/Sprite_frontview_64.png"));
    sprintf(path, "../inc/sprites/%s/sprite_indiv/64_64/", name);
    charTextures[indexCharTable].front = loadTexture(renderer, loadOptImage(strcat(path, "front/Sprite_frontview_64.png")));

    // Textures 64x64
    sprintf(path, "../inc/sprites/%s/sprite_indiv/64_64/", name);
    for (dirIndex = N; dirIndex <= W; dirIndex++)
    {
        if (dirIndex == N) dirFolder = "haut_gauche";
        else if (dirIndex == E) dirFolder = "haut_droit";
        else if (dirIndex == S) dirFolder = "bas_droit";
        else if (dirIndex == W) dirFolder = "bas_gauche";

        for (animIndex = 1; animIndex <= _NB_ANIM_; animIndex++)
        {
            sprintf(temp, "%s%s/Sprite_%s%d.png", path, dirFolder, dirFolder, animIndex);
            if(verbose == 3) printf("\033[36;01m[CHARACTERS]\033[00m : Chargement de la texture : %s\n", temp);
            charTextures[indexCharTable].textures[dirIndex][animIndex-1] = loadTexture(renderer, loadOptImage(temp));
        }
    }

    // Textures 128x128
    sprintf(path, "../inc/sprites/%s/sprite_indiv/128_128/", name);
    for (dirIndex = W+1+N; dirIndex <= W+1+W; dirIndex++)
    {
        if (dirIndex == W+1+N) dirFolder = "haut_gauche";
        else if (dirIndex == W+1+E) dirFolder = "haut_droit";
        else if (dirIndex == W+1+S) dirFolder = "bas_droit";
        else if (dirIndex == W+1+W) dirFolder = "bas_gauche";

        for (animIndex = 128; animIndex < 128+_NB_ANIM_; animIndex++)
        {
            sprintf(temp, "%s%s/Sprite_%s%d.png", path, dirFolder, dirFolder, animIndex);
            if(verbose == 3) printf("\033[36;01m[CHARACTERS]\033[00m : Chargement de la texture : %s\n", temp);
            charTextures[indexCharTable].textures[dirIndex][animIndex-128] = loadTexture(renderer, loadOptImage(temp));
        }
    }

    indexCharTable++;

	if(verbose == 3) printf("\033[36;01m[CHARACTERS]\033[00m : Ajout de la texture de la classe [%s] à l'id %d\n", name, indexCharTable);

	return indexCharTable;
}



SDL_Texture * getCharTexture(char *name, Direction direction, int indexAnim)
// Returns the texture (64x64) of a given class in a given direction at a given animation index
{
    SDL_Texture * result = NULL;

    name[0] = tolower(name[0]);
    for (int i=0; i < _NB_CLASSES_; i++)
    {
        if (strcmp(name, charTextures[i].texture_name) == 0)
        {
            result = charTextures[i].textures[direction][indexAnim];
            break;
        }
    }

    if (result == NULL)
    {
        printf("\033[31;01m[CHARACTERS ERROR]\033[00m : Erreur lors du chargement de la texture [%s] (64x64)\n", name);
        exit(EXIT_FAILURE);
    }

    return result;
}



SDL_Texture * getBigCharTexture(char *name, Direction direction, int indexAnim)
// Returns the texture (128x128) of a given class in a given direction at a given animation index
{
    SDL_Texture * result = NULL;

    name[0] = tolower(name[0]);
    for (int i=0; i < _NB_CLASSES_; i++)
    {
        if (strcmp(name, charTextures[i].texture_name) == 0)
        {
            result = charTextures[i].textures[W+1+direction][indexAnim];
            break;
        }
    }

    if (result == NULL)
    {
        printf("\033[31;01m[CHARACTERS ERROR]\033[00m : Erreur lors du chargement de la texture [%s] (128x128)\n", name);
        exit(EXIT_FAILURE);
    }

    return result;
}



SDL_Texture * getCharFrontTexture(char *name)
// Returns the front texture of a given class
{
    name[0] = tolower(name[0]);
    SDL_Texture * result = NULL;

    for (int i=0; i < _NB_CLASSES_; i++)
    {
        if (strcmp(name, charTextures[i].texture_name) == 0)
        {
            result = charTextures[i].front;
            break;
        }
    }

    return result;
}



void setEntityToTile(Entity * entity, Coord tile)
// Set an entity to a specific tile
{    
    if (tile.x < 0 || tile.x >= _X_SIZE_ || tile.y < 0 || tile.y >= _Y_SIZE_)
    {
        printf("\033[31;01m[CHARACTERS ERROR]\033[00m : Coordinates out of boundaries\n");
        exit(EXIT_FAILURE);
    };

    if (entity == NULL || matrix == NULL)
    {
        printf("\033[31;01m[CHARACTERS ERROR]\033[00m : Pointer null on tile or entity\n");
        exit(EXIT_FAILURE);
    }

    matrix[tile.x*_X_SIZE_+tile.y].entity = entity;
}



void moveEntity(Coord from, Coord to)
// Move an entity already set on a tile to a new one
{
    if (from.x < 0 || from.x >= _X_SIZE_ || from.y < 0 || from.y >= _Y_SIZE_ || to.x < 0 || to.x >= _X_SIZE_ || to.y < 0 || to.y >= _Y_SIZE_)
    {
        printf("\033[31;01m[CHARACTERS ERROR]\033[00m : Coordinates out of boundaries\n");
        exit(EXIT_FAILURE);
    };

    if (matrix[from.x*_X_SIZE_+from.y].entity == NULL || matrix == NULL)
    {
        printf("\033[31;01m[CHARACTERS ERROR]\033[00m : Pointer null on tile or entity\n");
        exit(EXIT_FAILURE);
    }

    (*(matrix+to.x*_X_SIZE_+to.y)).entity = (*(matrix+from.x*_X_SIZE_+from.y)).entity;
    (*(matrix+to.x*_X_SIZE_+to.y)).entity->coords.x = to.x;
    (*(matrix+to.x*_X_SIZE_+to.y)).entity->coords.y = to.y;
    (*(matrix+from.x*_X_SIZE_+from.y)).entity = NULL;
    if (verbose >= 1) printf("\033[36;01m[CHARACTERS]\033[00m : Entité en [%d;%d] déplacée en [%d;%d]\n", from.x, from.y, (*(matrix+to.x*_X_SIZE_+to.y)).entity->coords.x, (*(matrix+to.x*_X_SIZE_+to.y)).entity->coords.y);
}


void switchEntities(Coord pos1, Coord pos2)
// Switch the positions of the 2 entities
{
    Tile * t1 = getTile(pos1);
    Tile * t2 = getTile(pos2);

    Entity * temp = t1->entity;
    t1->entity = t2->entity;
    t2->entity = temp;
    
    t2->entity->coords = pos2;
    t1->entity->coords = pos1;

    setSelected(pos1);
}



void createCharacters(Coord pos, int pdv)
// Create a new character at (x,y) pos
{
    Entity * entity = malloc(sizeof(Entity));
    entity->stats[0] = pdv;
    matrix[pos.x*_X_SIZE_+pos.y].entity = entity;
}



int displayCharacters(SDL_Renderer * renderer, TabTexture * cSprites, Entity * entity, int x, int y, int pxBase)
// Display the characters on the map
{
    // Display the character
    if (pxBase == 64)   displaySprite(renderer, getCharTexture(entity->cha_class->cla_name, entity->direction, entity->idAnim), x, y);
    else                displaySprite(renderer, getBigCharTexture(entity->cha_class->cla_name, entity->direction, entity->idAnim), x, y);

    // Display character's life points
    char temp[STR_SHORT];
    sprintf(temp, "%d", entity->stats[pv]);
    if (pxBase == 64)   displaySprite(renderer, getTexture(cSprites, "heart_icon"), x+(pxBase/4), y-(pxBase/4));
    else                displaySprite(renderer, getBigTexture(cSprites, "heart_icon"), x+(pxBase/4), y-(pxBase/4));
    displayText(renderer, x+(pxBase/2), y-(pxBase/4), (pxBase/64)*15, temp, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);

    return 0;
}
