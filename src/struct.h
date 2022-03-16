#ifndef struct_h
#define struct_h
/** \file struct.h
 * \brief Defining major structures.
 * \author Robin BUTEL, Thibault DOUCET, Lucien CHAUVIN
 * \details Code used to define major structures seperated into multiple categories : BASIC, ENTITY, ABILITY, TERRAIN, TEXTURE and COMMUNICATION.
 */

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "state.h"
#include "substruct.h"

/* BASIC STRUCTURES*/

/** \struct Coord
 * \brief Structure representing a coordinate, used to navigate the gameboard.
 */
typedef struct
{
    int x; //!< X value of the coordinate
    int y; //!< Y value of the coordinate
} Coord;

/** \struct Vector
 * \brief Structure representing a vector.
 * \details Is the same as Coord but wtih floats instead of ints.
 */
typedef struct
{
    float x; //!< X value of the vector
    float y; //!< Y value of the vector
} Vector;

/** \struct Timer
 * \brief
 */
typedef struct
{
    Uint32 start; //!<
    Uint32 end; //!<
} Timer;

/** \struct Log
 * \brief
 */
typedef struct
{
    char *message; //!<
    Uint32 time; //!<
} Log;

/** \struct music_t
 * \brief
 */
typedef struct{
    char *musicName; //!<
    int startFade; //!<
    Mix_Music *music_p; //!<
}music_t;

/* ENTITY STRUCTURES */

/** \struct action
 * \brief Structure representing necessary information for the communication of actions done, wether between functions, threads, or players.
 */
typedef struct {
    int char_id;    //!< Character Id of the caster/user.
    Coord c;        //!< Coordinate where the action was used.
    abilityId act;  //!< Id of the action/ability cast (use %NUM_AB to find number local to selected Entity).
} action;

/** \struct Ability
 * \brief Structure representing a castable ability.
 */
typedef struct
{
    abilityId ab_id;                                        //!< Ability Id.
    int ab_cost;                                            //!< Ability cost in action points (act_points).
    int ab_cooldown;                                        //!< Ability Cooldown.
    int range;                                              //!< Ability Range, used alongside caster's vision to determine appliable range.
    targetType target;                                      //!< Indicates which tiles can be targeted by this ability.
    Damage ** damage;                                       //!< A double pointer to this ability's damage structure.
    int nb_coords;                                          //!< Ability zone size, number of coordinates where ability will be applied, used to navigate through coord.
    Coord ** coord;                                         //!< A double pointer to this ability's list of coordinates which indicate it's zone.
    int nb_mods;                                            //!< Ability's number of modifiers, used to navigate through mods.
    Modifier ** mods;                                       //!< A double pointer to this ability's list of applicable modifiers.
    fnid fn_use;                                            //!< Indicates when to use special ability function method.
    int (*function)(Coord, struct entity_t *, StateList *); //!< Method that allows special behavior to happen during casting of ability. NULL if none is required.
    lang eng;                                               //!< Name and Desription of Ability in English.
} Ability;

/** \struct Class
 * \brief Structure representing the classes of the characters.
 */
typedef struct
{
    classId cla_id;                 //!< Class Id.
    char cla_name[STR_SHORT];       //!< Class Name.
    int basic_stats[NUM_STATS];     //!< Class' basic stats arranged in an array respecting the order of statId.
    lang Passive;                   //!< Name and Description of that class' passive ability.
    Ability * cla_abilities;        //!< Pointer to class' abilities.
} Class;

/** \struct entity_t
 * \brief  Defined as Entity. Structure representing the characters played by the player and his opponent.
 */
typedef struct entity_t
{
    int cha_id;                         //!< Character Id, positive if ally, negative if ennemy, is equal to classId + 1.
    char cha_name[STR_SHORT];           //!< Character Name.
    Class * cha_class;                  //!< Pointer to Character Class.
    lifeId active;                      //!< Boolean indicating if the character is alive or dead.
    Coord coords;                       //!< Coord indicating character's current position on the gameboard.
    Direction direction;                //!< Indicates direction character is facing.
    int idAnim;                         //!< Indicates animation stage.
    int act_points;                     //!< Number of action points (3 normally, 1 if paralyzed).
    int stats[NUM_STATS];               //!< Character Stats arranged in an array respecting the order of statId.
    int status_effect[NUM_STATUS];      //!< Array indicating current status_effects of character, respecting the order of statusId.
    int ab_cooldown[NUM_AB];            //!< Array indicating current ability cooldowns respecting the order of abilityId.
} Entity;
/** \typedef Entity
 * \brief Structure representing the characters played by the player and his opponent.
 */

