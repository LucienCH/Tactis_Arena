#include <stdlib.h>
#include <string.h>
#include "abilities.h"
#include "print.h"
#include "pointers.h"

/** \file init_classes.c
 * \brief Functions related to the intialisation and destruction of class information
 * \author Robin BUTEL
 */

err_t init_aoe()
{
    int i,x,y;

    aoe51=malloc(sizeof(Coord)*51);
    if(aoe51==NULL) return POINTER_NULL;
    i=0,x=-3,y=-4;
    while(x<=3)
    {
        if(!(( abs(x)==3 && abs(y)>=3) || (abs(x)==2 && abs(y)==4) ))
        {
            aoe51[i].x=x;
            aoe51[i].y=y;
            i++;
        }
        if((y==4))
        {
            y=-4;
            x++;
        }
        else y++;
    }
    if(i!=51)
    {
        if(verbose>=1)printf("%d\n",i);
        return INIT_COORD_ERR;
    }

    aoe103=malloc(sizeof(Coord)*103);
    if(aoe103==NULL) return POINTER_NULL;
    i=0,x=-5,y=-6;
    while(x<=5)
    {
        if(!(( abs(x)==5 && abs(y)>=3 ) || (abs(y)==6 && abs(x)>=2) || (abs(x)==4 && abs(y)>=4) || (abs(x)==3 && abs(y)==5)))
        {
            aoe103[i].x=x;
            aoe103[i].y=y;
            i++;
        }
        if((y==6))
        {
            y=-6;
            x++;
        }
        else y++;
    }

    if(i!=103)
    {
        if(verbose>=1)printf("%d\n",i);
        return INIT_COORD_ERR;
    }

    return OK;
}

err_t init_repetitives(Ability * Move)
{
    one_c = malloc(sizeof(Coord));
    if(one_c==NULL) return POINTER_NULL;
    one_a = malloc(sizeof(Damage));
    if(one_a==NULL) return POINTER_NULL;
    one_m = malloc(sizeof(Damage));
    if(one_m==NULL) return POINTER_NULL;

    Coord ctemp = {0,0};
    Damage dtmp1 = {1,atk};
    Damage dtmp2 = {1,magic};

    *one_c = ctemp;
    *one_a = dtmp1;
    *one_m = dtmp2;

    Ability ab_move = {Mvt,1,0,10,FREE_TILE,NULL,1,&one_c,0,NULL,NONE,NULL,{"Movement","Move to the selected tile"}};
    *Move = ab_move;

    return init_aoe();
}

err_t init_berserker(Class * c)
{
    FD = malloc(sizeof(Damage));
    if(FD==NULL) return POINTER_NULL;

    Damage dtmp = {0.8,atk};

    *FD = dtmp;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Slash,1,0,1,FOES,&one_a,1,&one_c,0,NULL,NONE,NULL,{"Slash", "Slash at an ennemy."}};
    *ab = abtemp1;
    Ability abtemp2 = {Killing_Blow,2,1,1,FOES,&one_a,1,&one_c,0,NULL,DURING,Killing_Blow_fn,{"Killing Blow","Slash at an ennemy, killing him increases attack permanently and gives one action point back."}};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Fury,2,2,0,ANY_TILE,NULL,1,&one_c,0,NULL,ONLY,Fury_fn,{"Fury","Remove all debuffs and permanently increase attack by the number of turns removed."}};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Frenzied_Dash,3,4,9,FREE_TILE,&FD,51,&aoe51,0,NULL,BEFORE,Frenzied_Dash_fn,{"Frenzied Dash","Jump to a tile, dealing aoe damage when landing."}};
    *(ab+3) = abtemp4;

    Class temp = 
    {
        Berserker,
        "Berserker",
        {20,8,10,10,0,12,12},
        {"Bloodlust","Killing two ennemies in the same turn refills action points and doubles mp and atk stats for one turn."},
        ab
    };

    *c = temp;

    if(verbose>=1) printf("%s initialized!\n", c->cla_name);

    return OK;
}

