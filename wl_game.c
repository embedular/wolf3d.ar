// WL_GAME.C

#include "wl_game.h"
#include "wl_menu.h"
#include "wl_draw.h"
#include "wl_agent.h"
#include "wl_act1.h"
#include "wl_act2.h"
#include "wl_main.h"
#include "wl_play.h"
#include "wl_inter.h"
#include "id_sd.h"
#include "id_ca.h"
#include "id_us.h"
#include "id_vl.h"
#include "id_vh.h"
#include "resources.h"
#include "retroport.h"
#include "foreign.h"
#include <math.h>
#include <string.h>


extern const uint8_t *demodata[1 + T_DEMO3 - T_DEMO0];


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

bool         ingame,fizzlein;
gametype     gamestate;
uint8_t      bordercol=VIEWCOLOR;        // color of the Change View/Ingame border

#ifdef SPEAR
int32_t         spearx,speary;
unsigned        spearangle;
bool         spearflag;
#endif

#ifdef USE_FEATUREFLAGS
int ffDataTopLeft, ffDataTopRight, ffDataBottomLeft, ffDataBottomRight;
#endif

//
// ELEVATOR BACK MAPS - REMEMBER (-1)!!
//
static int ElevatorBackTo[]={1,1,7,3,5,3};

void SetupGameLevel (void);
void DrawPlayScreen (void);
void LoadLatchMem (void);
void GameLoop (void);

/*
=============================================================================

                             LOCAL VARIABLES

=============================================================================
*/



//===========================================================================
//===========================================================================


/*
==========================
=
= SetSoundLoc - Given the location of an object (in terms of global
=       coordinates, held in globalsoundx and globalsoundy), munges the values
=       for an approximate distance from the left and right ear, and puts
=       those values into leftchannel and rightchannel.
=
= JAB
=
==========================
*/

int leftchannel, rightchannel;
#define ATABLEMAX 15
static const uint8_t righttable[ATABLEMAX][ATABLEMAX * 2] = {
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 0, 0, 0, 0, 0, 1, 3, 5, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 4, 0, 0, 0, 0, 0, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 4, 1, 0, 0, 0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 5, 4, 2, 1, 0, 1, 2, 3, 5, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 5, 4, 3, 2, 2, 3, 3, 5, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 6, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}
};
static const uint8_t lefttable[ATABLEMAX][ATABLEMAX * 2] = {
{ 8, 8, 8, 8, 8, 8, 8, 8, 5, 3, 1, 0, 0, 0, 0, 0, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 0, 0, 0, 0, 0, 4, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 1, 0, 0, 0, 1, 4, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 5, 3, 2, 1, 0, 1, 2, 4, 5, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 5, 3, 3, 2, 2, 3, 4, 5, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 5, 4, 4, 4, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 6, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}
};

void
SetSoundLoc(fixed gx,fixed gy)
{
    fixed   xt,yt;
    int     x,y;

//
// translate point to view centered coordinates
//
    gx -= viewx;
    gy -= viewy;

//
// calculate newx
//
    xt = FixedMul(gx,viewcos);
    yt = FixedMul(gy,viewsin);
    x = (xt - yt) >> TILESHIFT;

//
// calculate newy
//
    xt = FixedMul(gx,viewsin);
    yt = FixedMul(gy,viewcos);
    y = (yt + xt) >> TILESHIFT;

    if (y >= ATABLEMAX)
        y = ATABLEMAX - 1;
    else if (y <= -ATABLEMAX)
        y = -ATABLEMAX;
    if (x < 0)
        x = -x;
    if (x >= ATABLEMAX)
        x = ATABLEMAX - 1;
    leftchannel  =  lefttable[x][y + ATABLEMAX];
    rightchannel = righttable[x][y + ATABLEMAX];

#if 0
    CenterWindow(8,1);
    US_PrintSigned(leftchannel);
    US_Print(",");
    US_PrintSigned(rightchannel);
    VW_UpdateScreen();
#endif
}

