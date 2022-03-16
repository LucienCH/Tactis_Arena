
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
#include "struct.h"
#include "map.h"

/** \file animations.c
 * \brief Animations method functions
 * \details Contains all animations relative functions for displaying and loading
 * \author Thibault DOUCET
 */

/* =============== CONSTANTES =============== */

#define _NB_MAX_TEXTURES_ 50    /**< Size of the animTexture table (loaded textures) */

/* =============== VARIABLES =============== */

/* =============== FONCTIONS DE GESTION DES TEXTURES D'ANIMATIONS =============== */

err_t addAnimTexture(abilityId id, char *name, int start_index, int end_index, bool aoe, bool on_ground, int speed, char *sound_effect)
// Pre-load animation textures for future loading
{
    int index = 0;
    int tabIndex = 0;
    char tempSmall[STR_LONG];
    char tempBig[STR_LONG];

    index = id;

    if (verbose == 3)
        printf("\033[36;01m[ANIMATIONS]\033[00m : Ajout de la texture [%s] à l'id %d\n", name, index);

    animTextures[index].id = id;
    animTextures[index].nb_steps = end_index - start_index + 1;
    animTextures[index].aoe = aoe;
    animTextures[index].on_ground = on_ground;
    animTextures[index].speed = speed;

    for (int i = start_index; i <= end_index; i++)
    {
        sprintf(tempSmall, "../inc/sprites/attacks/sprite_indiv/64_64/%s%d.png", name, i);
        sprintf(tempBig, "../inc/sprites/attacks/sprite_indiv/128_128/%s%d.png", name, i);

        if (verbose >= 3)
            printf("\033[36;01m[ANIMATIONS]\033[00m : Chargement de la texture : %s\n", tempSmall);

        // Exception for Eruption and Icy Winds sprites (if optimized they're displayed as white squares)
        if (!strcmp(name, "mage/explosion") || !strcmp(name, "mage/baseice"))
        {
            animTextures[index].spritesSmall[tabIndex] = loadTexture(renderer, loadImage(tempSmall));
        }
        else
        {
            animTextures[index].spritesSmall[tabIndex] = loadTexture(renderer, loadOptImage(tempSmall));
        }

        if (verbose >= 3)
            printf("\033[36;01m[ANIMATIONS]\033[00m : Ajout de la texture [%s] à l'id %d\n", name, index);
        //animTextures[index].spritesBig[tabIndex] = loadTexture(renderer, loadOptImage(tempBig));
        
        animTextures[index].spritesBig[tabIndex] = NULL;
        tabIndex++;
    }

    while (tabIndex < _NB_MAX_ANIM_)
    {
        animTextures[index].spritesSmall[tabIndex] = NULL;
        animTextures[index].spritesBig[tabIndex++] = NULL;
    }

    if (verbose == 3)
        printf("\033[36;01m[ANIMATIONS]\033[00m : Chargement du fichier son : %s\n", sound_effect);
    animTextures[index].sound_effect = Mix_LoadWAV(sound_effect);

    return OK;
}

SDL_Texture *getAnimTexture(abilityId id, int index, bool big)
// Returns the texture of a given animation
{
    SDL_Texture *result = NULL;

    for (int i = 0; i < NB_AB; i++)
    {
        if (id == animTextures[i].id)
        {
            if (big)
                result = animTextures[i].spritesBig[index];
            else
                result = animTextures[i].spritesSmall[index];
            break;
        }
    }

    if (result == NULL)
    {
        printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : Aucune animation trouvée pour l'abilité ID %d - Chargement de la texture impossible\n", id);
        exit(EXIT_FAILURE);
    }

    return result;
}

int getAnimSteps(abilityId id)
// Returns the number of sprites for the animation of the given ability id
{
    int result = -1;

    printf("Calcul nb animations\n");

    for (int i = 0; i < NB_AB; i++)
    {
        if (id == animTextures[i].id)
        {
            result = animTextures[i].nb_steps;
            printf("Calcul nb animations OK\n");
            break;
        }
    }

    if (result == -1)
    {
        printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : Aucune animation trouvée pour l'abilité ID %d - Aucun retour sur le nombre d'animations\n", id);
        exit(EXIT_FAILURE);
    }

    return result;
}

