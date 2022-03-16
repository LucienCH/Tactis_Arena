#ifndef init_classes_h
#define init_classes_h
/** \file init_classes.h
 * \brief Functions related to the intialisation and destruction of class information
 */
#include "substruct.h"

/** \fn init_aoe()
 * \brief Initialises AOE attack zones used for multiple different abilities
 * \return err_t INIT_COORD_ERR if initialised coordinates don't match the predicted number, err_t OK otherwise
 */
err_t init_aoe();

/** \fn init_repetitives(Ability * Move)
 * \brief Initialises structures used for multiple different abilities, also intialises the Movement ability
 * \param Move A pointer to an ability to store the created Movement ability
 * \return what init_aoe returns : err_t INIT_COORD_ERR if initialised coordinates don't match the predicted number, err_t OK otherwise
 */
err_t init_repetitives(Ability * Move);


/** \fn init_berserker(Class * c)
 * \brief Initialises the Berserker Class
 * \param c A pointer to a Class structure to store the Berserker's Class information
 * \return err_t POINTER_NULL if malloc fails, err_t OK otherwise
 */
err_t init_berserker(Class * c);

/** \fn init_ranger(Class * c)
 * \brief Initialises the Ranger Class
 * \param c A pointer to a Class structure to store the Ranger's Class information
 * \return err_t POINTER_NULL if malloc fails, err_t OK otherwise
 */
err_t init_ranger(Class * c);

/** \fn init_goliath(Class * c)
 * \brief Initialises the Goliath Class
 * \param c A pointer to a Class structure to store the Goliath's Class information
 * \return err_t POINTER_NULL if malloc fails, err_t OK otherwise
 */
err_t init_goliath(Class * c);

/** \fn init_mage(Class * c, Ability movesets[3][NUM_AB])
 * \brief Initialises the Mage Class
 * \param c A pointer to a Class structure to store the Mage's Class information
 * \param movesets A 2 dimension array to store the Mage's three different movesets
 * \return err_t POINTER_NULL if malloc fails, err_t OK otherwise
 */
err_t init_mage(Class * c, Ability movesets[3][NUM_AB]);

/** \fn init_valkyrie(Class * c)
 * \brief Initialises the Valkyrie Class
 * \param c A pointer to a Class structure to store the Valkyrie's Class information
 * \return err_t POINTER_NULL if malloc fails, err_t OK otherwise
 */
err_t init_valkyrie(Class * c);

/** \fn init_angel(Class * c, Ability *pass)
 * \brief Initialises the Angel Class
 * \param c A pointer to a Class structure to store the Angel's Class information
 * \param pass A pointer to an Ability structure to store Aura, Angel's passive ability
 * \return err_t POINTER_NULL if malloc fails, err_t OK otherwise
 */
err_t init_angel(Class * c, Ability *pass);


/** \fn ability_destroy(Ability * a)
 * \brief Destroys a given Ability structure
 * \details Frees all dynamically allocated pointers if they are not NULL, and sets them to NULL to avoid any double frees, this is why the Ability function has double pointers to Damage, Coord and Modifier so that we can limit ram usage by using the same structures for redundant information
 * \param a The ability to destroy
 * \return err_t OK
 */
err_t ability_destroy(Ability * a);

/** \fn class_destroy(Class * c)
 * \brief Destroys a given Class
 * \details A loop going through all of a Class' abilties and calling ability_destroy for each of them. Because of the Mage's peculiar moveset, this function does not destroy the mage, his abilities are destroyed by calling ability_destroy in a double loop navigating mage_ab (done in destoy_game)
 * \param c The class to destroy
 * \return err_t OK
 */
err_t class_destroy(Class * c);


#endif