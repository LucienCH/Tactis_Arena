/**
 * \file map.c
 * \brief Map related functions
 * \details Contains all functions required for map and interface
 * \author Thibault DOUCET
 * \version 0.9.9
 * \date 30/04/2020
 */

/* =============== DEPENDENCES =============== */

#include <stdio.h>
#include <math.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "game_window.h"
#include "characters.h"
#include "common.h"
#include "grid.h"
#include "gameplay.h"
#include "turn.h"
#include "display.h"
#include "textures.h"
#include "init.h"
#include "string.h"
#include "border.h"
#include "text.h"
#include "animations.h"

/* =============== CONSTANTES =============== */

#define _NB_MAX_TEXTURES_ 50

/* =============== VARIABLES =============== */

char selectedAbilityDesc[STR_LONG];
char hoverAbilityDesc[STR_LONG];
Coord borderTab[MAXRANGE];
Coord rangeTab[_X_SIZE_ * _Y_SIZE_];
Coord drawPos = {-1, -1};
Entity *selectedEntity;

/* =============== FONCTIONS =============== */

float crossProduct(Vector AB, Vector AC)
// Renvoie le produit vectoriel
{
	return ((AB.y * AC.x) - (AB.x * AC.y));
}

int hoverTile(int xpos, int ypos, int mx, int my)
// Set the tile selected according to 2D iso from 2D coordinates
{
	int xIndex, yIndex, xIsoOrigin, yIsoOrigin, xTile, yTile;
	float cpAB, cpBC, cpDC, cpAD;
	Entity *tempEntity;

	unhover();

	// Position de l'origine de la map en 2D isométrique
	xIsoOrigin = xpos;
	yIsoOrigin = ypos + _Y_SIZE_ * (pxBase / 4);

	// Coordonnées 2D -> 2D iso
	xIndex = floor(((my - yIsoOrigin) / (pxBase / 2) + ((mx - xIsoOrigin) / pxBase))) - 1;
	yIndex = ceil((((mx - xIsoOrigin) / pxBase) - (my - yIsoOrigin) / (pxBase / 2))) - 1;

	if (my < yIsoOrigin)
	{
		xIndex--;
		yIndex++;
	}

	xTile = xpos + ((((xIndex + yIndex) / 2) + 1) * pxBase);
	yTile = ypos + ((_Y_SIZE_ - (yIndex - xIndex)) * (pxBase / 4) + (pxBase / 4));

	// Calcul des coordonnées des 4 coins de la tile
	Coord A = {xTile, yTile};
	Coord B = {xTile + (pxBase / 2), yTile - (pxBase / 2)};
	Coord C = {xTile + pxBase, yTile};
	Coord D = {xTile + (pxBase / 2), yTile + (pxBase / 4)};

	// Calcul des coordonnées des vecteurs de la tile
	Vector AB = {B.x - A.x, B.y - A.y};
	Vector AM = {mx - A.x, my - A.y};
	Vector BC = {C.x - B.x, C.y - B.y};
	Vector BM = {mx - B.x, my - B.y};
	Vector DC = {C.x - D.x, C.y - D.y};
	Vector DM = {mx - D.x, my - D.y};
	Vector AD = {D.x - A.x, D.y - A.y};
	Vector A2M = {mx - A.x, my - A.y};
	cpAB = crossProduct(AB, AM);
	cpBC = crossProduct(BC, BM);
	cpDC = crossProduct(DC, DM);
	cpAD = crossProduct(AD, A2M);

	// Sélection de la case sélectionnée en fonction de la position relative du clic et des vecteurs
	if (cpAB > 0)
	{
		xIndex--;
	}
	else if (cpBC > 0)
	{
		yIndex++;
	}
	else if (cpDC < 0)
	{
		xIndex++;
	}
	else if (cpAD < 0)
	{
		yIndex--;
	}

	if (xIndex > _X_SIZE_ - 1 || yIndex > _Y_SIZE_ - 1 || xIndex < 0 || yIndex < 0)
	{
		return 0;
	}

	(*(matrix + xIndex * _X_SIZE_ + yIndex)).hovered = 1;

	if (selected_ability != -1)
	{
		Coord center = {xIndex, yIndex};
		tempEntity = getEntity(getSelectedPos());
		if (selected_ability != Mvt)
		{
			if (tempEntity->cha_class->cla_abilities[selected_ability % NUM_AB].nb_coords > 1 && isInRange(borderTab, center))
			{
				for (int i = 0; i < tempEntity->cha_class->cla_abilities[selected_ability % NUM_AB].nb_coords; i++)
				{
					Coord highlight = add_coords(center, *((*(tempEntity->cha_class->cla_abilities[selected_ability % NUM_AB].coord)) + i));
					if (isInGrid(highlight))
						setHovered(highlight);
				}
			}
		}
		else
		{
			setHovered(center);
		}
	}

	return 1;
}