AnimTexture getAnim(abilityId id)
// Return the ability from an id
{
    AnimTexture result;
    bool found = FALSE;

    printf("Recherche de l'animation\n");

    for (int i = 0; i < NB_AB; i++)
    {
        if (id == animTextures[i].id)
        {
            result = animTextures[i];
            found = TRUE;
            printf("Animation trouvee\n");
            break;
        }
    }

    if (found == FALSE)
    {
        printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : Aucune animation trouvée pour l'abilité ID %d - Aucune animation chargée\n", id);
        exit(EXIT_FAILURE);
    }

    return result;
}

err_t loadAnimationTextures()
// Load all the animations relative textures
{
    // Berserker abilities animations
    addAnimTexture(Slash, "berserker/slice", 64, 76, FALSE, FALSE, 20, "../inc/sound_effects/abilities/Slash.wav");
    addAnimTexture(Killing_Blow, "berserker/slice", 64, 76, FALSE, FALSE, 20, "../inc/sound_effects/abilities/Slash.wav");
    addAnimTexture(Fury, "berserker/red_aura", 64, 68, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Fury.wav");
    addAnimTexture(Frenzied_Dash, "berserker/earthquake_ultimate", 64, 68, TRUE, TRUE, 100, "../inc/sound_effects/abilities/Frenzied_Dash.wav");

    // Ranger abilities animations
    addAnimTexture(Bolt, "ranger/bolt", 64, 69, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Bolt.wav");
    addAnimTexture(Focus, "ranger/focus_aura", 64, 68, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Focus.wav");
    addAnimTexture(Trap, "ranger/bolt", 64, 69, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Trap.wav");
    addAnimTexture(Deadeye, "ranger/eye", 64, 69, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Deadeye.wav");

    // Goliath abilities animations
    addAnimTexture(Bash, "goliath/boomed", 64, 66, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Bash.wav");
    addAnimTexture(Shields_Up, "goliath/shieldsup", 64, 71, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Shield_Up.wav");
    addAnimTexture(Detain, "goliath/jailed", 64, 70, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Detain.wav");
    addAnimTexture(Banner, "goliath/bluebanneer", 64, 69, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Banner.wav");

    // Mage abilities animations
    // --- fire
    addAnimTexture(Fireball, "mage/basefire", 64, 72, FALSE, FALSE, 40, "../inc/sound_effects/abilities/Fireball.wav");
    addAnimTexture(FlameCharge, "mage/engulf", 64, 73, TRUE, FALSE, 40, "../inc/sound_effects/abilities/Flame_Charge.wav");
    addAnimTexture(Flare, "mage/flare", 64, 71, FALSE, FALSE, 40, "../inc/sound_effects/abilities/Flare.wav");
    addAnimTexture(Eruption, "mage/explosion", 67, 72, TRUE, FALSE, 100, "../inc/sound_effects/abilities/Eruption.wav");
    // --- ice
    addAnimTexture(Icy_Winds, "mage/baseice", 67, 74, FALSE, FALSE, 40, "../inc/sound_effects/abilities/Icy_Wind.wav");
    addAnimTexture(Freeze, "mage/freeze", 64, 74, FALSE, FALSE, 20, "../inc/sound_effects/abilities/Freeze.wav");
    addAnimTexture(Frozen_Armor, "mage/icearmor", 64, 70, FALSE, FALSE, 40, "../inc/sound_effects/abilities/Frost_Armor.wav");
    addAnimTexture(Blizzard, "mage/blizzard", 1, 10, TRUE, FALSE, 20, "../inc/sound_effects/abilities/Blizzard.wav");
    // --- electricity
    addAnimTexture(Shock, "mage/baseelec", 64, 68, FALSE, FALSE, 80, "../inc/sound_effects/abilities/Shock.wav");
    addAnimTexture(Volt_Switch, "mage/caseelec", 643, 652, TRUE, FALSE, 30, "../inc/sound_effects/abilities/Volt_Switch.wav");
    addAnimTexture(Shock_Therapy, "mage/scan", 64, 69, FALSE, FALSE, 80, "../inc/sound_effects/abilities/Shock_Therapy.wav");
    addAnimTexture(Lightning_Chain, "mage/baseelec", 64, 68, FALSE, FALSE, 80, "../inc/sound_effects/abilities/Shock.wav");

    // Valkyrie abilities animation
    addAnimTexture(Thrust, "valkyrie/peck", 64, 68, TRUE, FALSE, 80, "../inc/sound_effects/abilities/Thrust.wav");
    addAnimTexture(Odins_Eyes, "valkyrie/crows", 64, 69, TRUE, FALSE, 60, "../inc/sound_effects/abilities/Odin's_Eyes.wav");
    addAnimTexture(Life_Transfer, "valkyrie/peck", 64, 68, TRUE, FALSE, 80, "../inc/sound_effects/abilities/Life_Transfer.wav");
    addAnimTexture(Gates_of_Valhalla, "valkyrie/gate", 64, 70, FALSE, FALSE, 60, "../inc/sound_effects/abilities/Gates_of_Valhalla.wav");

    // Angel abilities animations
    addAnimTexture(Condemn, "angel/condemn", 64, 71, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Condemn.wav");
    addAnimTexture(Holy_Storm, "angel/tornade", 64, 68, TRUE, TRUE, 100, "../inc/sound_effects/abilities/Holy_Storm.wav");
    addAnimTexture(Last_Sacrifice, "angel/wings", 64, 68, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Last_Sarifice.wav");
    addAnimTexture(Gods_Blessing, "angel/oberon", 64, 69, FALSE, FALSE, 100, "../inc/sound_effects/abilities/God's_Blessing.wav");
    addAnimTexture(Aura, "angel/heal_aura", 64, 68, FALSE, FALSE, 100, "../inc/sound_effects/abilities/Aura.wav");

    animTextures_loaded = TRUE;

    return OK;
}

bool isLoaded(abilityId id)
// Returns if the given ability has been loaded
{
    bool result = FALSE;

    for (int i = 0; i < NB_AB && result == FALSE; i++)
    {
        if (id == animTextures[i].id)
        {
            result = TRUE;
        }
    }

    return result;
}

bool isOnGround(abilityId id)
// Returns if the animation have to be played on the ground or not
{
    return getAnim(id).on_ground;
}

/* =============== FONCTIONS D'AFFICHAGE DES ANIMATIONS =============== */

err_t play_ability_animation(Ability ab, Coord pos)
{
    Coord temp;
    int nbSteps = animTextures[ab.ab_id].nb_steps;
    AnimTexture animation = animTextures[ab.ab_id];

    if (animation.on_ground == 0)
    {
        pos.x--;
        pos.y++;
    }
    if (verbose >= 2)
        printf("GROUND OK\n");

    displayMap(renderer, XPOS, YPOS);
    
    if (verbose >= 2)
        printf("1ST DISPLAYMAP OK\n");

    Mix_PlayChannel(-1, animation.sound_effect, 0);

    if (verbose >= 2)
        printf("SOUND OK\n");

    if (verbose >= 2)
        printf("ABILITY HAS %d STEPS\n", nbSteps);

    if (animation.aoe == TRUE)
    {
        if (verbose >= 2)
            printf("ABILITY HAS AOE ANIMATION\n");
        for (int i = 0; i < nbSteps; i++)
        {
            if (animation.spritesSmall[i] == NULL)
            {
                printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : La texture vaut NULL\n");
                return POINTER_NULL;
            }

            for (int j = 0; j < ab.nb_coords; j++)
            {
                Coord drawPos = add_coords(pos, *((*(ab.coord)) + j));
                if (isInGrid(drawPos))
                {
                    temp = to2D(drawPos);
                    if (pxBase == 64)
                        displaySprite(renderer, animTextures[ab.ab_id].spritesSmall[i], temp.x, temp.y);
                    else
                        displaySprite(renderer, animTextures[ab.ab_id].spritesSmall[i], temp.x, temp.y);
                }
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(animation.speed);
            displayMap(renderer, XPOS, YPOS);
            if (verbose >= 2)
                printf("STEP %d/%d completed\n", i + 1, nbSteps);
        }
    }
    else
    {
        if (verbose >= 2)
            printf("ABILITY HAS SINGLE TARGET ANIMATION\n");
        for (int i = 0; i < nbSteps; i++)
        {
            if (animation.spritesSmall[i] == NULL)
            {
                printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : La texture vaut NULL\n");
                return POINTER_NULL;
            }

            temp = to2D(pos);
            if (pxBase == 64)
                displaySprite(renderer, animTextures[ab.ab_id].spritesSmall[i], temp.x, temp.y);
            else
                displaySprite(renderer, animTextures[ab.ab_id].spritesSmall[i], temp.x, temp.y);

            SDL_RenderPresent(renderer);
            SDL_Delay(animation.speed);
            displayMap(renderer, XPOS, YPOS);
            if (verbose >= 2)
                printf("STEP %d/%d completed\n", i + 1, nbSteps);
        }
    }
    return OK;
}