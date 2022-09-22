// WL_MAIN.C

#include "wl_main.h"
#include "embedul.ar/source/core/device/board.h"
#include "wl_def.h"
#include "wl_play.h"
#include "wl_game.h"
#include "wl_inter.h"
#include "wl_menu.h"
#include "id_sd.h"
#include "id_vh.h"
#include "id_us.h"
#include "id_pm.h"
#include "id_in.h"
#include "id_ca.h"
#include "retroport.h"
#include <stdarg.h>
#include <stdlib.h>

/*
=============================================================================

                             WOLFENSTEIN 3-D

                        An Id Software production

                             by John Carmack

=============================================================================
*/

/*
=============================================================================

                             LOCAL CONSTANTS

=============================================================================
*/


#define FOCALLENGTH     (0x5700l)               // in global coordinates
#define VIEWGLOBAL      0x10000                 // globals visable flush to wall

#define VIEWWIDTH       256                     // size of view window
#define VIEWHEIGHT      144

/*
=============================================================================

                            GLOBAL VARIABLES

=============================================================================
*/

//char    str[80];
int     dirangle[9] = {0,ANGLES/8,2*ANGLES/8,3*ANGLES/8,4*ANGLES/8,
                       5*ANGLES/8,6*ANGLES/8,7*ANGLES/8,ANGLES};

//
// proejection variables
//
fixed    focallength;
unsigned screenofs;
int      viewscreenx, viewscreeny;
int      viewwidth;
int      viewheight;
short    centerx;
int      shootdelta;           // pixels away from centerx a target can be
fixed    scale;
int32_t  heightnumerator;


void    Quit (const char *error,...);

bool startgame;
bool loadedgame;
int     mouseadjustment;

//char    configdir[256] = "";
//char    configname[13] = "config.";

//
// Command line parameter variables
//
bool param_debugmode = false;
bool param_nowait = false;
int     param_difficulty = 1;           // default is "normal"
int     param_tedlevel = -1;            // default is not to start a level
int     param_joystickindex = 0;

int     param_joystickhat = -1;
const int     param_audiobuffer = 512;

int     param_mission = 0;
bool param_goodtimes = false;
bool param_ignorenumchunks = false;

/*
=============================================================================

                            LOCAL VARIABLES

=============================================================================
*/


/*
====================
=
= ReadConfig
=
====================
*/

void ReadConfig(void)
{
    viewsize = 21;

    SD_SetMusicMode (smm_AdLib);
    SD_SetSoundMode (sdm_AdLib);
    SD_SetDigiDevice (sds_SoundBlaster);
}

/*
====================
=
= WriteConfig
=
====================
*/

void WriteConfig(void)
{
#if 0
    char configpath[300];

#ifdef _arch_dreamcast
    fs_unlink(configname);
#endif

    if(configdir[0])
        snprintf(configpath, sizeof(configpath), "%s/%s", configdir, configname);
    else
        strcpy(configpath, configname);

    const int file = open(configpath, O_CREAT | O_WRONLY | O_BINARY, 0644);
    if (file != -1)
    {
        word tmp=0xfefa;
        write(file,&tmp,sizeof(tmp));
        write(file,Scores,sizeof(HighScore) * MaxScores);

        write(file,&SoundMode,sizeof(SoundMode));
        write(file,&MusicMode,sizeof(MusicMode));
        write(file,&DigiMode,sizeof(DigiMode));

        write(file,&mouseenabled,sizeof(mouseenabled));
        write(file,&joystickenabled,sizeof(joystickenabled));
        bool dummyJoypadEnabled = false;
        write(file,&dummyJoypadEnabled,sizeof(dummyJoypadEnabled));
        bool dummyJoystickProgressive = false;
        write(file,&dummyJoystickProgressive,sizeof(dummyJoystickProgressive));
        int dummyJoystickPort = 0;
        write(file,&dummyJoystickPort,sizeof(dummyJoystickPort));

        write(file,dirscan,sizeof(dirscan));
        write(file,buttonscan,sizeof(buttonscan));
        write(file,buttonmouse,sizeof(buttonmouse));
        write(file,buttonjoy,sizeof(buttonjoy));

        write(file,&viewsize,sizeof(viewsize));
        write(file,&mouseadjustment,sizeof(mouseadjustment));

        close(file);
    }
#ifdef _arch_dreamcast
    DC_SaveToVMU(configname, NULL);
#endif

#endif
}


//===========================================================================

/*
=====================
=
= NewGame
=
= Set up new game to start from the beginning
=
=====================
*/

void NewGame (int difficulty,int episode)
{
    memset (&gamestate,0,sizeof(gamestate));
    gamestate.difficulty = difficulty;
    gamestate.weapon = gamestate.bestweapon
            = gamestate.chosenweapon = wp_pistol;
    gamestate.health = 100;
    gamestate.ammo = STARTAMMO;
    gamestate.lives = 3;
    gamestate.nextextra = EXTRAPOINTS;
    gamestate.episode=episode;

    startgame = true;
}

//===========================================================================