/*
==========================
=
= SetSoundLocGlobal - Sets up globalsoundx & globalsoundy and then calls
=       UpdateSoundLoc() to transform that into relative channel volumes. Those
=       values are then passed to the Sound Manager so that they'll be used for
=       the next sound played (if possible).
=
= JAB
=
==========================
*/
void PlaySoundLocGlobal(uint16_t s,fixed gx,fixed gy)
{
    SetSoundLoc(gx, gy);
    SD_PositionSound(leftchannel, rightchannel);

    int channel = SD_PlaySound((soundnames) s);
    if(channel)
    {
        channelSoundPos[channel - 1].globalsoundx = gx;
        channelSoundPos[channel - 1].globalsoundy = gy;
        channelSoundPos[channel - 1].valid = 1;
    }
}

void UpdateSoundLoc(void)
{
/*    if (SoundPositioned)
    {
        SetSoundLoc(globalsoundx,globalsoundy);
        SD_SetPosition(leftchannel,rightchannel);
    }*/

    for(int i = 0; i < MIX_CHANNELS; i++)
    {
        if(channelSoundPos[i].valid)
        {
            SetSoundLoc(channelSoundPos[i].globalsoundx,
                channelSoundPos[i].globalsoundy);
            SD_SetPosition(i, leftchannel, rightchannel);
        }
    }
}

/*
**      JAB End
*/

/*
==========================
=
= ScanInfoPlane
=
= Spawn all actors and mark down special places
=
==========================
*/

