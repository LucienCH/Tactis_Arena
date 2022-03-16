#ifndef substruct_h
#define substruct_h
/** \file substruct.h
 * \brief Defining fundamental structures and enumerations.
 * \author Robin BUTEL, Thibault DOUCET
 * \details Use of substruct to limit clogging up struct.h and to define fundamental structures in a different file.
 */
#include "common.h"

#define NUM_STATS 7     //!< Indicates the total number of character stats.
#define NUM_CLASS 6     //!< Indicates the total number of created classes.
#define NUM_AB 4        //!< Indicates the total number of abilities per character (omiting movement and passives).
#define NUM_STATUS 10   //!< Indicates the total number of created status effects.
#define NB_AB 32        //!< Indicates the total number of created Abilities, used for animations.

/* ENUMERATIONS */
/** \enum statId
 * \brief Labeling the character's stats.
 */
typedef enum 
{
    pv/**<Health*/, mv/**<Movement Range*/, vis/**<Vision (used with spell range to calculate castable range)*/, 
    atk/**<Physical attack (used with spell damage modifier to calculate damage)*/, magic/**<Magical attack (used with spell damage modifier to calculate damage)*/, 
    res_physic/**<Physical Resistance (reduces incoming physical damage)*/, res_magic/**<Magical Resistance (reduces incoming magical damage)*/
} statId;

/** \enum classId
 * \brief Labeling the different classes.
 */
typedef enum 
{
    Berserker/**<Class id of 0*/, Ranger/**<Class id of 1*/, Goliath/**<Class id of 2*/, 
    Mage/**<Class id of 3*/, Valkyrie/**<Class id of 4*/, Angel/**<Class id of 5*/
} classId;

/** \enum abilityId
 * \brief Labeling character abilities.
 */
typedef enum 
{  
    Slash/**<Berserker Skill 0*/, Killing_Blow/**<Berserker Skill 1*/, Fury/**<Berserker Skill 2*/, Frenzied_Dash/**<Berserker Skill 3*/,
    Bolt/**<Ranger Skill 0*/, Focus/**<Ranger Skill 1*/, Trap/**<Ranger Skill 2*/, Deadeye/**<Ranger Skill 3*/, 
    Bash/**<Goliath Skill 0*/, Shields_Up/**<Goliath Skill 1*/, Detain/**<Goliath Skill 2*/, Banner/**<Goliath Skill 3*/, 
    Fireball/**<Fire Mage Skill 0*/, FlameCharge/**<Fire Mage Skill 1*/, Flare/**<Fire Mage Skill 2*/, Eruption/**<Fire Mage Skill 3*/, 
    Icy_Winds/**<Ice Mage Skill 0*/, Freeze/**<Ice Mage Skill 1*/, Frozen_Armor/**<Ice Mage Skill 2*/, Blizzard/**<Ice Mage Skill 3*/, 
    Shock/**<Electric Mage Skill 0*/, Volt_Switch/**<Electric Mage Skill 1*/, Shock_Therapy/**<Electric Mage Skill 2*/, Lightning_Chain/**<Electric Mage Skill 3*/, 
    Thrust/**<Valkyrie Skill 0*/, Odins_Eyes/**<Valkyrie Skill 1*/, Life_Transfer/**<Valkyrie Skill 2*/, Gates_of_Valhalla/**<Valkyrie Skill 3*/, 
    Condemn/**<Angel Skill 0*/, Holy_Storm/**<Angel Skill 1*/, Last_Sacrifice/**<Angel Skill 2*/, Gods_Blessing/**<Angel Skill 3*/, 
    Aura/**<Angel Passive*/, Mvt/**<Movement*/
} abilityId;

/** \enum lifeId
 * \brief Boolean labeling character dead/alive state.
 */
typedef enum {Dead/**<0*/, Alive/**<1*/} lifeId;

/** \enum statusId
 * \brief Labeling the different status effects.
 */