void DiskFlopAnim(int x,int y)
{
    static int8_t which=0;
    if (!x && !y)
        return;
    VWB_DrawPic(x,y,C_DISKLOADING1PIC+which);
    VW_UpdateScreen();
    which^=1;
}


int32_t DoChecksum(uint8_t *source,unsigned size,int32_t checksum)
{
    unsigned i;

    for (i=0;i<size-1;i++)
    checksum += source[i]^source[i+1];

    return checksum;
}


/*
==================
=
= SaveTheGame
=
==================
*/

extern statetype s_grdstand;
extern statetype s_player;

bool SaveTheGame(FILE *file,int x,int y)
{
    (void) file;
    (void) x;
    (void) y;

#if 0
//    struct diskfree_t dfree;
//    int32_t avail,size,checksum;
    int checksum;
    objtype *ob;
    objtype nullobj;
    statobj_t nullstat;

/*    if (_dos_getdiskfree(0,&dfree))
        Quit("Error in _dos_getdiskfree call");

    avail = (int32_t)dfree.avail_clusters *
                  dfree.bytes_per_sector *
                  dfree.sectors_per_cluster;

    size = 0;
    for (ob = player; ob ; ob=ob->next)
        size += sizeof(*ob);
    size += sizeof(nullobj);

    size += sizeof(gamestate) +
            sizeof(LRstruct)*LRpack +
            sizeof(tilemap) +
            sizeof(actorat) +
            sizeof(laststatobj) +
            sizeof(statobjlist) +
            sizeof(doorposition) +
            sizeof(pwallstate) +
            sizeof(pwalltile) +
            sizeof(pwallx) +
            sizeof(pwally) +
            sizeof(pwalldir) +
            sizeof(pwallpos);

    if (avail < size)
    {
        Message(STR_NOSPACE1"\n"STR_NOSPACE2);
        return false;
    }*/

    checksum = 0;

    DiskFlopAnim(x,y);
    fwrite(&gamestate,sizeof(gamestate),1,file);
    checksum = DoChecksum((uint8_t *)&gamestate,sizeof(gamestate),checksum);

    DiskFlopAnim(x,y);
    fwrite(&LevelRatios[0],sizeof(LRstruct)*LRpack,1,file);
    checksum = DoChecksum((uint8_t *)&LevelRatios[0],sizeof(LRstruct)*LRpack,checksum);

    DiskFlopAnim(x,y);
    fwrite(tilemap,sizeof(tilemap),1,file);
    checksum = DoChecksum((uint8_t *)tilemap,sizeof(tilemap),checksum);
    DiskFlopAnim(x,y);

    int i;
    for(i=0;i<MAPSIZE;i++)
    {
        for(int j=0;j<MAPSIZE;j++)
        {
            uint16_t actnum;
            objtype *objptr=actorat[i][j];
            if(ISPOINTER(objptr))
                actnum=0x8000 | (uint16_t)(objptr-objlist);
            else
                actnum=(uint16_t)(uintptr_t)objptr;
            fwrite(&actnum,sizeof(actnum),1,file);
            checksum = DoChecksum((uint8_t *)&actnum,sizeof(actnum),checksum);
        }
    }

    fwrite (areaconnect,sizeof(areaconnect),1,file);
    fwrite (areabyplayer,sizeof(areabyplayer),1,file);

    // player object needs special treatment as it's in WL_AGENT.CPP and not in
    // WL_ACT2.CPP which could cause problems for the relative addressing

    ob = player;
    DiskFlopAnim(x,y);
    memcpy(&nullobj,ob,sizeof(nullobj));
    nullobj.state=(statetype *) ((uintptr_t)nullobj.state-(uintptr_t)&s_player);
    fwrite(&nullobj,sizeof(nullobj),1,file);
    ob = ob->next;

    DiskFlopAnim(x,y);
    for (; ob ; ob=ob->next)
    {
        memcpy(&nullobj,ob,sizeof(nullobj));
        nullobj.state=(statetype *) ((uintptr_t)nullobj.state-(uintptr_t)&s_grdstand);
        fwrite(&nullobj,sizeof(nullobj),1,file);
    }
    nullobj.active = ac_badobject;          // end of file marker
    DiskFlopAnim(x,y);
    fwrite(&nullobj,sizeof(nullobj),1,file);

    DiskFlopAnim(x,y);
    uint16_t laststatobjnum=(uint16_t) (laststatobj-statobjlist);
    fwrite(&laststatobjnum,sizeof(laststatobjnum),1,file);
    checksum = DoChecksum((uint8_t *)&laststatobjnum,sizeof(laststatobjnum),checksum);

    DiskFlopAnim(x,y);
    for(i=0;i<MAXSTATS;i++)
    {
        memcpy(&nullstat,statobjlist+i,sizeof(nullstat));
        nullstat.visspot=(uint8_t *) ((uintptr_t) nullstat.visspot-(uintptr_t)spotvis);
        fwrite(&nullstat,sizeof(nullstat),1,file);
        checksum = DoChecksum((uint8_t *)&nullstat,sizeof(nullstat),checksum);
    }

    DiskFlopAnim(x,y);
    fwrite (doorposition,sizeof(doorposition),1,file);
    checksum = DoChecksum((uint8_t *)doorposition,sizeof(doorposition),checksum);
    DiskFlopAnim(x,y);
    fwrite (doorobjlist,sizeof(doorobjlist),1,file);
    checksum = DoChecksum((uint8_t *)doorobjlist,sizeof(doorobjlist),checksum);

    DiskFlopAnim(x,y);
    fwrite (&pwallstate,sizeof(pwallstate),1,file);
    checksum = DoChecksum((uint8_t *)&pwallstate,sizeof(pwallstate),checksum);
    fwrite (&pwalltile,sizeof(pwalltile),1,file);
    checksum = DoChecksum((uint8_t *)&pwalltile,sizeof(pwalltile),checksum);
    fwrite (&pwallx,sizeof(pwallx),1,file);
    checksum = DoChecksum((uint8_t *)&pwallx,sizeof(pwallx),checksum);
    fwrite (&pwally,sizeof(pwally),1,file);
    checksum = DoChecksum((uint8_t *)&pwally,sizeof(pwally),checksum);
    fwrite (&pwalldir,sizeof(pwalldir),1,file);
    checksum = DoChecksum((uint8_t *)&pwalldir,sizeof(pwalldir),checksum);
    fwrite (&pwallpos,sizeof(pwallpos),1,file);
    checksum = DoChecksum((uint8_t *)&pwallpos,sizeof(pwallpos),checksum);

    //
    // WRITE OUT CHECKSUM
    //
    fwrite (&checksum,sizeof(checksum),1,file);

    fwrite (&lastgamemusicoffset,sizeof(lastgamemusicoffset),1,file);
#endif

    return(true);
}