static void ScanInfoPlane(void)
{
    uint16_t        x,y;
    uint16_t        tile;
    const uint16_t  *start;

    start = mapsegs[1];
    for (y=0;y<mapheight;y++)
    {
        //RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);

        for (x=0;x<mapwidth;x++)
        {
            tile = (uint16_t) *start++;
            if (!tile)
                continue;

            switch (tile)
            {
                case 19:
                case 20:
                case 21:
                case 22:
                    SpawnPlayer(x,y,NORTH+tile-19);
                    break;

                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:

                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:

                case 39:
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:

                case 47:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:

                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                case 61:
                case 62:

                case 63:
                case 64:
                case 65:
                case 66:
                case 67:
                case 68:
                case 69:
                case 70:
                case 71:
                case 72:
#ifdef SPEAR
                case 73:                        // TRUCK AND SPEAR!
                case 74:
#elif defined(USE_DIR3DSPR)                     // just for the example
                case 73:
#endif
                    SpawnStatic(x,y,tile-23);
                    break;

//
// P wall
//
                case 98:
                    if (!loadedgame)
                        gamestate.secrettotal++;
                    break;

//
// guard
//
                case 180:
                case 181:
                case 182:
                case 183:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 144:
                case 145:
                case 146:
                case 147:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 108:
                case 109:
                case 110:
                case 111:
                    SpawnStand(en_guard,x,y,tile-108);
                    break;


                case 184:
                case 185:
                case 186:
                case 187:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 148:
                case 149:
                case 150:
                case 151:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 112:
                case 113:
                case 114:
                case 115:
                    SpawnPatrol(en_guard,x,y,tile-112);
                    break;

                case 124:
                    SpawnDeadGuard (x,y);
                    break;
//
// officer
//
                case 188:
                case 189:
                case 190:
                case 191:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 152:
                case 153:
                case 154:
                case 155:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 116:
                case 117:
                case 118:
                case 119:
                    SpawnStand(en_officer,x,y,tile-116);
                    break;


                case 192:
                case 193:
                case 194:
                case 195:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 156:
                case 157:
                case 158:
                case 159:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 120:
                case 121:
                case 122:
                case 123:
                    SpawnPatrol(en_officer,x,y,tile-120);
                    break;


//
// ss
//
                case 198:
                case 199:
                case 200:
                case 201:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 162:
                case 163:
                case 164:
                case 165:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 126:
                case 127:
                case 128:
                case 129:
                    SpawnStand(en_ss,x,y,tile-126);
                    break;


                case 202:
                case 203:
                case 204:
                case 205:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 166:
                case 167:
                case 168:
                case 169:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 130:
                case 131:
                case 132:
                case 133:
                    SpawnPatrol(en_ss,x,y,tile-130);
                    break;

//
// dogs
//
                case 206:
                case 207:
                case 208:
                case 209:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 170:
                case 171:
                case 172:
                case 173:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 134:
                case 135:
                case 136:
                case 137:
                    SpawnStand(en_dog,x,y,tile-134);
                    break;


                case 210:
                case 211:
                case 212:
                case 213:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 174:
                case 175:
                case 176:
                case 177:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    CC_Fallthrough;
                case 138:
                case 139:
                case 140:
                case 141:
                    SpawnPatrol(en_dog,x,y,tile-138);
                    break;

//
// boss
//
#ifndef SPEAR
                case 214:
                    SpawnBoss (x,y);
                    break;
                case 197:
                    SpawnGretel (x,y);
                    break;
                case 215:
                    SpawnGift (x,y);
                    break;
                case 179:
                    SpawnFat (x,y);
                    break;
                case 196:
                    SpawnSchabbs (x,y);
                    break;
                case 160:
                    SpawnFakeHitler (x,y);
                    break;
                case 178:
                    SpawnHitler (x,y);
                    break;
#else
                case 106:
                    SpawnSpectre (x,y);
                    break;
                case 107:
                    SpawnAngel (x,y);
                    break;
                case 125:
                    SpawnTrans (x,y);
                    break;
                case 142:
                    SpawnUber (x,y);
                    break;
                case 143:
                    SpawnWill (x,y);
                    break;
                case 161:
                    SpawnDeath (x,y);
                    break;

#endif

//
// mutants
//
                case 252:
                case 253:
                case 254:
                case 255:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 18;
                    CC_Fallthrough;
                case 234:
                case 235:
                case 236:
                case 237:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 18;
                    CC_Fallthrough;
                case 216:
                case 217:
                case 218:
                case 219:
                    SpawnStand(en_mutant,x,y,tile-216);
                    break;

                case 256:
                case 257:
                case 258:
                case 259:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 18;
                    CC_Fallthrough;
                case 238:
                case 239:
                case 240:
                case 241:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 18;
                    CC_Fallthrough;
                case 220:
                case 221:
                case 222:
                case 223:
                    SpawnPatrol(en_mutant,x,y,tile-220);
                    break;

//
// ghosts
//
#ifndef SPEAR
                case 224:
                    SpawnGhosts (en_blinky,x,y);
                    break;
                case 225:
                    SpawnGhosts (en_clyde,x,y);
                    break;
                case 226:
                    SpawnGhosts (en_pinky,x,y);
                    break;
                case 227:
                    SpawnGhosts (en_inky,x,y);
                    break;
#endif
            }
        }
    }
}

//==========================================================================

/*
==================
=
= SetupGameLevel
=
==================
*/

