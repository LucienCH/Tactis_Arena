#include "gameplay.h"
#include "init_classes.h"
#include "init.h"
#include "display.h"
#include "border.h"
#include "text.h"
#include "game_window.h"
#include "animations.h"

/** \file passives.c
 * \brief Contains functions in relation to passive abilities
 * \author Robin BUTEL
 */

int Bloodlust_counter = 0;
bool Sentinel_counter = TRUE;

err_t activate_bloodlust(Entity *e, StateList * list)
{
    if(verbose>=1)printf("Bloodlust was triggered!\n");

    char log[STR_LONG];
    sprintf(log, "%s's Bloodlust was triggered", e->cha_name);
    addLog(log);

    selected_ability = -1;
    unhover();

    if (isLoaded(Fury))
        play_ability_animation(e->cha_class->cla_abilities[Fury%NUM_AB], e->coords);

    e->stats[atk] *= 2;

    e->act_points += 3;

    Status v = {e->stats[atk]/2,atk,1};
    
    Bloodlust_counter = -1;

    return list_add(list, v, e);
}

err_t activate_aura(Entity *e, StateList *list)
{   
    char log[STR_LONG];
    sprintf(log, "%s's Aura was triggered", e->cha_name);
    addLog(log);
    if(verbose>=1)printf("\n%s\n",log);

    /*selected_ability = -1;
    unhover();

    play_ability_animation(Aura_ab, e->coords);*/

    apply_to(Aura_ab, e, list, e->coords);

    return OK;
}

bool sentinel_check(Entity *e)
{

    if(Sentinel_counter)
    {
        if(verbose>=2)printf("Launching Sentinel Check\n");
        Entity * r;
        StateList * list;

        if(same_team(e,Allies))
        {
            r = &Foes[Ranger];
            list = stReceived;
        }
        else
        {
            r = &Allies[Ranger];
            list = stSent;
        }
        if(r->active != Dead)
        {
            int sight = get_range(r, Bolt);

            Coord t[MAXRANGE];

            setActionBorder(r->coords,sight,t);
            
            if(isInRange(t, e->coords))
            {
                Ability active_ab = r->cha_class->cla_abilities[Bolt%NUM_AB];
                char log[STR_LONG];
                sprintf(log, "%s's Sentinel was triggered", r->cha_name);
                if(verbose>=1)printf("%s\n",log);
                addLog(log);

                Sentinel_counter = FALSE;
                selected_ability = -1;
                unhover();

                if (isLoaded(active_ab.ab_id))
                    play_ability_animation(active_ab, e->coords);

                if(apply_to(active_ab, r, list, e->coords) != 0)
                {
                    return TRUE;
                }
            }
        }
    }
    else
    {
        if(verbose>=2)printf("Sentinel already triggered this turn\n");
    }
    return FALSE;
}