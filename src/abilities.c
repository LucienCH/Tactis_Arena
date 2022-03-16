#include "struct.h"
#include "init.h"
#include "gameplay.h"
#include "grid.h"
#include "game_window.h"
#include "characters.h"
#include "display.h"
#include "deplacement.h"
#include "border.h"
#include "print.h"
#include "animations.h"

/** \file abilities.c
 * \brief Ability method functions
 * \details Contains all ability methode functions as well as small other functions relative purely to abilities
 * \author Robin BUTEL
 */

int Killing_Blow_fn(Coord c, Entity * e, StateList * list)
{
    Status v = {3,atk,0};
    Entity * t;
    t = getEntity(c);
    if(t!=NULL)
    {
        if(t->active == Dead)
        {
            char log[STR_LONG];
            sprintf(log, "%s's Killing Blow was triggered", e->cha_name);
            addLog(log);
            apply_stat_change(v, t, list, TRUE);
            e->act_points+=1;
        }
    }
    return 0;
}

int Fury_fn(Coord c, Entity * e, StateList * list)
{
    int turns = 0;
    List_Elem * v;

    start_list(stSent);
    while((v = list_search(stSent, e, -1)) !=NULL)
    {
        if(!list_check(stSent))
        {
            turns += v->value->duration;
            remove_mod(v->value,e, TRUE);
            list_remove(stSent);
        }
        list_next(stSent);
    }

    start_list(stReceived);
    while((v = list_search(stReceived, e, -1)) !=NULL)
    {
        if(!list_check(stReceived))
        {
            turns += v->value->duration;
            remove_mod(v->value,e, TRUE);
            list_remove(stReceived);
        }
        list_next(stReceived);
    }

    Status d = {turns, atk, 0};

    apply_stat_change(d, e, list, TRUE);

    return 0;
}

int Frenzied_Dash_fn(Coord c, Entity * e, StateList * list)
{
    moveEntity(e->coords, c);
    return 0;
}

int Focus_fn(Coord c, Entity * e, StateList * list)
{
    List_Elem * v;

    start_list(stSent);
    while((v = list_search(stSent, e, -1)) !=NULL)
    {
        if(list_check(stSent))
        {
            list_change(stSent, 2);
        }
        else
        {
            if(list_change(stSent, -1)!=NULL)
            {
                remove_mod(v->value,e, TRUE);
                list_remove(stSent);
            }
        }
        list_next(stSent);
    }

    start_list(stReceived);
    while((v = list_search(stReceived, e, -1)) !=NULL)
    {
        if(list_check(stReceived))
        {
            list_change(stReceived, 2);
        }
        else
        {
            if(list_change(stReceived, -1)!=NULL)
            {
                remove_mod(v->value,e, TRUE);
                list_remove(stReceived);
            }
        }
        list_next(stReceived);
    }

    return 0;
}

int Trap_fn(Coord c, Entity * e, StateList * list)
{
    Trap_t t = {e->cha_id, same_team(e, Allies)};
    Set_Trap(t, c);
    return 0;
}

bool trap_check(Entity *e)
{
    Status s = {0, Cripple, 2};

    Trap_t trap = Get_Trap(e->coords);

    if(verbose>=2)printf("Trap Check\n");

    if(verbose>=3)
    {
        printf("Trap Id = %d\n", trap.cha_id);
        printf("Trap Visibility : %d\n", trap.visible);
    }

    if(!same_team(e, (void *)&trap))
    {
        StateList * list;
        int Id;
        char log[STR_LONG];

        if(same_team(e,Allies))
        {
            list = stSent;
            Id = Foes[Ranger].cha_id;
        }
        else
        {
            list = stReceived;
            Id = Allies[Ranger].cha_id;
        }

        trap.visible = TRUE;
        Set_Trap(trap, e->coords);

        sprintf(log,"%s stepped into a trap", e->cha_name);
        addLog(log);

        apply_status(s, e, list, Id, TRUE);

        return TRUE;
    }

    return FALSE;
}

int Detain_fn(Coord c, Entity *e, StateList * list)
{
    Tile * t= getTile(c);
    t->entity = NULL;

    return 0;
}