/*ABILITY STRUCTURES*/

/** \struct Trap_t
 * \brief Structure representing the existence of a Trap.
 * \details Each tile of the gameboard will have a Trap_t structure.
 */
typedef struct
{
    int cha_id;     //!< Trap Id. Is equal to zero if there is no trap, is positive if friendly trap, is negative if ennemy trap. Name of variable is cha_id for use in functions that compare Entity's teams.
    bool visible;   //!< Visibility of the trap (is visible if same team or if flare found it).
} Trap_t;

/* TERRAIN STRUCTURES */

/** \struct Tile
 * \brief Structure representing a tile on the map
 */
typedef struct
{
    int tile_id; //!< Id of the block that will be displayed on the tile.
    int selected; //!< 1 if the tile has been selected by the user, else 0.
    int hovered; //!< 1 if the tile is hovered by the user, else 0.
    Entity * entity; //!< Pointer to the entity that is on the tile.
    Trap_t trap; //!< Pointer to the trap if one is on the tile.
    int walkable; //!< Define if characters wan walk on this tile (1 if they can, else 0);
} Tile;

/* TEXTURE STRUCTURES */

/** \struct TabTexture
 * \brief Structure of how general textures are loaded
 */
typedef struct
{
    char * texture_name; //!< Name of the texture (used for searching).
    SDL_Texture *texture; //!< Pointer to the texture (64x64)
    SDL_Texture *big_texture; //!< Pointer to the big texture (128x128)
} TabTexture;

/** \struct CharTexture
 * \brief Structure of how characters textures are loaded
 */
typedef struct
{
    char * texture_name; //!< Name of the texture (used for searching).
    SDL_Texture *front; //!< Pointer to the texture of the character facing front.
    SDL_Texture *textures[8][6]; //!< Matrix of the 4*2 (4 directions in 2 sizes) with 6 sprites each.
} CharTexture;

/** \struct AnimTexture
 * \brief Structure of how animations textures are loaded
 */
typedef struct
{
    abilityId id; //!< Id of the ability linked to the animation.
    SDL_Texture *spritesSmall[_NB_MAX_ANIM_]; //!< Table of all the textures needed for the animation in 64x64 resolution.
    SDL_Texture *spritesBig[_NB_MAX_ANIM_]; //!< Table of all the textures needed for the animation in 128x128 resolution.
    int nb_steps; //!< Number of sprites of the animation.
    bool aoe; //!< Define if the animation should be displayed on each tile of an AOE (Area Of Effect) ability.
    bool on_ground; //!< Define if the animation should be displayed on the ground or at head-level.
    int speed; //!< Tile in miliseconds between each sprite.
    Mix_Chunk *sound_effect; //!< Pointer to the sound_effect played with the animation.
} AnimTexture;

/** \struct TextCache
 * \brief Structure of how text are saved in cache
 * \details This is used for texts displayed with the caching option in displaySprite, this is used for recurrent and constant text and allows to display it without re-creating any surface
 */
typedef struct
{
    char *police; //!< String representing police used for a text.
    int size; //!< Size of the displayed text.
    char *content; //!< String displayed.
    SDL_Color color; //!< Color of the text displayed.
    SDL_Surface *surface; //!< Surface generated by SDL to display the text.
    Uint32 time; //!< Time of the first occurence (and creation) if the text.
} TextCache;

/* COMMUNICATION STRUCTURES */

/** \struct t_user
 * \brief Simple structure to get the name of the client
 */
typedef struct User{
  char pseudo[64]; //!< Name of the client connected to the server
} t_user;

/** \struct MultiTile
 * \brief Simplified version of Tile structure
 * \details Used in map sending for multiplayer purposes
 */
typedef struct{
    int tile_id; //!< Id of the block that will be displayed on the tile.
}MultiTile;

/** \struct ServerStatus_t
 * \brief Structure to get status
 * \details Used to get status of connection, get the name of the map selected by the host and giv the bases to create the map with Multitile
 */
typedef struct{
    int isServerStartGame; //!< Status of the server between -2 and 4
    char mapNameGame[50]; //!< Name of the map selected by the host
    MultiTile multiMap[_X_SIZE_][_Y_SIZE_]; //!< Array of Multitile to recreate the map client side
}ServerStatus_t;

#endif
