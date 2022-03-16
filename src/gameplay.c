#include <string.h>
#include <stdlib.h>
#include "init.h"
#include "grid.h"
#include "game_window.h"
#include "display.h"
#include "gameplay.h"
#include "deplacement.h"
#include "text.h"
#include "print.h"
#include "turn.h"
#include "servFcnt.h"

/** \file gameplay.c
 * \brief Many Miscellaneous Functions related to gameplay
 * \author Robin BUTEL
 */

#define DEF_MULT 15 //!< Defining the defense multiplier for balancing purposes

err_t rec_id_swap(action * a)
{
    a->char_id *= -1;

    return OK;
}

Entity * e_from_id(int Id)
{
    if(Id<0)
    {
        return &Foes[(Id*-1)-1];
    }
    else
    {
        return &Allies[Id-1];
    }
}

Ability * ab_from_id(abilityId Id, Entity *e)
{
    return &(e->cha_class->cla_abilities[Id%NUM_AB]);
}

err_t get_team(Entity *e, Entity **all, bool same)
{
    if(same)
    {
        if(e->cha_id<0)
        {
            *all = Foes;
        }
        else
        {
            *all = Allies;
        }
    }
    else
    {
        if(e->cha_id<0)
        {
            *all = Allies;
        }
        else
        {
            *all = Foes;
        }
    }

    return OK;
}

winId game_over()
{
    winId all_dead = LOSE;
    int i;

    for(i=0; i<NUM_CLASS && all_dead; i++)
    {
        all_dead = Allies[i].active ? ONGOING : LOSE;
    }

    if(!all_dead)
    {
        all_dead = WIN;
        for(i=0; i<NUM_CLASS && all_dead; i++)
        {
            all_dead = Foes[i].active ? ONGOING : WIN;
        }
    }

    return all_dead;
}

int get_range(Entity *e, abilityId ab)
{
    float vision = e->stats[vis];
    float range_mod = e->cha_class->cla_abilities[ab%NUM_AB].range;


    int range = (range_mod * (vision/10)) + 0.4;

    if(range<0)
    {
        range = 0;
    }

    return range;
    
}

int count_dead_allies(Entity *e)
{
    Entity * all;
    get_team(e, &all, TRUE);
    int i, death_count = 0;
    for(i=0; i<NUM_CLASS; i++)
    {
        if((all+i)->active == Dead)
        {
            death_count++;
        }
    }

    return death_count;
}

Entity ** get_dead_allies(Entity *e, Entity * tab[])
{
    Entity * all;
    get_team(e, &all, TRUE);
    int i,j=0;
    for(i=0; i<NUM_CLASS; i++)
    {
        if((all+i)->active == Dead)
        {
            tab[j++] = all+i;
        }
    }

    return (Entity **)tab;
}

int get_cost(Entity *e, abilityId Id)
{
    if(Id!=Mvt)
    {
        return e->cha_class->cla_abilities[Id%NUM_AB].ab_cost;
    }
    else
    {
        return 1;
    }
}

int get_cooldown(Entity * e, abilityId Id)
{
    if(Id!=Mvt)
    {
        return e->ab_cooldown[Id%NUM_AB];
    }
    else
    {
        return 0;
    }
}

char * get_name(Entity * e, abilityId ab_id)
{
    if(ab_id!=Mvt)
    {
        return e->cha_class->cla_abilities[ab_id%NUM_AB].eng.name;
    }
    else
    {
        return Move_ab.eng.name;
    }
    
}

char * get_desc(Entity * e, abilityId ab_id)
{
    if(ab_id!=Mvt)
    {
        return e->cha_class->cla_abilities[ab_id%NUM_AB].eng.desc;
    }
    else
    {
        return Move_ab.eng.desc;
    }
}

