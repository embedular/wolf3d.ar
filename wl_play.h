#pragma once

#include "wl_def.h"

/*
=============================================================================

                            WL_PLAY DEFINITIONS

=============================================================================
*/

#define BASEMOVE                35
#define RUNMOVE                 70
#define BASETURN                35
#define RUNTURN                 70

#define JOYSCALE                2

#define ACTORAT_EMPTY                   0
#define ACTORAT_OBJLIST_IDX(v)          (v+1)
#define ACTORAT_BLOCKING_TILE           151
#define ACTORAT_TILE                    152
#define ACTORAT_DOORNUM(v)              (v+192)

#define ACTORAT_IS_OBJLIST_IDX(v)       (v > 0 && v < 151)
#define ACTORAT_IS_BLOCKING_TILE(v)     (v == ACTORAT_BLOCKING_TILE)
#define ACTORAT_IS_TILE(v)              (v == ACTORAT_TILE) // check tilemap
#define ACTORAT_IS_ANY_TILE(v)          (ACTORAT_IS_BLOCKING_TILE(v) \
                                         || ACTORAT_IS_TILE(v))
#define ACTORAT_IS_DOORNUM(v)           (v > 191)

#define ACTORAT_GET_OBJLIST_PTR(v)      (&objlist[v-1])
#define ACTORAT_GET_DOORNUM(v)          (v-192)

#define OBJLIST_GET_PTR2IDX(p)          (uint8_t)(p - objlist)

#define SPOTVIS_SET(b)          (spotvis[(b) >> 5] |= 1 << ((b) & 0x1F))
#define SPOTVIS_CHECK(b)        (spotvis[(b) >> 5] & (1 << ((b) & 0x1F)))
#define SPOTVIS_XY2B(x,y)       (((x)<<mapshift)+(y))
#define SPOTVIS_SETXY(x,y)      SPOTVIS_SET(SPOTVIS_XY2B(x,y))
#define SPOTVIS_CHECKXY(x,y)    SPOTVIS_CHECK(SPOTVIS_XY2B(x,y))

extern  uint8_t         tilemap[MAPSIZE][MAPSIZE];
extern  uint8_t         actorat[MAPSIZE][MAPSIZE];
extern  uint32_t        spotvis[(MAPAREA) >> 5];

extern  objtype         *player;

extern  unsigned        tics;
extern  int             viewsize;

extern  int             lastgamemusicoffset;

//
// current user input
//
extern  int         controlx,controly;              // range from -100 to 100
extern  bool8       buttonstate[NUMBUTTONS];
extern  objtype     objlist[MAXACTORS];
extern  bool8       buttonheld[NUMBUTTONS];
extern  exit_t      playstate;
extern  bool8       madenoise;
extern  statobj_t   statobjlist[MAXSTATS];
extern  statobj_t   *laststatobj;
extern  objtype     *newobj,*killerobj;
extern  doorobj_t   doorobjlist[MAXDOORS];
extern  doorobj_t   *lastdoorobj;
extern  int         godmode;

extern  bool8       demorecord,demoplayback;
extern  int8_t      *demoptr, *lastdemoptr;
extern  memptr      demobuffer;

//
// control info
//

void    InitActorList (void);
void    GetNewActor (void);
void    PlayLoop (void);

void    CenterWindow(uint16_t w,uint16_t h);

void    FinishPaletteShifts (void);

void    RemoveObj (objtype *gone);
void    PollControls (void);
int     StopMusic(void);
void    StartMusic(void);
void    ContinueMusic(int offs);
void    StartDamageFlash (int damage);
void    StartBonusFlash (void);

#ifdef SPEAR
extern  int32_t     funnyticount;           // FOR FUNNY BJ FACE
#endif

extern  objtype     *objfreelist;     // *obj,*player,*lastobj,

extern  bool        noclip,ammocheat;
extern  int         singlestep, extravbls;
