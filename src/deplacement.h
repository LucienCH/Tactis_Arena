#ifndef deplacement_h
#define deplacement_h
#include "struct.h"
#include "grid.h"

/** \file deplacement.h
 * \brief Countains all fonctions needed to move a character
 */

/** \fn closest_free_tile(Coord c)
 * \brief Indicates the closest and walkable cell to the coordinate
 * \param c Actual coordinate
 * \return The closest and walkable coordinate
 */
Coord closest_free_tile(Coord c);


/** \fn fill_tiles(Coord c, int matrice[_X_SIZE_][_Y_SIZE_], int max)
 * \brief Fill in the matrix that indicates the number of cells between the starting coordinate and the current one
 * \param c Starting coordinate
 * \param matrice The matrix that will contain the number of boxes
 * \param max The max distance possible for the character to move
 * \return The filled matrix
 */
int * fill_tiles(Coord c, int matrice[_X_SIZE_][_Y_SIZE_], int max);


/** \fn pathfinding(int matrice[_X_SIZE_][_Y_SIZE_], Coord tabcoord[], Coord goal)
 * \brief Find the shortest path between the base coordinate and the desired one.
 * \param matrice The matrix that countains all the numbers (cf fn fill_tiles)
 * \param tabcoord The table that will contain each movement step
 * \param goal The final coordinates
 * \return tabcoord filled with all coords needed to move
 */
Coord * pathfinding(int matrice[_X_SIZE_][_Y_SIZE_], Coord tabcoord[], Coord goal);


/** \fn simple_move(Entity * e, Coord tabcoord[])
 * \brief Moves the character on each cells from the starting point to the finish one without animation
 * \param e The selected character
 * \param tabcoord The table that wich contain each movement step
 * \return OK(cf err_t)
 */
err_t simple_move(Entity * e, Coord tabcoord[]);


/** \fn total_move(Entity * e, Coord tabcoord[])
 * \brief Moves the character on each cells from the starting point to the finish one with animation
 * \param e The selected character
 * \param tabcoord The table that wich contain each movement step
 * \return OK(cf err_t)
 */
err_t total_move(Entity * e, Coord tabcoord[]);

#endif