void SetupGameLevel (void)
{
    int  x,y;
    const uint16_t * map;
    uint16_t tile;

//    PreloadGraphics ();
//    VW_FadeIn();

    if (!loadedgame)
    {
        gamestate.TimeCount
            = gamestate.secrettotal
            = gamestate.killtotal
            = gamestate.treasuretotal
            = gamestate.secretcount
            = gamestate.killcount
            = gamestate.treasurecount
            = pwallstate = pwallpos = facetimes = 0;
        LastAttacker = NULL;
        killerobj = NULL;
    }

    if (demoplayback || demorecord)
        US_InitRndT (false);
    else
        US_InitRndT (true);

//
// load the level
//
    CA_CacheMap (gamestate.mapon+10*gamestate.episode);
    mapon-=gamestate.episode*10;

    // So much information to be processed on a single frame
    // on RETRO-CIAA LPC43xx
//    PreloadUpdate (10, 100);
//
// copy the wall data to a data segment array
//
    memset (tilemap,0,sizeof(tilemap));

//    PreloadUpdate (20, 100);

    memset (actorat,0,sizeof(actorat));

//    PreloadUpdate (30, 100);

    map = mapsegs[0];
    for (y=0;y<mapheight;y++)
    {
        RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);

        for (x=0;x<mapwidth;x++)
        {
            tile = *map++;
            if (tile<AREATILE)
            {
                // solid wall
                tilemap[x][y] = (uint8_t) tile;
                actorat[x][y] = ACTORAT_TILE;
            }
            else
            {
                // area floor
                tilemap[x][y] = 0;
                actorat[x][y] = ACTORAT_EMPTY;
            }
        }
    }

//    PreloadUpdate (40, 100);

//
// spawn doors
//
    InitActorList ();                       // start spawning things with a clean slate
//    PreloadUpdate (50, 100);
    InitDoorList ();
//    PreloadUpdate (60, 100);
    InitStaticList ();
//    PreloadUpdate (70, 100);

    map = mapsegs[0];
    for (y=0;y<mapheight;y++)
    {
        RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);

        for (x=0;x<mapwidth;x++)
        {
            tile = *map++;
            if (tile >= 90 && tile <= 101)
            {
                // door
                switch (tile)
                {
                    case 90:
                    case 92:
                    case 94:
                    case 96:
                    case 98:
                    case 100:
                        SpawnDoor (x,y,1,(tile-90)/2);
                        break;
                    case 91:
                    case 93:
                    case 95:
                    case 97:
                    case 99:
                    case 101:
                        SpawnDoor (x,y,0,(tile-91)/2);
                        break;
                }
            }
        }
    }

//    PreloadUpdate (80, 100);
//
// spawn actors
//
    ScanInfoPlane ();

//    PreloadUpdate (90, 100);
//
// take out the ambush markers
//
    map = mapsegs[0];
    for (y=0;y<mapheight;y++)
    {
        RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);

        for (x=0;x<mapwidth;x++)
        {
            tile = *map++;
            if (tile == AMBUSHTILE)
            {
                tilemap[x][y] = 0;
                //if ( (unsigned)(uintptr_t)actorat[x][y] == ACTORAT_TILE)
                if (actorat[x][y] == ACTORAT_TILE)
                    actorat[x][y] = ACTORAT_EMPTY;

                if (*map >= AREATILE)
                    tile = *map;
                if (*(map-1-mapwidth) >= AREATILE)
                    tile = *(map-1-mapwidth);
                if (*(map-1+mapwidth) >= AREATILE)
                    tile = *(map-1+mapwidth);
                if ( *(map-2) >= AREATILE)
                    tile = *(map-2);

                // sgermino: mapsegs are now readonly
                // *(map-1) = tile;
            }
        }
    }

//    PreloadUpdate (100, 100);
}


//==========================================================================


/*
===================
=
= DrawPlayBorderSides
=
= To fix window overwrites
=
===================
*/
void DrawPlayBorderSides(void)
{
    if(viewsize == 21) return;

	const int sw = (int) screenWidth;
	const int sh = (int) screenHeight;
	const int vw = viewwidth;
	const int vh = viewheight;
	const int px = 1; // size of one "pixel"

	const int h  = sh - px * STATUSLINES;
	const int xl = sw / 2 - vw / 2;
	const int yl = (h - vh) / 2;

    if(xl != 0)
    {
	    VWB_Bar(0,            0, xl - px,     h, bordercol);                 // left side
	    VWB_Bar(xl + vw + px, 0, xl - px * 2, h, bordercol);                 // right side
    }

    if(yl != 0)
    {
	    VWB_Bar(0, 0,            sw, yl - px, bordercol);                    // upper side
	    VWB_Bar(0, yl + vh + px, sw, yl - px, bordercol);                    // lower side
    }

    if(xl != 0)
    {
        // Paint game view border lines
	    VWB_Bar(xl - px, yl - px, vw + px, px,          0);                      // upper border
	    VWB_Bar(xl,      yl + vh, vw + px, px,          bordercol - 2);          // lower border
	    VWB_Bar(xl - px, yl - px, px,      vh + px,     0);                      // left border
	    VWB_Bar(xl + vw, yl - px, px,      vh + px * 2, bordercol - 2);          // right border
	    VWB_Bar(xl - px, yl + vh, px,      px,          bordercol - 3);          // lower left highlight
    }
    else
    {
        // Just paint a lower border line
        VWB_Bar(0, yl+vh, vw, px, bordercol-2);       // lower border
    }
}


