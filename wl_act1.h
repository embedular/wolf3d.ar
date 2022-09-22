#pragma once

#include "wl_def.h"

/*
=============================================================================

                             WL_ACT1 DEFINITIONS

=============================================================================
*/

extern  doorobj_t   doorobjlist[MAXDOORS];
extern  doorobj_t   *lastdoorobj;
extern  uint8_t     doornum;

extern  uint16_t    doorposition[MAXDOORS];

extern  uint8_t     areaconnect[NUMAREAS][NUMAREAS];

extern  bool8       areabyplayer[NUMAREAS];

extern uint16_t     pwallstate;
extern uint16_t     pwallpos;        // amount a pushable wall has been moved (0-63)
extern uint16_t     pwallx,pwally;
extern uint8_t      pwalldir,pwalltile;


void InitDoorList (void);
void InitStaticList (void);
void SpawnStatic (int tilex, int tiley, int type);
void SpawnDoor (int tilex, int tiley, bool vertical, int lock);
void MoveDoors (void);
void MovePWalls (void);
void OpenDoor (int door);
void PlaceItemType (int itemtype, int tilex, int tiley);
void PushWall (int checkx, int checky, int dir);
void OperateDoor (int door);
void InitAreas (void);
