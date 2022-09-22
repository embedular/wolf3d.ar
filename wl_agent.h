#pragma once

#include "wl_def.h"


/*
=============================================================================

                             WL_AGENT DEFINITIONS

=============================================================================
*/

extern  short    anglefrac;
extern  int      facecount, facetimes;
extern  uint16_t plux,pluy;         // player coordinates scaled to unsigned
extern  int32_t  thrustspeed;
extern  objtype  *LastAttacker;

void    Thrust (int angle, int32_t speed);
void    SpawnPlayer (uint16_t tilex, uint16_t tiley, short dir);
void    TakeDamage (int points,objtype *attacker);
void    GivePoints (int32_t points);
void    GetBonus (statobj_t *check);
void    GiveWeapon (int weapon);
void    GiveAmmo (int ammo);
void    GiveKey (int key);

//
// player state info
//

void    StatusDrawFace(unsigned picnum);
void    DrawFace (void);
void    DrawHealth (void);
void    HealSelf (int points);
void    DrawLevel (void);
void    DrawLives (void);
void    GiveExtraMan (void);
void    DrawScore (void);
void    DrawWeapon (void);
void    DrawKeys (void);
void    DrawAmmo (void);