/*
===================
=
= DrawStatusBorder
=
===================
*/

void DrawStatusBorder (uint8_t color)
{
    (void) color;
#if 0
    int statusborderw = (screenWidth-scaleFactor*320)/2;

    VWB_Bar (0,0,screenWidth,screenHeight-scaleFactor*(STATUSLINES-3),color);
    VWB_Bar (0,screenHeight-scaleFactor*(STATUSLINES-3),
        statusborderw+scaleFactor*8,scaleFactor*(STATUSLINES-4),color);
    VWB_Bar (0,screenHeight-scaleFactor*2,screenWidth,scaleFactor*2,color);
    VWB_Bar (screenWidth-statusborderw-scaleFactor*8, screenHeight-scaleFactor*(STATUSLINES-3),
        statusborderw+scaleFactor*8,scaleFactor*(STATUSLINES-4),color);

    VWB_Bar (statusborderw+scaleFactor*9, screenHeight-scaleFactor*3,
        scaleFactor*97, scaleFactor*1, color-1);
    VWB_Bar (statusborderw+scaleFactor*106, screenHeight-scaleFactor*3,
        scaleFactor*161, scaleFactor*1, color-2);
    VWB_Bar (statusborderw+scaleFactor*267, screenHeight-scaleFactor*3,
        scaleFactor*44, scaleFactor*1, color-3);
    VWB_Bar (screenWidth-statusborderw-scaleFactor*9, screenHeight-scaleFactor*(STATUSLINES-4),
        scaleFactor*1, scaleFactor*20, color-2);
    VWB_Bar (screenWidth-statusborderw-scaleFactor*9, screenHeight-scaleFactor*(STATUSLINES/2-4),
        scaleFactor*1, scaleFactor*14, color-3);
#endif
}


/*
===================
=
= DrawPlayBorder
=
===================
*/

void DrawPlayBorder (void)
{
    // sgermino: venir a ver esto
    return;
#if 0
	const int px = scaleFactor; // size of one "pixel"

    if (bordercol != VIEWCOLOR)
        DrawStatusBorder(bordercol);
    else
    {
        const int statusborderw = (screenWidth-px*320)/2;
        VWB_Bar (0, screenHeight-px*STATUSLINES,
            statusborderw+px*8, px*STATUSLINES, bordercol);
        VWB_Bar (screenWidth-statusborderw-px*8, screenHeight-px*STATUSLINES,
            statusborderw+px*8, px*STATUSLINES, bordercol);
    }

    if((unsigned) viewheight == screenHeight) return;

    VWB_Bar (0,0,screenWidth,screenHeight-px*STATUSLINES,bordercol);

    const int xl = screenWidth/2-viewwidth/2;
    const int yl = (screenHeight-px*STATUSLINES-viewheight)/2;
    VWB_Bar (xl,yl,viewwidth,viewheight,0);

    if(xl != 0)
    {
        // Paint game view border lines
        VWB_Bar(xl-px, yl-px, viewwidth+px, px, 0);                      // upper border
        VWB_Bar(xl, yl+viewheight, viewwidth+px, px, bordercol-2);       // lower border
        VWB_Bar(xl-px, yl-px, px, viewheight+px, 0);                     // left border
        VWB_Bar(xl+viewwidth, yl-px, px, viewheight+2*px, bordercol-2);  // right border
        VWB_Bar(xl-px, yl+viewheight, px, px, bordercol-3);              // lower left highlight
    }
    else
    {
        // Just paint a lower border line
        VWB_Bar(0, yl+viewheight, viewwidth, px, bordercol-2);       // lower border
    }
#endif
}


