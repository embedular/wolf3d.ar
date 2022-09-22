#include "mapdata_wl1.h"
#include "id_ca.h"
#include <assert.h>


#define MP(x,y)         mapsegs_plane##y = (const uint16_t *) mapsegs_##x##_plane_##y
#define ENABLED_MAP(x)  case x : MP(x,0); MP(x,1); selmap = x; break;
#define DEFAULT_MAP(x)  default: MP(x,0); MP(x,1); selmap = x; break;


#include "dump/mapsegs_0_plane_0.c"
#include "dump/mapsegs_0_plane_1.c"
#include "dump/mapsegs_1_plane_0.c"
#include "dump/mapsegs_1_plane_1.c"


int mapdata_select_map (int mapnum)
{
    int selmap = -1;

    switch (mapnum)
    {
        ENABLED_MAP(0)
        ENABLED_MAP(1)

        DEFAULT_MAP(0)
    }

    assert (mapsegs_plane0 && mapsegs_plane1 && selmap != -1);

//    memcpy (mapsegs_plane0, plane0, MAPAREA * 2 * sizeof(word));
//    memcpy (mapsegs_plane1, plane1, MAPAREA * 2 * sizeof(word));

    return selmap;
}