int Banner_fn(Coord c, Entity * e, StateList * list)
{
    Entity * all;
    get_team(e, &all, TRUE);

    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        reset_cooldowns(all+i);
    }

    char log[STR_LONG];
    char log_2[STR_SHORT];

    if(same_team(e, Allies))
    {
        sprintf(log_2, "ally");
    }
    else
    {
        sprintf(log_2, "ennemy");
    }
    
    sprintf(log, "%s has reset all %s characters' cooldowns", e->cha_name, log_2);
    addLog(log);

    return 0;
}

int mage_switch(Coord c, Entity * e, StateList * list)
{
    if(e->cha_id>0)
    {
        int i = rand()%2;

        if((Ability*)mage_ab+(i*NUM_AB) != e->cha_class->cla_abilities)
        {
            e->cha_class->cla_abilities = (Ability*)mage_ab+(i*NUM_AB);
        }
        else
        {
            e->cha_class->cla_abilities = (Ability*)mage_ab+(2*NUM_AB);
        }
    }
    return 0;
}

int FlameCharge_fn(Coord c, Entity * e, StateList * list)
{
    Ability F = e->cha_class->cla_abilities[FlameCharge%NUM_AB];
    
    Coord length;
    length = compare_coords(e->coords,c);
    if(verbose>=2)print_Coord(&length, "Flamecharge path length : ");
    int l = abs(length.x)+abs(length.y);

    Coord * path = malloc(sizeof(Coord)*l);
    Coord temp = {0,0};

    int i;
    for(i=0; i<l; i++)
    {
        if(abs(length.x)>=abs(length.y))
        {
            if(length.x > 0)
            {
                temp.x += 1;
                length.x += -1;
            }
            else
            {
                temp.x += -1;
                length.x += 1;
            }
        }
        else
        {
            if(length.y > 0)
            {
                temp.y += 1;
                length.y += -1;
            }
            else
            {
                temp.y += -1;
                length.y += 1;
            }
        }
        path[i] = temp;
    }

    if(verbose>=2)
    {
        printf("Flamecharge Path : \n");
        print_Coord_list(&path,l,"  ");
    }

    F.coord = &path;
    F.nb_coords = l;
    F.fn_use = NONE;

    Tile * t =getTile(e->coords);
    t->entity = NULL;
    selected_ability = -1;
    unhover();

    printf("%s\n", error_message[play_ability_animation(F, e->coords)]);
    int n = apply_to(F,e,list,e->coords);
    t->entity = e;
    moveEntity(e->coords, c);
    free(path);
    return n;
}

int Flare_fn(Coord c, Entity * e, StateList * list)
{
    Entity * all;
    get_team(e, &all, TRUE);

    Status b_vis = {4,vis,2};
    Status b_mv = {4,mv,2};

    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        apply_stat_change(b_vis,all+i,list, FALSE);
        apply_stat_change(b_mv,all+i,list, FALSE);
    }

    if(all == Allies)
    {
        addLog("Allies' vision and movement were increased by 4 for 2 turns");
    }
    else
    {
        addLog("Ennemies' vision and movement were increased by 4 for 2 turns");
    }

    Ability a = e->cha_class->cla_abilities[Flare%NUM_AB];
    Trap_t t;
    Coord c2;

    for(i=0; i<a.nb_coords; i++)
    {
        c2 = add_coords(c , *((*(a.coord))+i));
        if(isInGrid(c2))
        {
            t = Get_Trap(c2);
            if(t.cha_id<0)
            {
                t.visible = TRUE;
                Set_Trap(t,c2);
            }
        }
    }

    return 0;
}

int Blizzard_fn(Coord c, Entity * e, StateList * list)
{
    freezeWater(c);
    return 0;
}

int Volt_Switch_fn(Coord c, Entity * e, StateList * list)
{
    Entity * t = getEntity(c);

    char log[STR_LONG];
    sprintf(log, "%s switched with %s", e->cha_name, t->cha_name);
    addLog(log);

    if(t->status_effect[Cripple])
    {
        Status s;
        remove_mod(remove_from_list(t,Cripple,&s),t, TRUE);
    }

    switchEntities(c, e->coords);

    return 0;
}