err_t init_ranger(Class * c)
{
    DE = malloc(sizeof(Modifier)*3);
    if(DE==NULL) return POINTER_NULL;
    Modifier t[3] = {{{7,vis,3},1,ALLIES},{{0,Piercing,3},1,ALLIES},{{-8,mv,3},1,ALLIES}};
    *DE=t[0];
    *(DE+1)=t[1];
    *(DE+2)=t[2];

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Bolt,1,0,6,FOES,&one_a,1,&one_c,0,NULL,NONE,NULL,{"Bolt","Attack an ennemy from range."}};
    *ab = abtemp1;
    Ability abtemp2 = {Focus,2,4,0,ANY_TILE,NULL,1,&one_c,0,NULL,ONLY,Focus_fn,{"Focus","Increase by two turns all buffs and decrease by one all debuffs."}};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Trap,2,2,4,FREE_TILE,NULL,1,&one_c,0,NULL,ONLY,Trap_fn,{"Trap","Put a hidden trap that will cripple an ennemy that walks over it."}};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Deadeye,3,5,0,ANY_TILE,NULL,1,&one_c,3,&DE,NONE,NULL,{"Deadeye","Increases range and makes bolt piercing but reduces mv for three turns."}};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Ranger,
        "Ranger",
        {20,12,10,8,0,8,8},
        {"Sentinel","The first ennemy that moves inside a ranger's range every turn, will get bolted."},
        ab
    };
    
    *c = temp;

    if(verbose>=1) printf("%s initialized!\n", c->cla_name);

    return OK;
}

err_t init_goliath(Class * c)
{

    Ba = malloc(sizeof(Modifier)*2);
    if(Ba==NULL) return POINTER_NULL;
    Modifier t[2]= {{{-5,res_physic,1},1,FOES},{{-5,res_magic,1},1,FOES}};
    *Ba = t[0];
    *(Ba+1) = t[1];

    SU = malloc(sizeof(Modifier));
    if(SU==NULL) return POINTER_NULL;
    Modifier f= {{0,Guarding,1},1,ALLIES};
    *SU = f;

    D = malloc(sizeof(Modifier));
    if(D==NULL) return POINTER_NULL;
    Modifier l= {{0,Detained,3},1,FOES};
    *D = l;

    P = malloc(sizeof(Modifier));
    if(P==NULL) return POINTER_NULL;
    Modifier ut= {{0,Provoked,1},1,FOES};
    *P= ut;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Bash,1,0,1,FOES,&one_a,1,&one_c,2,&Ba,NONE,NULL,{"Bash","Damage and reduce the ennemy's resistances for a turn."}};
    *ab = abtemp1;
    Ability abtemp2 = {Shields_Up,2,1,0,ANY_TILE,NULL,1,&one_c,1,&SU,NONE,NULL,{"Shields Up","Increase passive block chance to 70% for one turn."}};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Detain,2,4,2,FOES,NULL,1,&one_c,1,&D,AFTER,Detain_fn,{"Detain","Capture an Ennemy for three turns."}};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Banner,3,5,0,ANY_TILE,NULL,103,&aoe103,1,&P,BEFORE,Banner_fn,{"Banner","Provoke all ennemies in a zone for one turn and reset all allies' cooldowns."}};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Goliath,
        "Goliath",
        {20,6,10,6,0,14,12},
        {"Guardian","Has a 30% chance to block incoming damage."},
        ab
    };
    
    *c = temp;

    if(verbose>=1) printf("%s initialized!\n", c->cla_name);

    return OK;
}