//===========================================================================

/*
==================
=
= LoadTheGame
=
==================
*/

bool LoadTheGame(FILE *file,int x,int y)
{
    (void) file;
    (void) x;
    (void) y;

#if 0
    int32_t checksum,oldchecksum;
    objtype nullobj;
    statobj_t nullstat;

    checksum = 0;

    DiskFlopAnim(x,y);
    fread (&gamestate,sizeof(gamestate),1,file);
    checksum = DoChecksum((byte *)&gamestate,sizeof(gamestate),checksum);

    DiskFlopAnim(x,y);
    fread (&LevelRatios[0],sizeof(LRstruct)*LRpack,1,file);
    checksum = DoChecksum((byte *)&LevelRatios[0],sizeof(LRstruct)*LRpack,checksum);

    DiskFlopAnim(x,y);
    SetupGameLevel ();

    DiskFlopAnim(x,y);
    fread (tilemap,sizeof(tilemap),1,file);
    checksum = DoChecksum((byte *)tilemap,sizeof(tilemap),checksum);

    DiskFlopAnim(x,y);

    int actnum=0, i;
    for(i=0;i<MAPSIZE;i++)
    {
        for(int j=0;j<MAPSIZE;j++)
        {
            fread (&actnum,sizeof(word),1,file);
            checksum = DoChecksum((byte *) &actnum,sizeof(word),checksum);
            if(actnum&0x8000)
                actorat[i][j]=objlist+(actnum&0x7fff);
            else
                actorat[i][j]=(objtype *)(uintptr_t) actnum;
        }
    }

    fread (areaconnect,sizeof(areaconnect),1,file);
    fread (areabyplayer,sizeof(areabyplayer),1,file);

    InitActorList ();
    DiskFlopAnim(x,y);
    fread (player,sizeof(*player),1,file);
    player->state=(statetype *) ((uintptr_t)player->state+(uintptr_t)&s_player);

    while (1)
    {
        DiskFlopAnim(x,y);
        fread (&nullobj,sizeof(nullobj),1,file);
        if (nullobj.active == ac_badobject)
            break;
        GetNewActor ();
        nullobj.state=(statetype *) ((uintptr_t)nullobj.state+(uintptr_t)&s_grdstand);
        // don't copy over the links
        memcpy (newobj,&nullobj,sizeof(nullobj)-8);
    }

    DiskFlopAnim(x,y);
    word laststatobjnum;
    fread (&laststatobjnum,sizeof(laststatobjnum),1,file);
    laststatobj=statobjlist+laststatobjnum;
    checksum = DoChecksum((byte *)&laststatobjnum,sizeof(laststatobjnum),checksum);

    DiskFlopAnim(x,y);
    for(i=0;i<MAXSTATS;i++)
    {
        fread(&nullstat,sizeof(nullstat),1,file);
        checksum = DoChecksum((byte *)&nullstat,sizeof(nullstat),checksum);
        nullstat.visspot=(byte *) ((uintptr_t)nullstat.visspot+(uintptr_t)spotvis);
        memcpy(statobjlist+i,&nullstat,sizeof(nullstat));
    }

    DiskFlopAnim(x,y);
    fread (doorposition,sizeof(doorposition),1,file);
    checksum = DoChecksum((byte *)doorposition,sizeof(doorposition),checksum);
    DiskFlopAnim(x,y);
    fread (doorobjlist,sizeof(doorobjlist),1,file);
    checksum = DoChecksum((byte *)doorobjlist,sizeof(doorobjlist),checksum);

    DiskFlopAnim(x,y);
    fread (&pwallstate,sizeof(pwallstate),1,file);
    checksum = DoChecksum((byte *)&pwallstate,sizeof(pwallstate),checksum);
    fread (&pwalltile,sizeof(pwalltile),1,file);
    checksum = DoChecksum((byte *)&pwalltile,sizeof(pwalltile),checksum);
    fread (&pwallx,sizeof(pwallx),1,file);
    checksum = DoChecksum((byte *)&pwallx,sizeof(pwallx),checksum);
    fread (&pwally,sizeof(pwally),1,file);
    checksum = DoChecksum((byte *)&pwally,sizeof(pwally),checksum);
    fread (&pwalldir,sizeof(pwalldir),1,file);
    checksum = DoChecksum((byte *)&pwalldir,sizeof(pwalldir),checksum);
    fread (&pwallpos,sizeof(pwallpos),1,file);
    checksum = DoChecksum((byte *)&pwallpos,sizeof(pwallpos),checksum);

    if (gamestate.secretcount)      // assign valid floorcodes under moved pushwalls
    {
        word *map, *obj; word tile, sprite;
        map = mapsegs[0]; obj = mapsegs[1];
        for (y=0;y<mapheight;y++)
            for (x=0;x<mapwidth;x++)
            {
                tile = *map++; sprite = *obj++;
                if (sprite == PUSHABLETILE && !tilemap[x][y]
                    && (tile < AREATILE || tile >= (AREATILE+NUMMAPS)))
                {
                    if (*map >= AREATILE)
                        tile = *map;
                    if (*(map-1-mapwidth) >= AREATILE)
                        tile = *(map-1-mapwidth);
                    if (*(map-1+mapwidth) >= AREATILE)
                        tile = *(map-1+mapwidth);
                    if ( *(map-2) >= AREATILE)
                        tile = *(map-2);

                    *(map-1) = tile; *(obj-1) = 0;
                }
            }
    }

    Thrust(0,0);    // set player->areanumber to the floortile you're standing on

    fread (&oldchecksum,sizeof(oldchecksum),1,file);

    fread (&lastgamemusicoffset,sizeof(lastgamemusicoffset),1,file);
    if(lastgamemusicoffset<0) lastgamemusicoffset=0;


    if (oldchecksum != checksum)
    {
        Message(STR_SAVECHT1"\n"
                STR_SAVECHT2"\n"
                STR_SAVECHT3"\n"
                STR_SAVECHT4);

        IN_ClearKeysDown();
        IN_Ack();

        gamestate.oldscore = gamestate.score = 0;
        gamestate.lives = 1;
        gamestate.weapon =
            gamestate.chosenweapon =
            gamestate.bestweapon = wp_pistol;
        gamestate.ammo = 8;
    }

    return true;
#endif
    return false;
}

