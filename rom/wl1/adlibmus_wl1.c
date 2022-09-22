#include "wl_def.h"

#include "dump/adlibmus_len.c"

//#include "dump/audiosegs_261_music_adlib.c"
//#include "dump/audiosegs_262_music_adlib.c"
//#include "dump/audiosegs_263_music_adlib.c"
#include "dump/audiosegs_264_music_adlib.c"
//#include "dump/audiosegs_265_music_adlib.c"
//#include "dump/audiosegs_266_music_adlib.c"
//#include "dump/audiosegs_267_music_adlib.c"
#include "dump/audiosegs_268_music_adlib.c"
//#include "dump/audiosegs_269_music_adlib.c"
//#include "dump/audiosegs_270_music_adlib.c"
//#include "dump/audiosegs_271_music_adlib.c"
#include "dump/audiosegs_272_music_adlib.c"
//#include "dump/audiosegs_273_music_adlib.c"
//#include "dump/audiosegs_274_music_adlib.c"
#include "dump/audiosegs_275_music_adlib.c"
//#include "dump/audiosegs_276_music_adlib.c"
#include "dump/audiosegs_277_music_adlib.c"
/*
#include "dump/audiosegs_278_music_adlib.c"
#include "dump/audiosegs_279_music_adlib.c"
#include "dump/audiosegs_280_music_adlib.c"
#include "dump/audiosegs_281_music_adlib.c"
#include "dump/audiosegs_282_music_adlib.c"
#include "dump/audiosegs_283_music_adlib.c"
*/
#include "dump/audiosegs_284_music_adlib.c"
//#include "dump/audiosegs_285_music_adlib.c"
//#include "dump/audiosegs_286_music_adlib.c"
//#include "dump/audiosegs_287_music_adlib.c"

#define ADLIBMUS_OFFS_0     audiosegs_261_music_adlib
#define ADLIBMUS_OFFS_1     audiosegs_262_music_adlib
#define ADLIBMUS_OFFS_2     audiosegs_263_music_adlib
#define ADLIBMUS_OFFS_3     audiosegs_264_music_adlib
#define ADLIBMUS_OFFS_4     audiosegs_265_music_adlib
#define ADLIBMUS_OFFS_5     audiosegs_266_music_adlib
#define ADLIBMUS_OFFS_6     audiosegs_267_music_adlib
#define ADLIBMUS_OFFS_7     audiosegs_268_music_adlib
#define ADLIBMUS_OFFS_8     audiosegs_269_music_adlib
#define ADLIBMUS_OFFS_9     audiosegs_270_music_adlib
#define ADLIBMUS_OFFS_10	audiosegs_271_music_adlib
#define ADLIBMUS_OFFS_11	audiosegs_272_music_adlib
#define ADLIBMUS_OFFS_12	audiosegs_273_music_adlib
#define ADLIBMUS_OFFS_13	audiosegs_274_music_adlib
#define ADLIBMUS_OFFS_14	audiosegs_275_music_adlib
#define ADLIBMUS_OFFS_15	audiosegs_276_music_adlib
#define ADLIBMUS_OFFS_16	audiosegs_277_music_adlib
#define ADLIBMUS_OFFS_17	audiosegs_278_music_adlib
#define ADLIBMUS_OFFS_18	audiosegs_279_music_adlib
#define ADLIBMUS_OFFS_19	audiosegs_280_music_adlib
#define ADLIBMUS_OFFS_20	audiosegs_281_music_adlib
#define ADLIBMUS_OFFS_21	audiosegs_282_music_adlib
#define ADLIBMUS_OFFS_22	audiosegs_283_music_adlib
#define ADLIBMUS_OFFS_23	audiosegs_284_music_adlib
#define ADLIBMUS_OFFS_24	audiosegs_285_music_adlib
#define ADLIBMUS_OFFS_25	audiosegs_286_music_adlib
#define ADLIBMUS_OFFS_26	audiosegs_287_music_adlib


const uint8_t *adlibmus[LASTMUSIC] =
{
    NULL, //    ADLIBMUS_OFFS_0,		// CORNER_MUS,              // 0
    NULL, //	ADLIBMUS_OFFS_1,		// DUNGEON_MUS,             // 1
    NULL, //	ADLIBMUS_OFFS_2,		// WARMARCH_MUS,            // 2
	ADLIBMUS_OFFS_3,		// GETTHEM_MUS,             // 3
    NULL, //	ADLIBMUS_OFFS_4,		// HEADACHE_MUS,            // 4
    NULL, //	ADLIBMUS_OFFS_5,		// HITLWLTZ_MUS,            // 5
    NULL, //	ADLIBMUS_OFFS_6,		// INTROCW3_MUS,            // 6
	ADLIBMUS_OFFS_7,		// NAZI_NOR_MUS,            // 7
    NULL, //	ADLIBMUS_OFFS_8,		// NAZI_OMI_MUS,            // 8
    NULL, //	ADLIBMUS_OFFS_9,		// POW_MUS,                 // 9
    NULL, //	ADLIBMUS_OFFS_10,		// SALUTE_MUS,              // 10
	ADLIBMUS_OFFS_11,		// SEARCHN_MUS,             // 11
    NULL, //	ADLIBMUS_OFFS_12,		// SUSPENSE_MUS,            // 12
    NULL, //	ADLIBMUS_OFFS_13,		// VICTORS_MUS,             // 13
	ADLIBMUS_OFFS_14,		// WONDERIN_MUS,            // 14
    NULL, //	ADLIBMUS_OFFS_15,		// FUNKYOU_MUS,             // 15
	ADLIBMUS_OFFS_16,		// ENDLEVEL_MUS,            // 16
    NULL, //    ADLIBMUS_OFFS_17,		// GOINGAFT_MUS,            // 17
    NULL, //	ADLIBMUS_OFFS_18,		// PREGNANT_MUS,            // 18
    NULL, //	ADLIBMUS_OFFS_19,		// ULTIMATE_MUS,            // 19
    NULL, //	ADLIBMUS_OFFS_20,		// NAZI_RAP_MUS,            // 20
    NULL, //	ADLIBMUS_OFFS_21,		// ZEROHOUR_MUS,            // 21
    NULL, //	ADLIBMUS_OFFS_22,		// TWELFTH_MUS,             // 22
    ADLIBMUS_OFFS_23,		// ROSTER_MUS,              // 23
    NULL, //	ADLIBMUS_OFFS_24,		// URAHERO_MUS,             // 24
    NULL, //	ADLIBMUS_OFFS_25,		// VICMARCH_MUS,            // 25
    NULL, //	ADLIBMUS_OFFS_26,		// PACMAN_MUS,              // 26
};
