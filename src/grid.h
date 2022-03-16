#ifndef grid_h
#define grid_h

/* =============== DEPENDENCES =============== */


#include "struct.h"
#include "textures.h"


/* =============== GRILLES DE JEU =============== */


extern Tile grid[_X_SIZE_][_Y_SIZE_];				// Grille du jeu
extern Tile blankGrid[_X_SIZE_][_Y_SIZE_];          // Grille de l'éditeur de map
extern Tile *matrix;
extern Tile *blankMatrix;


/* =============== FONCTIONS =============== */


void createGrid(Tile * grid, int seed, int x, int y);

void debugGrid(Tile * grid, int x, int y);

Entity * getEntity(Coord pos);

int Set_Trap(Trap_t trap, Coord pos);

Trap_t Get_Trap(Coord pos);

Tile getSelected();

Coord getSelectedPos();

void unselect();

void unhover();

void setHovered(Coord pos);

void setSelected(Coord pos);

void freezeWater(Coord pos);

Tile * getTile(Coord pos);

void turnRight(Entity * entity);

void turnLeft(Entity * entity);

bool isInGrid(Coord pos);

bool isWalkable(Coord pos);

bool isInCoordTab(Coord tab[], Coord pos);

Coord to2D(Coord pos);


#endif