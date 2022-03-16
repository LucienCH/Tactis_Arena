/* =============== DEPENDENCES =============== */


#include <stdio.h>
#include <string.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "struct.h"
#include "characters.h"


/* =============== CONSTANTES =============== */


#define _NB_MAX_TEXTURES_ 200
#define _NB_CLASSES_ 6
#define _X_SIZE_ 30
#define _Y_SIZE_ 30


/* =============== DICTIONNAIRES DE TEXTURES =============== */


TabTexture cSprites[_NB_MAX_TEXTURES_];
TabTexture textures[_NB_MAX_TEXTURES_];
CharTexture charTextures[_NB_CLASSES_];
AnimTexture animTextures[NB_AB+1];


/* =============== VARIABLES =============== */


int pxBase = 64;							// Resolution of a bloc texture (can be 64 or 128)
int nbSprites = 0;

bool textures_loaded = FALSE;
bool cSprites_loaded = FALSE;
bool charTextures_loaded = FALSE;
bool animTextures_loaded = FALSE;


/* =============== FONCTIONS =============== */


SDL_Surface *optimize(SDL_Surface *surf)
// Optimizes the surface for the display format
{
	SDL_Surface *opt = SDL_ConvertSurfaceFormat(surf, surf->format->format, 0);

	if (opt)
	{
		SDL_FreeSurface(surf);
		return opt;
	}
	
	return surf;
}



void freeTextures(TabTexture * textures)
// Free all the textures in the given textures table
{
	int nbTextures = sizeof(textures) / sizeof(textures[0]);

	for (int i=0; i < nbTextures; i++)
	{
		if (textures[i].texture != NULL)
		{
			SDL_DestroyTexture(textures[i].texture);
			textures[i].texture = NULL;
		}

		if (textures[i].big_texture != NULL)
		{
			SDL_DestroyTexture(textures[i].big_texture);
			textures[i].big_texture = NULL;
		}
	}
}

err_t freeAllTextures()
{
	int charTextures_size = sizeof(charTextures) / sizeof(charTextures[0]);
	int animTextures_size = sizeof(textures) / sizeof(animTextures[0]);

	// Removes loaded general textures
	freeTextures(textures);

	// Removes loaded characters info and icons textures
	freeTextures(cSprites);

	// Removes loaded characters textures
	for (int i=0; i < charTextures_size; i++)
	{
		if (charTextures[i].front != NULL)
		{
			SDL_DestroyTexture(charTextures[i].front);
			charTextures[i].front = NULL;
		}
		for (int j=0; j < 8; j++)
		{
			for (int k=0; k < 6; k++)
			{
				if (charTextures[i].textures[j][k] != NULL)
				{
					SDL_DestroyTexture(charTextures[i].textures[j][k]);
					charTextures[i].textures[j][k] = NULL;
				}
			}
		}
	}

	// Removes loaded characters textures
	for (int i=0; i < animTextures_size; i++)
	{
		for (int j=0; j < 13; j++)
		{
			if (animTextures[i].spritesSmall[j] != NULL)
			{
				SDL_DestroyTexture(animTextures[i].spritesSmall[j]);
				animTextures[i].spritesSmall[j] = NULL;
			}
			if (animTextures[i].spritesBig[j] != NULL)
			{
				SDL_DestroyTexture(animTextures[i].spritesBig[j]);
				animTextures[i].spritesBig[j] = NULL;
			}
		}
	}

	textures_loaded = FALSE;
	cSprites_loaded = FALSE;
	charTextures_loaded = FALSE;
	animTextures_loaded = FALSE;

	indexCharTable = 0;

	return OK;
}



int addTextureToTable(TabTexture * texturesTable, SDL_Texture * texture, SDL_Texture * big_texture, char * texture_name)
// Add to the TabTexture table the given texture and its big one
{
	int index = 0;

	while (texturesTable[index].texture_name != NULL)
	{
		index++;
	}

	texturesTable[index].texture = texture;
	texturesTable[index].big_texture = big_texture;
	texturesTable[index].texture_name = texture_name;

	if (verbose >= 3) printf("\033[36;01m[TEXTURES]\033[00m : Ajout de la texture [%s] à l'id %d\n", texture_name, index);

	return index;
}



SDL_Texture * getTexture(TabTexture * textures, const char * texture_name)
// Return the texture associated with its name
{
	int index = 0;

	while (textures[index].texture_name != NULL)
	{
		index++;
	}

	for (int i=0; i < index; i++)
	{
		if (strcmp(textures[i].texture_name, texture_name) == 0)
		{
			return textures[i].texture;
		}
	}

	printf("\033[31;01m[TEXTURES ERROR]\033[00m : Aucune texture trouvée pour '%s' (64x64)\n", texture_name);
	exit(EXIT_FAILURE);
}



