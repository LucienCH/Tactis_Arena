#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "init.h"

/** \file print.c
 * \brief Contains functions for printing various structures to standard output
 * \author Robin BUTEL
 */

err_t print_class_name(classId Id, char tab[STR_SHORT])
{
    printf("%sClass : %s\n", tab, classes[Id].cla_name);
    return OK;
}

err_t print_int(int * i, char tab[STR_SHORT])
{
    printf("%s%d\n", tab, *i);
    return OK;
}

err_t print_abilityId(abilityId id, char tab[STR_SHORT])
{
    printf("%sId : %s\n", tab, IdNames[id]);
    return OK;
}

err_t print_targetType(targetType target, char tab[STR_SHORT])
{
    printf("%sTarget : %s\n", tab, targetNames[target]);
    return OK;
}

err_t print_statId(statId id, char tab[STR_SHORT])
{
    printf("%sStat : %s\n", tab, statName[id]);
    return OK;
}

err_t print_statusId(statusId id, char tab[STR_SHORT])
{
    printf("%sStatus : %s\n", tab, statusName[id]);
    return OK;
}

err_t print_fnid(fnid id,  char tab[STR_SHORT])
{
    printf("%sfnUse : %s\n", tab, fnNames[id]);
    return OK;
}

err_t print_Damage(Damage **d, char tab[STR_SHORT])
{
    if(d!=NULL)
    {
        printf("%sDamage : \n", tab);
        char tab2[STR_SHORT];
        sprintf(tab2, "%s   ", tab);
        printf("%sMultiplier : %.2f\n", tab2, (*d)->multiplier);
        print_statId((*d)->type, tab2);
    }
    else
    {
        printf("%sDamage : NULL\n", tab);
    }
    
    return OK;
}

err_t print_Coord(Coord * c, char tab[STR_SHORT])
{
    if(c!=NULL)
    {
        printf("%sX : %d  Y : %d\n", tab, c->x, c->y);
    }
    else
    {
        printf("%sCoords : NULL\n", tab);
    }
    return OK;
}

err_t print_Coord_list(Coord ** c, int nb, char tab[STR_SHORT])
{
    if(c!=NULL)
    {
        printf("%sCoord_nb : %d\n", tab, nb);
        char tab2[STR_SHORT];
        sprintf(tab2, "%s   ", tab);

        int i;
        for(i=0; i<nb; i++)
        {
            print_Coord((*c)+i, tab2);
        }
    }
    else
    {
        printf("%sCoords : NULL\n", tab);
    }
    
    return OK;
}

err_t print_action(action *a, char tab[STR_SHORT])
{
    printf("%sChar Id : %d\n", tab, a->char_id);
    print_abilityId(a->act, tab);
    print_Coord(&(a->c), tab);
    return OK;
}

err_t print_Status(Status s, char tab[STR_SHORT])
{
    char tab2[STR_SHORT];
    sprintf(tab2, "%s   ", tab);

    if(s.value!=0)
    {
        printf("%sStat Change : \n", tab);
        printf("%sValue : %d\n", tab2, s.value);
        print_statId(s.stat, tab2);
    }
    else
    {
        printf("%sStatus Effect : \n", tab);
        print_statusId(s.stat, tab2);
    }

    printf("%sDuration : %d\n", tab2, s.duration);
    
    return OK;
}

err_t print_Modifier(Modifier *m, char tab[STR_SHORT])
{
    if(m!=NULL)
    {
        printf("%sModifier : \n", tab);
        char tab2[STR_SHORT];
        sprintf(tab2, "%s   ", tab);

        print_Status(m->effect, tab2);
        printf("%sChance : %f\n", tab2, m->chance);
        print_targetType(m->t, tab2);
    }
    else
    {
        printf("%sModifier : NULL\n", tab);
    }

    return OK;
}

err_t print_Modifier_list(Modifier **m, int nb, char tab[STR_SHORT])
{
    if(m!=NULL)
    {
        printf("%sMod_nb : %d\n", tab, nb);
        char tab2[STR_SHORT];
        sprintf(tab2, "%s   ", tab);

        int i;
        for(i=0; i<nb; i++)
        {
            print_Modifier((*m)+i, tab2);
        }
    }
    else
    {
        printf("%sModifiers : NULL\n", tab);
    }
    
    return OK;
}

err_t print_Ability(Ability *a, char tab[STR_SHORT])
{
    if(a!=NULL)
    {
        printf("%s%s : \n", tab, a->eng.name);
        char tab2[STR_SHORT];
        sprintf(tab2, "%s   ", tab);

        print_abilityId(a->ab_id, tab2);
        printf("%sCost : %d\n", tab2, a->ab_cost);
        printf("%sCooldown : %d\n", tab2, a->ab_cooldown);
        printf("%sRange : %d\n", tab2, a->range);
        print_Damage(a->damage, tab2);
        print_Coord_list(a->coord, a->nb_coords, tab2);
        print_Modifier_list(a->mods, a->nb_mods, tab2);
        print_fnid(a->fn_use, tab2);
        printf("%sDesc : %s\n", tab2, a->eng.desc);
    }
    else
    {
        printf("%sAbility : NULL\n", tab);
    }

    return OK;
}

err_t print_StateList(StateList * list, char tab[STR_SHORT])
{
    if(list!=NULL)
    {
        if(!list_empty(list))
        {
            printf("\n%sStateList : \n\n", tab);

            start_list(list);
            while(!out_of_list(list))
            {
                printf("%sTarget : %s\n", tab, list->ec->entity->cha_name);
                print_Status(*(list->ec->value), tab);
                list_next(list);
            }
        }
        else
        {
            printf("%sStateList Empty\n", tab);
        }
    }
    else
    {
        printf("List is NULL\n");
    }
    return OK;
}

err_t print_grid(Coord tab[])
{
    char matrice[_X_SIZE_][_Y_SIZE_];

    int i,j;

    for(i=0; i<_X_SIZE_; i++)
    {
        for(j=0; j<_Y_SIZE_; j++)
        {
            matrice[i][j] = ' ';
        }
    }

    for(i=0; tab[i].x!=-99; i++)
    {
        matrice[tab[i].x][tab[i].y] = 'X';
    }    

    for(i=0; i<_X_SIZE_; i++)
    {
        printf("|");
        for(j=0; j<_Y_SIZE_; j++)
        {
            printf("%c|", matrice[i][j]);
        }
        printf("\n");
    }

    return OK;
}

err_t print_mage_passive(Entity *e)
{
    if(e->cha_class->cla_abilities == (Ability *)&mage_ab[0])
    {
        printf("%s : Fire\n", e->cha_name);
    }
    else if(e->cha_class->cla_abilities == (Ability *)&mage_ab[1])
    {
        printf("%s : Ice\n", e->cha_name);
    }
    else
    {
        printf("%s : Electricity\n", e->cha_name);
    }

    return OK;
}