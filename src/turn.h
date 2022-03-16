#ifndef turn_h
#define turn_h

/** \file turn.h
 * \brief Header file for main functions relevant to players' turns
 */

#include "struct.h"

extern bool is_online;          //!< TRUE if the game is online, used to not try to send socket information when testing in offline
extern bool game_setup;         //!< TRUE if game is setting up, used to communicate game setup state between threads
extern bool applying_action;    //!< TRUE if an action is being applied not in the main thread, used to communicate to the communication loop to tell it not to override current action
extern bool loop_active;        //!< TRUE if the main loop is running, used to communicate with the communication thread wether an action can be applied
extern bool opponent_set;       //!< TRUE if an action was received from the opponent, used to communicate with the main loop to apply this action
extern winId game_over_global;  //!< Gloabl winId variable to keep track of game state in between threads
extern action a_global;         //!< Global action variable where received action is kept to communicate it between threads

/** \fn coin_flip() 
 * \brief Gives a random outcome between TRUE and FALSE
 * \details Decides which player starts by using the rand function
 * \return TRUE if the local(host) player starts
 */
bool coin_flip();

/** \fn coin_receive() 
 * \brief Receives the result of the coin flip
 * \details This function returns the opposite of what it received so that both coin_flip and coin_received return TRUE if their local player starts
 * \return TRUE if local(client) player starts
 */
bool coin_receive();

/** \fn online_setup() 
 * \brief Sets necessary variables to their correct values for online play
 * \details Sets is_online to TRUE and turn_active to FALSE
 * \return err_t OK
 */
err_t online_setup();

/** \fn endgame_message(winId Id) 
 * \brief Temporary function, sends a log telling the player wether they have won or lost
 * \param Id the winId determined at the end of the game in which the output of the game was stored
 * \return err_t OK
 */
err_t endgame_message(winId Id);

/** \fn your_turn() 
 * \brief Indicates if it's the local player's turn
 * \return TRUE if local player's turn
 */
bool your_turn();

/** \fn apply_movement(action a) 
 * \brief Applies the movement specified by the taken action
 * \details calls functions pathfinding to find path to target and total move to apply the movement specified
 * \param a the action variable that has the necessary information to apply the movement
 * \return err_t OK
 */
err_t apply_movement(action a);

/** \fn apply_action(action a) 
 * \brief Applies the ability specified by the taken action
 * \details Determines when and if ability method needs to be called, then calls apply_to (to apply damage and mods), if caster is ennemy mage, sets correct ability, applies ability cooldown and calculats ennemies killed to trigger bloodlust if necessary.
 * \param a the action variable that has the necessary information to apply the ability
 * \return err_t OK
 */
err_t apply_action(action a);

/** \fn turn_start(Entity *e) 
 * \brief Applies necessary actions to start a turn
 * \details Restores action points as well as applies Angel's Aura
 * \param e The list of entities whose turn will start
 * \return err_t OK
 */
err_t turn_start(Entity *e);

/** \fn turn_end(Entity *e, StateList *list)
 * \brief Applies necessary actions to end a turn
 * \details Decreases buff and debuff durations, applies burning damage, and decreases ability cooldowns
 * \param e the list of entities whoe turn just ended
 * \param list the statelist to decrease buffs/debuffs from, stSent if local turn, stReceived if opposing turn
 * \return err_t OK
 */
err_t turn_end(Entity *e, StateList *list);

/** \fn test_turn() 
 * \brief Temporary test function used to applied turn_start and turn_end while giving playability back to local player, for testing gameplay offline
 * \return err_t OK
 */
err_t test_turn();

/** \fn set_endturn() 
 * \brief Communicates an end button input
 * \details Sets necessary variables to end local turn, if not local turn, shows log warning
 * \return err_t OK
 */
err_t set_endturn();

/** \fn play_check() 
 * \brief Checks if allies still have characters who can play
 * \details Used to automaticaly give playability to opponent once all action points are used
 * \return TRUE if local player can still play
 */
bool play_check();

/** \fn opponent_action() 
 * \brief Calls necessary functions to apply action received from opponent
 * \details Action to apply is stored in a_global
 * \return err_t OK
 */
err_t opponent_action();

/** \fn action_set(action a) 
 * \brief Calls necessary functions to apply local action
 * \details Communicates cast action with opponent
 * \param a action variable containing information on cast ability
 * \return err_t OK
 */
err_t action_set(action a);

/** \fn local_turn() 
 * \brief Main local turn loop function
 * \details Checks for game over conditions
 * \return winId indicating if game was lost or won
 */
winId local_turn();

/** \fn opposing_turn() 
 * \brief Main opposing turn loop function
 * \details Receives opponent actions and indicates to main thread an action was received
 * \return winId indicating if game was lost or won
 */
winId opposing_turn();

/** \fn game_loop(winId (*turn1)(void), winId (*turn2)(void)) 
 * \brief Game loop used for turn management
 * \param turn1 either local_turn or opposing_turn depending on who is going to play first, determined by the coin toss
 * \param turn2 either local_turn or opposing_turn depending on who is going to play second, determined by the coin toss
 * \return winId indicating if game was lost or won
 */
winId game_loop(winId (*turn1)(void), winId (*turn2)(void));

/** \fn init_client() 
 * \brief Initialises things client side
 * \details Initialises character spawns for the client, calls game_loop with corresponding parametres
 * \return winId indicating if game was lost or won
 */
winId init_client();

/** \fn init_server() 
 * \brief Initialises things server side
 * \details Initialises character spawns for the host, calls game_loop with corresponding parametres
 * \return winId indicating if game was lost or won
 */
winId init_server();

#endif

