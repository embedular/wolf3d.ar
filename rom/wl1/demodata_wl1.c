#include "wl_def.h"

#if 0
#include "dump/grchunk_139_demo.c"
#include "dump/grchunk_140_demo.c"
#include "dump/grchunk_141_demo.c"
#include "dump/grchunk_142_demo.c"


const uint8_t *demodata[1 + T_DEMO3 - T_DEMO0] =
{
    grchunk_139_demo,
    grchunk_140_demo,
    grchunk_141_demo,
    grchunk_142_demo
};
#endif

#include "demo_0_custom.c"     // L1E1
#include "demo_1_custom.c"     // L2E1

const uint8_t *demodata[1 + T_DEMO3 - T_DEMO0] =
{
    demo_0_custom,
    demo_1_custom,
    demo_0_custom,
    demo_1_custom
};