int selectTile(int xpos, int ypos, int mx, int my)
// Set the tile selected according to 2D iso from 2D coordinates
{
	int xIndex, yIndex, xIsoOrigin, yIsoOrigin, xTile, yTile;
	float cpAB, cpBC, cpDC, cpAD;

	// On déselectionne toutes les cases
	unselect();

	// Position de l'origine de la map en 2D isométrique
	xIsoOrigin = xpos;
	yIsoOrigin = ypos + _Y_SIZE_ * (pxBase / 4);

	// Coordonnées 2D -> 2D iso
	xIndex = floor(((my - yIsoOrigin) / (pxBase / 2) + ((mx - xIsoOrigin) / pxBase))) - 1;
	yIndex = ceil((((mx - xIsoOrigin) / pxBase) - (my - yIsoOrigin) / (pxBase / 2))) - 1;

	if (my < yIsoOrigin)
	{
		xIndex--;
		yIndex++;
	}

	xTile = xpos + ((((xIndex + yIndex) / 2) + 1) * pxBase);
	yTile = ypos + ((_Y_SIZE_ - (yIndex - xIndex)) * (pxBase / 4) + (pxBase / 4));

	// Calcul des coordonnées des 4 coins de la tile
	Coord A = {xTile, yTile};
	Coord B = {xTile + (pxBase / 2), yTile - (pxBase / 2)};
	Coord C = {xTile + pxBase, yTile};
	Coord D = {xTile + (pxBase / 2), yTile + (pxBase / 4)};

	// Calcul des coordonnées des vecteurs de la tile
	Vector AB = {B.x - A.x, B.y - A.y};
	Vector AM = {mx - A.x, my - A.y};
	Vector BC = {C.x - B.x, C.y - B.y};
	Vector BM = {mx - B.x, my - B.y};
	Vector DC = {C.x - D.x, C.y - D.y};
	Vector DM = {mx - D.x, my - D.y};
	Vector AD = {D.x - A.x, D.y - A.y};
	Vector A2M = {mx - A.x, my - A.y};
	cpAB = crossProduct(AB, AM);
	cpBC = crossProduct(BC, BM);
	cpDC = crossProduct(DC, DM);
	cpAD = crossProduct(AD, A2M);

	// Sélection de la case sélectionnée en fonction de la position relative du clic et des vecteurs
	if (cpAB > 0)
	{
		xIndex--;
	}
	else if (cpBC > 0)
	{
		yIndex++;
	}
	else if (cpDC < 0)
	{
		xIndex++;
	}
	else if (cpAD < 0)
	{
		yIndex--;
	}

	if (xIndex > _X_SIZE_ - 1 || yIndex > _Y_SIZE_ - 1 || xIndex < 0 || yIndex < 0)
	{
		selected_ability = -1;
		selectedEntity = NULL;
		return 0;
	}

	if (verbose >= 1)
		printf("\033[36;01m[MAP]\033[00m : Case sélectionnée : %d, %d\n", xIndex, yIndex);
	(*(matrix + xIndex * _X_SIZE_ + yIndex)).selected = 1;
	Coord selectedTile = {xIndex, yIndex};
	if (selectedEntity != NULL)
	{
		action act = {selectedEntity->cha_id, selectedTile, selected_ability};
		if (selected_ability != -1)
		{
			if (Cast_check(act, borderTab))
			{
				Entity *caster = selectedEntity;
				action_set(act);
				selected_ability = -1;
				setSelected(caster->coords);
			}
			else
			{
				selected_ability = -1;
				setSelected(selectedEntity->coords);
			}
		}
		else
		{
			selectedEntity = getEntity(selectedTile);
		}
	}
	else
	{
		selectedEntity = getEntity(selectedTile);
	}

	return 1;
}