/*
===================
=
= DrawPlayScreen
=
===================
*/

void DrawPlayScreen (void)
{
    DrawFace ();
    DrawHealth ();
//    DrawLives ();
//    DrawLevel ();
    DrawWeapon ();
    DrawAmmo ();
    DrawKeys ();
//    DrawScore ();
}


//==========================================================================

/*
==================
=
= StartDemoRecord
=
==================
*/
#ifndef DEMOSEXTERN
char    demoname[13] = "DEMO?.";
#endif

#define MAXDEMOSIZE     8192


//==========================================================================

/*
==================
=
= PlayDemo
=
= Fades the screen out, then starts a demo.  Exits with the screen unfaded
=
==================
*/

void PlayDemo (int demonumber)
{
    int length;
#ifdef DEMOSEXTERN
// debug: load chunk
    #ifndef SPEARDEMO
    //int dems[4]={T_DEMO0,T_DEMO1,T_DEMO2,T_DEMO3};
    #else
    int dems[1]={T_DEMO0};
    #endif

//    CA_CacheGrChunk(dems[demonumber]);
    demoptr = (int8_t *) demodata[demonumber];
#else
    demoname[4] = '0'+demonumber;
    CA_LoadFile (demoname,&demobuffer);
    demoptr = (int8_t *)demobuffer;
#endif

    NewGame (1,0);
    gamestate.mapon = *demoptr++;
    gamestate.difficulty = gd_hard;
    length = READWORD((uint8_t **)&demoptr);
    // TODO: Seems like the original demo format supports 16 MB demos
    //       But T_DEM00 and T_DEM01 of Wolf have a 0xd8 as third length size...
    demoptr++;
    lastdemoptr = demoptr-4+length;

    VW_FadeOut ();

    SETFONTCOLOR(0,15);
    //DrawPlayScreen ();

    startgame = false;
    demoplayback = true;

    SetupGameLevel ();
    StartMusic ();

    PlayLoop ();

#ifdef DEMOSEXTERN
//    UNCACHEGRCHUNK(dems[demonumber]);
#else
    MM_FreePtr (&demobuffer);
#endif

    demoplayback = false;

    StopMusic ();
    ClearMemory ();
}

//==========================================================================

/*
==================
=
= Died
=
==================
*/

#define DEATHROTATE 2