SDL_Texture * getBigTexture(TabTexture * textures, const char * texture_name)
// Return the texture associated with its name
{
	int index = 0;

	while (textures[index].texture_name != NULL)
	{
		index++;
	}

	for (int i=0; i < index; i++)
	{
		if (strcmp(textures[i].texture_name, texture_name) == 0)
		{
			return textures[i].big_texture;
		}
	}

	printf("\033[31;01m[TEXTURES ERROR]\033[00m : Aucune texture trouvée pour '%s' (128x128)\n", texture_name);
	exit(EXIT_FAILURE);
}



SDL_Surface * loadOptImage(char * img)
// Load a PNG image into a surface
{
	SDL_RWops *rwop = NULL;
	SDL_Surface *surface = NULL;

	rwop=SDL_RWFromFile(img, "rb");
	surface=IMG_LoadPNG_RW(rwop);
	if(!surface) {
		printf("\033[31;01m[TEXTURES ERROR]\033[00m : Erreur lors du chargement de l'image %s : %s\n", img, IMG_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeRW(rwop);
	rwop = NULL;

	return optimize(surface);
}



SDL_Surface * loadImage(char * img)
// Load a PNG image into a surface
{
	SDL_RWops *rwop = NULL;
	SDL_Surface *surface = NULL;

	rwop=SDL_RWFromFile(img, "rb");
	surface=IMG_LoadPNG_RW(rwop);
	if(!surface) {
		printf("\033[31;01m[TEXTURES ERROR]\033[00m : Erreur lors du chargement de l'image %s : %s\n", img, IMG_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeRW(rwop);
	rwop = NULL;

	return surface;
}



SDL_Texture * loadTexture(SDL_Renderer * renderer, SDL_Surface * surface)
// Create a texture from a surface
{
	SDL_Texture * texture = NULL;

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(!texture){
		printf("\033[31;01m[TEXTURES ERROR]\033[00m : Erreur lors de la creation de la texture : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeSurface(surface);
	surface = NULL;

	return texture;
}



int loadSprites(SDL_Renderer * renderer, TabTexture * cSprites)
// Load all the textures needed for the characters
{
    int nbSprites = 0;
    int nbChar = 0;

    freeTextures(cSprites);

    if (verbose >= 1) printf("\033[36;01m[CHARACTERS]\033[00m : Chargement des textures des personnages...\n");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/icons/heart_16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/icons/heart_32.png")),
                        "heart_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/icons/mv_icon_16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/icons/mv_icon_32.png")),
                        "mv_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/icons/star_icon_16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/icons/star_icon_32.png")),
                        "star_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/icons/atk_icon_16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/icons/atk_icon_32.png")),
                        "atk_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/icons/magic_icon_16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/icons/magic_icon_32.png")),
                        "magic_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/icons/ra_16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/icons/ra_32.png")),
                        "ra_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/icons/rm_16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/icons/rm_32.png")),
                        "rm_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/onlyspeed16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/onlyspeed32.png")),
                        "speed_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/onlyvision16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/onlyvision32.png")),
                        "vision_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/atkup16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/atkup32.png")),
                        "atk_up");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/atkdown16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/atkdown32.png")),
                        "atk_down");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/magicup16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/magicup32.png")),
                        "magic_up");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/magicdown16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/magicdown32.png")),
                        "magic_down");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/raup16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/raup32.png")),
                        "ra_up");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/radown16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/radown32.png")),
                        "ra_down");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/rmup16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/rmup32.png")),
                        "rm_up");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/rmdown16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/rmdown32.png")),
                        "rm_down");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/speedup16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/speedup32.png")),
                        "speed_up");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/speeddown16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/speeddown32.png")),
                        "speed_down");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/visionup16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/visionup32.png")),
                        "vis_up");

	addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/visiondown16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/visiondown32.png")),
                        "vis_down");

	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/freezed16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/freezed32.png")),
                        "frozen");

	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/blessed16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/blessed32.png")),
                        "blessed");

	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/burned16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/burned32.png")),
                        "burning");

	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/crippled16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/crippled32.png")),
                        "crippled");
				
	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/guarding16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/guarding32.png")),
                        "guarding");

	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/jailed16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/jailed32.png")),
                        "jailed");
			
	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/paralyzed16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/paralyzed32.png")),
                        "paralyzed");

	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/piercing16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/piercing32.png")),
                        "piercing");

	addTextureToTable(  cSprites,
                        NULL,//loadTexture(renderer, loadOptImage("../inc/img/status/16_16/summoned16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/summoned32.png")),
                        "summoned");

	nbSprites = addTextureToTable(  cSprites,
                        loadTexture(renderer, loadOptImage("../inc/img/status/16_16/provoke16.png")),
                        loadTexture(renderer, loadOptImage("../inc/img/status/32_32/provoke32.png")),
                        "provoke");

    addCharacterTexture(renderer, "angel");
    addCharacterTexture(renderer, "berserker");
    addCharacterTexture(renderer, "goliath");
    addCharacterTexture(renderer, "mage");
    addCharacterTexture(renderer, "ranger");
    nbChar = addCharacterTexture(renderer, "valkyrie");

    if (verbose >= 1) printf("\033[36;01m[CHARACTERS]\033[00m : %d texture(s) d'icone(s) chargée(s) !\n", nbSprites+1);
    if (verbose >= 1) printf("\033[36;01m[CHARACTERS]\033[00m : %d texture(s) de personnage(s) chargée(s) !\n", nbChar+1);

	cSprites_loaded = TRUE;
	charTextures_loaded = TRUE;

    return nbSprites;
}



