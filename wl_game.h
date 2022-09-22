#pragma once

#include "wl_def.h"


/*
=============================================================================

                         WL_GAME DEFINITIONS

=============================================================================
*/

extern  bool            ingame;
extern  gametype        gamestate;
extern  uint8_t         bordercol;

#ifndef DEMOSEXTERN
extern  char            demoname[13];
#endif

void    SetupGameLevel (void);
void    GameLoop (void);
void    DrawPlayBorder (void);
void    DrawStatusBorder (uint8_t color);
void    DrawPlayScreen (void);
void    DrawPlayBorderSides (void);

void    PlayDemo (int demonumber);
void    RecordDemo (void);

#ifdef SPEAR
extern  int32_t            spearx,speary;
extern  unsigned        spearangle;
extern  bool         spearflag;
#endif

#define ClearMemory SD_StopDigitized

// JAB
#define PlaySoundLocTile(s,tx,ty)       PlaySoundLocGlobal(s,(((int32_t)(tx) << TILESHIFT) + (1L << (TILESHIFT - 1))),(((int32_t)ty << TILESHIFT) + (1L << (TILESHIFT - 1))))
#define PlaySoundLocActor(s,ob)         PlaySoundLocGlobal(s,(ob)->x,(ob)->y)
void    PlaySoundLocGlobal(uint16_t s,fixed gx,fixed gy);
void UpdateSoundLoc(void);
