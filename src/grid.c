/**
 * \file grid.c
 * \brief Contains all the functions relative to the grid of the game
 * \author Thibault DOUCET
 * \version 0.0.1
 * \date 28/01/2020
 */


/* =============== DEPENDENCES =============== */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"
#include "characters.h"
#include "common.h"
#include "game_window.h"
#include "struct.h"


/* =============== CONSTANTES =============== */


#define _X_SIZE_ 30
#define _Y_SIZE_ 30


/* =============== GRILLES DE JEU =============== */


Tile grid[_X_SIZE_][_Y_SIZE_];				// Grille du jeu
Tile blankGrid[_X_SIZE_][_Y_SIZE_];          // Grille de l'éditeur de map
Tile *matrix = &grid[0][0];
Tile *blankMatrix = &blankGrid[0][0];


/* =============== FONCTIONS =============== */


/**
 * \fn void createGrid(Entity * grid, int x, int y)
 * \param grid A grid of entity
 * \param x Row's number
 * \param y Column's number
 * \brief Initialize the grid with x*y size in parameter with default entities
 */
void createGrid(Tile * grid, int seed, int x, int y)
// create the grid with x*y size
{
    Trap_t trap = {0, FALSE};

    for (int i = 0; i < x*y; i++){
        grid[i].tile_id = rand()%seed;
        grid[i].selected = 0;
        grid[i].hovered = 0;
        grid[i].entity = NULL;
        grid[i].trap = trap;
        grid[i].walkable = grid[i].tile_id == 4 ? FALSE : TRUE;
    }
}

/**
 * \fn void debugGrid(Entity * grid, int x, int y)
 * \param grid Pointer to the begining of the entity matrix
 * \param x Row's number
 * \param y Column's number
 * \brief Display the entity ids of the matrix
 */
void debugGrid(Tile * grid, int x, int y)
// Debug by displaying the grid on the cmd
{
    for (int i = 0; i < x; i++){
        for (int j = 0; j < y; j++){
            if(verbose>=2)printf("%d|", (*(grid+i*x+j)).tile_id);
            if(verbose>=2)printf("%d ", (*(grid+i*x+j)).walkable);
            if ((*(grid+i*x+j)).entity != NULL && verbose >=2) printf("X");
        }
        if(verbose>=2)printf("\n");
    }
}

Entity * getEntity(Coord pos)
// Returns entity at given coord, if not return NULL
{
    if ((*(matrix+pos.x*_X_SIZE_+pos.y)).entity == NULL) return NULL;
    else return (*(matrix+pos.x*_X_SIZE_+pos.y)).entity;
}

int Set_Trap(Trap_t trap, Coord pos)
// Set a trap on a tile
{
    (*(matrix+pos.x*30+pos.y)).trap = trap;
    return 0;
}

Trap_t Get_Trap(Coord pos)
// Returns the trap at the given coordinates
{
    return (*(matrix+pos.x*_X_SIZE_+pos.y)).trap;
}

Tile getSelected()
// Return the selected tile
{
    Tile result;

    for (int i=0; i < _X_SIZE_; i++)
    {
        for (int j=0; j < _Y_SIZE_; j++)
        {
            if ((*(matrix+i*_X_SIZE_+j)).selected == 1)
            {
                result = (*(matrix+i*_X_SIZE_+j));
                break;
            }
        }
    }

    return result;
}

Coord getSelectedPos()
// Return the coordinates of the selected tile
{
    Coord result = {-1, -1};

    for (int i=0; i < _X_SIZE_; i++)
    {
        for (int j=0; j < _Y_SIZE_; j++)
        {
            if ((*(matrix+i*_X_SIZE_+j)).selected == 1)
            {
                result.x = i;
                result.y = j;
                break;
            }
        }
    }

    return result;
}

void unselect()
// Unselect the selected tile
{
    for (int i=0; i < _X_SIZE_; i++)
    {
        for (int j=0; j < _Y_SIZE_; j++)
        {
            if ((*(matrix+i*_X_SIZE_+j)).selected == 1) (*(matrix+i*_X_SIZE_+j)).selected = 0;
        }
    }
}

void unhover()
// Unselect the selected tile
{
    for (int i=0; i < _X_SIZE_; i++)
    {
        for (int j=0; j < _Y_SIZE_; j++)
        {
            if ((*(matrix+i*_X_SIZE_+j)).hovered == 1) (*(matrix+i*_X_SIZE_+j)).hovered = 0;
        }
    }
}

void setHovered(Coord pos)
// Set the tile at pos selected
{
    (*(matrix+pos.x*_X_SIZE_+pos.y)).hovered = 1;
}

void setSelected(Coord pos)
// Set the tile at pos selected
{
    unselect();
    (*(matrix+pos.x*_X_SIZE_+pos.y)).selected = 1;
}

void freezeWater(Coord pos)
// Turns the given water block to an ice block
{
    if ((*(matrix+pos.x*_X_SIZE_+pos.y)).tile_id == WATER)
    {
        (*(matrix+pos.x*_X_SIZE_+pos.y)).tile_id = ICE;
        (*(matrix+pos.x*_X_SIZE_+pos.y)).walkable = 1;
    }
    else
    {
        if ((*(matrix+pos.x*_X_SIZE_+pos.y)).tile_id != BLANK)
        {
            (*(matrix+pos.x*_X_SIZE_+pos.y)).tile_id = SNOW;
        }
    }
}

Tile * getTile(Coord pos)
// Returns the tile at the given coordinates
{
    return (matrix+pos.x*_X_SIZE_+pos.y);
}

void turnRight(Entity * entity)
// Turn the entity to the right
{
    if (entity->direction == W) entity->direction = N;
    else entity->direction++;
}

void turnLeft(Entity * entity)
// Turn the entity to the left
{
    if (entity->direction == N) entity->direction = W;
    else entity->direction--;
}

bool isInGrid(Coord pos)
// Return true if the given coord is in the grid
{
    return (pos.x < _X_SIZE_ && pos.x >= 0 && pos.y < _Y_SIZE_ && pos.y >= 0);
}

bool isWalkable(Coord pos)
// Return true if the given tile is walkable
{
    return (*(matrix+pos.x*_X_SIZE_+pos.y)).walkable == 1;
}

bool isInCoordTab(Coord tab[], Coord pos)
// Return true if the given coord is in the coord table
{
    int index = 0;
    bool result = FALSE;

    while (tab[index].x != -99)
    {
        if (tab[index].x == pos.x && tab[index].y == pos.y)
        {
            result = TRUE;
            break;
        }
        index++;
    }

    return result;
}

Coord to2D(Coord pos)
{
    Coord result;

    result.x = XPOS+(pos.y+1)*(pxBase/2)+(pos.x+1)*(pxBase/2);
	result.y = YPOS+pos.x*(pxBase/4)+(_Y_SIZE_-pos.y)*(pxBase/4);

    return result;
}