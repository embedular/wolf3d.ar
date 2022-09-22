#pragma once

#include <stdint.h>
#include "resources.h"

// on full version!
#define NUMMAPS             60
#ifdef USE_FLOORCEILINGTEX
    #define MAPPLANES       3
#else
    #define MAPPLANES       2
#endif


// mapsegs takes half the available SRAM! (16 Kb each plane)
extern const uint16_t       *mapsegs[MAPPLANES];
extern const uint16_t       *mapsegs_plane0;
extern const uint16_t       *mapsegs_plane1;
//extern  uint16_t          mapsegs_plane0[MAPAREA*2];
//extern  uint16_t          mapsegs_plane1[MAPAREA*2];
extern  int                 mapon;

// in ROM
extern const uint8_t        *adlibsnd[LASTSOUND];
extern const uint8_t        *adlibmus[LASTMUSIC];
extern const uint16_t       adlibmus_len[LASTMUSIC];


//===========================================================================

void CA_Startup (void);
void CA_Shutdown (void);

void CA_CacheMap (int mapnum);
