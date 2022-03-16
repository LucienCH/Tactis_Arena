#ifndef init_h
#define init_h
/** \file init.h
 * \brief Functions related to the intialisation and destruction of game information
 */
#include "struct.h"

extern StateList * stSent;          //!< The Statelist containing all modifiers applied during the local player's turn
extern StateList * stReceived;      //!< The Statelist containing all modifiers applied during the opponent's turn
extern Class classes[NUM_CLASS];    //!< An array to store all the classes
extern Ability mage_ab[3][NUM_AB];  //!< A 2 dimensional array to store the Mage's three movesets
extern Entity Allies[NUM_CLASS];    //!< An array to store the local player's entities
extern Entity Foes[NUM_CLASS];      //!< An array to store the opponent's entities
extern Ability Aura_ab;             //!< An ability structure to store the Angel's passive ability
extern Ability Move_ab;             //!< An ability structure to store the Movement ability

/** \fn init_spawn(Entity * e, Coord c)
 * \brief Sets the spawn of an entity
 * \param e The entity to spawn
 * \param c The coordinate to spawn him at
 * \return err_t OK
 */
err_t init_spawn(Entity * e, Coord c);

/** \fn ent_common_init(Entity *e)
 * \brief initialises common variables among all entities
 * \details initialises an entities' cooldowns to zero, his status effects to zero, his stats to that of his class's basic stats, his active to Alive, his action points to 3 and his animation Id to zero.
 * \param e The entity to initialise
 * \return err_t OK
 */
err_t ent_common_init(Entity *e);

/** \fn init_Entity(Entity * e,Coord spawn[NUM_CLASS], Direction d, char title[STR_SHORT], int id_mod)
 * \brief Initialises a team of entities
 * \param e A pointer to entity to store the initialised team
 * \param spawn An array of coordinates depicting where to spawn each entity
 * \param d The direction which those entities will be facing (W for host, S for client)
 * \param title The title put before the class name for the entitie's name (Friendly for Allies, Ennemy for Foes)
 * \param id_mod Set to -1 if initialising opponents, else set to 1
 * \return err_t OK
 */
err_t init_Entity(Entity * e,Coord spawn[NUM_CLASS], Direction d, char title[STR_SHORT], int id_mod);

/** \fn ent_init_test()
 * \brief Test function used to quickly intialise all entities for offline playtesting
 * \return err_t OK
 */
err_t ent_init_test();

/** \fn init_classes()
 * \brief Calls all init_classes functions to init all classes
 * \return err_t OK
 */
err_t init_classes();

/** \fn destroy_game()
 * \brief Destroys all classes and their abilities
 * \return err_t OK
 */
err_t destroy_game();

#endif