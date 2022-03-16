#ifndef passives_h
#define passives_h
/** \file passives.h
 * \brief Contains functions in relation to passive abilities
 */
#include "common.h"

extern int Bloodlust_counter; //!< Counter used to keep track of kills made by the Berserker for Bloodlust activation
extern bool Sentinel_counter; //!< Boolean keeps track if sentinel already went off this round : TRUE already triggered, FALSE not yet triggered

/** \fn activate_bloodlust(Entity *e, StateList * list)
 * \brief Activates Berserker's passive Ability Bloodlust
 * \param e A pointer to the Bloodlusting Berserker
 * \param list The active list (stSent if local turn, stReceived if opponent's turn)
 * \return what list_add returns, err_t POINTER_NULL if malloc fails, err_t OK otherwise
 */
err_t activate_bloodlust(Entity *e, StateList * list);

/** \fn activate_aura(Entity *e, StateList * list)
 * \brief Activates Angel's passive Ability Aura
 * \param e A pointer to the active player's Angel
 * \param list The active list (stSent if local turn, stReceived if opponent's turn)
 * \return err_t OK
 */
err_t activate_aura(Entity *e, StateList * list);

/** \fn sentinel_check(Entity *e)
 * \brief Checks if an entity is in the opposing Ranger's bolt range
 * \param e A pointer to the active entity
 * \return TRUE if sentinel was triggered, FALSE otherwise
 */
err_t sentinel_check(Entity *e);

#endif