err_t init_mage(Class * c, Ability movesets[3][NUM_AB])
{
    d = malloc(sizeof(Damage));
    if(d==NULL) return POINTER_NULL;
    Damage l = {1.4,magic};
    *d = l;

    u = malloc(sizeof(Damage));
    if(u==NULL) return POINTER_NULL;
    Damage r = {1.2,magic};
    *u = r;

    red = malloc(sizeof(Damage));
    if(red==NULL) return POINTER_NULL;
    Damage k = {0.8,magic};
    *red = k;

    fire1= malloc(sizeof(Modifier));
    if(fire1==NULL) return POINTER_NULL;
    Modifier f1 = {{0,Burning,3},0.6,BOTH};
    *fire1 = f1;

    fire2= malloc(sizeof(Modifier));
    if(fire2==NULL) return POINTER_NULL;
    Modifier f2 = {{0,Burning,3},1,BOTH};
    *fire2 = f2;

    ice1= malloc(sizeof(Modifier));
    if(ice1==NULL) return POINTER_NULL;
    Modifier ie1 = {{0,Freezing,2},0.3,FOES};
    *ice1 = ie1;

    ice2= malloc(sizeof(Modifier));
    if(ice2==NULL) return POINTER_NULL;
    Modifier ie2 = {{0,Freezing,2},1,FOES};
    *ice2 = ie2;

    ice3= malloc(sizeof(Modifier)*2);
    if(ice3==NULL) return POINTER_NULL;
    Modifier ie3[2] = {ie1,{{-8,vis,3},1,FOES}};
    *ice3 = ie3[0];
    *(ice3+1) = ie3[1];

    armor= malloc(sizeof(Modifier)*2);
    if(armor==NULL) return POINTER_NULL;
    Modifier arm[2] = {{{6,res_physic,2},1,ALLIES},{{6,res_magic,2},1,ALLIES}};
    *armor = arm[0];
    *(armor+1) = arm[1];

    thera= malloc(sizeof(Modifier)*2);
    if(thera==NULL) return POINTER_NULL;
    Modifier py[2] = {{{6,atk,2},1,ALLIES},{{6,magic,2},1,ALLIES}};
    *thera = py[0];
    *(thera+1) = py[1];

    aoe75=malloc(sizeof(Coord)*75);
    if(aoe75==NULL) return POINTER_NULL;
    int i=0,x=-4,y=-5;
    while(x<=4)
    {
        if(!(( abs(x)==4 && abs(y)>=3 ) || (abs(y)==5 && abs(x)>=2)  || (abs(x)==3 && abs(y)==4)))
        {
            aoe75[i].x=x;
            aoe75[i].y=y;
            i++;
        }
        if((y==5))
        {
            y=-5;
            x++;
        }
        else y++;
    }

    if(i!=75)
    {  
        if(verbose>=1)printf("%d\n",i);
        return INIT_COORD_ERR;
    }



    Ability abtemp1 = {Fireball,1,0,8,ANY_TILE,&one_m,1,&one_c,1,&fire1,AFTER,mage_switch,{"Fireball","Deals damage and has a chance to set target ablaze."}};
    movesets[0][0] = abtemp1;
    Ability abtemp2 = {FlameCharge,2,3,10,FREE_TILE,&one_m,1,&one_c,1,&fire2,ONLY,FlameCharge_fn,{"FlameCharge","Engulf yourself in flames and dash, dealing damage and burning entities in your path."}};
    movesets[0][1] = abtemp2;
    Ability abtemp3 = {Flare,2,3,5,ANY_TILE,NULL,103,&aoe103,0,NULL,ONLY,Flare_fn,{"Flare","Increases all allies' vision and their mp for two turns, will spot traps in the chosen area."}};
    movesets[0][2] = abtemp3;
    Ability abtemp4 = {Eruption,3,5,12,ANY_TILE,&d,75,&aoe75,1,&fire2,NONE,NULL,{"Eruption","Deal massive damage in a zone and burn all entities."}};
    movesets[0][3] = abtemp4;

    Ability abtemp11 = {Icy_Winds,1,0,8,ANY_TILE,&one_m,1,&one_c,1,&ice1,AFTER,mage_switch,{"Icy Winds","Deals damage and has a chance to freeze target."}};
    movesets[1][0] = abtemp11;
    Ability abtemp12 = {Freeze,2,3,8,FOES,&red,1,&one_c,1,&ice2,NONE,NULL,{"Freeze","Deal damage to and freeze an ennemy. "}};
    movesets[1][1] = abtemp12;
    Ability abtemp13 = {Frozen_Armor,2,3,6,ALLIES,NULL,1,&one_c,2,&armor,NONE,NULL,{"Frozen Armor","Increase ally physical and magic resistances."}};
    movesets[1][2] = abtemp13;
    Ability abtemp14 = {Blizzard,3,5,12,ANY_TILE,&u,75,&aoe75,2,&ice3,DURING,Blizzard_fn,{"Blizzard","Deal damage in a zone and highly reduce ennemy vision for a turn, has a chance to freeze ennemies. Water tiles in the are will also freeze."}};
    movesets[1][3] = abtemp14;

    Ability abtemp21 = {Shock,1,0,8,ANY_TILE,&d,1,&one_c,0,NULL,AFTER,mage_switch,{"Shock","Zap an ennemy."}};
    movesets[2][0] = abtemp21;
    Ability abtemp22 = {Volt_Switch,2,3,10,ALLIES,&one_m,51,&aoe51,0,NULL,BEFORE,Volt_Switch_fn,{"Volt Switch","Switch spots with an ally and deal electric damage around caster. Switching with a crippled ally will free him."}};
    movesets[2][1] = abtemp22;
    Ability abtemp23 = {Shock_Therapy,2,3,6,ALLIES,NULL,1,&one_c,2,&thera,NONE,NULL,{"Shock Therapy","Increase ally atk and magic."}};
    movesets[2][2] = abtemp23;
    Ability abtemp24 = {Lightning_Chain,3,5,8,FOES,&d,1,&one_c,0,NULL,AFTER,Lightning_Chain_fn,{"Lightning Chain","Zap an ennemy, the bolt will bounce to another close ennemy until either there are no ennemies close enough or it has bounced 3 times."}};
    movesets[2][3] = abtemp24;


    Class temp = 
    {
        Mage,
        "Mage",
        {20,7,10,0,10,8,11},
        {"Elementalist","After every use of a basic attack, change element (Fire, Ice, Electricity)."},
        NULL
    };

    *c = temp;

    if(verbose>=1) printf("%s initialized!\n", c->cla_name);

    return OK;
}