//===========================================================================

/*
==========================
=
= ShutdownId
=
= Shuts down all ID_?? managers
=
==========================
*/

void ShutdownId (void)
{
    US_Shutdown ();         // This line is completely useless...
    SD_Shutdown ();
    PM_Shutdown ();
    IN_Shutdown ();
    VW_Shutdown ();
    CA_Shutdown ();
}


//===========================================================================


/*
====================
=
= CalcProjection
=
= Uses focallength
=
====================
*/

void CalcProjection (int32_t focal)
{
    int     halfview;
    double  facedist;

    focallength = focal;
    facedist = focal+MINDIST;
    halfview = viewwidth/2;                                 // half view in pixels

    //
    // calculate scale value for vertical height calculations
    // and sprite x calculations
    //
    scale = (fixed) (halfview*facedist/(VIEWGLOBAL/2));

    //
    // divide heightnumerator by a posts distance to get the posts height for
    // the heightbuffer.  The pixel height is height>>2
    //
    heightnumerator = (TILEGLOBAL*scale)>>6;

    //
    // calculate the angle offset from view angle of each pixel's ray
    // (pixelangle Now in ROM)
}



//===========================================================================

/*
==========================
=
= SignonScreen
=
==========================
*/

void SignonScreen (void)                        // VGA version
{
    VL_SetVGAPlaneMode ();

    RETROPORT_FillFromStorage (RETROPORT_CACHED_ELEMENT_SIGNON);
}


/*
==========================
=
= FinishSignon
=
==========================
*/

