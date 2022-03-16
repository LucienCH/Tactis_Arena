#ifndef gameplay_h
#define gameplay_h
/** \file gameplay.h
 * \brief Many Miscellaneous Functions related to gameplay
 */
#include "struct.h"

/** \fn rec_id_swap(action * a)
 * \brief Sets received character Ids to their local counterpart
 * \details Makes all received char_ids negative
 * \param a the received action
 * \return err_t OK
 */
err_t rec_id_swap(action * a);

/** \fn e_from_id(int Id)
 * \brief Finds the entity corresponding to a character Id
 * \param Id The wanted character's Id
 * \return A pointer to the entity
 */
Entity * e_from_id(int Id);

/** \fn ab_from_id(abilityId Id, Entity *e)
 * \brief Finds the ability corresponding to an ability Id
 * \param Id The wanted ability's Id
 * \param e The Entity who has this ability
 * \return A pointer to the ability
 */
Ability * ab_from_id(abilityId Id, Entity *e);

/** \fn get_team(Entity *e, Entity **all, bool same)
 * \brief Finds the allied/ennemy team from an entity
 * \param e The entity
 * \param all a pointer to store the wanted team
 * \param same set as TRUE if the team allied to e is wanted or FALSE if the ennemy team is wanted
 * \return err_t OK
 */
err_t get_team(Entity *e, Entity **all, bool same);

/** \fn game_over()
 * \brief verifies the state of the game
 * \return the state of the game: ONGOING if still going, WIN if local has won, LOSE if local lost
 */
winId game_over();

/** \fn get_range(Entity *e, abilityId ab)
 * \brief Finds the castable range of an ability
 * \details Uses character vision and ability's range value to calculate castable range
 * \param e The Entity who has this ability
 * \param ab The abilityId of the selected ability
 * \return the castable range
 */
int get_range(Entity *e, abilityId ab);

/** \fn count_dead_allies(Entity *e)
 * \brief Counts dead allies
 * \param e An entity on the team to count
 * \return the number of dead allies
 */
int count_dead_allies(Entity *e);

/** \fn get_dead_allies(Entity *e, Entity * tab[])
 * \brief Puts dead allies in an array and returns it
 * \param e An entity on the team to get
 * \param tab An array to store the dead allies
 * \return a pointer to the dead allies array
 */
Entity ** get_dead_allies(Entity *e, Entity * tab[]);


/** \fn get_cost(Entity *e, abilityId Id)
 * \brief get the cost of an ability
 * \param e The Entity who has this ability
 * \param Id The abilityId of the selected ability
 * \return the cost
 */
int get_cost(Entity *e, abilityId Id);

/** \fn get_cooldown(Entity *e, abilityId Id)
 * \brief get the cooldown of an ability
 * \param e The Entity who has this ability
 * \param Id The abilityId of the selected ability
 * \return the cooldown
 */
int get_cooldown(Entity * e, abilityId Id);

/** \fn get_name(Entity *e, abilityId ab_id)
 * \brief get the name of an ability
 * \param e The Entity who has this ability
 * \param ab_id The abilityId of the selected ability
 * \return the name
 */
char * get_name(Entity * e, abilityId ab_id);

/** \fn get_desc(Entity * e, abilityId ab_id)
 * \brief get the description of an ability
 * \param e The Entity who has this ability
 * \param ab_id The abilityId of the selected ability
 * \return the description
 */
char * get_desc(Entity * e, abilityId ab_id);

/** \fn able_ability(Entity *e, abilityId ab_id, bool show_logs)
 * \brief Get the castability of an ability
 * \details Checks cooldowns, action points, and status effects to return the castability of the ability
 * \param e The Entity who has this ability
 * \param ab_id The abilityId of the selected ability
 * \param show_logs Set to TRUE if function should send logs to player, FALSE if not
 * \return the castability of the ability
 */
castabilityId able_ability(Entity *e, abilityId ab_id, bool show_logs);


/** \fn is_ally(Entity *e)
 * \brief Find if an entity is an ally
 * \param e entity to check
 * \return TRUE if entity is an ally, FALSE if not
 */
bool is_ally(Entity *e);

/** \fn same_team(Entity *a, Entity *b)
 * \brief Checks if two entities are on the same team
 * \details can put either Allies or Foes as one of the parametres to find out if an entity is part of Allies or of Foes
 * \param a First Entity to compare
 * \param b Second Entity to compare
 * \return TRUE if entities are on the same team, FALSE otherwise
 */
bool same_team(Entity *a, Entity *b);


/** \fn end_Detain(Entity *e)
 * \brief Ends the detainement of an entity
 * \details finds a place to make the detained entity reappear at the end of its detainement
 * \param e The detained entity
 * \return err_t OK
 */
err_t end_Detain(Entity *e);

/** \fn tile_type(Coord c, targetType targeting, Entity * e)
 * \brief Compares tile to ability cast requirements
 * \details Finds if the targeted tile is of a targeting type corresponding to that of the cast ability
 * \param c The coordinate to check
 * \param targeting the targetType of the ability cast
 * \param e The caster, to know which team he is on
 * \return TRUE if the ability can be cast there, FALSE otherwise
 */
bool tile_type(Coord c, targetType targeting, Entity * e);

/** \fn reset_cooldowns(Entity * e)
 * \brief Resets an entity's cooldowns to zero
 * \param e The entity whose cooldowns to reset
 * \return err_t OK
 */
err_t reset_cooldowns(Entity * e);


/** \fn same_coord(Coord a, Coord b)
 * \brief Finds if two coords are the same
 * \param a First Coordinate to compare
 * \param b Second Coordinate to compare
 * \return TRUE if the coordinates are the same, FALSE otherwise
 */
bool same_coord(Coord a, Coord b);