castabilityId able_ability(Entity *e, abilityId ab_id, bool show_logs)
{
    if(e->act_points < get_cost(e, ab_id))
    {
        if(show_logs)addLog("Not enough action points for that");
        return Locked_c;
    }
    else if(get_cooldown(e, ab_id)!=0)
    {
        if(show_logs)
        {
            char log[STR_SHORT];
            sprintf(log, "%s is on cooldown", get_name(e, ab_id));
            addLog(log);
        }
        return Locked_c;
    }
    else if(e->status_effect[Freezing])
    {
        if(show_logs)
        {
            char log[STR_SHORT];
            sprintf(log, "%s is frozen", get_name(e, ab_id));
            addLog(log);
        }
        return Frozen_c;
    }
    else if(e->status_effect[Cripple])
    {
        if(ab_id == Mvt || ab_id == Frenzied_Dash || ab_id == FlameCharge || ab_id == Volt_Switch)
        {
            return Crippled_c;
        }
    }

    return Castable_c;
}

bool is_ally(Entity *e)
{
    if(e->cha_id>0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool same_team(Entity *a, Entity *b)
{
    if(a->cha_id<0&&b->cha_id<0)
    {
        if(verbose>=3)printf("Same Team Exit 1\n");
        return TRUE;
    }
    else if(a->cha_id>0&&b->cha_id>0)
    {
        if(verbose>=3)printf("Same Team Exit 2\n");
        return TRUE;
    }
    else if(a->cha_id == 0 || b->cha_id ==0)
    {
        if(verbose>=3)printf("Same Team Exit 3\n");
        return TRUE;
    }
    else
    {
        if(verbose>=3)printf("Same Team Exit 4\n");
        return FALSE;
    }
}

err_t end_Detain(Entity *e)
{
    Entity * all;
    get_team(e, &all, FALSE);
    e->coords = (all + Goliath)->coords;
    free_spawn(e);
    return OK;
}

bool tile_type(Coord c, targetType targeting, Entity * e)
{

    if(verbose>=1)print_targetType(targeting, "");

    if(targeting!=ANY_TILE)
    {
        Tile * t = getTile(c);

        if(t->entity==NULL)
        {
            if(targeting == FREE_TILE && t->walkable)
            {
                return TRUE;
            }
        }
        else if(targeting==BOTH)
        {
            return TRUE;
        }
        else if(same_team(e,t->entity) && targeting == ALLIES)
        {
            return TRUE;
        }
        else if((!same_team(e,t->entity)) && targeting == FOES)
        {
            return TRUE;
        }
    }
    else
    {
        return TRUE;
    }

    return FALSE;
}

err_t reset_cooldowns(Entity * e)
{
    int i;
    for(i=0; i<NUM_AB; i++)
    {
        e->ab_cooldown[i] = 0;
    }

    return OK;
}

bool same_coord(Coord a, Coord b)
{
    return a.x == b.x && a.y == b.y;
}

bool closer_coords(Coord a, Coord b)
{
    return sqrt(a.x*a.x+a.y+a.y) <= sqrt(b.x*b.x+b.y*b.y);
}

Coord compare_coords(Coord a, Coord b)
{
    Coord c;

    c.x = b.x - a.x;
    c.y = b.y - a.y;

    return c;
}

Coord add_coords(Coord a, Coord b)
{
    Coord c;

    c.x = a.x + b.x;
    c.y = a.y + b.y;

    if(verbose>=3)printf("Coords a(%d,%d) and b(%d,%d) added to give c(%d,%d)\n", a.x, a.y, b.x, b.y, c.x, c.y);

    return c;
}

err_t free_spawn(Entity *e)
{
    if(e != NULL)
    {
        if(verbose>=2)printf("\n%s : \n", e->cha_name);
        if(verbose>=2)printf("    before : x : %d y : %d\n", e->coords.x, e->coords.y);
        Coord spawn = closest_free_tile(e->coords);
        if(verbose>=2)printf("    return : x : %d y : %d\n", spawn.x, spawn.y);
        e->coords = spawn;
        if(verbose>=2)printf("    after : x : %d y : %d\n", e->coords.x, e->coords.y);
        Tile *t = getTile(spawn);
        t->entity = e;

        return OK;
    }
    else
    {
        return POINTER_NULL;
    }
}

err_t remove_mod(Status * stat, Entity * e, bool show_log)
{
    if(stat!=NULL)
    {
        char log[STR_LONG];

        if(stat->value==0)
        {
            if(verbose>=2)printf("Modifier to remove is a status effect!\n");
            e->status_effect[stat->stat] = 0;
            sprintf(log, "%s is no longer %s", e->cha_name, statusName[stat->stat]);
            
            if(stat->stat == Summoned)
            {
                new_death(e);
            }
            else if(stat->stat == Detained)
            {
                end_Detain(e);
            }
            else if (stat->stat == Cripple)
            {
                Trap_t trap = {0,FALSE};
                Set_Trap(trap, e->coords);
            }
        }
        else
        {
            if(verbose>=2)printf("Modifier to remove is a stat change of a value of %d\n", stat->value);
            if(verbose>=2)printf("Stat before the change : %d\n", e->stats[stat->stat]);

            e->stats[stat->stat] += stat->value *-1;

            if(verbose>=2)printf("Stat after the change : %d\n", e->stats[stat->stat]);

            if(stat->value < 0)
            {
                sprintf(log, "%s no longer has decreased %s", e->cha_name, statName[stat->stat]);
            }
            else
            {
                sprintf(log, "%s no longer has increased %s", e->cha_name, statName[stat->stat]);
            }
        }

        if(verbose>=2)printf("%s\n", log);

        if(show_log)
        {
            addLog(log);
        }

        return OK;
    }
    else
    {
        return POINTER_NULL;
    }
}

Status * remove_from_list(Entity * e, statusId status, Status *s)
{
    char log[STR_LONG];
    if(verbose>=2)
    {
        sprintf(log,"Removing from list %s's ",e->cha_name);
        print_statusId(status, log);
    }

    List_Elem * v;

    start_list(stReceived);
    v = list_search(stReceived,e, status);
    if(v!=NULL)
    {   
        *s = *(v->value);
        list_remove(stReceived);
    }
    else
    {
        start_list(stSent);
        v = list_search(stSent,e, status);
        if(v!=NULL)
        {
            *s = *(v->value);
            list_remove(stSent);
        }
    }

    if(verbose>=2)
    {
        sprintf(log,"Finished removing from list %s's ",e->cha_name);
        print_statusId(status, log);
    }

    return s;
}

err_t new_death(Entity * e)
{
    reset_cooldowns(e);

    e->active = Dead;
    e->stats[pv]=20;

    List_Elem * v;

    start_list(stSent);
    while((v = list_search(stSent,e,-1))!=NULL)
    {
        remove_mod(v->value, e, FALSE);
        list_remove(stSent);
        list_next(stSent);
    }

    start_list(stReceived);
    while((v = list_search(stReceived,e,-1))!=NULL)
    {
        remove_mod(v->value, e, FALSE);
        list_remove(stReceived);
        list_next(stReceived);
    }

    if(e->cha_class->cla_id == Goliath)
    {
        StateList * list;
        if(same_team(e, Allies))
        {
            list = stSent;
        }
        else
        {
            list = stReceived;
        }

        start_list(list);
        while((v = list_search(list, NULL, Detained))!=NULL)
        {
            remove_mod(v->value, v->entity, TRUE);
            list_remove(list);
            list_next(list);
        }
    }

    char log[STR_LONG];
    sprintf(log, "%s has died", e->cha_name);
    addLog(log);

    //PLAY DEATH ANIMATION
    Tile * t = getTile(e->coords);
    t->entity = NULL;

    if(verbose>=2)printf("%s reset after death\n", e->cha_name);

    return OK;
}

bool death_check(Entity * e)
{
    if(e->stats[pv]<=0)
    {
        if(verbose>=1)printf("%s was killed!\n", e->cha_name);
        new_death(e);
        return TRUE;
    }
    else
        return FALSE;
}

bool apply_check(Modifier * mod, Entity * target)
{
    bool flag;

    if(your_turn())
    {
        if(mod!=NULL)
        {
            if((mod->chance*100)>=((rand()%100)+1))
            {
                if(verbose>=1)printf("Modifier Landed\n");
                flag = TRUE;
            }
            else
            {
                if(verbose>=1)printf("Modifier not Landed\n");
                flag = FALSE;
            }
        }
        else if(target->cha_class->cla_id == Goliath)
        {
            int block = target->status_effect[Guarding] == 1 ? 70 : 30;
            if(verbose>=2)printf("Block chance : %d\n", block);

            if(block>=(rand()%100+1))
            {
                if(verbose>=1)printf("Block Successful!\n");              
                flag = FALSE;
            }
            else
            {
                if(verbose>=2)printf("Block Failed!\n");
                flag = TRUE;
            }
        }
        else
        {
            flag = TRUE;
        }
        if(is_online)
        {
            printf("%s",error_message[sendStruct(&flag, sizeof(bool), socketConnected, (err_t (*)(void *, char *))print_int)]);
        }
    }
    else if(is_online)
    {
        recep(&flag, sizeof(bool),socketConnected, (err_t (*)(void *, char *))print_int); 
    }
    else
    {
        flag = TRUE;
    }

    return flag;
}

bool apply_damage(Damage * d, Entity * caster, Entity * target, bool show_log)
{
    char log[STR_LONG];

    float frozen = target->status_effect[Freezing] == 1 ? 6 : 0;

    float crippled = target->status_effect[Cripple] == 1 ? 1.75 : 1;

    if(crippled==1.75)
        if(verbose>=1)printf("%s is crippled!\n", target->cha_name);

    if(frozen==6)
        if(verbose>=1)printf("%s is frozen and has increased resistances!\n", target->cha_name);

    if(verbose>=2)printf("%s's health before the attack : %d\n", target->cha_name, target->stats[pv]);

    int d_value;
    float stat_dmg = caster->stats[d->type];
    float stat_def = target->stats[d->type+2];

    if(caster->status_effect[Piercing])
    {
        if(verbose>=2)printf("%s has piercing!\n", caster->cha_name);
        d_value = (stat_dmg * d->multiplier * crippled) +0.4;
        sprintf(log, "%s pierced %s's defence and dealt %d damage", caster->cha_name, target->cha_name, d_value);
    }
    else
    {
        d_value = ((stat_dmg/(1+((stat_def+frozen)/DEF_MULT))) * d->multiplier * crippled) + 0.4;
        sprintf(log, "%s dealt %d damage to %s", caster->cha_name, d_value, target->cha_name);
    }

    if(verbose>=2)
    {
        printf("stat_dmg : %f\n", stat_dmg);
        printf("stat_def : %f\n", stat_def);
        printf("multiplier : %f\n", d->multiplier);
        printf("DEF_MULT : %d\n", DEF_MULT);
        printf("frozen : %f\n", frozen);
        printf("crippled : %f\n", crippled);
        printf("d_value : %d\n", d_value);
    }

    target->stats[pv] -= d_value;

    if(verbose>=2)printf("%s's health after the attack : %d\n", target->cha_name, target->stats[pv]);

    if(verbose>=1)printf("%s\n",log);
    if(show_log)addLog(log);

    return death_check(target);

}

err_t apply_status(Status s, Entity *target, StateList *list, int caster_id, bool show_log)
{
    char log[STR_LONG];
    Status renew;

    if(verbose>=2)printf("Modifier is a status effect!\n");

    if(target->status_effect[s.stat] != 0)
    {
        remove_from_list(target, s.stat, &renew);
        sprintf(log, "%s is still %s", target->cha_name, statusName[s.stat]);
        if(verbose>=1)printf("%s\n", log);
    }

    else if(s.stat==Provoked)
    {
        target->status_effect[s.stat] = caster_id;

        Entity * caster = e_from_id(caster_id);

        sprintf(log, "%s is provoked by %s", target->cha_name, caster->cha_name);
    }

    else if(s.stat==Burning && target->status_effect[Freezing])
    {
        remove_mod(remove_from_list(target, Freezing, &renew), target, TRUE);
        if(verbose>=1)printf("Attempting to burn %s has thawed him out!\n", target->cha_name);
        sprintf(log, "Attempting to burn %s has thawed him out", target->cha_name);
        if(show_log)addLog(log);
        return OK;
    }

    else if(s.stat==Freezing)
    {
        if(target->status_effect[Burning])
        {
            if(verbose>=1)printf("Target is burning, cannot be frozen!\n");
            sprintf(log, "%s is burning and cannot be frozen", target->cha_name);
            if(show_log)addLog(log);
            return OK;
        }
        else
        {
            target->status_effect[s.stat] = 1;
            sprintf(log, "%s is %s", target->cha_name, statusName[s.stat]);
        }
    }

    else
    {
        target->status_effect[s.stat] = 1;
        sprintf(log, "%s is %s", target->cha_name, statusName[s.stat]);
    }

    if(s.duration!=0)
    {
        if(verbose>=2)printf("Duration != 0\n");
        char log_2[STR_SHORT];
        sprintf(log_2, " for %d turns", s.duration);
        strcat(log, log_2);
        if(verbose>=1)printf("%s\n",log);
        if(show_log)addLog(log);
        return list_add(list, s, target);
    }
    else
    {
        if(verbose>=2)printf("Duration == 0\n");
        strcat(log, " permanently");
        if(verbose>=1)printf("%s\n",log);
        if(show_log)addLog(log);
        return OK;
    }
}

err_t apply_stat_change(Status s, Entity * target, StateList * list, bool show_log)
{
    if(s.value!=0)
    {
        char log[STR_LONG];

        if(verbose>=2)printf("Modifier is a stat change of %d\n", s.value);
        if(verbose>=2)printf("Stat before the change : %d\n", target->stats[s.stat]);

        target->stats[s.stat] += s.value;

        if(target->stats[s.stat]>20)
        {
            s.value -= target->stats[s.stat] - 20;
            target->stats[s.stat] = 20;
        }
        else if(target->stats[s.stat]<0)
        {
            s.value -= target->stats[s.stat];
            target->stats[s.stat] = 0;
        }

        sprintf(log, "%s's %s was altered by %d", target->cha_name, statName[s.stat], s.value);
        
        if(verbose>=2)printf("Stat after the change : %d\n", target->stats[s.stat]);

        if(s.duration!=0)
        {
            char log_2[STR_SHORT];
            sprintf(log_2, " for %d turns", s.duration);
            strcat(log, log_2);
            if(verbose>=1)printf("%s\n",log);

            if(s.value != 0)
            {
                if(show_log)addLog(log);
            }

            return list_add(list, s, target);
        }
        else
        {
            strcat(log, " permanently");
            if(verbose>=1)printf("%s\n",log);

            if(s.value!=0)
            {
                if(show_log)addLog(log);
            }
        }
    }
    return OK;
}

err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id)
{
    if(m.effect.value==0)
    {
        return apply_status(m.effect,target, list, caster_id, TRUE);
    }
    else
    {
        return apply_stat_change(m.effect,target, list, TRUE);
    }

    return OK;
}

int apply_to(Ability active_ab, Entity * active_ent, StateList * list, Coord starting_point)
{
    if(verbose>=3)print_Ability(&active_ab, "");

    int death_count = 0;
    int i,j;
    Coord c;
    Entity * e;

        for(i=0; i<active_ab.nb_coords; i++)
        {
            c=add_coords(starting_point, *((*(active_ab.coord))+i));

            if(isInGrid(c))
            {
                e=getEntity(c);
                
                if(e!=NULL)
                {
                    if(verbose>=1)printf("%s was found in the zone!\n", e->cha_name);

                    if(!same_team(e,active_ent))
                    {
                        if(verbose>=1)printf("%s is an Ennemy!\n", e->cha_name);

                        if(active_ab.damage!=NULL)
                        {
                            if(apply_check(NULL,e))
                            {
                                if(apply_damage(*(active_ab.damage), active_ent, e, TRUE))
                                {
                                    death_count++;
                                }
                            }
                            else
                            {
                                char log[STR_LONG];
                                sprintf(log, "%s blocked incoming damage", e->cha_name);
                                addLog(log);
                            }
                        }
                    }
                    else
                    {
                        if(verbose>=1)printf("%s is an Ally!\n", e->cha_name);
                    }

                    if(active_ab.mods!=NULL && e->active!=Dead)
                    {
                        for(j=0; j<active_ab.nb_mods; j++)
                        {
                            if((!same_team(e,active_ent) && ((*(active_ab.mods))+j)->t!=ALLIES) || ((same_team(e,active_ent) && ((*(active_ab.mods))+j)->t!=FOES)))
                            {
                                if(apply_check((*(active_ab.mods))+j,e))
                                {
                                    apply_mod(*((*(active_ab.mods))+j),e, list, active_ent->cha_id);
                                }
                            }
                        }
                    }
                }
                if(active_ab.fn_use==DURING)
                {
                    death_count += active_ab.function(c,active_ent,list); 
                }
            }
        }

    return death_count;
}