void FinishSignon (void)
{
#ifndef SPEAR
    VW_Bar (0,133,235,10,VL_GetPixel(0,0));

    WindowX = 0;
    WindowW = RETROPORT_SCREEN_WIDTH;
    PrintY = 133;

    #ifndef JAPAN
        //SETFONTCOLOR(14,4);
        SETFONTCOLOR(253,160);

        #ifdef SPANISH
            US_CPrint ("Oprima un boton");
        #else
            US_CPrint ("Press a key");
        #endif
    #endif

    VIDEO_CopyFrame ();
    VH_UpdateScreen();

    if (!param_nowait)
        IN_Ack ();

    #ifndef JAPAN
        VW_Bar (0,133,235,10,VL_GetPixel(0,0));

        PrintY = 133;
        // SETFONTCOLOR(10,4);
        SETFONTCOLOR(93,160);

        #ifdef SPANISH
        US_CPrint ("Pensando...");
        #else
        US_CPrint ("Working...");
        #endif

        VIDEO_CopyFrame ();
        VH_UpdateScreen();
    #endif

    SETFONTCOLOR(0,15);
#else
    VH_UpdateScreen();

    if (!param_nowait)
        VW_WaitVBL(3*70);
#endif
}

//===========================================================================

#if 0
/*
=====================
=
= InitDigiMap
=
=====================
*/

// channel mapping:
//  -1: any non reserved channel
//   0: player weapons
//   1: boss weapons

static const int wolfdigimap[] =
    {
        // These first sounds are in the upload version
#ifndef SPEAR
        HALTSND,                0,  -1,
        DOGBARKSND,             1,  -1,
        CLOSEDOORSND,           2,  -1,
        OPENDOORSND,            3,  -1,
        ATKMACHINEGUNSND,       4,   0,
        ATKPISTOLSND,           5,   0,
        ATKGATLINGSND,          6,   0,
        SCHUTZADSND,            7,  -1,
        GUTENTAGSND,            8,  -1,
        MUTTISND,               9,  -1,
        BOSSFIRESND,            10,  1,
        SSFIRESND,              11, -1,
        DEATHSCREAM1SND,        12, -1,
        DEATHSCREAM2SND,        13, -1,
        DEATHSCREAM3SND,        13, -1,
        TAKEDAMAGESND,          14, -1,
        PUSHWALLSND,            15, -1,

        LEBENSND,               20, -1,
        NAZIFIRESND,            21, -1,
        SLURPIESND,             22, -1,

        YEAHSND,                32, -1,

#ifndef UPLOAD
        // These are in all other episodes
        DOGDEATHSND,            16, -1,
        AHHHGSND,               17, -1,
        DIESND,                 18, -1,
        EVASND,                 19, -1,

        TOT_HUNDSND,            23, -1,
        MEINGOTTSND,            24, -1,
        SCHABBSHASND,           25, -1,
        HITLERHASND,            26, -1,
        SPIONSND,               27, -1,
        NEINSOVASSND,           28, -1,
        DOGATTACKSND,           29, -1,
        LEVELDONESND,           30, -1,
        MECHSTEPSND,            31, -1,

        SCHEISTSND,             33, -1,
        DEATHSCREAM4SND,        34, -1,         // AIIEEE
        DEATHSCREAM5SND,        35, -1,         // DEE-DEE
        DONNERSND,              36, -1,         // EPISODE 4 BOSS DIE
        EINESND,                37, -1,         // EPISODE 4 BOSS SIGHTING
        ERLAUBENSND,            38, -1,         // EPISODE 6 BOSS SIGHTING
        DEATHSCREAM6SND,        39, -1,         // FART
        DEATHSCREAM7SND,        40, -1,         // GASP
        DEATHSCREAM8SND,        41, -1,         // GUH-BOY!
        DEATHSCREAM9SND,        42, -1,         // AH GEEZ!
        KEINSND,                43, -1,         // EPISODE 5 BOSS SIGHTING
        MEINSND,                44, -1,         // EPISODE 6 BOSS DIE
        ROSESND,                45, -1,         // EPISODE 5 BOSS DIE

#endif
#else
//
// SPEAR OF DESTINY DIGISOUNDS
//
        HALTSND,                0,  -1,
        CLOSEDOORSND,           2,  -1,
        OPENDOORSND,            3,  -1,
        ATKMACHINEGUNSND,       4,   0,
        ATKPISTOLSND,           5,   0,
        ATKGATLINGSND,          6,   0,
        SCHUTZADSND,            7,  -1,
        BOSSFIRESND,            8,   1,
        SSFIRESND,              9,  -1,
        DEATHSCREAM1SND,        10, -1,
        DEATHSCREAM2SND,        11, -1,
        TAKEDAMAGESND,          12, -1,
        PUSHWALLSND,            13, -1,
        AHHHGSND,               15, -1,
        LEBENSND,               16, -1,
        NAZIFIRESND,            17, -1,
        SLURPIESND,             18, -1,
        LEVELDONESND,           22, -1,
        DEATHSCREAM4SND,        23, -1,         // AIIEEE
        DEATHSCREAM3SND,        23, -1,         // DOUBLY-MAPPED!!!
        DEATHSCREAM5SND,        24, -1,         // DEE-DEE
        DEATHSCREAM6SND,        25, -1,         // FART
        DEATHSCREAM7SND,        26, -1,         // GASP
        DEATHSCREAM8SND,        27, -1,         // GUH-BOY!
        DEATHSCREAM9SND,        28, -1,         // AH GEEZ!
        GETGATLINGSND,          38, -1,         // Got Gat replacement

#ifndef SPEARDEMO
        DOGBARKSND,             1,  -1,
        DOGDEATHSND,            14, -1,
        SPIONSND,               19, -1,
        NEINSOVASSND,           20, -1,
        DOGATTACKSND,           21, -1,
        TRANSSIGHTSND,          29, -1,         // Trans Sight
        TRANSDEATHSND,          30, -1,         // Trans Death
        WILHELMSIGHTSND,        31, -1,         // Wilhelm Sight
        WILHELMDEATHSND,        32, -1,         // Wilhelm Death
        UBERDEATHSND,           33, -1,         // Uber Death
        KNIGHTSIGHTSND,         34, -1,         // Death Knight Sight
        KNIGHTDEATHSND,         35, -1,         // Death Knight Death
        ANGELSIGHTSND,          36, -1,         // Angel Sight
        ANGELDEATHSND,          37, -1,         // Angel Death
        GETSPEARSND,            39, -1,         // Got Spear replacement
#endif
#endif
        LASTSOUND
    };