/** \fn closer_coords(Coord a, Coord b)
 * \brief Finds which coord is closer to the origin
 * \param a First Coordinate to compare
 * \param b Second Coordinate to compare
 * \return TRUE if the first coordinate is closer or they are tied, FALSE otherwise
 */
bool closer_coords(Coord a, Coord b);

/** \fn compare_coords(Coord a, Coord b)
 * \brief Substracts coordinate a from coordinate b
 * \param a Coordinate to substract
 * \param b Coordinate to substract from
 * \return the coordinate resulting from the substraction
 */
Coord compare_coords(Coord a, Coord b);

/** \fn add_coords(Coord a, Coord b)
 * \brief Adds two coordinates together
 * \param a First Coordinate to add
 * \param b Second Coordinate to add
 * \return the coordinate resulting from the addition
 */
Coord add_coords(Coord a, Coord b);


/** \fn free_spawn(Entity *e)
 * \brief Spawns an entity on a free tile
 * \param e The entity to spawn
 * \return err_t POINTER_NULL if e is NULL, err_t OK otherwise
 */
err_t free_spawn(Entity *e);

/** \fn remove_mod(Status * stat, Entity * e, bool show_log)
 * \brief Removes a modifier from an entity
 * \param stat The mod to remove
 * \param e The targeted entity
 * \param show_log set to TRUE if logs should be sent ot the player, FALSE if not
 * \return err_t POINTER_NULL if e is NULL, err_t OK otherwise
 */
err_t remove_mod(Status * stat, Entity * e, bool show_log);

/** \fn remove_from_list(Entity * e, statusId status, Status * s)
 * \brief Removes a ceratin element from both lists
 * \details uses the list_search function to find a ceratin element a to remove it from both lists
 * \param e The targeted entity
 * \param status the mod to remove
 * \param s A status pointer to store the status removed from the list so that it may be removed from the entity
 * \return A pointer to the status removed (s)
 */
Status * remove_from_list(Entity * e, statusId status, Status * s);


/** \fn new_death(Entity * e)
 * \brief Kills an entity
 * \details Sets active to dead and resets entity for revival
 * \param e The dead entity
 * \return err_t OK
 */
err_t new_death(Entity * e);

/** \fn death_check(Entity * e)
 * \brief Checks if an ennemy is dead
 * \details If the entity is dead, it calls new_death
 * \param e The entity to check
 * \return TRUE if dead, FALSE if not
 */
bool death_check(Entity * e);

/** \fn apply_check(Modifier * mod, Entity * target);
 * \brief Checks if a random modifier lands
 * \details If local turn, randomises outcome, if not, waits for outcome to be received
 * \param mod The mod attempting to be landed, NULL if is damage
 * \param target The target entity
 * \return TRUE if mod/damage lands, FALSE otherwise
 */
bool apply_check(Modifier * mod, Entity * target);

/** \fn apply_damage(Damage * d, Entity * caster, Entity * target, bool show_log)
 * \brief Calculates and applies damage
 * \details Uses target and caster stats as well as various status effects to determine damage
 * \param d Structure describing damage to apply
 * \param caster Caster entity
 * \param target Target entity
 * \param show_log set to TRUE if logs should be sent ot the player, FALSE if not
 * \return TRUE if target dies, FALSE otherwise
 */
bool apply_damage(Damage * d, Entity * caster, Entity * target, bool show_log);


/** \fn apply_status(Status s, Entity *target, StateList *list, int caster_id, bool show_log)
 * \brief Applies a status effect to an entity
 * \details Takes a status effect in the form of a Status structure and translates it's effects onto a given entity
 * \param s The status to apply
 * \param target The target entity
 * \param list The active list (if local turn the stSent, if not then stReceived)
 * \param caster_id The id of the caster (in case the status is provocation)
 * \param show_log set to TRUE if logs should be sent ot the player, FALSE if not
 * \return what list_add(err_t POINTER_NULL or err_t NULL) returns if status is applied and is not permanent, else err_t OK
 */
err_t apply_status(Status s, Entity *target, StateList *list, int caster_id, bool show_log);

/** \fn apply_stat_change(Status s, Entity * target, StateList * list, bool show_log)
 * \brief Applies a stat change to an entity
 * \details Takes a stat change in the form if a Status structure and applies these changes onto a given entity
 * \param s The stat change to apply
 * \param target The target entity
 * \param list The active list (if local turn stSent, if not stReceived)
 * \param show_log set to TRUE if logs should be sent ot the player, FALSE if not
 * \return what list_add(err_t POINTER_NULL or err_t NULL) returns if status is applied and is not permanent, else err_t OK
 */
err_t apply_stat_change(Status s, Entity * target, StateList * list, bool show_log);

/** \fn apply_mod(Modifier m, Entity * target, StateList * list, int caster_id)
 * \brief Calls necessary functions to apply a modifier
 * \details A hub function that determines wether the effect to apply is a status effect or s stat change, then calls the correct function accordingly
 * \param m The modifier to apply
 * \param target Th target entity
 * \param list The active list (if local turn stSent, if not stReceived)
 * \param caster_id The id of the caster (in case the status is provocation)
 * \return err_t OK
 */
err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id);


/** \fn apply_to(Ability active_ab, Entity * active_ent, StateList * list, Coord starting_point)
 * \brief Main ability function that applies damage and mods
 * \details Loop function that goes over all affected coordinates of an ability 
 * \param active_ab The cast ability
 * \param active_ent The caster of the ability
 * \param list The active list (if local turn stSent, if not stReceived)
 * \param starting_point The coordinates at which the ability was cast
 * \return err_t OK
 */
int apply_to(Ability active_ab, Entity * active_ent, StateList * list, Coord starting_point);

#endif