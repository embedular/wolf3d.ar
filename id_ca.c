// ID_CA.C

// this has been customized for WOLF

/*
=============================================================================

Id Software Caching Manager
---------------------------

Must be started BEFORE the memory manager, because it needs to get the headers
loaded into the data segment

=============================================================================
*/

#include "id_ca.h"

#ifdef UPLOAD
    #include "rom/wl1/mapdata_wl1.h"
    #include "rom/wl1/pictdata_wl1.h"
#else
    #include "rom/wl6/mapdata_wl6.h"
    #include "rom/wl6/pictdata_wl6.h"
#endif

/*
=============================================================================

                             LOCAL CONSTANTS

=============================================================================
*/

/*
=============================================================================

                             GLOBAL VARIABLES

=============================================================================
*/
int                 mapon;

const uint16_t      * mapsegs[MAPPLANES];
const uint16_t      * mapsegs_plane0;
const uint16_t      * mapsegs_plane1;
//word            mapsegs_plane0[MAPAREA*2];
//word            mapsegs_plane1[MAPAREA*2];

/*
=============================================================================

                             LOCAL VARIABLES

=============================================================================
*/


/*
=============================================================================

                            LOW LEVEL ROUTINES

=============================================================================
*/

/*
=============================================================================

                                         CACHE MANAGER ROUTINES

=============================================================================
*/

/*
======================
=
= CA_Startup
=
= Open all files and load in headers
=
======================
*/

void CA_Startup (void)
{
    pictdata_set_pictable ();

    mapon = -1;
}

//==========================================================================


/*
======================
=
= CA_Shutdown
=
= Closes all files
=
======================
*/

void CA_Shutdown (void)
{
}

/*
======================
=
= CA_CacheMap
=
= WOLF: This is specialized for a 64*64 map size
=
======================
*/

void CA_CacheMap (int mapnum)
{
    mapon = mapdata_select_map (mapnum);

#if MAPPLANES == 2
    mapsegs[0] = mapsegs_plane0;
    mapsegs[1] = mapsegs_plane1;
#else
    #error "Unsupported number of MAPPLANES"
#endif
}
