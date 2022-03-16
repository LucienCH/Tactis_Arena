#ifndef pointers_h
#define pointers_h
/** \file pointers.h
 * \brief Declaration of pointers used for initialisation in a header file to reduce clog in init_classes.c
 * \author Robin BUTEL
 */
#include "struct.h"

Coord * one_c;      //!< Pointer to a reused Coord structure indicating a single targeted coordinate (0,0)
Damage * one_a;     //!< Pointer to a reused Damage structure indicating a base damage modifier (1) for physical damage
Damage * one_m;     //!< Pointer to a reused Damage structure indicating a base damage modifier (1) for magic damage
Coord * aoe51;      //!< Pointer to a reused Coord structure indicating a zone of 51 Tiles
Coord * aoe103;     //!< Pointer to a reused Coord structure indicating a zone of 103 Tiles

Damage * FD;        //!< Pointer to the Coord structure for the Berserker's Frenzied Dash

Modifier * DE;      //!< Pointer to the Modifier structure for the Ranger's Deadeye

Modifier * Ba;      //!< Pointer to a Modifier structure for the Goliath's Bash
Modifier * SU;      //!< Pointer to a Modifier structure for the Goliath's Shields Up
Modifier * D;       //!< Pointer to a Modifier structure for the Goliath's Detained
Modifier * P;       //!< Pointer to a Modifier structure for the Goliath's Banner

Damage *d;          //!< Pointer to a Damage structure for the Mage's Eruption, Shock and Lightning Chain
Damage *u;          //!< Pointer to a Damage Structure for the Mage's Blizzard
Damage *red;        //!< Pointer to a Damage structure for the Mage's Freeze
Modifier *fire1;    //!< Pointer to a Modifier structure for the Mage's Fireball
Modifier *fire2;    //!< Pointer to a Modifier structure for the Mage's Flamecharge and Eruption
Modifier *ice1;     //!< Pointer to a Modifier structure for the Mage's Icy Winds
Modifier *ice2;     //!< Pointer to a Modifier structure for the Mage's Freeze
Modifier *ice3;     //!< Pointer to a Modifier structure for the Mage's Blizzard
Modifier *armor;    //!< Pointer to a Modifier structure for the Mage's Frozen Armor
Modifier *thera;    //!< Pointer to a Modifier structure for the Mage's Shock Therapy
Coord *aoe75;       //!< Pointer to a Coord structure for the Mage's Eruption and Blizzard

Modifier *para;     //!< Pointer to a Modifier structure for the Valkyrie's Thrust and Odin's Eyes

Modifier *heal;     //!< Pointer to a Modifier structure for the Angel's Condemn and Holy Storm
Modifier *passive;  //!< Pointer to a Modifier structure for the Angel's Passive Aura
Modifier *bless;    //!< Pointer to a Modifier structure for the Angel's God's Blessing

#endif