err_t init_valkyrie(Class * c)
{
    para= malloc(sizeof(Modifier));
    if(para==NULL) return POINTER_NULL;
    Modifier p = {{0,Paralyzed,2},0.3,FOES};
    *para = p;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Thrust,1,0,1,ANY_TILE,&one_a,1,&one_c,1,&para,AFTER,Thrust_fn,{"Thrust","Thrust your spear forward, dealing damage to the tile selected adn the tile behind it."}};
    *ab = abtemp1;
    Ability abtemp2 = {Odins_Eyes,2,2,8,ANY_TILE,&one_m,51,&aoe51,1,&para,NONE,NULL,{"Odin's Eyes","Call forth a swarm of crows to deal magic damage to an area."}};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Life_Transfer,2,3,8,ALLIES,NULL,1,&one_c,0,NULL,ONLY,Life_Transfer_fn,{"Life Transfer","Select an Ally, heal the ally back to full and slow the closest ennemy for three turns by the amount healed."}};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Gates_of_Valhalla,3,5,0,ANY_TILE,NULL,0,NULL,0,NULL,ONLY,Gates_of_Valhalla_fn,{"Gates of Valhalla","Resurrect all dead allies for a turn."}};
    *(ab+3) = abtemp4;


    Class temp = 
    {
        Valkyrie,
        "Valkyrie",
        {20,10,10,10,6,10,8},
        {"Precision","Damaging an ennemy has a chance to paralyze them."},
        ab
    };
    
    *c = temp;

    if(verbose>=1) printf("%s initialized!\n", c->cla_name);

    return OK;
}

