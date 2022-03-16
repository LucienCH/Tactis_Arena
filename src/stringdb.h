#ifndef stringdb_h
#define stringdb_h
/** \file stringdb.h
 * \brief Defining multiple string databases for printing usages
 * \author Robin BUTEL
 * \details Allows the use of enumerations to quickly have access to a printable string version of their none numerical "value" by storing them in arrays.
 */

#include "substruct.h"

char error_message[NUM_ERROR][STR_LONG] = {"", 
                                            "\033[31;01m[ERROR]\033[00m : Pointer is Null\n", 
                                            "\033[31;01m[ERROR]\033[00m : SDL error\n", 
                                            "\033[31;01m[ERROR]\033[00m : Coordinates out of limits\n", 
                                            "\033[31;01m[ERROR]\033[00m : String too long\n",
                                            "\033[31;01m[ERROR]\033[00m : Ability coordinates were not initialised correctly\n",
                                            "\033[31;01m[ERROR]\033[00m : Client not initialised  correctly\n",
                                            "\033[31;01m[ERROR]\033[00m : Server not initialised correctly\n",
                                            "\033[31;01m[ERROR]\033[00m : Structure not sent correctly\n",
                                            "\033[31;01m[ERROR]\033[00m : Text already in cache, cannot add it twice\n"};


char statName[NUM_STATS][STR_SHORT] = {"pv", "mv", "vis", "atk", "magic", "res_physic", "res_magic"};
char IdNames[Mvt+1][STR_SHORT] = {"Slash", "Killing_Blow", "Fury", "Frenzied_Dash", "Bolt", "Focus", "Trap", "Deadeye", "Bash", "Shields_Up", "Detain", "Banner", "Fireball", "FlameCharge", "Flare", "Eruption", "Icy_Winds", "Freeze", "Frozen_Armor", "Blizzard", "Shock", "Volt_Switch", "Shock_Therapy", "Lightning_Chain", "Thrust", "Odins_Eyes", "Life_Transfer", "Gates_of_Valhalla", "Condemn", "Holy_Storm", "Last_Sacrfice", "Gods_Blessing", "Aura", "Mvt"};
char statusName[NUM_STATUS][STR_SHORT] = {"Crippled", "Detained", "Provoked", "Burning", "Frozen", "Paralyzed", "Blessed", "Piercing", "Guarding", "Summoned"};
char targetNames[5][STR_SHORT] = {"FOES", "ALLIES", "BOTH", "FREE_TILE", "ANY_TILE"};
char fnNames[5][STR_SHORT] = {"NONE", "DURING", "BEFORE", "ONLY", "AFTER"};

#endif