typedef enum 
{
    Cripple/**<Character cannot move and takes increased damage from most sources*/, 
    Detained/**<Character is catured by an ennemy, unable to do anything for three turns or until detainer is killed*/, 
    Provoked/**<Character may only target provoker with abilities*/, 
    Burning/**<Character takes damage at the beginning of every turn, prevents him from being frozen*/, 
    Freezing/**<Character is unable to do anything but has increased resistances*/,
    Paralyzed/**<Character only has 1 action point per turn*/, 
    Blessed/**<Character's cooldowns don't get applied*/, 
    Piercing/**<Character ignores armor when attacking*/, 
    Guarding/**<Character has increased block chance*/, 
    Summoned/**<Character has been summoned for a turn*/
} statusId;

/** \enum castabilityId
 * \brief Labeling states in which a spell cannot be cast
 */
typedef enum 
{
    Castable_c/**<Spell is castable*/, Locked_c/**<Not enough action points to cast the spell, or it is on cooldown*/, 
    Frozen_c/**<Character is frozen and cannot cast the spell*/, Crippled_c/**<Character is crippled and the spell includes movement*/
} castabilityId;

/** \enum targetType
 * \brief Labeling tile states to identify if tile can be targeted by a spell
 */
typedef enum 
{
    FOES/**<A foe is on this tile*/, ALLIES/**<An ally is on this tile*/, BOTH/**<Either a foe or an ally are on this tile*/,
    FREE_TILE/**<This tile is walkeable and contains no entity*/, ANY_TILE/**<Any Tile*/
} targetType;

/** \enum fnid
 * \brief Labeling Indicators on ability method usage
 */
typedef enum
{
    NONE/**<There is no method, use only the loop*/, DURING/**<Apply during the loop, to every tile*/, BEFORE/**<Apply before the loop and the activation of cooldown*/, 
    ONLY/**<Only use the method, don't use the loop*/, AFTER/**<Apply after the loop*/
} fnid;

/** \enum Direction
 * \brief Labeling character directions
 */
typedef enum
{
    N/**<NORTH/TOPLEFT*/, E/**<EAST/TOPRIGHT*/, S/**<SOUTH/BOTTOMRIGHT*/, W/**<SOUTH/BOTTOMLEFT*/
} Direction;

/** \enum winId
 * \brief Labeling game states
 */
typedef enum
{
    ONGOING/**<Game is not over*/, WIN/**<Game is over, you have won*/, LOSE/**<Game is over, you have lost*/
} winId;

struct entity_t;

/*Entity Sub Structures*/

/** \struct lang
 * \brief Language structure that indicates name and description of a certain object.
 */
typedef struct
{
    char name[STR_SHORT];   //!< Object Name.
    char desc[STR_LONG];    //!< Object Description.
}lang;

/** \struct Damage
 * \brief Damage structure indicating a spell's damage information.
 */
typedef struct
{
    float multiplier;   //!< Damage multiplier to be used with caster's specified stat type to find gross damage.
    statId type;        //!< Indicates if the damage uses the caster's atk stat or his magic stat.
} Damage;

/** \struct Status
 * \brief Structure representing stat modifier or status effect information (used to keep track of buffs and debuffs).
 */
typedef struct
{
    int value;      //!< Indicates the value of the stat change, if equal to 0 then it is a status effect instead.
    int stat;       //!< Indicates the stat changed(statId), or if value equals 0, indicates the status effect(statusId).
    int duration;   //!< Indicates the duration of the modifier, if equal to 0 then it is a permanent modifier.
} Status;

/** \struct Modifier
 * \brief Structure representing a stat modifier or status effect information to determine if it will land.
 */
typedef struct
{
    Status effect;  //!< A Status structure, containing relevant information as to what the modifier itself is.
    float chance;   //!< The chance the modifier has of landing.
    targetType t;   //!< If the modifier can land on allies, foes, or both.
}Modifier;

//STRINGS OF ENUMERATIONS FOR PRINTING PURPOSES

extern char statName[NUM_STATS][STR_SHORT];     //!< extern string database containing string version of stat names
extern char IdNames[Mvt+1][STR_SHORT];          //!< extern string database containing string version of ability names
extern char statusName[NUM_STATUS][STR_SHORT];  //!< extern string database containing string version of status effect names
extern char targetNames[5][STR_SHORT];          //!< extern string database containing string version of the target type enumeration
extern char fnNames[5][STR_SHORT];              //!< extern string database containing string version of the function use enumeration

#endif