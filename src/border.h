#ifndef border_h
#define border_h
/** \file border.h
 * \brief Functions related to zones and borders
 */
#include "struct.h"
#include "grid.h"

/** \fn isInRange(Coord coorTab[], Coord target)
 * \brief Checks if certain coordinates are in a zone
 * \details Draws a line from the edge of the grid through the point and then to the other side, depending on the number of times the zone's border was crossed, that determines wether the coordinates are contained in the zone
 * \param coorTab the border of the zone
 * \param target the target coordinates
 * \return TRUE if the target is in range, FALSE otherwise
 */
bool isInRange(Coord coorTab[], Coord target);

/** \fn normalize(Coord c)
 * \brief Normalizes coordinates back into the grid
 * \details checks if coordinates are out of the grid and normalizes back to the closest point in the grid
 * \param c the coordinate to normalize
 * \return the normalized coord
 */
Coord normalize(Coord c);

/** \fn setActionBorder(Coord start, int range, Coord tab[])
 * \brief Creates an abilities' cast border
 * \details uses ability range and caster's vision to create a castable border in which that spell can be cast
 * \param start the caster's coordinates
 * \param range the result of the range function which uses the caster's vision and the ability's range to determine the castable range
 * \param tab an array in which to store the border
 * \return a pointer to the border (tab)
 */
Coord * setActionBorder(Coord start, int range, Coord tab[]);

/** \fn setActionZone(int range, Coord Border[], Coord Zone[])
 * \brief Creates an abilities' cast zone
 * \details uses the border and the isInRange function to fill out the zone
 * \param range the result of the range function which uses the caster's vision and the ability's range to determine the castable range, used to determine if a zone is needed
 * \param Border the border created by setActionBorder
 * \param Zone an array in which to store the zone
 * \return a pointer to the zone (Zone)
 */
Coord * setActionZone(int range, Coord Border[], Coord Zone[]);

/** \fn setMovementBorder(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[])
 * \brief Creates a movement range border
 * \details uses a matrix returned by fill_tiles to find the limits of the character's movement range, creates a border by finding all coords adjacent to none walkable tiles
 * \param matrice the matrix returned by fill_tiles
 * \param tab an array in which to store the border
 * \return a pointer to the border (tab)
 */
Coord * setMovementBorder(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[]);

/** \fn setMovementZone(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[])
 * \brief Creates a movement range zone
 * \details uses a matrix returned by fill_tiles to find the limits of the character's movement range, creates a zone by finding all walkeable coords
 * \param matrice the matrix returned by fill_tiles
 * \param tab an array in which to store the zone
 * \return a pointer to the zone (tab)
 */
Coord * setMovementZone(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[]);

/** \fn get_border(int cha_id, abilityId Id, Coord coorTab[], Coord zone[])
 * \brief Get the zone and the border for a certain ability/movement
 * \details calls the corresponding functions to create the border and zone for the given ability
 * \param cha_id the id of the entity who cast the ability, usefull for finding the starting coordinates
 * \param Id the abilityId of the cast ability
 * \param coorTab an array in which to store the border
 * \param zone an array in which to store the zone
 * \return a pointer to the zone (zone)
 */
Coord * get_border(int cha_id, abilityId Id, Coord coorTab[], Coord zone[]);

/** \fn Cast_check(action a, Coord coorTab[])
 * \brief Checks if an ability can be cast
 * \param a the action cast
 * \param coorTab the castable zone border to use with isInRange to verify the chosen tile
 * \details Checks if the selected coordinates are in the castable zone, as well as double checks if the character is not stopped, by status effects or cooldowns and such, from casting the ability
 * \return TRUE if the ability can be cast, FALSE otherwise
 */
bool Cast_check(action a, Coord coorTab[]);



#endif