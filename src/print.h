#ifndef print_h
#define print_h
/** \file print.h
 * \brief Contains functions for printing various structures to standard output
 */
#include "struct.h"

/** \fn print_class_name(classId Id, char tab[STR_SHORT])
 * \brief prints classId in string form
 * \param Id classId to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_class_name(classId Id, char tab[STR_SHORT]);

/** \fn print_int(int * i, char tab[STR_SHORT])
 * \brief prints integer
 * \details mainly used to test our sendStruct and recep functions
 * \param i integer to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_int(int * i, char tab[STR_SHORT]);

/** \fn print_abilityId(abilityId id, char tab[STR_SHORT])
 * \brief prints abilityId enumeration in string form
 * \param id abilityId to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_abilityId(abilityId id, char tab[STR_SHORT]);

/** \fn print_targetType(targetType target, char tab[STR_SHORT])
 * \brief prints targetType in string form
 * \param target targetType to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_targetType(targetType target, char tab[STR_SHORT]);

/** \fn print_statId(statId id, char tab[STR_SHORT])
 * \brief prints statId in string form
 * \param id statId to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_statId(statId id, char tab[STR_SHORT]);

/** \fn print_statusId(statusId id, char tab[STR_SHORT])
 * \brief prints statusId in string form
 * \param id statusId to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_statusId(statusId id, char tab[STR_SHORT]);

/** \fn print_fnid(fnid id, char tab[STR_SHORT])
 * \brief prints fnid in string form
 * \param id fnid to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_fnid(fnid id,  char tab[STR_SHORT]);

/** \fn print_Damage(Damage **d, char tab[STR_SHORT])
 * \brief prints Damage structure
 * \param d Damage structure to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_Damage(Damage **d, char tab[STR_SHORT]);

/** \fn print_Coord(Coord * c, char tab[STR_SHORT])
 * \brief prints Coord structure
 * \param c Coord structure to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_Coord(Coord * c, char tab[STR_SHORT]);

/** \fn print_Coord_list(Coord ** c, int nb, char tab[STR_SHORT])
 * \brief prints a list of Coord structures
 * \param c list of Coord structures to print
 * \param nb number of coordinates in the list
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_Coord_list(Coord ** c, int nb, char tab[STR_SHORT]);

/** \fn print_action(action *a, char tab[STR_SHORT])
 * \brief prints an action structure
 * \param a action structure to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_action(action *a, char tab[STR_SHORT]);

/** \fn print_Status(Status s, char tab[STR_SHORT])
 * \brief prints a Status structure
 * \param s Status structure to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_Status(Status s, char tab[STR_SHORT]);

/** \fn print_Modifier(Modifier *m, char tab[STR_SHORT])
 * \brief prints a Modifier structure
 * \param m Modifier structure to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_Modifier(Modifier *m, char tab[STR_SHORT]);

/** \fn print_Modifier_list(Modifier **m, int nb, char tab[STR_SHORT])
 * \brief prints a list of Modifier structures
 * \param m list of Modifier structures to print
 * \param nb number of Modifiers in the list
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_Modifier_list(Modifier **m, int nb, char tab[STR_SHORT]);

/** \fn print_Ability(Ability *a, char tab[STR_SHORT])
 * \brief prints an Ability structure
 * \param a Ability structure to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_Ability(Ability *a, char tab[STR_SHORT]);

/** \fn print_StateList(StateList * list, char tab[STR_SHORT])
 * \brief prints a Statelist structure
 * \param list The Statelist to print
 * \param tab String to print before the name (used for formating the display)
 * \return err_t OK
 */
err_t print_StateList(StateList * list, char tab[STR_SHORT]);

/** \fn print_grid(Coord tab[])
 * \brief Prints a test grid
 * \param tab Coord array with coordinates where you want an X to be printed
 * \return err_t OK
 */
err_t print_grid(Coord tab[]);

/** \fn print_mage_passive(Entity *e)
 * \brief Prints the Mage's current moveset
 * \param e The Mage
 * \return err_t OK
 */
err_t print_mage_passive(Entity *e);

#endif