void Died (void)
{
    float   fangle;
    int32_t dx,dy;
    int     iangle,curangle,clockwise,counter,change;

    if (screenfaded)
    {
        ThreeDRefresh ();
        VW_FadeIn ();
    }

    gamestate.weapon = (weapontype) -1;                     // take away weapon
    SD_PlaySound (PLAYERDEATHSND);

    //
    // swing around to face attacker
    //
    if(killerobj)
    {
        dx = killerobj->x - player->x;
        dy = player->y - killerobj->y;

        fangle = (float) atan2((float) dy, (float) dx);     // returns -pi to pi
        if (fangle<0)
            fangle = (float) (M_PI*2+fangle);

        iangle = (int) (fangle/(M_PI*2)*ANGLES);
    }
    else
    {
        iangle = player->angle + ANGLES / 2;
        if(iangle >= ANGLES) iangle -= ANGLES;
    }

    if (player->angle > iangle)
    {
        counter = player->angle - iangle;
        clockwise = ANGLES-player->angle + iangle;
    }
    else
    {
        clockwise = iangle - player->angle;
        counter = player->angle + ANGLES-iangle;
    }

    curangle = player->angle;

    if (clockwise<counter)
    {
        //
        // rotate clockwise
        //
        if (curangle>iangle)
            curangle -= ANGLES;
        do
        {
            change = tics*DEATHROTATE;
            if (curangle + change > iangle)
                change = iangle-curangle;

            curangle += change;
            player->angle += change;
            if (player->angle >= ANGLES)
                player->angle -= ANGLES;

            ThreeDRefresh ();
            CalcTics ();
        } while (curangle != iangle);
    }
    else
    {
        //
        // rotate counterclockwise
        //
        if (curangle<iangle)
            curangle += ANGLES;
        do
        {
            change = -(int)tics*DEATHROTATE;
            if (curangle + change < iangle)
                change = iangle-curangle;

            curangle += change;
            player->angle += change;
            if (player->angle < 0)
                player->angle += ANGLES;

            ThreeDRefresh ();
            CalcTics ();
        } while (curangle != iangle);
    }

    //
    // fade to red
    //
    FinishPaletteShifts ();

    VH_UpdateScreen();

    VL_Bar (viewscreenx,viewscreeny,viewwidth,viewheight,128/*4*/);

    FizzleFade(viewscreenx,viewscreeny,viewwidth,viewheight,70,false);

    IN_UserInput(100);
    SD_WaitSoundDone ();
    ClearMemory();

    gamestate.lives--;

    if (gamestate.lives > -1)
    {
        gamestate.health = 100;
        gamestate.weapon = gamestate.bestweapon
            = gamestate.chosenweapon = wp_pistol;
        gamestate.ammo = STARTAMMO;
        gamestate.keys = 0;
        pwallstate = pwallpos = 0;
        gamestate.attackframe = gamestate.attackcount =
            gamestate.weaponframe = 0;

        if(viewsize != 21)
        {
            DrawKeys ();
            DrawWeapon ();
            DrawAmmo ();
            DrawHealth ();
            DrawFace ();
            DrawLives ();
        }
    }
}

//==========================================================================

/*
===================
=
= GameLoop
=
===================
*/