#error aca
void InitDigiMap (void)
{
    const int *map;

    for (map = wolfdigimap; *map != LASTSOUND; map += 3)
    {
        DigiMap[map[0]] = map[1];
        DigiChannel[map[1]] = map[2];
        SD_PrepareSound(map[1], map[0]);
    }
}
#endif

#if 0
#ifndef SPEAR
static CP_iteminfo MusicItems={CTL_X,CTL_Y,6,0,32};
static CP_itemtype MusicMenu[]=
    {
        {1,"Get Them!",0},
        {1,"Searching",0},
        {1,"P.O.W.",0},
        {1,"Suspense",0},
        {1,"War March",0},
        {1,"Around The Corner!",0},

        {1,"Nazi Anthem",0},
        {1,"Lurking...",0},
        {1,"Going After Hitler",0},
        {1,"Pounding Headache",0},
        {1,"Into the Dungeons",0},
        {1,"Ultimate Conquest",0},

        {1,"Kill the S.O.B.",0},
        {1,"The Nazi Rap",0},
        {1,"Twelfth Hour",0},
        {1,"Zero Hour",0},
        {1,"Ultimate Conquest",0},
        {1,"Wolfpack",0}
    };
#else
CP_iteminfo MusicItems={CTL_X,CTL_Y-20,9,0,32};
CP_itemtype MusicMenu[]=
    {
        {1,"Funky Colonel Bill",0},
        {1,"Death To The Nazis",0},
        {1,"Tiptoeing Around",0},
        {1,"Is This THE END?",0},
        {1,"Evil Incarnate",0},
        {1,"Jazzin' Them Nazis",0},
        {1,"Puttin' It To The Enemy",0},
        {1,"The SS Gonna Get You",0},
        {1,"Towering Above",0}
    };
#endif

#ifndef SPEARDEMO
void DoJukebox(void)
{
    int which,lastsong=-1;
    unsigned start;
    unsigned songs[]=
        {
#ifndef SPEAR
            GETTHEM_MUS,
            SEARCHN_MUS,
            POW_MUS,
            SUSPENSE_MUS,
            WARMARCH_MUS,
            CORNER_MUS,

            NAZI_OMI_MUS,
            PREGNANT_MUS,
            GOINGAFT_MUS,
            HEADACHE_MUS,
            DUNGEON_MUS,
            ULTIMATE_MUS,

            INTROCW3_MUS,
            NAZI_RAP_MUS,
            TWELFTH_MUS,
            ZEROHOUR_MUS,
            ULTIMATE_MUS,
            PACMAN_MUS
#else
            XFUNKIE_MUS,             // 0
            XDEATH_MUS,              // 2
            XTIPTOE_MUS,             // 4
            XTHEEND_MUS,             // 7
            XEVIL_MUS,               // 17
            XJAZNAZI_MUS,            // 18
            XPUTIT_MUS,              // 21
            XGETYOU_MUS,             // 22
            XTOWER2_MUS              // 23
#endif
        };

    IN_ClearKeysDown();
    if (!AdLibPresent && !SoundBlasterPresent)
        return;

    MenuFadeOut();

#ifndef SPEAR
#ifndef UPLOAD
    start = ((RETROPORT_GetTicks()/10)%3)*6;
#else
    start = 0;
#endif
#else
    start = 0;
#endif

#ifdef SPEAR
    CacheLump (BACKDROP_LUMP_START,BACKDROP_LUMP_END);
#else
//    CacheLump (CONTROLS_LUMP_START,CONTROLS_LUMP_END);
#endif
    fontnumber=1;
    ClearMScreen ();
    VWB_DrawPic(76,128,C_MOUSELBACKPIC);
    DrawStripes (10);
    SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);

#ifndef SPEAR
    DrawWindow (CTL_X-2,CTL_Y-6,210,13*7,BKGDCOLOR);
#else
    DrawWindow (CTL_X-2,CTL_Y-26,280,13*10,BKGDCOLOR);
