#pragma once

#include "wl_def.h"
#include "retroport.h"

/*
=============================================================================

                            WL_DRAW DEFINITIONS

=============================================================================
*/

//
// math tables
//
extern  const int16_t   pixelangle[RETROPORT_SCREEN_WIDTH]; // (IN ROM)
extern  const int32_t   finetangent[FINEANGLES/4];          // (IN ROM)
extern  const fixed     sintable[ANGLES+ANGLES/4];          // (IN ROM)
extern  const fixed     * costable;    // points to part of sintable

extern  uint16_t        wallheight[RETROPORT_SCREEN_WIDTH];
extern  const uint16_t  horizwall[MAXWALLTILES];
extern  const uint16_t  vertwall[MAXWALLTILES];
extern  int32_t         lasttimecount;
extern  int32_t         frameon;

extern  unsigned        screenloc[3];

extern  bool            fizzlein, fpscounter;

extern  fixed           viewx,viewy;                    // the focal point
extern  fixed           viewsin,viewcos;

void    ThreeDRefresh (void);
void    CalcTics (void);

typedef struct
{
    uint16_t leftpix,rightpix;
    uint16_t dataofs[64];
// table data after dataofs[rightpix-leftpix+1]
} t_compshape;