int displayAbilities(SDL_Renderer *renderer)
// Display the abilities menu
{
	Entity *tempEntity = getEntity(getSelectedPos());
	// Abilities icons
	if (selected_ability != Last_Sacrifice)
	{
		castabilityId moveable = able_ability(tempEntity, Mvt, FALSE);
		if (moveable == Castable_c)
		{
			displaySprite(renderer, getTexture(textures, "move"), 16, yWinSize - 80);
			displayText(renderer, 21, yWinSize - 80 + 5, 20, "1", "../inc/font/Pixels.ttf", 49, 174, 196, FALSE);
		}
		else
		{
			if (moveable == Crippled_c)
				displaySprite(renderer, getTexture(textures, "crippled_attack"), 16, yWinSize - 80);
			else if (moveable == Frozen_c)
				displaySprite(renderer, getTexture(textures, "frozen_attack"), 16, yWinSize - 80);
			else if (moveable == Locked_c)
				displaySprite(renderer, getTexture(textures, "locked_attack"), 16, yWinSize - 80);

			displayText(renderer, 21, yWinSize - 80 + 5, 20, "1", "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
		}
		for (int i = 0; i < 4; i++)
		{
			char abCost[10];
			char abCooldown[10];
			int cost = get_cost(tempEntity, tempEntity->cha_class->cla_abilities[i].ab_id);
			int cd = get_cooldown(tempEntity, tempEntity->cha_class->cla_abilities[i].ab_id);
			sprintf(abCost, "%d", cost);
			sprintf(abCooldown, "%d", cd);
			castabilityId castable = able_ability(tempEntity, tempEntity->cha_class->cla_abilities[i].ab_id, FALSE);
			if (castable == Castable_c)
			{
				displaySprite(renderer, getTexture(textures, "attack"), 16 + (i + 1) * 80, yWinSize - 80);
				displayText(renderer, 16 + (i + 1) * 80 + 5, yWinSize - 80 + 5, 20, abCost, "../inc/font/Pixels.ttf", 49, 174, 196, FALSE);
				if (cd != 0)
					displayText(renderer, 16 + (i + 1) * 80 + 49, yWinSize - 35, 20, abCooldown, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
			}
			else
			{
				if (castable == Crippled_c)
					displaySprite(renderer, getTexture(textures, "crippled_attack"), 16 + (i + 1) * 80, yWinSize - 80);
				else if (castable == Frozen_c)
					displaySprite(renderer, getTexture(textures, "frozen_attack"), 16 + (i + 1) * 80, yWinSize - 80);
				else if (castable == Locked_c)
					displaySprite(renderer, getTexture(textures, "locked_attack"), 16 + (i + 1) * 80, yWinSize - 80);

				displayText(renderer, 16 + (i + 1) * 80 + 5, yWinSize - 80 + 5, 20, abCost, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
				if (cd != 0)
					displayText(renderer, 16 + (i + 1) * 80 + 49, yWinSize - 35, 20, abCooldown, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
			}
		}
		displaySprite(renderer, getTexture(textures, "turn_right"), 16 + 5 * 80, yWinSize - 80);
		displaySprite(renderer, getTexture(textures, "turn_left"), 16 + 6 * 80, yWinSize - 80);
	}
	else
	// Dead allies menu for last sacrifice ability
	{
		Entity *deadAllies[count_dead_allies(tempEntity)];
		get_dead_allies(tempEntity, deadAllies);
		for (int i = 0; i < count_dead_allies(tempEntity); i++)
		{
			displaySprite(renderer, getCharFrontTexture(deadAllies[i]->cha_class->cla_name), 16 + (i)*80 + 5, yWinSize - 80 + 5);
		}
	}

	return 0;
}

char *clearStr(char *str)
{
	for (int i = 0; i <= strlen(str); i++)
	{
		str[i] = '\0';
	}

	return str;
}


int displayInterface(SDL_Renderer *renderer)
// Display the UI
{
	Entity *tempEntity = getEntity(getSelectedPos());
	char passive[100];
	char pv_text[STR_SHORT];
	char pa_text[STR_SHORT];
	char pm_text[STR_SHORT];
	char vis_text[STR_SHORT];
	char atk_text[STR_SHORT];
	char magic_text[STR_SHORT];
	char ra_text[STR_SHORT];
	char rm_text[STR_SHORT];
	char duration[STR_SHORT];
	int xBuff;
	int yBuff;
	List_Elem *mod;

	SDL_Rect statDesc;
	statDesc.x = 10;
	statDesc.y = 200;
	statDesc.w = 400;
	statDesc.h = 25;

	SDL_Rect buffIcon;
	buffIcon.w = 32;
	buffIcon.h = 32;

	SDL_Rect portrait;
	portrait.w = 64;
	portrait.h = 100;

	char cdText[STR_LONG];
	char damageText[STR_LONG];
	char descText[STR_LONG];
	int yDesc = 110;
	Ability * temp_ab = NULL;

	if (tempEntity != NULL)
	{
		if (is_ally(tempEntity))
		{
			displayAbilities(renderer);

			if (selected_ability != -1)
			{
				temp_ab = ab_from_id(selected_ability, tempEntity);
				if (temp_ab->ab_cooldown != 0 && selected_ability != Mvt)
				{
					sprintf(cdText, "Cooldown : %d turn(s)", temp_ab->ab_cooldown);
					displayText(renderer, 16, yWinSize - yDesc, 20, cdText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
					yDesc += 20;
				}
				if (temp_ab->damage != NULL && selected_ability != Mvt)
				{
					if ((*(temp_ab->damage))->type == atk)
					{
						sprintf(damageText, "Base damage : %.0f (Physic)", (*(temp_ab->damage))->multiplier * tempEntity->stats[atk]);
						displayText(renderer, 16, yWinSize - yDesc, 20, damageText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
					}
					else if ((*(temp_ab->damage))->type == magic)
					{
						sprintf(damageText, "Base damage : %.0f (Magic)", (*(temp_ab->damage))->multiplier * tempEntity->stats[magic]);
						displayText(renderer, 16, yWinSize - yDesc, 20, damageText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
					}
					yDesc += 20;
				}
				sprintf(descText, "%s", get_desc(tempEntity, selected_ability));
				displayText(renderer, 16, yWinSize - yDesc, 20, descText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
				sprintf(selectedAbilityDesc, "%s", strToUpper(get_name(tempEntity, selected_ability)));
				displayText(renderer, 16, yWinSize - (yDesc+20), 20, selectedAbilityDesc, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
			}
			else
			{
				if (hover_ability >= 0)
				{
					temp_ab = ab_from_id(hover_ability, tempEntity);
					if (temp_ab->ab_cooldown != 0 && hover_ability != Mvt)
					{
						sprintf(cdText, "Cooldown : %d turn(s)", temp_ab->ab_cooldown);
						displayText(renderer, 16, yWinSize - yDesc, 20, cdText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
						yDesc += 20;
					}
					if (temp_ab->damage != NULL && hover_ability != Mvt)
					{
						if ((*(temp_ab->damage))->type == atk)
						{
							sprintf(damageText, "Base damage : %.0f (Physic)", (*(temp_ab->damage))->multiplier * tempEntity->stats[atk]);
							displayText(renderer, 16, yWinSize - yDesc, 20, damageText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
						}
						else if ((*(temp_ab->damage))->type == magic)
						{
							sprintf(damageText, "Base damage : %.0f (Magic)", (*(temp_ab->damage))->multiplier * tempEntity->stats[magic]);
							displayText(renderer, 16, yWinSize - yDesc, 20, damageText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
						}
						yDesc += 20;
					}
					sprintf(descText, "%s", get_desc(tempEntity, hover_ability));
					displayText(renderer, 16, yWinSize - yDesc, 20, descText, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
					sprintf(hoverAbilityDesc, "%s", strToUpper(get_name(tempEntity, hover_ability)));
					displayText(renderer, 16, yWinSize - (yDesc+20), 20, hoverAbilityDesc, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
				}
			}
		}

		// Display the ID card of the selected entity
		displaySprite(renderer, getTexture(textures, "id_card"), 10, 10);
		displayText(renderer, 382, 156, 18, "?", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
		displaySprite(renderer, getCharFrontTexture(tempEntity->cha_class->cla_name), 51, 62);
		displayText(renderer, 170, 40, 20, tempEntity->cha_name, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

		// -- entity health
		displaySprite(renderer, getBigTexture(cSprites, "heart_icon"), 170, 70);
		sprintf(pv_text, "%d", tempEntity->stats[pv]);
		displayText(renderer, 200, 70, 30, pv_text, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);

		// -- entity action points
		displaySprite(renderer, getBigTexture(cSprites, "star_icon"), 165, 102);
		sprintf(pa_text, "%d", tempEntity->act_points);
		displayText(renderer, 202, 106, 30, pa_text, "../inc/font/Pixels.ttf", 49, 174, 196, FALSE);

		// -- entity mouvement points
		displaySprite(renderer, getBigTexture(cSprites, "speed_icon"), 240, 102);
		sprintf(pm_text, "%d", tempEntity->stats[mv]);
		if (tempEntity->stats[mv] > tempEntity->cha_class->basic_stats[mv])
			displayText(renderer, 277, 106, 30, pm_text, "../inc/font/Pixels.ttf", 0, 255, 0, FALSE);
		else if (tempEntity->stats[mv] < tempEntity->cha_class->basic_stats[mv])
			displayText(renderer, 277, 106, 30, pm_text, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
		else
			displayText(renderer, 277, 106, 30, pm_text, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

		// -- entity vision points
		displaySprite(renderer, getBigTexture(cSprites, "vision_icon"), 315, 102);
		sprintf(vis_text, "%d", tempEntity->stats[vis]);
		if (tempEntity->stats[vis] > tempEntity->cha_class->basic_stats[vis])
			displayText(renderer, 352, 106, 30, vis_text, "../inc/font/Pixels.ttf", 0, 255, 0, FALSE);
		else if (tempEntity->stats[vis] < tempEntity->cha_class->basic_stats[vis])
			displayText(renderer, 352, 106, 30, vis_text, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
		else
			displayText(renderer, 352, 106, 30, vis_text, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

		// -- attack stat
		displaySprite(renderer, getBigTexture(cSprites, "atk_icon"), 25, 142);
		sprintf(atk_text, "%d", tempEntity->stats[atk]);
		if (tempEntity->stats[atk] > tempEntity->cha_class->basic_stats[atk])
			displayText(renderer, 62, 146, 30, atk_text, "../inc/font/Pixels.ttf", 0, 255, 0, FALSE);
		else if (tempEntity->stats[atk] < tempEntity->cha_class->basic_stats[atk])
			displayText(renderer, 62, 146, 30, atk_text, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
		else
			displayText(renderer, 62, 146, 30, atk_text, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

		// -- magic stat
		displaySprite(renderer, getBigTexture(cSprites, "magic_icon"), 110, 142);
		sprintf(magic_text, "%d", tempEntity->stats[magic]);
		if (tempEntity->stats[magic] > tempEntity->cha_class->basic_stats[magic])
			displayText(renderer, 147, 146, 30, magic_text, "../inc/font/Pixels.ttf", 0, 255, 0, FALSE);
		else if (tempEntity->stats[magic] < tempEntity->cha_class->basic_stats[magic])
			displayText(renderer, 147, 146, 30, magic_text, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
		else
			displayText(renderer, 147, 146, 30, magic_text, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

		// -- physical resistance
		displaySprite(renderer, getBigTexture(cSprites, "ra_icon"), 195, 142);
		sprintf(ra_text, "%d", tempEntity->stats[res_physic]);
		if (tempEntity->stats[res_physic] > tempEntity->cha_class->basic_stats[res_physic])
			displayText(renderer, 232, 146, 30, ra_text, "../inc/font/Pixels.ttf", 0, 255, 0, FALSE);
		else if (tempEntity->stats[res_physic] < tempEntity->cha_class->basic_stats[res_physic])
			displayText(renderer, 232, 146, 30, ra_text, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
		else
			displayText(renderer, 232, 146, 30, ra_text, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

		// -- magic resistance
		displaySprite(renderer, getBigTexture(cSprites, "rm_icon"), 280, 142);
		sprintf(rm_text, "%d", tempEntity->stats[res_magic]);
		if (tempEntity->stats[res_magic] > tempEntity->cha_class->basic_stats[res_magic])
			displayText(renderer, 317, 146, 30, rm_text, "../inc/font/Pixels.ttf", 0, 255, 0, FALSE);
		else if (tempEntity->stats[res_magic] < tempEntity->cha_class->basic_stats[res_magic])
			displayText(renderer, 317, 146, 30, rm_text, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
		else
			displayText(renderer, 317, 146, 30, rm_text, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);

		// -- stats description if hovering stats icon
		if (hover_stats != 0)
		{
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 90, 90, 90, SDL_ALPHA_OPAQUE / 2);
			SDL_RenderFillRect(renderer, &statDesc);
			switch (hover_stats)
			{
			case 1:
				displayText(renderer, 15, 205, 15, "Attack : Increase physical damage", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
				break;
			case 2:
				displayText(renderer, 15, 205, 15, "Magic : Increase magical damage", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
				break;
			case 3:
				displayText(renderer, 15, 205, 15, "Armor : Reduce physical damage sustained", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
				break;
			case 4:
				displayText(renderer, 15, 205, 15, "Magic Armor : Reduce magical damage sustained", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
				break;
			case 5:
				displayText(renderer, 15, 205, 15, "Action points (AP) : Used to cast abilities", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
				break;
			case 6:
				displayText(renderer, 15, 205, 15, "Mouvement points (MP) : Range where you can move", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
				break;
			case 7:
				displayText(renderer, 15, 205, 15, "Vision : Increase the abilities' range", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
				break;
			}
		}

		if (hover_passive_help == 1)
		{
			sprintf(passive, "Passive : %s", tempEntity->cha_class->Passive.name);
			displayText(renderer, mouse_position.x + 20, mouse_position.y + 20, 20, passive, "../inc/font/Pixels.ttf", 238, 165, 53, TRUE);
			displayText(renderer, mouse_position.x + 20, mouse_position.y + 40, 20, tempEntity->cha_class->Passive.desc, "../inc/font/Pixels.ttf", 238, 165, 53, TRUE);
			if (tempEntity->cha_class->cla_id == Angel)
			{
				Coord center = getSelectedPos();
				if (Aura_ab.nb_coords > 1)
				{
					for (int i = 0; i < Aura_ab.nb_coords; i++)
					{
						Coord highlight = add_coords(center, *((*(Aura_ab.coord)) + i));
						if (isInGrid(highlight))
							setHovered(highlight);
					}
				}
			}
		}

		// Stat buff and debuff
		xBuff = 420;
		yBuff = 10;
		StateList *temp;
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) temp = stReceived;
			else 		temp = stSent;
			start_list(temp);
			mod = list_search(temp, tempEntity, -1);
			while (mod != NULL)
			{
				buffIcon.x = xBuff;
				buffIcon.y = yBuff;
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(renderer, 90, 90, 90, SDL_ALPHA_OPAQUE / 2);
				SDL_RenderFillRect(renderer, &buffIcon);
				// Buffs
				if (mod->value->value > 0)
				{
					switch (mod->value->stat)
					{
						case atk:
							displaySprite(renderer, getBigTexture(cSprites, "atk_up"), xBuff, yBuff);
							break;
						case magic:
							displaySprite(renderer, getBigTexture(cSprites, "magic_up"), xBuff, yBuff);
							break;
						case res_physic:
							displaySprite(renderer, getBigTexture(cSprites, "ra_up"), xBuff, yBuff);
							break;
						case res_magic:
							displaySprite(renderer, getBigTexture(cSprites, "rm_up"), xBuff, yBuff);
							break;
						case vis:
							displaySprite(renderer, getBigTexture(cSprites, "vis_up"), xBuff, yBuff);
							break;
						case mv:
							displaySprite(renderer, getBigTexture(cSprites, "speed_up"), xBuff, yBuff);
							break;
					}
				}
				// Debuffs
				else if (mod->value->value < 0)
				{
					switch (mod->value->stat)
					{
						case atk:
							displaySprite(renderer, getBigTexture(cSprites, "atk_down"), xBuff, yBuff);
							break;
						case magic:
							displaySprite(renderer, getBigTexture(cSprites, "magic_down"), xBuff, yBuff);
							break;
						case res_physic:
							displaySprite(renderer, getBigTexture(cSprites, "ra_down"), xBuff, yBuff);
							break;
						case res_magic:
							displaySprite(renderer, getBigTexture(cSprites, "rm_down"), xBuff, yBuff);
							break;
						case vis:
							displaySprite(renderer, getBigTexture(cSprites, "vis_down"), xBuff, yBuff);
							break;
						case mv:
							displaySprite(renderer, getBigTexture(cSprites, "speed_down"), xBuff, yBuff);
							break;
					}
				}
				// Status
				else
				{
					switch (mod->value->stat)
					{
						case Blessed:
							displaySprite(renderer, getBigTexture(cSprites, "blessed"), xBuff, yBuff);
							break;
						case Detained:
							displaySprite(renderer, getBigTexture(cSprites, "jailed"), xBuff, yBuff);
							break;
						case Freezing:
							displaySprite(renderer, getBigTexture(cSprites, "frozen"), xBuff, yBuff);
							break;
						case Provoked:
							displaySprite(renderer, getBigTexture(cSprites, "provoke"), xBuff, yBuff);
							break;
						case Burning:
							displaySprite(renderer, getBigTexture(cSprites, "burning"), xBuff, yBuff);
							break;
						case Paralyzed:
							displaySprite(renderer, getBigTexture(cSprites, "paralyzed"), xBuff, yBuff);
							break;
						case Piercing:
							displaySprite(renderer, getBigTexture(cSprites, "piercing"), xBuff, yBuff);
							break;
						case Guarding:
							displaySprite(renderer, getBigTexture(cSprites, "guarding"), xBuff, yBuff);
							break;
						case Summoned:
							displaySprite(renderer, getBigTexture(cSprites, "summoned"), xBuff, yBuff);
							break;
					}
				}
				sprintf(duration, "%d", mod->value->duration);
				displayText(renderer, xBuff, yBuff, 15, duration, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
				yBuff += 32;
				if (yBuff > 138)
				{
					yBuff = 10;
					xBuff += 32;
				}
				list_next(temp);
				mod = list_search(temp, tempEntity, -1);
			}
		}
	}

	// List of characters
	int xPort;
	char actPts[STR_SHORT];
	for (int i = 0; i < _NB_CLASSES_; i++)
	{
		xPort = xWinSize - 10 - ((i + 1) * 64);
		portrait.x = xPort;
		portrait.y = 10;
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		if (tempEntity != NULL && Allies[i].cha_name == tempEntity->cha_name)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE / 2);
		else
			SDL_SetRenderDrawColor(renderer, 90, 90, 90, SDL_ALPHA_OPAQUE / 2);
		SDL_RenderFillRect(renderer, &portrait);
		displaySprite(renderer, getCharFrontTexture(Allies[i].cha_class->cla_name), xPort, 18);
		sprintf(actPts, "%d", Allies[i].act_points);
		if (Allies[i].active == Alive)
		{
			if (Allies[i].status_effect[Detained])
			{
				displaySprite(renderer, getTexture(textures, "detained_char"), portrait.x, portrait.y);
			}
			else
			{
				if (Allies[i].act_points >= 3)
					displayText(renderer, xPort + 20, 90, 20, actPts, "../inc/font/Pixels.ttf", 48, 129, 162, FALSE);
				else if (Allies[i].act_points == 0)
					displayText(renderer, xPort + 20, 90, 20, actPts, "../inc/font/Pixels.ttf", 255, 0, 0, FALSE);
				else
					displayText(renderer, xPort + 20, 90, 20, actPts, "../inc/font/Pixels.ttf", 255, 255, 255, FALSE);
				displaySprite(renderer, getTexture(cSprites, "star_icon"), xPort + 30, 90);
			}
		}
		else 
		{
			displaySprite(renderer, getTexture(textures, "dead_char"), portrait.x, portrait.y);
		}
	}

	// Logs
	Coord logPos;
	logPos.x = 20;
	if (tempEntity != NULL)
	{
		if (hover_stats == 0)
			logPos.y = 200;
		else
			logPos.y = 233;
	}
	else
	{
		logPos.y = 20;
	}
	removeOldLogs(SDL_GetTicks());
	displayLog(renderer, logPos);

	// Next turn button
	if (your_turn())
	{
		if (hover_next_turn == TRUE)
		{
			displayText(renderer, xWinSize - 280, yWinSize - 110, 20, "Skip turn", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
			displaySprite(renderer, getTexture(textures, "end_turn_hover"), xWinSize - 280, yWinSize - 80);
		}
		else
		{
			displaySprite(renderer, getTexture(textures, "end_turn"), xWinSize - 280, yWinSize - 80);
		}
	}
	else
	{
		displaySprite(renderer, getTexture(textures, "locked_end_turn"), xWinSize - 280, yWinSize - 80);
	}

	return 0;
}

int displayMap(SDL_Renderer *renderer, int x, int y)
// Display the map
{
	Coord blockPos;
	Coord rangerBorderTab[MAXRANGE];
	Coord rangerRangeTab[_X_SIZE_ * _Y_SIZE_];

	/* Le fond de la fenêtre sera blanc */
	SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

	if (selected_ability != -1)
	{
		get_border(getEntity(getSelectedPos())->cha_id, selected_ability, borderTab, rangeTab);
	}
	if (hover_passive_help)
	{
		if (getEntity(getSelectedPos())->cha_class->cla_id == Ranger)
		{
			get_border(getEntity(getSelectedPos())->cha_id, Bolt, rangerBorderTab, rangerRangeTab);
		}
	}

	for (int i = 0; i < _X_SIZE_; i++)
	{
		for (int j = (_Y_SIZE_ - 1); j >= 0; j--)
		{

			blockPos.x = x + (j + 1) * (pxBase / 2) + (i + 1) * (pxBase / 2);
			blockPos.y = y + i * (pxBase / 4) + (_Y_SIZE_ - j) * (pxBase / 4);

			if (blockPos.x >= -pxBase && blockPos.x <= xWinSize && blockPos.y >= -pxBase && blockPos.y <= yWinSize)
			{

				// Affichage block
				if ((*(matrix + i * _X_SIZE_ + j)).tile_id != BLANK)
				{
					if ((*(matrix + i * _X_SIZE_ + j)).tile_id == 1)
					{
						if (pxBase == 64)
							displaySprite(renderer, getTexture(textures, "block"), blockPos.x, blockPos.y);
						else
							displaySprite(renderer, getBigTexture(textures, "block"), blockPos.x, blockPos.y);
					}
					else if ((*(matrix + i * _X_SIZE_ + j)).tile_id <= 7)
					{
						if (pxBase == 64)
							displaySprite(renderer, textures[(*(matrix + i * _X_SIZE_ + j)).tile_id].texture, blockPos.x, blockPos.y);
						else
							displaySprite(renderer, textures[(*(matrix + i * _X_SIZE_ + j)).tile_id].big_texture, blockPos.x, blockPos.y);
					}
				}

				// Affichage pièges
				if ((*(matrix + i * _X_SIZE_ + j)).trap.visible == TRUE)
				{
					if (pxBase == 64)
						displaySprite(renderer, getTexture(textures, "trap"), blockPos.x, blockPos.y - (pxBase / 3));
					else
						displaySprite(renderer, getBigTexture(textures, "trap"), blockPos.x, blockPos.y - (pxBase / 3));
				}

				// Affichage portée d'attaque (si compétence sélectionnée)
				Entity *tempEntity = NULL;
				tempEntity = getEntity(getSelectedPos());
				if (tempEntity != NULL)
				{
					if (selected_ability != -1 && is_ally(tempEntity))
					{
						drawPos.x = i;
						drawPos.y = j;
						if (isInCoordTab(rangeTab, drawPos) || isInCoordTab(borderTab, drawPos))
						{
							if (pxBase == 64)
								displaySprite(renderer, getTexture(textures, "ability_range"), blockPos.x, blockPos.y);
							else
								displaySprite(renderer, getBigTexture(textures, "ability_range"), blockPos.x, blockPos.y);
						}
					}
				}

				// Passive description if hovering info icon
				if (hover_passive_help == 1)
				{
					if (tempEntity->cha_class->cla_id == Ranger)
					{
						drawPos.x = i;
						drawPos.y = j;
						if (isInCoordTab(rangerRangeTab, drawPos) || isInCoordTab(rangerBorderTab, drawPos))
						{
							if (pxBase == 64)
								displaySprite(renderer, getTexture(textures, "selection_hover"), blockPos.x, blockPos.y);
							else
								displaySprite(renderer, getBigTexture(textures, "selection_hover"), blockPos.x, blockPos.y);
						}
					}
				}

				// Affichage équipe
				if ((*(matrix + i * _X_SIZE_ + j)).entity != NULL)
				{
					if (pxBase == 64)
					{
						if ((*(matrix + i * _X_SIZE_ + j)).entity->cha_id > 0)
							displaySprite(renderer, getTexture(textures, "blue_team"), blockPos.x, blockPos.y);
						else
							displaySprite(renderer, getTexture(textures, "red_team"), blockPos.x, blockPos.y);
					}
					else
					{
						if ((*(matrix + i * _X_SIZE_ + j)).entity->cha_id > 0)
							displaySprite(renderer, getBigTexture(textures, "blue_team"), blockPos.x, blockPos.y);
						else
							displaySprite(renderer, getBigTexture(textures, "red_team"), blockPos.x, blockPos.y);
					}
				}

				// Affichage tuile de sélection
				if ((*(matrix + i * _X_SIZE_ + j)).selected == 1)
				{
					if (pxBase == 64)
						displaySprite(renderer, getTexture(textures, "selection"), blockPos.x, blockPos.y);
					else
						displaySprite(renderer, getBigTexture(textures, "selection"), blockPos.x, blockPos.y);
				}

				// Affichage tuile hover
				if ((*(matrix + i * _X_SIZE_ + j)).hovered == 1)
				{
					if (pxBase == 64)
						displaySprite(renderer, getTexture(textures, "selection_hover"), blockPos.x, blockPos.y);
					else
						displaySprite(renderer, getBigTexture(textures, "selection_hover"), blockPos.x, blockPos.y);
				}

				if ((*(matrix + i * _X_SIZE_ + j)).entity != NULL)
				{
					displayCharacters(renderer, cSprites, (*(matrix + i * _X_SIZE_ + j)).entity, blockPos.x, blockPos.y - pxBase / 1.6, pxBase);
				}

				if (camMove != -1)
				{
					if (mouse_position.x <= xWinSize && mouse_position.x >= xWinSize - 20)
					{
						displaySprite(renderer, getTexture(textures, "arrow_right"), xWinSize - 64, mouse_position.y - 32);
					}
					if (mouse_position.x >= 0 && mouse_position.x <= 20)
					{
						displaySprite(renderer, getTexture(textures, "arrow_left"), 0, mouse_position.y - 32);
					}
					if (mouse_position.y <= yWinSize && mouse_position.y >= yWinSize - 20)
					{
						displaySprite(renderer, getTexture(textures, "arrow_down"), mouse_position.x - 32, yWinSize - 64);
					}
					if (mouse_position.y <= 20 && mouse_position.y >= 0)
					{
						displaySprite(renderer, getTexture(textures, "arrow_up"), mouse_position.x - 32, 0);
					}
				}

				/*/ -- DEBUG Affichage des indices des tuiles --
				char pos[6];
				sprintf(pos, "%d,%d", i, j);
				displayText(renderer, imgDestRect.x+(pxBase/2)-10, imgDestRect.y+(pxBase/4), (pxBase/64)*10, pos, "../inc/font/Pixels.ttf", 255, 50, 50);
				// -- DEBUG --*/
			}
		}
	}

	displayInterface(renderer);

	/* -- DEBUG Affichage des coordonnées d'affichage de la map
	char str[12];
	sprintf(str, "%d, %d", x, y);
	displayText(renderer, 20, 20, 20, str, "../inc/font/Pixels.ttf", 255, 255, 255);
	// -- DEBUG --*/

	SDL_RenderPresent(renderer);

	return 1;
}