#endif

    DrawMenu (&MusicItems,&MusicMenu[start]);

    SETFONTCOLOR (READHCOLOR,BKGDCOLOR);
    PrintY=15;
    WindowX = 0;
    WindowY = RETROPORT_SCREEN_WIDTH;
    US_CPrint ("Robert's Jukebox");

    SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);
    VW_UpdateScreen();
    MenuFadeIn();

    do
    {
        which = HandleMenu(&MusicItems,&MusicMenu[start],NULL);
        if (which>=0)
        {
            if (lastsong >= 0)
                MusicMenu[start+lastsong].active = 1;

            StartCPMusic(songs[start + which]);
            MusicMenu[start+which].active = 2;
            DrawMenu (&MusicItems,&MusicMenu[start]);
            VW_UpdateScreen();
            lastsong = which;
        }
    } while(which>=0);

    MenuFadeOut();
    IN_ClearKeysDown();
#ifdef SPEAR
    UnCacheLump (BACKDROP_LUMP_START,BACKDROP_LUMP_END);
#else
//    UnCacheLump (CONTROLS_LUMP_START,CONTROLS_LUMP_END);
#endif
}
#endif
#endif
/*
==========================
=
= InitGame
=
= Load a few things right away
=
==========================
*/

static void InitGame()
{
    bool didjukebox=false;

    RETROPORT_Init ();

    SignonScreen ();

	VW_UpdateScreen();

    VH_Startup ();
    IN_Startup ();
    PM_Startup ();
    SD_Startup ();
    CA_Startup ();
    US_Startup ();

    ReadConfig ();

//    SetupSaveGames();

//
// HOLDING DOWN 'M' KEY?
//
	IN_ProcessEvents();

    /*
 //   if (Keyboard[sc_M])
    {
        //DoJukebox();
        //didjukebox=true;
    }
    else
 * */
    {
        //
        // draw intro screen stuff
        //
        IntroScreen ();
    }
//
// load in and lock down some basic chunks

    // Tables are now precalculated and stored in ROM
    // BuildTables ();          // trig tables
    // SetupWalls ();

    NewViewSize (viewsize);

//
// initialize variables
//

    if(!didjukebox)
       FinishSignon();

#ifdef NOTYET
    vdisp = (byte *) (0xa0000+PAGE1START);
    vbuf = (byte *) (0xa0000+PAGE2START);
#endif
}

//===========================================================================

/*
==========================
=
= SetViewSize
=
==========================
*/

bool SetViewSize (unsigned width, unsigned height)
{
    viewwidth = width&~15;                  // must be divisable by 16
    viewheight = height&~1;                 // must be even
    centerx = viewwidth/2-1;
    shootdelta = viewwidth/10;
    if((unsigned) viewheight == screenHeight)
        viewscreenx = viewscreeny = screenofs = 0;
    else
    {
        viewscreenx = (screenWidth-viewwidth) / 2;
        viewscreeny = (screenHeight-STATUSLINES-viewheight)/2;
        screenofs = viewscreeny*screenWidth+viewscreenx;
    }

//
// calculate trace angles and projection constants
//
    CalcProjection (FOCALLENGTH);

    return true;
}


void ShowViewSize (int width)
{
    int oldwidth,oldheight;

    oldwidth = viewwidth;
    oldheight = viewheight;

    if(width == 21)
    {
        viewwidth = screenWidth;
        viewheight = screenHeight;
        VWB_Bar (0, 0, screenWidth, screenHeight, 0);
    }
    else if(width == 20)
    {
        viewwidth = screenWidth;
        viewheight = screenHeight - STATUSLINES;
        DrawPlayBorder ();
    }
    else
    {
        viewwidth = width*16*screenWidth/320;
        viewheight = (int) (width*16*HEIGHTRATIO*screenHeight/200);
        DrawPlayBorder ();
    }

    viewwidth = oldwidth;
    viewheight = oldheight;
}


void NewViewSize (int width)
{
    viewsize = width;
    if(viewsize == 21)
        SetViewSize(screenWidth, screenHeight);
    else if(viewsize == 20)
        SetViewSize(screenWidth, screenHeight - STATUSLINES);
    else
        SetViewSize(width*16*screenWidth/320, (unsigned) (width*16*HEIGHTRATIO*screenHeight/200));
}



//===========================================================================

/*
==========================
=
= Quit
=
==========================
*/