err_t init_angel(Class * c, Ability *pass)
{
    heal= malloc(sizeof(Modifier));
    if(heal==NULL) return POINTER_NULL;
    Modifier p = {{6,pv,0},1,ALLIES};
    *heal = p;

    passive= malloc(sizeof(Modifier));
    if(passive==NULL) return POINTER_NULL;
    Modifier h = {{4,pv,0},1,ALLIES};
    *passive = h;

    bless= malloc(sizeof(Modifier));
    if(bless==NULL) return POINTER_NULL;
    Modifier pb = {{0,Blessed,1},1,ALLIES};
    *bless = pb;

    Ability * ab=malloc(sizeof(Ability)*NUM_AB);
    if(ab==NULL) return POINTER_NULL;

    Ability abtemp1 = {Condemn,1,0,6,BOTH,&one_m,1,&one_c,1,&heal,NONE,NULL,{"Condemn","Deal magic damage to an ennemy or heal an ally"}};
    *ab = abtemp1;
    Ability abtemp2 = {Holy_Storm,2,3,8,ANY_TILE,&one_m,51,&aoe51,1,&heal,NONE,NULL,{"Holy Storm","Deal magic damage to ennemies in an area and heal all allies in the area."}};
    *(ab+1) = abtemp2;
    Ability abtemp3 = {Last_Sacrifice,2,5,0,ANY_TILE,NULL,0,NULL,0,NULL,ONLY,Last_Sacrifice_fn,{"Last Sacrfice","Kill self to resurrect an Ally."}};
    *(ab+2) = abtemp3;
    Ability abtemp4 = {Gods_Blessing,3,5,8,ALLIES,NULL,1,&one_c,1,&bless,DURING,Gods_Blessing_fn,{"God's Blessing","Double an Ally's action points and apply no cooldowns for a turn."}};
    *(ab+3) = abtemp4;

    Ability abpass = {Aura,0,0,0,ANY_TILE,NULL,103,&aoe103,1,&passive,NONE,NULL,{"Aura",""}};
    *pass = abpass;

    Class temp = 
    {
        Angel,
        "Angel",
        {20,8,10,0,8,12,12},
        {"Aura","Heal all nearby allies at start of your turn."},
        ab
    };
    
    *c = temp;

    if(verbose>=1) printf("%s initialized!\n", c->cla_name);

    return OK;
}

/* DESTRUCTION*/

err_t ability_destroy(Ability * a)
{
    if(a!=NULL)
    {
        if(verbose>=2) printf("Destroying %s's pointers!\n", a->eng.name);

        if(verbose>=2) printf("Damage...");
        if(a->damage!=NULL&&*(a->damage)!=NULL)
        {
            free(*(a->damage));
            *(a->damage) = NULL;
            if(verbose>=2) printf("freed\n");
        }
        else
        {
            if(verbose>=2) printf("null\n");
        }

        if(verbose>=2) printf("Coords...");
        if(a->coord!=NULL&&*(a->coord)!=NULL)
        {
            free(*(a->coord));
            *(a->coord) = NULL;
            if(verbose>=2) printf("freed\n");
        }
        else
        {
            if(verbose>=2) printf("null\n");
        }

        if(verbose>=2) printf("Modifiers...");
        if(a->mods!=NULL&&*(a->mods)!=NULL)
        {  
            free(*(a->mods));
            *(a->mods) = NULL;
            if(verbose>=2) printf("freed\n");
        }
        else
        {
            if(verbose>=2) printf("NULL\n");
        }

        if(verbose>=1) printf("%s's pointers destroyed!\n", a->eng.name);
    }

    return OK;
}

err_t class_destroy(Class * c)
{
    if(c->cla_id!=Mage)
    {
        char name[STR_SHORT];

        if(c!=NULL)
        {
            strcpy(name, c->cla_name);

            if(verbose>=1) printf("Starting destruction of %s!\n", name);

            if(c->cla_abilities!=NULL)
            {
                int i;
                for(i=0; i<NUM_AB; i++)
                {
                    ability_destroy(c->cla_abilities+i);
                }

                free(c->cla_abilities);
                c->cla_abilities = NULL;
                if(verbose>=1) printf("%s's abilities destroyed!\n", name);
            }

            if(verbose>=1) printf("Destruction of %s finished!\n\n", name);
        }
    }
    return OK;
}