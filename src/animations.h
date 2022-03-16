#ifndef animations_h
#define animations_h

/** \file animations.h
 * \brief Animations method functions
 * \author Thibault DOUCET
 */

/* =============== DEPENDENCES =============== */


#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"


/* =============== VARIABLES =============== */

/** \fn addAnimTexture(abilityId id, char *name, int start_index, int end_index, bool aoe, bool on_ground, int speed)
 * \brief Add the different textures to the loaded animations textures table to a given abilityId
 * \details Load the textures in a AnimTexture structure put in the animTextures table
 * \param id Id of the ability
 * \param name Specific path to the images to load (<class>/<anim_name>)
 * \param start_index Index of the first texture to load
 * \param end_index Index of the last texture to load
 * \param aoe Specifies if the animation should be displayed on each tile of aoe abilities
 * \param on_ground Specifies if the animation should be displayed on the ground or on the head of the characters
 * \param speed Time in ms between each sprite of the animation
 * \return an error code (err_t)
 */
err_t addAnimTexture(abilityId id, char *name, int start_index, int end_index, bool aoe, bool on_ground, int speed);

/** \fn getAnimTexture(abilityId id, int index, bool big)
 * \brief Get the texture of the given ability at a specific index
 * \details Looks at the animTextures table to return the wanted texture
 * \param id Id of the ability
 * \param index Index of the sprite of the animation
 * \param big Bool to choose if it has to return the 64x64 texture or the 128x128 (TRUE for 128x128)
 * \return a pointer to the texture (SDl_Texture *)
 */
SDL_Texture *getAnimTexture(abilityId id, int index, bool big);

/** \fn getAnimSteps(abilityId id)
 * \brief Return the number of sprites of an animation
 * \param id Id of the ability
 * \return the number of sprites of the animation (int)
 */
int getAnimSteps(abilityId id);

/** \fn loadAnimationTextures()
 * \brief Load all the animations textures
 * \details Uses addAnimTexture method for all the game's abilities
 * \return void
 */
err_t loadAnimationTextures();

/** \fn play_ability_animation(Ability ab, Coord pos)
 * \brief Play the animation of the given ability
 * \details Display each step of an animation
 * \param ab Ability to display its animation
 * \param pos Coordinates on the map the animation will be displayed
 * \return an error code (err_t)
 */
err_t play_ability_animation(Ability ab, Coord pos);

/** \fn isLoaded(abilityId id)
 * \brief Check if the animation of a given ability has correctly been loaded
 * \details Looks for the animation in the animTextures table
 * \param id Id of the ability
 * \return a boolean (TRUE if the animation is loaded)
 */
bool isLoaded(abilityId id);

/** \fn isOnGround(abilityId id)
 * \brief Check if the animation should be displayed on the ground
 * \param id Id of the ability
 * \return a boolean (TRUE if the animation should be displayed on the ground)
 */
bool isOnGround(abilityId id);


#endif