void Quit (const char *errorStr, ...)
{
#ifdef NOTYET
    byte *screen;
#endif
    char error[256];
    if(errorStr != NULL)
    {
        va_list vlist;
        va_start(vlist, errorStr);
        vsnprintf(error, sizeof(error), errorStr, vlist);
        va_end(vlist);
    }
    else error[0] = 0;

#if 0
    if (!pictable)  // don't try to display the red box before it's loaded
    {
        ShutdownId();
        if (error && *error)
        {
#ifdef NOTYET
            SetTextCursor(0,0);
#endif
            puts(error);
#ifdef NOTYET
            SetTextCursor(0,2);
#endif
            VW_WaitVBL(100);
        }
        exit(1);
    }

    if (!error || !*error)
    {
#ifdef NOTYET
        #ifndef JAPAN
        CA_CacheGrChunk (ORDERSCREEN);
        screen = grsegs[ORDERSCREEN];
        #endif
#endif
        WriteConfig ();
    }
#ifdef NOTYET
    else
    {
        CA_CacheGrChunk (ERRORSCREEN);
        screen = grsegs[ERRORSCREEN];
    }
#endif
#endif

    ShutdownId ();

#if 0
    if (error && *error)
    {
#ifdef NOTYET
        memcpy((byte *)0xb8000,screen+7,7*160);
        SetTextCursor(9,3);
#endif
        puts(error);
#ifdef NOTYET
        SetTextCursor(0,7);
#endif
        VW_WaitVBL(200);
        exit(1);
    }
    else
    if (!error || !(*error))
    {
#ifdef NOTYET
        #ifndef JAPAN
        memcpy((byte *)0xb8000,screen+7,24*160); // 24 for SPEAR/UPLOAD compatibility
        #endif
        SetTextCursor(0,23);
#endif
    }
#endif

    if (error[0])
    {
        LOG_Debug (NOBJ, error);
    }
}

//===========================================================================



/*
=====================
=
= DemoLoop
=
=====================
*/


static void DemoLoop()
{
    int LastDemo = 0;

//
// check for launch from ted
//
    if (param_tedlevel != -1)
    {
        param_nowait = true;
        EnableEndGameMenuItem();
        NewGame(param_difficulty,0);

#ifndef SPEAR
        gamestate.episode = param_tedlevel/10;
        gamestate.mapon = param_tedlevel%10;
#else
        gamestate.episode = 0;
        gamestate.mapon = param_tedlevel;
#endif
        GameLoop();
        Quit (NULL);
    }


//
// main game cycle
//

#ifndef DEMOTEST

    #ifndef UPLOAD

        #ifndef GOODTIMES
        #ifndef SPEAR
        #ifndef JAPAN
        if (!param_nowait)
            NonShareware();
        #endif
        #else
            #ifndef GOODTIMES
            #ifndef SPEARDEMO
            extern void CopyProtection(void);
            if(!param_goodtimes)
                CopyProtection();
            #endif
            #endif
        #endif
        #endif
    #endif

    StartCPMusic(INTROSONG);

#ifndef JAPAN
    if (!param_nowait)
        PG13 ();
#endif

#endif

    while (1)
    {
        while (!param_nowait)
        {
//
// title page
//
#ifndef DEMOTEST

#ifdef SPEAR
            SDL_Color pal[256];
            CA_CacheGrChunk (TITLEPALETTE);
            VL_ConvertPalette(grsegs[TITLEPALETTE], pal, 256);

            CA_CacheGrChunk (TITLE1PIC);
            VWB_DrawPic (0,0,TITLE1PIC);
            UNCACHEGRCHUNK (TITLE1PIC);

            CA_CacheGrChunk (TITLE2PIC);
            VWB_DrawPic (0,80,TITLE2PIC);
            UNCACHEGRCHUNK (TITLE2PIC);
            VW_UpdateScreen ();
            VL_FadeIn(0,255,pal,30);

            UNCACHEGRCHUNK (TITLEPALETTE);
#else
            // CA_CacheScreen (TITLEPIC);
            //VWB_DrawPic (0, 0, TITLEPIC);
            RETROPORT_FillFromStorage (RETROPORT_CACHED_ELEMENT_TITLEPIC);

            VW_UpdateScreen ();
            VW_FadeIn();
#endif
            if (IN_UserInput(TickBase*15))
                break;
            VW_FadeOut();
//
// credits page
//
            // CA_CacheScreen (CREDITSPIC);
            //VWB_DrawPic (0, 0, CREDITSPIC);
            RETROPORT_FillFromStorage (RETROPORT_CACHED_ELEMENT_CREDITS);

            VW_UpdateScreen();
            VW_FadeIn ();
            if (IN_UserInput(TickBase*10))
                break;
            VW_FadeOut ();
//
// high scores
//
            DrawHighScores ();
            VW_UpdateScreen ();
            VW_FadeIn ();

            if (IN_UserInput(TickBase*10))
                break;
#endif
//
// demo
//

            #ifndef SPEARDEMO
            PlayDemo (LastDemo++%4);
            #else
            PlayDemo (0);
            #endif

            if (playstate == ex_abort)
                break;
            VW_FadeOut();
            if(screenHeight % RETROPORT_SCREEN_HEIGHT != 0)
                VL_ClearScreen(0);
            StartCPMusic(INTROSONG);
        }

        VW_FadeOut ();

        US_ControlPanel (0);

        if (startgame || loadedgame)
        {
            GameLoop ();
            if(!param_nowait)
            {
                VW_FadeOut();
                StartCPMusic(INTROSONG);
            }
        }
    }
}


/*
==========================
=
= main
=
==========================
*/

int wl_main (int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    CheckForEpisodes();

    InitGame();

    DemoLoop();

    Quit("Demo loop exited???");
    return 1;
}