int loadMapTextures(SDL_Renderer *renderer)
// Load all the map related textures
{
	int index;

	if (verbose >= 1)
		printf("\033[36;01m[TEXTURES]\033[00m : Effacement des textures pré-existantes...\n");

	freeTextures(textures);

	if (verbose >= 1)
		printf("\033[36;01m[TEXTURES]\033[00m : Chargement des textures du jeu...\n");

	// Loading blank pattern textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/blank_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/blank_128.png")),
					  "blank");

	// Loading non-selected pattern textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_128.png")),
					  "block");

	// Loading blue selected pattern textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_blue_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_blue_128.png")),
					  "blue_selected");

	// Loading red selected pattern textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_red_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_red_128.png")),
					  "red_selected");

	// Loading water block textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/water_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/water_128.png")),
					  "water");

	// Loading sand block textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/sand_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/sand_128.png")),
					  "sand");

	// Loading ice block textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/ice_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/ice_128.png")),
					  "ice");

	// Loading snow block textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_snow_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/blocks/block_snow_128.png")),
					  "snow");

	// Loading trap textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/sprites/traps/Beartrap64.png")),
					  loadTexture(renderer, loadOptImage("../inc/sprites/traps/Beartrap128.png")),
					  "trap");

	// Loading selection textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/selection_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/interface/selection_128.png")),
					  "selection");

	// Loading selection hover textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/hover_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/interface/hover_128.png")),
					  "selection_hover");

	// Loading ability range textures
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/ability_range_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/interface/ability_range_128.png")),
					  "ability_range");

	// Loading arrow right texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/icons/arrow_right_64.png")),
					  NULL,
					  "arrow_right");

	// Loading arrow left texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/icons/arrow_left_64.png")),
					  NULL,
					  "arrow_left");

	// Loading arrow up texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/icons/arrow_up_64.png")),
					  NULL,
					  "arrow_up");

	// Loading arrow down texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/icons/arrow_down_64.png")),
					  NULL,
					  "arrow_down");

	// Red team texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/red_team_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/interface/red_team_128.png")),
					  "red_team");

	// Blue team texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/blue_team_64.png")),
					  loadTexture(renderer, loadOptImage("../inc/img/interface/blue_team_128.png")),
					  "blue_team");

	// Loading attack logo
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/attack_logo_64.png")),
					  NULL,
					  "attack");

	// Loading locked attack logo
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/locked_attack_logo_64.png")),
					  NULL,
					  "locked_attack");

	// Loading frozen attack logo
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/frozen_attack_logo_64.png")),
					  NULL,
					  "frozen_attack");

	// Loading crippled attack logo
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/crippled_attack_logo_64.png")),
					  NULL,
					  "crippled_attack");

	// Loading move logo
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/move_logo_64.png")),
					  NULL,
					  "move");

	// Clockwise turn icon
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/clockwise_icon_64.png")),
					  NULL,
					  "turn_right");

	// Anti-clockwise turn icon
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/anti_clockwise_icon_64.png")),
					  NULL,
					  "turn_left");

	// Loading end of turn button
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/turn_end_grey.png")),
					  NULL,
					  "end_turn");

	// Loading end of turn button (hover)
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/turn_end_hover.png")),
					  NULL,
					  "end_turn_hover");

	// Loading end of turn button (locked)
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/locked_turn_end.png")),
					  NULL,
					  "locked_end_turn");

	// Loading dead character texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/dead_64.png")),
					  NULL,
					  "dead_char");

	// Loading detained char texture
	addTextureToTable(textures,
					  loadTexture(renderer, loadOptImage("../inc/img/interface/detained_64.png")),
					  NULL,
					  "detained_char");

	// Loading ID card texture
	index = addTextureToTable(textures,
							  loadTexture(renderer, loadOptImage("../inc/img/interface/id_card_2.png")),
							  NULL,
							  "id_card");

	if (verbose >= 1)
		printf("\033[36;01m[TEXTURES]\033[00m : %d texture(s) chargée(s) !\n", index + 1);

	textures_loaded = TRUE;

	return index + 1;
}