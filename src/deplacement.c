#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct.h"
#include "file_coord.h"
#include "gameplay.h"
#include "grid.h"
#include "characters.h"
#include "passives.h"
#include "map.h"
#include "game_window.h"
#include "print.h"
#include "display.h"
#include "abilities.h"
#include "turn.h"

/**
 * \file deplacement.c
 * \brief Mouvement program for TacticsArena
 * \details Contains pathfinding and movement fonctions
 * \author Louis LAFAY, Robin BUTEL, Thibault DOUCET
 */

Coord closest_free_tile(Coord c)
{
    Coord active = {-1,-1};
    File *maFile = initialiser();
    enfiler(maFile, c);
    Tile * t;
    int i;
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!file_vide(maFile))
    {
        active = defiler(maFile);
        t = getTile(active);
        if(verbose>=3)printf("x : %d    y : %d\n", active.x, active.y);
        if(verbose>=3)printf("Null : %d    Walk : %d\n", t->entity == NULL, t->walkable);

        if(t->entity == NULL && t->walkable)
        {
            detruire_file(maFile);
            break;
        }
        else
        {
            for(i=0; i<4; i++)
            {
                Coord a = add_coords(active, add[i]);
                if(isInGrid(a))
                {
                    enfiler(maFile, a);
                }
            }
        }
    }

    return active;
}


int * fill_tiles(Coord c, int matrice[_X_SIZE_][_Y_SIZE_], int max)
{
    //On initialise la matrice à -1
    int i, j;
    for(i = 0; i < _X_SIZE_; i ++)
    {
        for(j = 0; j < _Y_SIZE_; j ++)
        {
            matrice[i][j] = -1;
        }
    }

    File *maFile = initialiser();
    enfiler(maFile, c);
    matrice[c.x][c.y] = 0;
    Coord active;
    Coord active_2;
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    Tile * t;

    while(!file_vide(maFile))
    {
        active = defiler(maFile);

        if(verbose>=3)printf("Active : %d %d\n", active.x, active.y);

        for(i=0; i<4; i++)
        {
            active_2 = add_coords(active, add[i]);
            if(verbose>=3)printf("    Active_2 : %d %d\n", active_2.x, active_2.y);
            if(isInGrid(active_2))
            {
                if(verbose>=3)printf("        In Grid\n");
                t = getTile(active_2);

                if(matrice[active_2.x][active_2.y]==-1 && t->walkable && t->entity == NULL)
                {
                    if(verbose>=3)printf("        Adding\n");
                    matrice[active_2.x][active_2.y] = matrice[active.x][active.y] + 1;
                    if(matrice[active_2.x][active_2.y] < max)
                    {
                        enfiler(maFile, active_2);
                    }
                }
                else
                {
                    if(verbose>=3)printf("        Not Adding\n");
                }
            }
        }
    }
    if(verbose>=3)printf("Fill Tiles Done\n");
    return (int *)matrice;
}

Coord * pathfinding(int matrice[_X_SIZE_][_Y_SIZE_], Coord tabcoord[], Coord goal)
{
    Coord tab_bis[MAXRANGE];
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    Coord active;
    Coord is_lowest; 
    int i, lowest = 1000, t = 0;
    tab_bis[t++] = goal;
    for(i = 0; i < 4; i++)
    {
        active = add_coords(goal, add[i]);
        if(isInGrid(active) && matrice[active.x][active.y] < lowest && matrice[active.x][active.y] != -1)
        {
            is_lowest.x = active.x;
            is_lowest.y = active.y;
            lowest = matrice[active.x][active.y];
        }
    }

    while(matrice[is_lowest.x][is_lowest.y] > 0)
    {
        tab_bis[t++] = is_lowest;
        for(i = 0; i < 4; i ++)
        {
            active = add_coords(is_lowest , add[i]);
            if(isInGrid(active) && matrice[active.x][active.y] == lowest - 1)
            {
                is_lowest.x = active.x;
                is_lowest.y = active.y;
                lowest -= 1; 
                break;
            }
        }   
    }

    if(verbose>=2)
    {
        printf("Goal : ");
        print_Coord(&goal, "");
        printf("Origin : ");
        print_Coord(&is_lowest, "");
    }

    for(t -= 1, i = 0; t >= 0; t --, i ++)
    {
        tabcoord[i] = tab_bis[t];
    }

    tabcoord[i].x = -99;
    tabcoord[i].y = -99;
    if(verbose>=1)printf("Path Found\n");
    return tabcoord;
}

err_t simple_move(Entity * e, Coord tabcoord[])
{
    int i;
    Coord c;

    selected_ability = -1;

    if(verbose>=3)
    {
        for(i=0; tabcoord[i].x!=-99; i++)
        {
            print_Coord(&tabcoord[i], "");
        }
    }

    unselect();

    for(i = 0; tabcoord[i].x != -99; i ++)
    {
        c = tabcoord[i];
        if(verbose>=3)printf("Moving from (%d,%d) to (%d,%d)...\n", e->coords.x, e->coords.y, c.x, c.y);
        moveEntity(e->coords, c);
        Coord target = {e->coords.x, e->coords.y};
        setSelected(target);
        if(verbose>=3)printf("Completed\n");
        SDL_Delay(250);
        if(sentinel_check(e))
        {
            break;
        }
    }

    return OK;
}


err_t total_move(Entity * e, Coord tabcoord[])
{
    int i, cpt = 0;
    Coord c;
    Direction dir;
    Mix_Chunk *footstep = Mix_LoadWAV("../inc/sound_effects/footstep.wav");

    selected_ability = -1;

    unselect();

    for(i = 0; tabcoord[i].x != -99; i ++)
    {
        c = tabcoord[i];
        if (verbose>=0) printf("Moving from (%d,%d) to (%d,%d)...\n", e->coords.x, e->coords.y, c.x, c.y);
        if (c.x == e->coords.x+1)
        {
            cpt++;
            dir = S;
        }
        else if (c.x == e->coords.x-1)
        {
            cpt++;
            dir = N;
        }
        else if (c.y == e->coords.y+1)
        {
            cpt++;
            dir = E;
        }
        else if (c.y == e->coords.y-1)
        {
            cpt++;
            dir = W;
        }

        if (cpt > 5) cpt = 0;

        e->direction = dir;
        e->idAnim = cpt;

        moveEntity(e->coords, c);
        Mix_PlayChannel(-1, footstep, 0);
        Coord target = {e->coords.x, e->coords.y};
        setSelected(target);
        if(verbose>=0)printf("Completed\n");
        
        displayMap(renderer, XPOS, YPOS);

        SDL_Delay(150);

        if(sentinel_check(e) || trap_check(e))
        {
            break;
        }
    }

    e->idAnim = 0;

    if(verbose>=0)printf("Leaving Total-Move\n");
    return OK;
}