void GameLoop (void)
{
    bool died;
#ifdef MYPROFILE
    clock_t start,end;
#endif

restartgame:
    ClearMemory ();
    SETFONTCOLOR(0,15);
//    VW_FadeOut();
    //DrawPlayScreen ();
    died = false;
    do
    {
        if (!loadedgame)
            gamestate.score = gamestate.oldscore;
        if(!died || viewsize != 21) DrawScore();

        startgame = false;
        if (!loadedgame)
        {
            if (!died)
            {
                ClearSplitVWB ();           // set up for double buffering in split screen
                VWB_Bar (0, 0, screenWidth, screenHeight, bordercol);
                PreloadGraphics ();
            }
            SetupGameLevel ();
        }

#ifdef SPEAR
        if (gamestate.mapon == 20)      // give them the key allways
        {
            gamestate.keys |= 1;
            DrawKeys ();
        }
#endif

        ingame = true;
        if(loadedgame)
        {
            ContinueMusic(lastgamemusicoffset);
            loadedgame = false;
        }
        else StartMusic ();

        if (!died)
        {
        //    PreloadGraphics ();   // TODO: Let this do something useful!
        }                           //          sgermino: done, above :)
        else
        {
            died = false;
            fizzlein = true;
        }

        DrawLevel ();

#ifdef SPEAR
startplayloop:
#endif

#ifdef RETROCIAA_SKIP_GAMEPLAY
        playstate = ex_completed;
#else
        PlayLoop ();
#endif

#ifdef SPEAR
        if (spearflag)
        {
            SD_StopSound();
            SD_PlaySound(GETSPEARSND);
            if (DigiMode != sds_Off)
            {
                Delay(150);
            }
            else
                SD_WaitSoundDone();

            ClearMemory ();
            gamestate.oldscore = gamestate.score;
            gamestate.mapon = 20;
            SetupGameLevel ();
            StartMusic ();
            player->x = spearx;
            player->y = speary;
            player->angle = (short)spearangle;
            spearflag = false;
            Thrust (0,0);
            goto startplayloop;
        }
#endif

        StopMusic ();
        ingame = false;

        if (demorecord && playstate != ex_warped)
        {
            // FinishDemoRecord ();
        }

        if (startgame || loadedgame)
            goto restartgame;

        switch (playstate)
        {
            case ex_completed:
            case ex_secretlevel:
                //if(viewsize == 21) DrawPlayScreen();
                gamestate.keys = 0;
           //     DrawKeys ();
                VW_FadeOut ();

                ClearMemory ();

                LevelCompleted ();              // do the intermission

#ifdef SPEARDEMO
                if (gamestate.mapon == 1)
                {
                    died = true;                    // don't "get psyched!"

                    VW_FadeOut ();

                    ClearMemory ();

                    CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                    strcpy(MainMenu[viewscores].string,STR_VS);
#endif
                    MainMenu[viewscores].routine = CP_ViewScores;
                    return;
                }
#endif

#ifdef JAPDEMO
                if (gamestate.mapon == 3)
                {
                    died = true;                    // don't "get psyched!"

                    VW_FadeOut ();

                    ClearMemory ();

                    CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                    strcpy(MainMenu[viewscores].string,STR_VS);
#endif
                    MainMenu[viewscores].routine = CP_ViewScores;
                    return;
                }
#endif

                gamestate.oldscore = gamestate.score;

#ifndef SPEAR
                //
                // COMING BACK FROM SECRET LEVEL
                //
                if (gamestate.mapon == 9)
                    gamestate.mapon = ElevatorBackTo[gamestate.episode];    // back from secret
                else
                    //
                    // GOING TO SECRET LEVEL
                    //
                    if (playstate == ex_secretlevel)
                        gamestate.mapon = 9;
#else

#define FROMSECRET1             3
#define FROMSECRET2             11

                //
                // GOING TO SECRET LEVEL
                //
                if (playstate == ex_secretlevel)
                    switch(gamestate.mapon)
                {
                    case FROMSECRET1: gamestate.mapon = 18; break;
                    case FROMSECRET2: gamestate.mapon = 19; break;
                }
                else
                    //
                    // COMING BACK FROM SECRET LEVEL
                    //
                    if (gamestate.mapon == 18 || gamestate.mapon == 19)
                        switch(gamestate.mapon)
                    {
                        case 18: gamestate.mapon = FROMSECRET1+1; break;
                        case 19: gamestate.mapon = FROMSECRET2+1; break;
                    }
#endif
                    else
                        //
                        // GOING TO NEXT LEVEL
                        //
                        gamestate.mapon++;
                break;

            case ex_died:
                Died ();
                died = true;                    // don't "get psyched!"

                if (gamestate.lives > -1)
                    break;                          // more lives left

                VW_FadeOut ();
                if(screenHeight % RETROPORT_SCREEN_HEIGHT != 0)
                    VL_ClearScreen(0);

                ClearMemory ();

                CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                MainMenu[viewscores].string = STR_VS;
#endif
                MainMenu[viewscores].routine = CP_ViewScores;
                return;

            case ex_victorious:
                //if(viewsize == 21) DrawPlayScreen();
#ifndef SPEAR
                VW_FadeOut ();
#else
                VL_FadeOut (0,255,C6(0),C6(17),C6(17),300);
#endif
                ClearMemory ();

                Victory ();

                ClearMemory ();

                CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                MainMenu[viewscores].string = STR_VS;
#endif
                MainMenu[viewscores].routine = CP_ViewScores;
                return;

            default:
                //if(viewsize == 21) DrawPlayScreen();
                ClearMemory ();
                break;
        }
    } while (1);
}