int Lightning_Chain_fn(Coord c, Entity * e, StateList * list)
{
    Entity * all;
    get_team(e, &all, FALSE);

    Coord ct = c;
    Coord t[MAXRANGE];
    int i,j,d=0;
    Entity * target = NULL;
    Coord closest = {-99, -99} , ctemp;

    for(i=0; i<3; i++)
    {
        setActionBorder(ct, 6, t);
        for(j=0; j<NUM_CLASS; j++)
        {
            if(isInRange(t, (all+j)->coords) && !same_coord(ct, (all+j)->coords))
            {
                if(closest.x == -99)
                {
                    target = all+j;
                    closest = compare_coords(ct, target->coords);
                }
                else
                {
                    ctemp = compare_coords(ct, (all+j)->coords);
                    if(closer_coords(ctemp, closest))
                    {
                        target = all+j;
                        closest = ctemp;
                    }
                }
            }
        }

        if(target != NULL)
        {
            char log[STR_LONG];
            sprintf(log, "The lightning bounced to %s", e->cha_name);
            addLog(log);

            selected_ability = -1;
            unhover();
            printf("%s\n", error_message[play_ability_animation(mage_ab[2][Lightning_Chain%NUM_AB], target->coords)]);

            apply_damage(*(mage_ab[2][Lightning_Chain%NUM_AB].damage), e, target, TRUE);
            ct = target->coords;
            closest.x = -99;
            target = NULL;
        }
        else
        {
            addLog("No one was close enough to bounce to");
            break;
        }
    }

    addLog("The lighting died out");

    return d;
}

int Thrust_fn(Coord c, Entity * e, StateList * list)
{
    Coord c2 = add_coords(c,compare_coords(e->coords,c));
    if(isInGrid(c2))
    {
        Entity * target = getEntity(c2);
        if(target!=NULL)
        {
            apply_mod(*(*(e->cha_class->cla_abilities[Thrust%NUM_AB].mods)), target, list, e->cha_id);
            if(apply_damage(*(e->cha_class->cla_abilities[Thrust%NUM_AB].damage), e, target, TRUE))
            {
                return 1;
            }
        }
    }
    return 0;
}

int Life_Transfer_fn(Coord c, Entity * e, StateList * list)
{
    Entity *f;
    Entity *t;

    Entity * all;
    get_team(e, &all, FALSE);

    int i;
    Coord closest = {-99,-99}, ctemp;

    for(i=0; i<NUM_CLASS; i++)
    {
        if((all+i)->active)
        {
            if(closest.x == -99)
            {
                t = all+i;
                closest = compare_coords(c, t->coords);
            }
            else
            {
                ctemp = compare_coords(c, (all+i)->coords);
                if(closer_coords(ctemp, closest))
                {
                    t = all+i;
                    closest = ctemp;
                }
            }
        }
    }

    f=getEntity(c);

    int h = f->cha_class->basic_stats[pv] - f->stats[pv];
    f->stats[pv] = 20;

    Status s = {-h, mv,3};

    apply_stat_change(s,t,list, TRUE);
    return 0;
}

int Gates_of_Valhalla_fn(Coord c, Entity * e, StateList * list)
{
    int count = count_dead_allies(e);
    Entity * tab[count];
    get_dead_allies(e, tab);

    int i;

    Status s = {0, Summoned, 1};

    for(i=0; i<count; i++)
    {
        apply_status(s,tab[i], list, e->cha_id, FALSE);
        tab[i]->active = Alive;
        free_spawn(tab[i]);
    }

    return 0;
}

int Last_Sacrifice_fn(Coord c, Entity * e, StateList * list)
{

    new_death(e);

    Entity * all;
    get_team(e, &all, TRUE);
    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        if(same_coord((all+i)->coords,c))
        {
            break;
        }
    }

    free_spawn(all+i);
    (all+i)->active = Alive;

    return 0;
}

int Gods_Blessing_fn(Coord c, Entity *e, StateList * list)
{
    Entity * t = getEntity(c);

    if(t->status_effect[Paralyzed])
    {
        t->act_points += 1;
    }
    else
    {
        t->act_points += 3;
    }

    return 0;
}