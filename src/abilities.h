#ifndef abilities_h
#define abilities_h
/** \file abilities.h
 * \brief Ability method functions
 */
#include "struct.h"

/** \fn Killing_Blow_fn(Coord c, Entity * e, StateList * list)
 * \brief Berserker's Killing Blow Method
 * \details Permenantly increases caster's attack if killing blow was lethal
 * \return the number of opposing entities killed during the method (always 0)
 */
int Killing_Blow_fn(Coord c, Entity * e, StateList * list);

/** \fn Fury_fn(Coord c, Entity * e, StateList * list)
 * \brief Berserker's Killing Blow Method
 * \details Uses list_search and list_check to find all debuffs and remove them, as well as apply the attack increase
 * \return the number of opposing entities killed during the method (always 0)
 */
int Fury_fn(Coord c, Entity * e, StateList * list);

/** \fn Frenzied_Dash_fn(Coord c, Entity * e, StateList * list)
 * \brief Berserker's Frenzied Dash Method
 * \details Applies the movement
 * \return the number of opposing entities killed during the method (always 0)
 */
int Frenzied_Dash_fn(Coord c, Entity * e, StateList * list);


/** \fn Focus_fn(Coord c, Entity * e, StateList * list)
 * \brief Ranger's Focus Method
 * \details Uses list_search and list_check to find all buffs and debuffs and apply corresponding action
 * \return the number of opposing entities killed during the method (always 0)
 */
int Focus_fn(Coord c, Entity * e, StateList * list);

/** \fn Trap_fn(Coord c, Entity * e, StateList * list)
 * \brief Ranger's Trap Method
 * \details Applies a trap to the specified tile
 * \return the number of opposing entities killed during the method (always 0)
 */
int Trap_fn(Coord c, Entity * e, StateList * list);

/** \fn trap_check(Entity *e)
 * \brief Checks for a trap under an entity
 * \return TRUE if there is an ennemy trap under the entity, else returns FALSE
 */
bool trap_check(Entity *e);


/** \fn Detain_fn(Coord c, Entity *e, StateList * list)
 * \brief Goliath's Detain Method
 * \details Removes the detained ennemy from the map
 * \return the number of opposing entities killed during the method (always 0)
 */
int Detain_fn(Coord c, Entity *e, StateList * list);

/** \fn Banner_fn(Coord c, Entity * e, StateList * list)
 * \brief Goliath's Banner Method
 * \details Resets all allies' cooldowns
 * \return the number of opposing entities killed during the method (always 0)
 */
int Banner_fn(Coord c, Entity * e, StateList * list);


/** \fn mage_switch(Coord c, Entity * e, StateList * list)
 * \brief Mage's Skill 1 Method
 * \details Switches local mage's element after casting spell 1
 * \return the number of opposing entities killed during the method (always 0)
 */
int mage_switch(Coord c, Entity * e, StateList * list);

/** \fn FlameCharge_fn(Coord c, Entity * e, StateList * list)
 * \brief Fire Mage's Flamecharge Method
 * \details Calculates Flamecharge's path and applies corresponding effects
 * \return the number of opposing entities killed during the method
 */
int FlameCharge_fn(Coord c, Entity * e, StateList * list);

/** \fn Flare_fn(Coord c, Entity * e, StateList * list)
 * \brief Fire Mage's Flare Method
 * \details Checks for and reveals traps
 * \return the number of opposing entities killed during the method (always 0)
 */
int Flare_fn(Coord c, Entity * e, StateList * list);

/** \fn Blizzard_fn(Coord c, Entity * e, StateList * list)
 * \brief Ice Mage's Blizzard Method
 * \details Changes water blocks to ice and other blocks to snow
 * \return the number of opposing entities killed during the method (always 0)
 */
int Blizzard_fn(Coord c, Entity * e, StateList * list);

/** \fn Volt_Switch_fn(Coord c, Entity * e, StateList * list)
 * \brief Electric Mage's Volt Switch Method
 * \details Switches the two entities
 * \return the number of opposing entities killed during the method (always 0)
 */
int Volt_Switch_fn(Coord c, Entity * e, StateList * list);

/** \fn Lightning_Chain_fn(Coord c, Entity * e, StateList * list)
 * \brief Electric Mage's Lightning Chain Method
 * \details Finds close enough ennemies and applies the damage to them
 * \return the number of opposing entities killed during the method
 */
int Lightning_Chain_fn(Coord c, Entity * e, StateList * list);


/** \fn Thrust_fn(Coord c, Entity * e, StateList * list)
 * \brief Valkyrie's Thrust Method
 * \details Applies the damage the extended range
 * \return the number of opposing entities killed during the method
 */
int Thrust_fn(Coord c, Entity * e, StateList * list);

/** \fn Life_Transfer_fn(Coord c, Entity * e, StateList * list)
 * \brief Valkyrie's Life Transfer Method
 * \details Calculates the healing required and then applies the corresponding heal and slow
 * \return the number of opposing entities killed during the method (always 0)
 */
int Life_Transfer_fn(Coord c, Entity * e, StateList * list);

/** \fn Gates_of_Valhalla_fn(Coord c, Entity * e, StateList * list)
 * \brief Valkyrie's Gates of Valhalla Method
 * \details Summons all dead allies
 * \return the number of opposing entities killed during the method (always 0)
 */
int Gates_of_Valhalla_fn(Coord c, Entity * e, StateList * list);


/** \fn Last_Sacrifice_fn(Coord c, Entity * e, StateList * list)
 * \brief Angel's Last Sacrifice Method
 * \details Kills self then ressurects corresponding ally
 * \return the number of opposing entities killed during the method (always 0)
 */
int Last_Sacrifice_fn(Coord c, Entity * e, StateList * list);

/** \fn Gods_Blessing_fn(Coord c, Entity * e, StateList * list)
 * \brief Angel's Gods Blessing Method
 * \details Doubles action points
 * \return the number of opposing entities killed during the method (always 0)
 */
int Gods_Blessing_fn(Coord c, Entity *e, StateList * list);

#endif