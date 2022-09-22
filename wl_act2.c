// WL_ACT2.C

#include "wl_act2.h"
#include "wl_play.h"
#include "wl_draw.h"
#include "wl_game.h"
#include "wl_agent.h"
#include "wl_state.h"
#include "wl_main.h"
#include "wl_act1.h"
#include "wl_menu.h"
#include "id_us.h"
#include "id_ca.h"
#include "id_sd.h"
#include "id_vh.h"
#include "resources.h"
#include "foreign.h"
#include <stdlib.h>
#include <math.h>

/*
=============================================================================

                               LOCAL CONSTANTS

=============================================================================
*/

#define PROJECTILESIZE  0xc000l

#define BJRUNSPEED      2048
#define BJJUMPSPEED     680


/*
=============================================================================

                              GLOBAL VARIABLES

=============================================================================
*/



/*
=============================================================================

                              LOCAL VARIABLES

=============================================================================
*/

/*
static const dirtype dirtable[9] =
    {northwest,north,northeast,west,nodir,east,southwest,south,southeast};
*/

static const uint16_t starthitpoints[4][NUMENEMIES] =
//
// BABY MODE
//
{
    {
        25,   // guards
        50,   // officer
        100,  // SS
        1,    // dogs
        850,  // Hans
        850,  // Schabbs
        200,  // fake hitler
        800,  // mecha hitler
        45,   // mutants
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts

        850,  // Gretel
        850,  // Gift
        850,  // Fat
        5,    // en_spectre,
        1450, // en_angel,
        850,  // en_trans,
        1050, // en_uber,
        950,  // en_will,
        1250  // en_death
    },
    //
    // DON'T HURT ME MODE
    //
    {
        25,   // guards
        50,   // officer
        100,  // SS
        1,    // dogs
        950,  // Hans
        950,  // Schabbs
        300,  // fake hitler
        950,  // mecha hitler
        55,   // mutants
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts

        950,  // Gretel
        950,  // Gift
        950,  // Fat
        10,   // en_spectre,
        1550, // en_angel,
        950,  // en_trans,
        1150, // en_uber,
        1050, // en_will,
        1350  // en_death
    },
    //
    // BRING 'EM ON MODE
    //
    {
        25,   // guards
        50,   // officer
        100,  // SS
        1,    // dogs

        1050, // Hans
        1550, // Schabbs
        400,  // fake hitler
        1050, // mecha hitler

        55,   // mutants
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts

        1050, // Gretel
        1050, // Gift
        1050, // Fat
        15,   // en_spectre,
        1650, // en_angel,
        1050, // en_trans,
        1250, // en_uber,
        1150, // en_will,
        1450  // en_death
    },
    //
    // DEATH INCARNATE MODE
    //
    {
        25,   // guards
        50,   // officer
        100,  // SS
        1,    // dogs

        1200, // Hans
        2400, // Schabbs
        500,  // fake hitler
        1200, // mecha hitler

        65,   // mutants
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts
        25,   // ghosts

        1200, // Gretel
        1200, // Gift
        1200, // Fat
        25,   // en_spectre,
        2000, // en_angel,
        1200, // en_trans,
        1400, // en_uber,
        1300, // en_will,
        1600  // en_death
    }
};

void    A_StartDeathCam (objtype *ob);


void    T_Path (objtype *ob);
void    T_Shoot (objtype *ob);
void    T_Bite (objtype *ob);
void    T_DogChase (objtype *ob);
void    T_Chase (objtype *ob);
void    T_Projectile (objtype *ob);
void    T_Stand (objtype *ob);

void A_DeathScream (objtype *ob);

extern  const statetype s_rocket;
extern  const statetype s_smoke1;
extern  const statetype s_smoke2;
extern  const statetype s_smoke3;
extern  const statetype s_smoke4;
extern  const statetype s_boom2;
extern  const statetype s_boom3;

void A_Smoke (objtype *ob);

const statetype s_rocket              = {ro_yes,SPR_ROCKET_1,3,(statefunc)T_Projectile,(statefunc)A_Smoke,&s_rocket};
const statetype s_smoke1              = {ro_no,SPR_SMOKE_1,3,NULL,NULL,&s_smoke2};
const statetype s_smoke2              = {ro_no,SPR_SMOKE_2,3,NULL,NULL,&s_smoke3};
const statetype s_smoke3              = {ro_no,SPR_SMOKE_3,3,NULL,NULL,&s_smoke4};
const statetype s_smoke4              = {ro_no,SPR_SMOKE_4,3,NULL,NULL,NULL};

const statetype s_boom1               = {ro_no,SPR_BOOM_1,6,NULL,NULL,&s_boom2};
const statetype s_boom2               = {ro_no,SPR_BOOM_2,6,NULL,NULL,&s_boom3};
const statetype s_boom3               = {ro_no,SPR_BOOM_3,6,NULL,NULL,NULL};

#ifdef SPEAR

extern  statetype s_hrocket;
extern  statetype s_hsmoke1;
extern  statetype s_hsmoke2;
extern  statetype s_hsmoke3;
extern  statetype s_hsmoke4;
extern  statetype s_hboom2;
extern  statetype s_hboom3;

void A_Smoke (objtype *ob);

statetype s_hrocket             = {ro_yes,SPR_HROCKET_1,3,(statefunc)T_Projectile,(statefunc)A_Smoke,&s_hrocket};
statetype s_hsmoke1             = {ro_no,SPR_HSMOKE_1,3,NULL,NULL,&s_hsmoke2};
statetype s_hsmoke2             = {ro_no,SPR_HSMOKE_2,3,NULL,NULL,&s_hsmoke3};
statetype s_hsmoke3             = {ro_no,SPR_HSMOKE_3,3,NULL,NULL,&s_hsmoke4};
statetype s_hsmoke4             = {ro_no,SPR_HSMOKE_4,3,NULL,NULL,NULL};

statetype s_hboom1              = {ro_no,SPR_HBOOM_1,6,NULL,NULL,&s_hboom2};
statetype s_hboom2              = {ro_no,SPR_HBOOM_2,6,NULL,NULL,&s_hboom3};
statetype s_hboom3              = {ro_no,SPR_HBOOM_3,6,NULL,NULL,NULL};

#endif

void    T_Schabb (objtype *ob);
void    T_SchabbThrow (objtype *ob);
void    T_Fake (objtype *ob);
void    T_FakeFire (objtype *ob);
void    T_Ghosts (objtype *ob);

void    A_Slurpie (objtype *ob);
void    A_HitlerMorph (objtype *ob);
void    A_MechaSound (objtype *ob);

/*
=================
=
= A_Smoke
=
=================
*/

void A_Smoke (objtype *ob)
{
    GetNewActor ();
#ifdef SPEAR
    if (ob->obclass == hrocketobj)
        newobj->state = &s_hsmoke1;
    else
#endif
        newobj->state = &s_smoke1;
    newobj->ticcount = 6;

    newobj->tilex = ob->tilex;
    newobj->tiley = ob->tiley;
    newobj->x = ob->x;
    newobj->y = ob->y;
    newobj->obclass = inertobj;
    newobj->active = ac_yes;

    newobj->flags = FL_NEVERMARK;
}


/*
===================
=
= ProjectileTryMove
=
= returns true if move ok
===================
*/

#define PROJSIZE        0x2000

bool ProjectileTryMove (objtype *ob)
{
    int     xl,yl,xh,yh,x,y;
    uint8_t av;

    xl = (ob->x-PROJSIZE) >> TILESHIFT;
    yl = (ob->y-PROJSIZE) >> TILESHIFT;

    xh = (ob->x+PROJSIZE) >> TILESHIFT;
    yh = (ob->y+PROJSIZE) >> TILESHIFT;

    //
    // check for solid walls
    //
    for (y=yl;y<=yh;y++)
        for (x=xl;x<=xh;x++)
        {
            av = actorat[x][y];
            if (av && !ACTORAT_IS_OBJLIST_IDX(av))
                return false;
        }

    return true;
}



/*
=================
=
= T_Projectile
=
=================
*/

void T_Projectile (objtype *ob)
{
    int32_t deltax,deltay;
    int32_t speed;

    speed = (int32_t)ob->speed*tics;

    deltax = FixedMul(speed,costable[ob->angle]);
    deltay = -FixedMul(speed,sintable[ob->angle]);

    if (deltax>0x10000l)
        deltax = 0x10000l;
    if (deltay>0x10000l)
        deltay = 0x10000l;

    ob->x += deltax;
    ob->y += deltay;

    deltax = LABS(ob->x - player->x);
    deltay = LABS(ob->y - player->y);

    if (!ProjectileTryMove (ob))
    {
#ifndef APOGEE_1_0          // actually the whole method is never reached in shareware 1.0
        if (ob->obclass == rocketobj)
        {
            PlaySoundLocActor(MISSILEHITSND,ob);
            ob->state = &s_boom1;
        }
#ifdef SPEAR
        else if (ob->obclass == hrocketobj)
        {
            PlaySoundLocActor(MISSILEHITSND,ob);
            ob->state = &s_hboom1;
        }
#endif
        else
#endif
            ob->state = NULL;               // mark for removal

        return;
    }

    if (deltax < PROJECTILESIZE && deltay < PROJECTILESIZE)
    {
        int damage = 0;
        // hit the player
        switch (ob->obclass)
        {
            case needleobj:
                damage = (US_RndT() >>3) + 20;
                break;
            case rocketobj:
            case hrocketobj:
            case sparkobj:
                damage = (US_RndT() >>3) + 30;
                break;
            case fireobj:
                damage = (US_RndT() >>3);
                break;
            default:
                break;
        }

        TakeDamage (damage,ob);
        ob->state = NULL;               // mark for removal
        return;
    }

    ob->tilex = (short)(ob->x >> TILESHIFT);
    ob->tiley = (short)(ob->y >> TILESHIFT);
}


/*
=============================================================================

GUARD

=============================================================================
*/

//
// guards
//

extern  const statetype s_grdstand;

extern  const statetype s_grdpath1;
extern  const statetype s_grdpath1s;
extern  const statetype s_grdpath2;
extern  const statetype s_grdpath3;
extern  const statetype s_grdpath3s;
extern  const statetype s_grdpath4;

extern  const statetype s_grdpain;
extern  const statetype s_grdpain1;

extern  const statetype s_grdgiveup;

extern  const statetype s_grdshoot1;
extern  const statetype s_grdshoot2;
extern  const statetype s_grdshoot3;
extern  const statetype s_grdshoot4;

extern  const statetype s_grdchase1;
extern  const statetype s_grdchase1s;
extern  const statetype s_grdchase2;
extern  const statetype s_grdchase3;
extern  const statetype s_grdchase3s;
extern  const statetype s_grdchase4;

extern  const statetype s_grddie1;
extern  const statetype s_grddie1d;
extern  const statetype s_grddie2;
extern  const statetype s_grddie3;
extern  const statetype s_grddie4;

const statetype s_grdstand            = {ro_yes,SPR_GRD_S_1,0,(statefunc)T_Stand,NULL,&s_grdstand};

const statetype s_grdpath1            = {ro_yes,SPR_GRD_W1_1,20,(statefunc)T_Path,NULL,&s_grdpath1s};
const statetype s_grdpath1s           = {ro_yes,SPR_GRD_W1_1,5,NULL,NULL,&s_grdpath2};
const statetype s_grdpath2            = {ro_yes,SPR_GRD_W2_1,15,(statefunc)T_Path,NULL,&s_grdpath3};
const statetype s_grdpath3            = {ro_yes,SPR_GRD_W3_1,20,(statefunc)T_Path,NULL,&s_grdpath3s};
const statetype s_grdpath3s           = {ro_yes,SPR_GRD_W3_1,5,NULL,NULL,&s_grdpath4};
const statetype s_grdpath4            = {ro_yes,SPR_GRD_W4_1,15,(statefunc)T_Path,NULL,&s_grdpath1};

const statetype s_grdpain             = {ro_pain,SPR_GRD_PAIN_1,10,NULL,NULL,&s_grdchase1};
const statetype s_grdpain1            = {ro_pain,SPR_GRD_PAIN_2,10,NULL,NULL,&s_grdchase1};

const statetype s_grdshoot1           = {ro_no,SPR_GRD_SHOOT1,20,NULL,NULL,&s_grdshoot2};
const statetype s_grdshoot2           = {ro_no,SPR_GRD_SHOOT2,20,NULL,(statefunc)T_Shoot,&s_grdshoot3};
const statetype s_grdshoot3           = {ro_no,SPR_GRD_SHOOT3,20,NULL,NULL,&s_grdchase1};

const statetype s_grdchase1           = {ro_yes,SPR_GRD_W1_1,10,(statefunc)T_Chase,NULL,&s_grdchase1s};
const statetype s_grdchase1s          = {ro_yes,SPR_GRD_W1_1,3,NULL,NULL,&s_grdchase2};
const statetype s_grdchase2           = {ro_yes,SPR_GRD_W2_1,8,(statefunc)T_Chase,NULL,&s_grdchase3};
const statetype s_grdchase3           = {ro_yes,SPR_GRD_W3_1,10,(statefunc)T_Chase,NULL,&s_grdchase3s};
const statetype s_grdchase3s          = {ro_yes,SPR_GRD_W3_1,3,NULL,NULL,&s_grdchase4};
const statetype s_grdchase4           = {ro_yes,SPR_GRD_W4_1,8,(statefunc)T_Chase,NULL,&s_grdchase1};

const statetype s_grddie1             = {ro_no,SPR_GRD_DIE_1,15,NULL,(statefunc)A_DeathScream,&s_grddie2};
const statetype s_grddie2             = {ro_no,SPR_GRD_DIE_2,15,NULL,NULL,&s_grddie3};
const statetype s_grddie3             = {ro_no,SPR_GRD_DIE_3,15,NULL,NULL,&s_grddie4};
const statetype s_grddie4             = {ro_no,SPR_GRD_DEAD,0,NULL,NULL,&s_grddie4};


#ifndef SPEAR
//
// ghosts
//
extern  const statetype s_blinkychase1;
extern  const statetype s_blinkychase2;
extern  const statetype s_inkychase1;
extern  const statetype s_inkychase2;
extern  const statetype s_pinkychase1;
extern  const statetype s_pinkychase2;
extern  const statetype s_clydechase1;
extern  const statetype s_clydechase2;

const statetype s_blinkychase1        = {ro_no,SPR_BLINKY_W1,10,(statefunc)T_Ghosts,NULL,&s_blinkychase2};
const statetype s_blinkychase2        = {ro_no,SPR_BLINKY_W2,10,(statefunc)T_Ghosts,NULL,&s_blinkychase1};

const statetype s_inkychase1          = {ro_no,SPR_INKY_W1,10,(statefunc)T_Ghosts,NULL,&s_inkychase2};
const statetype s_inkychase2          = {ro_no,SPR_INKY_W2,10,(statefunc)T_Ghosts,NULL,&s_inkychase1};

const statetype s_pinkychase1         = {ro_no,SPR_PINKY_W1,10,(statefunc)T_Ghosts,NULL,&s_pinkychase2};
const statetype s_pinkychase2         = {ro_no,SPR_PINKY_W2,10,(statefunc)T_Ghosts,NULL,&s_pinkychase1};

const statetype s_clydechase1         = {ro_no,SPR_CLYDE_W1,10,(statefunc)T_Ghosts,NULL,&s_clydechase2};
const statetype s_clydechase2         = {ro_no,SPR_CLYDE_W2,10,(statefunc)T_Ghosts,NULL,&s_clydechase1};
#endif

//
// dogs
//

extern  const statetype s_dogpath1;
extern  const statetype s_dogpath1s;
extern  const statetype s_dogpath2;
extern  const statetype s_dogpath3;
extern  const statetype s_dogpath3s;
extern  const statetype s_dogpath4;

extern  const statetype s_dogjump1;
extern  const statetype s_dogjump2;
extern  const statetype s_dogjump3;
extern  const statetype s_dogjump4;
extern  const statetype s_dogjump5;

extern  const statetype s_dogchase1;
extern  const statetype s_dogchase1s;
extern  const statetype s_dogchase2;
extern  const statetype s_dogchase3;
extern  const statetype s_dogchase3s;
extern  const statetype s_dogchase4;

extern  const statetype s_dogdie1;
extern  const statetype s_dogdie1d;
extern  const statetype s_dogdie2;
extern  const statetype s_dogdie3;
extern  const statetype s_dogdead;

const statetype s_dogpath1            = {ro_yes,SPR_DOG_W1_1,20,(statefunc)T_Path,NULL,&s_dogpath1s};
const statetype s_dogpath1s           = {ro_yes,SPR_DOG_W1_1,5,NULL,NULL,&s_dogpath2};
const statetype s_dogpath2            = {ro_yes,SPR_DOG_W2_1,15,(statefunc)T_Path,NULL,&s_dogpath3};
const statetype s_dogpath3            = {ro_yes,SPR_DOG_W3_1,20,(statefunc)T_Path,NULL,&s_dogpath3s};
const statetype s_dogpath3s           = {ro_yes,SPR_DOG_W3_1,5,NULL,NULL,&s_dogpath4};
const statetype s_dogpath4            = {ro_yes,SPR_DOG_W4_1,15,(statefunc)T_Path,NULL,&s_dogpath1};

const statetype s_dogjump1            = {ro_no,SPR_DOG_JUMP1,10,NULL,NULL,&s_dogjump2};
const statetype s_dogjump2            = {ro_no,SPR_DOG_JUMP2,10,NULL,(statefunc)T_Bite,&s_dogjump3};
const statetype s_dogjump3            = {ro_no,SPR_DOG_JUMP3,10,NULL,NULL,&s_dogjump4};
const statetype s_dogjump4            = {ro_no,SPR_DOG_JUMP1,10,NULL,NULL,&s_dogjump5};
const statetype s_dogjump5            = {ro_no,SPR_DOG_W1_1,10,NULL,NULL,&s_dogchase1};

const statetype s_dogchase1           = {ro_yes,SPR_DOG_W1_1,10,(statefunc)T_DogChase,NULL,&s_dogchase1s};
const statetype s_dogchase1s          = {ro_yes,SPR_DOG_W1_1,3,NULL,NULL,&s_dogchase2};
const statetype s_dogchase2           = {ro_yes,SPR_DOG_W2_1,8,(statefunc)T_DogChase,NULL,&s_dogchase3};
const statetype s_dogchase3           = {ro_yes,SPR_DOG_W3_1,10,(statefunc)T_DogChase,NULL,&s_dogchase3s};
const statetype s_dogchase3s          = {ro_yes,SPR_DOG_W3_1,3,NULL,NULL,&s_dogchase4};
const statetype s_dogchase4           = {ro_yes,SPR_DOG_W4_1,8,(statefunc)T_DogChase,NULL,&s_dogchase1};

const statetype s_dogdie1             = {ro_no,SPR_DOG_DIE_1,15,NULL,(statefunc)A_DeathScream,&s_dogdie2};
const statetype s_dogdie2             = {ro_no,SPR_DOG_DIE_2,15,NULL,NULL,&s_dogdie3};
const statetype s_dogdie3             = {ro_no,SPR_DOG_DIE_3,15,NULL,NULL,&s_dogdead};
const statetype s_dogdead             = {ro_no,SPR_DOG_DEAD,15,NULL,NULL,&s_dogdead};


//
// officers
//

extern  const statetype s_ofcstand;

extern  const statetype s_ofcpath1;
extern  const statetype s_ofcpath1s;
extern  const statetype s_ofcpath2;
extern  const statetype s_ofcpath3;
extern  const statetype s_ofcpath3s;
extern  const statetype s_ofcpath4;

extern  const statetype s_ofcpain;
extern  const statetype s_ofcpain1;

extern  const statetype s_ofcgiveup;

extern  const statetype s_ofcshoot1;
extern  const statetype s_ofcshoot2;
extern  const statetype s_ofcshoot3;
extern  const statetype s_ofcshoot4;

extern  const statetype s_ofcchase1;
extern  const statetype s_ofcchase1s;
extern  const statetype s_ofcchase2;
extern  const statetype s_ofcchase3;
extern  const statetype s_ofcchase3s;
extern  const statetype s_ofcchase4;

extern  const statetype s_ofcdie1;
extern  const statetype s_ofcdie2;
extern  const statetype s_ofcdie3;
extern  const statetype s_ofcdie4;
extern  const statetype s_ofcdie5;

const statetype s_ofcstand            = {ro_yes,SPR_OFC_S_1,0,(statefunc)T_Stand,NULL,&s_ofcstand};

const statetype s_ofcpath1            = {ro_yes,SPR_OFC_W1_1,20,(statefunc)T_Path,NULL,&s_ofcpath1s};
const statetype s_ofcpath1s           = {ro_yes,SPR_OFC_W1_1,5,NULL,NULL,&s_ofcpath2};
const statetype s_ofcpath2            = {ro_yes,SPR_OFC_W2_1,15,(statefunc)T_Path,NULL,&s_ofcpath3};
const statetype s_ofcpath3            = {ro_yes,SPR_OFC_W3_1,20,(statefunc)T_Path,NULL,&s_ofcpath3s};
const statetype s_ofcpath3s           = {ro_yes,SPR_OFC_W3_1,5,NULL,NULL,&s_ofcpath4};
const statetype s_ofcpath4            = {ro_yes,SPR_OFC_W4_1,15,(statefunc)T_Path,NULL,&s_ofcpath1};

const statetype s_ofcpain             = {ro_pain,SPR_OFC_PAIN_1,10,NULL,NULL,&s_ofcchase1};
const statetype s_ofcpain1            = {ro_pain,SPR_OFC_PAIN_2,10,NULL,NULL,&s_ofcchase1};

const statetype s_ofcshoot1           = {ro_no,SPR_OFC_SHOOT1,6,NULL,NULL,&s_ofcshoot2};
const statetype s_ofcshoot2           = {ro_no,SPR_OFC_SHOOT2,20,NULL,(statefunc)T_Shoot,&s_ofcshoot3};
const statetype s_ofcshoot3           = {ro_no,SPR_OFC_SHOOT3,10,NULL,NULL,&s_ofcchase1};

const statetype s_ofcchase1           = {ro_yes,SPR_OFC_W1_1,10,(statefunc)T_Chase,NULL,&s_ofcchase1s};
const statetype s_ofcchase1s          = {ro_yes,SPR_OFC_W1_1,3,NULL,NULL,&s_ofcchase2};
const statetype s_ofcchase2           = {ro_yes,SPR_OFC_W2_1,8,(statefunc)T_Chase,NULL,&s_ofcchase3};
const statetype s_ofcchase3           = {ro_yes,SPR_OFC_W3_1,10,(statefunc)T_Chase,NULL,&s_ofcchase3s};
const statetype s_ofcchase3s          = {ro_yes,SPR_OFC_W3_1,3,NULL,NULL,&s_ofcchase4};
const statetype s_ofcchase4           = {ro_yes,SPR_OFC_W4_1,8,(statefunc)T_Chase,NULL,&s_ofcchase1};

const statetype s_ofcdie1             = {ro_no,SPR_OFC_DIE_1,11,NULL,(statefunc)A_DeathScream,&s_ofcdie2};
const statetype s_ofcdie2             = {ro_no,SPR_OFC_DIE_2,11,NULL,NULL,&s_ofcdie3};
const statetype s_ofcdie3             = {ro_no,SPR_OFC_DIE_3,11,NULL,NULL,&s_ofcdie4};
const statetype s_ofcdie4             = {ro_no,SPR_OFC_DIE_4,11,NULL,NULL,&s_ofcdie5};
const statetype s_ofcdie5             = {ro_no,SPR_OFC_DEAD,0,NULL,NULL,&s_ofcdie5};


//
// mutant
//

extern  const statetype s_mutstand;

extern  const statetype s_mutpath1;
extern  const statetype s_mutpath1s;
extern  const statetype s_mutpath2;
extern  const statetype s_mutpath3;
extern  const statetype s_mutpath3s;
extern  const statetype s_mutpath4;

extern  const statetype s_mutpain;
extern  const statetype s_mutpain1;

extern  const statetype s_mutgiveup;

extern  const statetype s_mutshoot1;
extern  const statetype s_mutshoot2;
extern  const statetype s_mutshoot3;
extern  const statetype s_mutshoot4;

extern  const statetype s_mutchase1;
extern  const statetype s_mutchase1s;
extern  const statetype s_mutchase2;
extern  const statetype s_mutchase3;
extern  const statetype s_mutchase3s;
extern  const statetype s_mutchase4;

extern  const statetype s_mutdie1;
extern  const statetype s_mutdie2;
extern  const statetype s_mutdie3;
extern  const statetype s_mutdie4;
extern  const statetype s_mutdie5;

const statetype s_mutstand            = {ro_yes,SPR_MUT_S_1,0,(statefunc)T_Stand,NULL,&s_mutstand};

const statetype s_mutpath1            = {ro_yes,SPR_MUT_W1_1,20,(statefunc)T_Path,NULL,&s_mutpath1s};
const statetype s_mutpath1s           = {ro_yes,SPR_MUT_W1_1,5,NULL,NULL,&s_mutpath2};
const statetype s_mutpath2            = {ro_yes,SPR_MUT_W2_1,15,(statefunc)T_Path,NULL,&s_mutpath3};
const statetype s_mutpath3            = {ro_yes,SPR_MUT_W3_1,20,(statefunc)T_Path,NULL,&s_mutpath3s};
const statetype s_mutpath3s           = {ro_yes,SPR_MUT_W3_1,5,NULL,NULL,&s_mutpath4};
const statetype s_mutpath4            = {ro_yes,SPR_MUT_W4_1,15,(statefunc)T_Path,NULL,&s_mutpath1};

const statetype s_mutpain             = {ro_pain,SPR_MUT_PAIN_1,10,NULL,NULL,&s_mutchase1};
const statetype s_mutpain1            = {ro_pain,SPR_MUT_PAIN_2,10,NULL,NULL,&s_mutchase1};

const statetype s_mutshoot1           = {ro_no,SPR_MUT_SHOOT1,6,NULL,(statefunc)T_Shoot,&s_mutshoot2};
const statetype s_mutshoot2           = {ro_no,SPR_MUT_SHOOT2,20,NULL,NULL,&s_mutshoot3};
const statetype s_mutshoot3           = {ro_no,SPR_MUT_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_mutshoot4};
const statetype s_mutshoot4           = {ro_no,SPR_MUT_SHOOT4,20,NULL,NULL,&s_mutchase1};

const statetype s_mutchase1           = {ro_yes,SPR_MUT_W1_1,10,(statefunc)T_Chase,NULL,&s_mutchase1s};
const statetype s_mutchase1s          = {ro_yes,SPR_MUT_W1_1,3,NULL,NULL,&s_mutchase2};
const statetype s_mutchase2           = {ro_yes,SPR_MUT_W2_1,8,(statefunc)T_Chase,NULL,&s_mutchase3};
const statetype s_mutchase3           = {ro_yes,SPR_MUT_W3_1,10,(statefunc)T_Chase,NULL,&s_mutchase3s};
const statetype s_mutchase3s          = {ro_yes,SPR_MUT_W3_1,3,NULL,NULL,&s_mutchase4};
const statetype s_mutchase4           = {ro_yes,SPR_MUT_W4_1,8,(statefunc)T_Chase,NULL,&s_mutchase1};

const statetype s_mutdie1             = {ro_no,SPR_MUT_DIE_1,7,NULL,(statefunc)A_DeathScream,&s_mutdie2};
const statetype s_mutdie2             = {ro_no,SPR_MUT_DIE_2,7,NULL,NULL,&s_mutdie3};
const statetype s_mutdie3             = {ro_no,SPR_MUT_DIE_3,7,NULL,NULL,&s_mutdie4};
const statetype s_mutdie4             = {ro_no,SPR_MUT_DIE_4,7,NULL,NULL,&s_mutdie5};
const statetype s_mutdie5             = {ro_no,SPR_MUT_DEAD,0,NULL,NULL,&s_mutdie5};


//
// SS
//

extern  const statetype s_ssstand;

extern  const statetype s_sspath1;
extern  const statetype s_sspath1s;
extern  const statetype s_sspath2;
extern  const statetype s_sspath3;
extern  const statetype s_sspath3s;
extern  const statetype s_sspath4;

extern  const statetype s_sspain;
extern  const statetype s_sspain1;

extern  const statetype s_ssshoot1;
extern  const statetype s_ssshoot2;
extern  const statetype s_ssshoot3;
extern  const statetype s_ssshoot4;
extern  const statetype s_ssshoot5;
extern  const statetype s_ssshoot6;
extern  const statetype s_ssshoot7;
extern  const statetype s_ssshoot8;
extern  const statetype s_ssshoot9;

extern  const statetype s_sschase1;
extern  const statetype s_sschase1s;
extern  const statetype s_sschase2;
extern  const statetype s_sschase3;
extern  const statetype s_sschase3s;
extern  const statetype s_sschase4;

extern  const statetype s_ssdie1;
extern  const statetype s_ssdie2;
extern  const statetype s_ssdie3;
extern  const statetype s_ssdie4;

const statetype s_ssstand             = {ro_yes,SPR_SS_S_1,0,(statefunc)T_Stand,NULL,&s_ssstand};

const statetype s_sspath1             = {ro_yes,SPR_SS_W1_1,20,(statefunc)T_Path,NULL,&s_sspath1s};
const statetype s_sspath1s            = {ro_yes,SPR_SS_W1_1,5,NULL,NULL,&s_sspath2};
const statetype s_sspath2             = {ro_yes,SPR_SS_W2_1,15,(statefunc)T_Path,NULL,&s_sspath3};
const statetype s_sspath3             = {ro_yes,SPR_SS_W3_1,20,(statefunc)T_Path,NULL,&s_sspath3s};
const statetype s_sspath3s            = {ro_yes,SPR_SS_W3_1,5,NULL,NULL,&s_sspath4};
const statetype s_sspath4             = {ro_yes,SPR_SS_W4_1,15,(statefunc)T_Path,NULL,&s_sspath1};

const statetype s_sspain              = {ro_pain,SPR_SS_PAIN_1,10,NULL,NULL,&s_sschase1};
const statetype s_sspain1             = {ro_pain,SPR_SS_PAIN_2,10,NULL,NULL,&s_sschase1};

const statetype s_ssshoot1            = {ro_no,SPR_SS_SHOOT1,20,NULL,NULL,&s_ssshoot2};
const statetype s_ssshoot2            = {ro_no,SPR_SS_SHOOT2,20,NULL,(statefunc)T_Shoot,&s_ssshoot3};
const statetype s_ssshoot3            = {ro_no,SPR_SS_SHOOT3,10,NULL,NULL,&s_ssshoot4};
const statetype s_ssshoot4            = {ro_no,SPR_SS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_ssshoot5};
const statetype s_ssshoot5            = {ro_no,SPR_SS_SHOOT3,10,NULL,NULL,&s_ssshoot6};
const statetype s_ssshoot6            = {ro_no,SPR_SS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_ssshoot7};
const statetype s_ssshoot7            = {ro_no,SPR_SS_SHOOT3,10,NULL,NULL,&s_ssshoot8};
const statetype s_ssshoot8            = {ro_no,SPR_SS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_ssshoot9};
const statetype s_ssshoot9            = {ro_no,SPR_SS_SHOOT3,10,NULL,NULL,&s_sschase1};

const statetype s_sschase1            = {ro_yes,SPR_SS_W1_1,10,(statefunc)T_Chase,NULL,&s_sschase1s};
const statetype s_sschase1s           = {ro_yes,SPR_SS_W1_1,3,NULL,NULL,&s_sschase2};
const statetype s_sschase2            = {ro_yes,SPR_SS_W2_1,8,(statefunc)T_Chase,NULL,&s_sschase3};
const statetype s_sschase3            = {ro_yes,SPR_SS_W3_1,10,(statefunc)T_Chase,NULL,&s_sschase3s};
const statetype s_sschase3s           = {ro_yes,SPR_SS_W3_1,3,NULL,NULL,&s_sschase4};
const statetype s_sschase4            = {ro_yes,SPR_SS_W4_1,8,(statefunc)T_Chase,NULL,&s_sschase1};

const statetype s_ssdie1              = {ro_no,SPR_SS_DIE_1,15,NULL,(statefunc)A_DeathScream,&s_ssdie2};
const statetype s_ssdie2              = {ro_no,SPR_SS_DIE_2,15,NULL,NULL,&s_ssdie3};
const statetype s_ssdie3              = {ro_no,SPR_SS_DIE_3,15,NULL,NULL,&s_ssdie4};
const statetype s_ssdie4              = {ro_no,SPR_SS_DEAD,0,NULL,NULL,&s_ssdie4};


#ifndef SPEAR
//
// hans
//
extern  const statetype s_bossstand;

extern  const statetype s_bosschase1;
extern  const statetype s_bosschase1s;
extern  const statetype s_bosschase2;
extern  const statetype s_bosschase3;
extern  const statetype s_bosschase3s;
extern  const statetype s_bosschase4;

extern  const statetype s_bossdie1;
extern  const statetype s_bossdie2;
extern  const statetype s_bossdie3;
extern  const statetype s_bossdie4;

extern  const statetype s_bossshoot1;
extern  const statetype s_bossshoot2;
extern  const statetype s_bossshoot3;
extern  const statetype s_bossshoot4;
extern  const statetype s_bossshoot5;
extern  const statetype s_bossshoot6;
extern  const statetype s_bossshoot7;
extern  const statetype s_bossshoot8;


const statetype s_bossstand           = {ro_no,SPR_BOSS_W1,0,(statefunc)T_Stand,NULL,&s_bossstand};

const statetype s_bosschase1          = {ro_no,SPR_BOSS_W1,10,(statefunc)T_Chase,NULL,&s_bosschase1s};
const statetype s_bosschase1s         = {ro_no,SPR_BOSS_W1,3,NULL,NULL,&s_bosschase2};
const statetype s_bosschase2          = {ro_no,SPR_BOSS_W2,8,(statefunc)T_Chase,NULL,&s_bosschase3};
const statetype s_bosschase3          = {ro_no,SPR_BOSS_W3,10,(statefunc)T_Chase,NULL,&s_bosschase3s};
const statetype s_bosschase3s         = {ro_no,SPR_BOSS_W3,3,NULL,NULL,&s_bosschase4};
const statetype s_bosschase4          = {ro_no,SPR_BOSS_W4,8,(statefunc)T_Chase,NULL,&s_bosschase1};

const statetype s_bossdie1            = {ro_no,SPR_BOSS_DIE1,15,NULL,(statefunc)A_DeathScream,&s_bossdie2};
const statetype s_bossdie2            = {ro_no,SPR_BOSS_DIE2,15,NULL,NULL,&s_bossdie3};
const statetype s_bossdie3            = {ro_no,SPR_BOSS_DIE3,15,NULL,NULL,&s_bossdie4};
const statetype s_bossdie4            = {ro_no,SPR_BOSS_DEAD,0,NULL,NULL,&s_bossdie4};

const statetype s_bossshoot1          = {ro_no,SPR_BOSS_SHOOT1,30,NULL,NULL,&s_bossshoot2};
const statetype s_bossshoot2          = {ro_no,SPR_BOSS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_bossshoot3};
const statetype s_bossshoot3          = {ro_no,SPR_BOSS_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_bossshoot4};
const statetype s_bossshoot4          = {ro_no,SPR_BOSS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_bossshoot5};
const statetype s_bossshoot5          = {ro_no,SPR_BOSS_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_bossshoot6};
const statetype s_bossshoot6          = {ro_no,SPR_BOSS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_bossshoot7};
const statetype s_bossshoot7          = {ro_no,SPR_BOSS_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_bossshoot8};
const statetype s_bossshoot8          = {ro_no,SPR_BOSS_SHOOT1,10,NULL,NULL,&s_bosschase1};


//
// gretel
//
extern  const statetype s_gretelstand;

extern  const statetype s_gretelchase1;
extern  const statetype s_gretelchase1s;
extern  const statetype s_gretelchase2;
extern  const statetype s_gretelchase3;
extern  const statetype s_gretelchase3s;
extern  const statetype s_gretelchase4;

extern  const statetype s_greteldie1;
extern  const statetype s_greteldie2;
extern  const statetype s_greteldie3;
extern  const statetype s_greteldie4;

extern  const statetype s_gretelshoot1;
extern  const statetype s_gretelshoot2;
extern  const statetype s_gretelshoot3;
extern  const statetype s_gretelshoot4;
extern  const statetype s_gretelshoot5;
extern  const statetype s_gretelshoot6;
extern  const statetype s_gretelshoot7;
extern  const statetype s_gretelshoot8;


const statetype s_gretelstand         = {ro_no,SPR_GRETEL_W1,0,(statefunc)T_Stand,NULL,&s_gretelstand};

const statetype s_gretelchase1        = {ro_no,SPR_GRETEL_W1,10,(statefunc)T_Chase,NULL,&s_gretelchase1s};
const statetype s_gretelchase1s       = {ro_no,SPR_GRETEL_W1,3,NULL,NULL,&s_gretelchase2};
const statetype s_gretelchase2        = {ro_no,SPR_GRETEL_W2,8,(statefunc)T_Chase,NULL,&s_gretelchase3};
const statetype s_gretelchase3        = {ro_no,SPR_GRETEL_W3,10,(statefunc)T_Chase,NULL,&s_gretelchase3s};
const statetype s_gretelchase3s       = {ro_no,SPR_GRETEL_W3,3,NULL,NULL,&s_gretelchase4};
const statetype s_gretelchase4        = {ro_no,SPR_GRETEL_W4,8,(statefunc)T_Chase,NULL,&s_gretelchase1};

const statetype s_greteldie1          = {ro_no,SPR_GRETEL_DIE1,15,NULL,(statefunc)A_DeathScream,&s_greteldie2};
const statetype s_greteldie2          = {ro_no,SPR_GRETEL_DIE2,15,NULL,NULL,&s_greteldie3};
const statetype s_greteldie3          = {ro_no,SPR_GRETEL_DIE3,15,NULL,NULL,&s_greteldie4};
const statetype s_greteldie4          = {ro_no,SPR_GRETEL_DEAD,0,NULL,NULL,&s_greteldie4};

const statetype s_gretelshoot1        = {ro_no,SPR_GRETEL_SHOOT1,30,NULL,NULL,&s_gretelshoot2};
const statetype s_gretelshoot2        = {ro_no,SPR_GRETEL_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_gretelshoot3};
const statetype s_gretelshoot3        = {ro_no,SPR_GRETEL_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_gretelshoot4};
const statetype s_gretelshoot4        = {ro_no,SPR_GRETEL_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_gretelshoot5};
const statetype s_gretelshoot5        = {ro_no,SPR_GRETEL_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_gretelshoot6};
const statetype s_gretelshoot6        = {ro_no,SPR_GRETEL_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_gretelshoot7};
const statetype s_gretelshoot7        = {ro_no,SPR_GRETEL_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_gretelshoot8};
const statetype s_gretelshoot8        = {ro_no,SPR_GRETEL_SHOOT1,10,NULL,NULL,&s_gretelchase1};
#endif


/*
===============
=
= SpawnStand
=
===============
*/

void SpawnStand (enemy_t which, int tilex, int tiley, int dir)
{
    // sgermino: mapsegs is now readonly.
    const uint16_t *map;
    uint16_t tile;

    switch (which)
    {
        case en_guard:
            SpawnNewObj (tilex,tiley,&s_grdstand);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        case en_officer:
            SpawnNewObj (tilex,tiley,&s_ofcstand);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        case en_mutant:
            SpawnNewObj (tilex,tiley,&s_mutstand);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        case en_ss:
            SpawnNewObj (tilex,tiley,&s_ssstand);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        default:
            break;
    }

    map = mapsegs[0]+(tiley<<mapshift)+tilex;
    tile = *map;
    if (tile == AMBUSHTILE)
    {
        tilemap[tilex][tiley] = 0;

        if (*(map+1) >= AREATILE)
            tile = *(map+1);
        if (*(map-mapwidth) >= AREATILE)
            tile = *(map-mapwidth);
        if (*(map+mapwidth) >= AREATILE)
            tile = *(map+mapwidth);
        if ( *(map-1) >= AREATILE)
            tile = *(map-1);

        // sgermino: mapsegs is now readonly.
        // *map = tile;
        newobj->areanumber = tile-AREATILE;

        newobj->flags |= FL_AMBUSH;
    }

    newobj->obclass = (classtype)(guardobj + which);
    newobj->hitpoints = starthitpoints[gamestate.difficulty][which];
    newobj->dir = (dirtype)(dir * 2);
    newobj->flags |= FL_SHOOTABLE;
}



/*
===============
=
= SpawnDeadGuard
=
===============
*/

void SpawnDeadGuard (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_grddie4);
    DEMOIF_SDL
    {
        newobj->flags |= FL_NONMARK;    // walk through moving enemy fix
    }
    newobj->obclass = inertobj;
}



#ifndef SPEAR
/*
===============
=
= SpawnBoss
=
===============
*/

void SpawnBoss (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_bossstand);
    newobj->speed = SPDPATROL;

    newobj->obclass = bossobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_boss];
    newobj->dir = nodir;
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}

/*
===============
=
= SpawnGretel
=
===============
*/

void SpawnGretel (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_gretelstand);
    newobj->speed = SPDPATROL;

    newobj->obclass = gretelobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_gretel];
    newobj->dir = nodir;
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}
#endif

/*
===============
=
= SpawnPatrol
=
===============
*/

void SpawnPatrol (enemy_t which, int tilex, int tiley, int dir)
{
    switch (which)
    {
        case en_guard:
            SpawnNewObj (tilex,tiley,&s_grdpath1);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        case en_officer:
            SpawnNewObj (tilex,tiley,&s_ofcpath1);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        case en_ss:
            SpawnNewObj (tilex,tiley,&s_sspath1);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        case en_mutant:
            SpawnNewObj (tilex,tiley,&s_mutpath1);
            newobj->speed = SPDPATROL;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        case en_dog:
            SpawnNewObj (tilex,tiley,&s_dogpath1);
            newobj->speed = SPDDOG;
            if (!loadedgame)
                gamestate.killtotal++;
            break;

        default:
            break;
    }

    newobj->obclass = (classtype)(guardobj+which);
    newobj->dir = (dirtype)(dir*2);
    newobj->hitpoints = starthitpoints[gamestate.difficulty][which];
    newobj->distance = TILEGLOBAL;
    newobj->flags |= FL_SHOOTABLE;
    newobj->active = ac_yes;

    actorat[newobj->tilex][newobj->tiley] = ACTORAT_EMPTY;      // don't use original spot

    switch (dir)
    {
        case 0:
            newobj->tilex++;
            break;
        case 1:
            newobj->tiley--;
            break;
        case 2:
            newobj->tilex--;
            break;
        case 3:
            newobj->tiley++;
            break;
    }

    actorat[newobj->tilex][newobj->tiley] = ACTORAT_OBJLIST_IDX(
                                                OBJLIST_GET_PTR2IDX(newobj));
}



/*
==================
=
= A_DeathScream
=
==================
*/

void A_DeathScream (objtype *ob)
{
#ifndef UPLOAD
#ifndef SPEAR
    if (mapon==9 && !US_RndT())
#else
    if ((mapon==18 || mapon==19) && !US_RndT())
#endif
    {
        switch(ob->obclass)
        {
            case mutantobj:
            case guardobj:
            case officerobj:
            case ssobj:
            case dogobj:
                PlaySoundLocActor(DEATHSCREAM6SND,ob);
                return;

            default:
                break;
        }
    }
#endif

    switch (ob->obclass)
    {
        case mutantobj:
            PlaySoundLocActor(AHHHGSND,ob);
            break;

        case guardobj:
        {
            int sounds[9]={ DEATHSCREAM1SND,
                DEATHSCREAM2SND,
                DEATHSCREAM3SND,
#ifndef APOGEE_1_0
                DEATHSCREAM4SND,
                DEATHSCREAM5SND,
                DEATHSCREAM7SND,
                DEATHSCREAM8SND,
                DEATHSCREAM9SND
#endif
            };

#ifndef UPLOAD
            PlaySoundLocActor(sounds[US_RndT()%8],ob);
#else
            PlaySoundLocActor(sounds[US_RndT()%2],ob);
#endif
            break;
        }
        case officerobj:
            PlaySoundLocActor(NEINSOVASSND,ob);
            break;
        case ssobj:
            PlaySoundLocActor(LEBENSND,ob); // JAB
            break;
        case dogobj:
            PlaySoundLocActor(DOGDEATHSND,ob);      // JAB
            break;
#ifndef SPEAR
        case bossobj:
            SD_PlaySound(MUTTISND);                         // JAB
            break;
        case schabbobj:
            SD_PlaySound(MEINGOTTSND);
            break;
        case fakeobj:
            SD_PlaySound(HITLERHASND);
            break;
        case mechahitlerobj:
            SD_PlaySound(SCHEISTSND);
            break;
        case realhitlerobj:
            SD_PlaySound(EVASND);
            break;
#ifndef APOGEE_1_0
        case gretelobj:
            SD_PlaySound(MEINSND);
            break;
        case giftobj:
            SD_PlaySound(DONNERSND);
            break;
        case fatobj:
            SD_PlaySound(ROSESND);
            break;
#endif
#else
        case spectreobj:
            SD_PlaySound(GHOSTFADESND);
            break;
        case angelobj:
            SD_PlaySound(ANGELDEATHSND);
            break;
        case transobj:
            SD_PlaySound(TRANSDEATHSND);
            break;
        case uberobj:
            SD_PlaySound(UBERDEATHSND);
            break;
        case willobj:
            SD_PlaySound(WILHELMDEATHSND);
            break;
        case deathobj:
            SD_PlaySound(KNIGHTDEATHSND);
            break;
#endif
        default:
            break;
    }
}


/*
=============================================================================

                                SPEAR ACTORS

=============================================================================
*/

#ifdef SPEAR

void T_Launch (objtype *ob);
void T_Will (objtype *ob);

extern  statetype s_angelshoot1;
extern  statetype s_deathshoot1;
extern  statetype s_spark1;

//
// trans
//
extern  statetype s_transstand;

extern  statetype s_transchase1;
extern  statetype s_transchase1s;
extern  statetype s_transchase2;
extern  statetype s_transchase3;
extern  statetype s_transchase3s;
extern  statetype s_transchase4;

extern  statetype s_transdie0;
extern  statetype s_transdie01;
extern  statetype s_transdie1;
extern  statetype s_transdie2;
extern  statetype s_transdie3;
extern  statetype s_transdie4;

extern  statetype s_transshoot1;
extern  statetype s_transshoot2;
extern  statetype s_transshoot3;
extern  statetype s_transshoot4;
extern  statetype s_transshoot5;
extern  statetype s_transshoot6;
extern  statetype s_transshoot7;
extern  statetype s_transshoot8;


statetype s_transstand          = {ro_no,SPR_TRANS_W1,0,(statefunc)T_Stand,NULL,&s_transstand};

statetype s_transchase1         = {ro_no,SPR_TRANS_W1,10,(statefunc)T_Chase,NULL,&s_transchase1s};
statetype s_transchase1s        = {ro_no,SPR_TRANS_W1,3,NULL,NULL,&s_transchase2};
statetype s_transchase2         = {ro_no,SPR_TRANS_W2,8,(statefunc)T_Chase,NULL,&s_transchase3};
statetype s_transchase3         = {ro_no,SPR_TRANS_W3,10,(statefunc)T_Chase,NULL,&s_transchase3s};
statetype s_transchase3s        = {ro_no,SPR_TRANS_W3,3,NULL,NULL,&s_transchase4};
statetype s_transchase4         = {ro_no,SPR_TRANS_W4,8,(statefunc)T_Chase,NULL,&s_transchase1};

statetype s_transdie0           = {ro_no,SPR_TRANS_W1,1,NULL,(statefunc)A_DeathScream,&s_transdie01};
statetype s_transdie01          = {ro_no,SPR_TRANS_W1,1,NULL,NULL,&s_transdie1};
statetype s_transdie1           = {ro_no,SPR_TRANS_DIE1,15,NULL,NULL,&s_transdie2};
statetype s_transdie2           = {ro_no,SPR_TRANS_DIE2,15,NULL,NULL,&s_transdie3};
statetype s_transdie3           = {ro_no,SPR_TRANS_DIE3,15,NULL,NULL,&s_transdie4};
statetype s_transdie4           = {ro_no,SPR_TRANS_DEAD,0,NULL,NULL,&s_transdie4};

statetype s_transshoot1         = {ro_no,SPR_TRANS_SHOOT1,30,NULL,NULL,&s_transshoot2};
statetype s_transshoot2         = {ro_no,SPR_TRANS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_transshoot3};
statetype s_transshoot3         = {ro_no,SPR_TRANS_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_transshoot4};
statetype s_transshoot4         = {ro_no,SPR_TRANS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_transshoot5};
statetype s_transshoot5         = {ro_no,SPR_TRANS_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_transshoot6};
statetype s_transshoot6         = {ro_no,SPR_TRANS_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_transshoot7};
statetype s_transshoot7         = {ro_no,SPR_TRANS_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_transshoot8};
statetype s_transshoot8         = {ro_no,SPR_TRANS_SHOOT1,10,NULL,NULL,&s_transchase1};


/*
===============
=
= SpawnTrans
=
===============
*/

void SpawnTrans (int tilex, int tiley)
{
    //        word *map;
    //        word tile;

    if (SoundBlasterPresent && DigiMode != sds_Off)
        s_transdie01.tictime = 105;

    SpawnNewObj (tilex,tiley,&s_transstand);
    newobj->obclass = transobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_trans];
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


//
// uber
//
void T_UShoot (objtype *ob);

extern  statetype s_uberstand;

extern  statetype s_uberchase1;
extern  statetype s_uberchase1s;
extern  statetype s_uberchase2;
extern  statetype s_uberchase3;
extern  statetype s_uberchase3s;
extern  statetype s_uberchase4;

extern  statetype s_uberdie0;
extern  statetype s_uberdie01;
extern  statetype s_uberdie1;
extern  statetype s_uberdie2;
extern  statetype s_uberdie3;
extern  statetype s_uberdie4;
extern  statetype s_uberdie5;

extern  statetype s_ubershoot1;
extern  statetype s_ubershoot2;
extern  statetype s_ubershoot3;
extern  statetype s_ubershoot4;
extern  statetype s_ubershoot5;
extern  statetype s_ubershoot6;
extern  statetype s_ubershoot7;


statetype s_uberstand           = {ro_no,SPR_UBER_W1,0,(statefunc)T_Stand,NULL,&s_uberstand};

statetype s_uberchase1          = {ro_no,SPR_UBER_W1,10,(statefunc)T_Chase,NULL,&s_uberchase1s};
statetype s_uberchase1s         = {ro_no,SPR_UBER_W1,3,NULL,NULL,&s_uberchase2};
statetype s_uberchase2          = {ro_no,SPR_UBER_W2,8,(statefunc)T_Chase,NULL,&s_uberchase3};
statetype s_uberchase3          = {ro_no,SPR_UBER_W3,10,(statefunc)T_Chase,NULL,&s_uberchase3s};
statetype s_uberchase3s         = {ro_no,SPR_UBER_W3,3,NULL,NULL,&s_uberchase4};
statetype s_uberchase4          = {ro_no,SPR_UBER_W4,8,(statefunc)T_Chase,NULL,&s_uberchase1};

statetype s_uberdie0            = {ro_no,SPR_UBER_W1,1,NULL,(statefunc)A_DeathScream,&s_uberdie01};
statetype s_uberdie01           = {ro_no,SPR_UBER_W1,1,NULL,NULL,&s_uberdie1};
statetype s_uberdie1            = {ro_no,SPR_UBER_DIE1,15,NULL,NULL,&s_uberdie2};
statetype s_uberdie2            = {ro_no,SPR_UBER_DIE2,15,NULL,NULL,&s_uberdie3};
statetype s_uberdie3            = {ro_no,SPR_UBER_DIE3,15,NULL,NULL,&s_uberdie4};
statetype s_uberdie4            = {ro_no,SPR_UBER_DIE4,15,NULL,NULL,&s_uberdie5};
statetype s_uberdie5            = {ro_no,SPR_UBER_DEAD,0,NULL,NULL,&s_uberdie5};

statetype s_ubershoot1          = {ro_no,SPR_UBER_SHOOT1,30,NULL,NULL,&s_ubershoot2};
statetype s_ubershoot2          = {ro_no,SPR_UBER_SHOOT2,12,NULL,(statefunc)T_UShoot,&s_ubershoot3};
statetype s_ubershoot3          = {ro_no,SPR_UBER_SHOOT3,12,NULL,(statefunc)T_UShoot,&s_ubershoot4};
statetype s_ubershoot4          = {ro_no,SPR_UBER_SHOOT4,12,NULL,(statefunc)T_UShoot,&s_ubershoot5};
statetype s_ubershoot5          = {ro_no,SPR_UBER_SHOOT3,12,NULL,(statefunc)T_UShoot,&s_ubershoot6};
statetype s_ubershoot6          = {ro_no,SPR_UBER_SHOOT2,12,NULL,(statefunc)T_UShoot,&s_ubershoot7};
statetype s_ubershoot7          = {ro_no,SPR_UBER_SHOOT1,12,NULL,NULL,&s_uberchase1};


/*
===============
=
= SpawnUber
=
===============
*/

void SpawnUber (int tilex, int tiley)
{
    if (SoundBlasterPresent && DigiMode != sds_Off)
        s_uberdie01.tictime = 70;

    SpawnNewObj (tilex,tiley,&s_uberstand);
    newobj->obclass = uberobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_uber];
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
===============
=
= T_UShoot
=
===============
*/

void T_UShoot (objtype *ob)
{
    int     dx,dy,dist;

    T_Shoot (ob);

    dx = abs(ob->tilex - player->tilex);
    dy = abs(ob->tiley - player->tiley);
    dist = dx>dy ? dx : dy;
    if (dist <= 1)
        TakeDamage (10,ob);
}


//
// will
//
extern  statetype s_willstand;

extern  statetype s_willchase1;
extern  statetype s_willchase1s;
extern  statetype s_willchase2;
extern  statetype s_willchase3;
extern  statetype s_willchase3s;
extern  statetype s_willchase4;

extern  statetype s_willdie1;
extern  statetype s_willdie2;
extern  statetype s_willdie3;
extern  statetype s_willdie4;
extern  statetype s_willdie5;
extern  statetype s_willdie6;

extern  statetype s_willshoot1;
extern  statetype s_willshoot2;
extern  statetype s_willshoot3;
extern  statetype s_willshoot4;
extern  statetype s_willshoot5;
extern  statetype s_willshoot6;


statetype s_willstand           = {ro_no,SPR_WILL_W1,0,(statefunc)T_Stand,NULL,&s_willstand};

statetype s_willchase1          = {ro_no,SPR_WILL_W1,10,(statefunc)T_Will,NULL,&s_willchase1s};
statetype s_willchase1s         = {ro_no,SPR_WILL_W1,3,NULL,NULL,&s_willchase2};
statetype s_willchase2          = {ro_no,SPR_WILL_W2,8,(statefunc)T_Will,NULL,&s_willchase3};
statetype s_willchase3          = {ro_no,SPR_WILL_W3,10,(statefunc)T_Will,NULL,&s_willchase3s};
statetype s_willchase3s         = {ro_no,SPR_WILL_W3,3,NULL,NULL,&s_willchase4};
statetype s_willchase4          = {ro_no,SPR_WILL_W4,8,(statefunc)T_Will,NULL,&s_willchase1};

statetype s_willdeathcam        = {ro_no,SPR_WILL_W1,1,NULL,NULL,&s_willdie1};

statetype s_willdie1            = {ro_no,SPR_WILL_W1,1,NULL,(statefunc)A_DeathScream,&s_willdie2};
statetype s_willdie2            = {ro_no,SPR_WILL_W1,10,NULL,NULL,&s_willdie3};
statetype s_willdie3            = {ro_no,SPR_WILL_DIE1,10,NULL,NULL,&s_willdie4};
statetype s_willdie4            = {ro_no,SPR_WILL_DIE2,10,NULL,NULL,&s_willdie5};
statetype s_willdie5            = {ro_no,SPR_WILL_DIE3,10,NULL,NULL,&s_willdie6};
statetype s_willdie6            = {ro_no,SPR_WILL_DEAD,20,NULL,NULL,&s_willdie6};

statetype s_willshoot1          = {ro_no,SPR_WILL_SHOOT1,30,NULL,NULL,&s_willshoot2};
statetype s_willshoot2          = {ro_no,SPR_WILL_SHOOT2,10,NULL,(statefunc)T_Launch,&s_willshoot3};
statetype s_willshoot3          = {ro_no,SPR_WILL_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_willshoot4};
statetype s_willshoot4          = {ro_no,SPR_WILL_SHOOT4,10,NULL,(statefunc)T_Shoot,&s_willshoot5};
statetype s_willshoot5          = {ro_no,SPR_WILL_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_willshoot6};
statetype s_willshoot6          = {ro_no,SPR_WILL_SHOOT4,10,NULL,(statefunc)T_Shoot,&s_willchase1};


/*
===============
=
= SpawnWill
=
===============
*/

void SpawnWill (int tilex, int tiley)
{
    if (SoundBlasterPresent && DigiMode != sds_Off)
        s_willdie2.tictime = 70;

    SpawnNewObj (tilex,tiley,&s_willstand);
    newobj->obclass = willobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_will];
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
================
=
= T_Will
=
================
*/

void T_Will (objtype *ob)
{
    int32_t move;
    int     dx,dy,dist;
    bool dodge;

    dodge = false;
    dx = abs(ob->tilex - player->tilex);
    dy = abs(ob->tiley - player->tiley);
    dist = dx>dy ? dx : dy;

    if (CheckLine(ob))                                              // got a shot at player?
    {
        ob->hidden = false;
        if ( (unsigned) US_RndT() < (tics<<3) && objfreelist)
        {
            //
            // go into attack frame
            //
            if (ob->obclass == willobj)
                NewState (ob,&s_willshoot1);
            else if (ob->obclass == angelobj)
                NewState (ob,&s_angelshoot1);
            else
                NewState (ob,&s_deathshoot1);
            return;
        }
        dodge = true;
    }
    else
        ob->hidden = true;

    if (ob->dir == nodir)
    {
        if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        if (ob->distance < 0)
        {
            //
            // waiting for a door to open
            //
            OpenDoor (-ob->distance-1);
            if (doorobjlist[-ob->distance-1].action != dr_open)
                return;
            ob->distance = TILEGLOBAL;      // go ahead, the door is now open
            TryWalk(ob);
        }

        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        if (dist <4)
            SelectRunDir (ob);
        else if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

}


//
// death
//
extern  statetype s_deathstand;

extern  statetype s_deathchase1;
extern  statetype s_deathchase1s;
extern  statetype s_deathchase2;
extern  statetype s_deathchase3;
extern  statetype s_deathchase3s;
extern  statetype s_deathchase4;

extern  statetype s_deathdie1;
extern  statetype s_deathdie2;
extern  statetype s_deathdie3;
extern  statetype s_deathdie4;
extern  statetype s_deathdie5;
extern  statetype s_deathdie6;
extern  statetype s_deathdie7;
extern  statetype s_deathdie8;
extern  statetype s_deathdie9;

extern  statetype s_deathshoot1;
extern  statetype s_deathshoot2;
extern  statetype s_deathshoot3;
extern  statetype s_deathshoot4;
extern  statetype s_deathshoot5;


statetype s_deathstand          = {ro_no,SPR_DEATH_W1,0,(statefunc)T_Stand,NULL,&s_deathstand};

statetype s_deathchase1         = {ro_no,SPR_DEATH_W1,10,(statefunc)T_Will,NULL,&s_deathchase1s};
statetype s_deathchase1s        = {ro_no,SPR_DEATH_W1,3,NULL,NULL,&s_deathchase2};
statetype s_deathchase2         = {ro_no,SPR_DEATH_W2,8,(statefunc)T_Will,NULL,&s_deathchase3};
statetype s_deathchase3         = {ro_no,SPR_DEATH_W3,10,(statefunc)T_Will,NULL,&s_deathchase3s};
statetype s_deathchase3s        = {ro_no,SPR_DEATH_W3,3,NULL,NULL,&s_deathchase4};
statetype s_deathchase4         = {ro_no,SPR_DEATH_W4,8,(statefunc)T_Will,NULL,&s_deathchase1};

statetype s_deathdeathcam       = {ro_no,SPR_DEATH_W1,1,NULL,NULL,&s_deathdie1};

statetype s_deathdie1           = {ro_no,SPR_DEATH_W1,1,NULL,(statefunc)A_DeathScream,&s_deathdie2};
statetype s_deathdie2           = {ro_no,SPR_DEATH_W1,10,NULL,NULL,&s_deathdie3};
statetype s_deathdie3           = {ro_no,SPR_DEATH_DIE1,10,NULL,NULL,&s_deathdie4};
statetype s_deathdie4           = {ro_no,SPR_DEATH_DIE2,10,NULL,NULL,&s_deathdie5};
statetype s_deathdie5           = {ro_no,SPR_DEATH_DIE3,10,NULL,NULL,&s_deathdie6};
statetype s_deathdie6           = {ro_no,SPR_DEATH_DIE4,10,NULL,NULL,&s_deathdie7};
statetype s_deathdie7           = {ro_no,SPR_DEATH_DIE5,10,NULL,NULL,&s_deathdie8};
statetype s_deathdie8           = {ro_no,SPR_DEATH_DIE6,10,NULL,NULL,&s_deathdie9};
statetype s_deathdie9           = {ro_no,SPR_DEATH_DEAD,0,NULL,NULL,&s_deathdie9};

statetype s_deathshoot1         = {ro_no,SPR_DEATH_SHOOT1,30,NULL,NULL,&s_deathshoot2};
statetype s_deathshoot2         = {ro_no,SPR_DEATH_SHOOT2,10,NULL,(statefunc)T_Launch,&s_deathshoot3};
statetype s_deathshoot3         = {ro_no,SPR_DEATH_SHOOT4,10,NULL,(statefunc)T_Shoot,&s_deathshoot4};
statetype s_deathshoot4         = {ro_no,SPR_DEATH_SHOOT3,10,NULL,(statefunc)T_Launch,&s_deathshoot5};
statetype s_deathshoot5         = {ro_no,SPR_DEATH_SHOOT4,10,NULL,(statefunc)T_Shoot,&s_deathchase1};


/*
===============
=
= SpawnDeath
=
===============
*/

void SpawnDeath (int tilex, int tiley)
{
    if (SoundBlasterPresent && DigiMode != sds_Off)
        s_deathdie2.tictime = 105;

    SpawnNewObj (tilex,tiley,&s_deathstand);
    newobj->obclass = deathobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_death];
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}

/*
===============
=
= T_Launch
=
===============
*/

void T_Launch (objtype *ob)
{
    int32_t deltax,deltay;
    float   angle;
    int     iangle;

    deltax = player->x - ob->x;
    deltay = ob->y - player->y;
    angle = (float) atan2 ((float) deltay, (float) deltax);
    if (angle<0)
        angle = (float) (M_PI*2+angle);
    iangle = (int) (angle/(M_PI*2)*ANGLES);
    if (ob->obclass == deathobj)
    {
        T_Shoot (ob);
        if (ob->state == &s_deathshoot2)
        {
            iangle-=4;
            if (iangle<0)
                iangle+=ANGLES;
        }
        else
        {
            iangle+=4;
            if (iangle>=ANGLES)
                iangle-=ANGLES;
        }
    }

    GetNewActor ();
    newobj->state = &s_rocket;
    newobj->ticcount = 1;

    newobj->tilex = ob->tilex;
    newobj->tiley = ob->tiley;
    newobj->x = ob->x;
    newobj->y = ob->y;
    newobj->obclass = rocketobj;
    switch(ob->obclass)
    {
        case deathobj:
            newobj->state = &s_hrocket;
            newobj->obclass = hrocketobj;
            PlaySoundLocActor (KNIGHTMISSILESND,newobj);
            break;
        case angelobj:
            newobj->state = &s_spark1;
            newobj->obclass = sparkobj;
            PlaySoundLocActor (ANGELFIRESND,newobj);
            break;
        default:
            PlaySoundLocActor (MISSILEFIRESND,newobj);
    }

    newobj->dir = nodir;
    newobj->angle = iangle;
    newobj->speed = 0x2000l;
    newobj->flags = FL_NEVERMARK;
    newobj->active = ac_yes;
}



//
// angel
//
void A_Relaunch (objtype *ob);
void A_Victory (objtype *ob);
void A_StartAttack (objtype *ob);
void A_Breathing (objtype *ob);

extern  statetype s_angelstand;

extern  statetype s_angelchase1;
extern  statetype s_angelchase1s;
extern  statetype s_angelchase2;
extern  statetype s_angelchase3;
extern  statetype s_angelchase3s;
extern  statetype s_angelchase4;

extern  statetype s_angeldie1;
extern  statetype s_angeldie11;
extern  statetype s_angeldie2;
extern  statetype s_angeldie3;
extern  statetype s_angeldie4;
extern  statetype s_angeldie5;
extern  statetype s_angeldie6;
extern  statetype s_angeldie7;
extern  statetype s_angeldie8;
extern  statetype s_angeldie9;

extern  statetype s_angelshoot1;
extern  statetype s_angelshoot2;
extern  statetype s_angelshoot3;
extern  statetype s_angelshoot4;
extern  statetype s_angelshoot5;
extern  statetype s_angelshoot6;

extern  statetype s_angeltired;
extern  statetype s_angeltired2;
extern  statetype s_angeltired3;
extern  statetype s_angeltired4;
extern  statetype s_angeltired5;
extern  statetype s_angeltired6;
extern  statetype s_angeltired7;

extern  statetype s_spark1;
extern  statetype s_spark2;
extern  statetype s_spark3;
extern  statetype s_spark4;


statetype s_angelstand          = {ro_no,SPR_ANGEL_W1,0,(statefunc)T_Stand,NULL,&s_angelstand};

statetype s_angelchase1         = {ro_no,SPR_ANGEL_W1,10,(statefunc)T_Will,NULL,&s_angelchase1s};
statetype s_angelchase1s        = {ro_no,SPR_ANGEL_W1,3,NULL,NULL,&s_angelchase2};
statetype s_angelchase2         = {ro_no,SPR_ANGEL_W2,8,(statefunc)T_Will,NULL,&s_angelchase3};
statetype s_angelchase3         = {ro_no,SPR_ANGEL_W3,10,(statefunc)T_Will,NULL,&s_angelchase3s};
statetype s_angelchase3s        = {ro_no,SPR_ANGEL_W3,3,NULL,NULL,&s_angelchase4};
statetype s_angelchase4         = {ro_no,SPR_ANGEL_W4,8,(statefunc)T_Will,NULL,&s_angelchase1};

statetype s_angeldie1           = {ro_no,SPR_ANGEL_W1,1,NULL,(statefunc)A_DeathScream,&s_angeldie11};
statetype s_angeldie11          = {ro_no,SPR_ANGEL_W1,1,NULL,NULL,&s_angeldie2};
statetype s_angeldie2           = {ro_no,SPR_ANGEL_DIE1,10,NULL,(statefunc)A_Slurpie,&s_angeldie3};
statetype s_angeldie3           = {ro_no,SPR_ANGEL_DIE2,10,NULL,NULL,&s_angeldie4};
statetype s_angeldie4           = {ro_no,SPR_ANGEL_DIE3,10,NULL,NULL,&s_angeldie5};
statetype s_angeldie5           = {ro_no,SPR_ANGEL_DIE4,10,NULL,NULL,&s_angeldie6};
statetype s_angeldie6           = {ro_no,SPR_ANGEL_DIE5,10,NULL,NULL,&s_angeldie7};
statetype s_angeldie7           = {ro_no,SPR_ANGEL_DIE6,10,NULL,NULL,&s_angeldie8};
statetype s_angeldie8           = {ro_no,SPR_ANGEL_DIE7,10,NULL,NULL,&s_angeldie9};
statetype s_angeldie9           = {ro_no,SPR_ANGEL_DEAD,130,NULL,(statefunc)A_Victory,&s_angeldie9};

statetype s_angelshoot1         = {ro_no,SPR_ANGEL_SHOOT1,10,NULL,(statefunc)A_StartAttack,&s_angelshoot2};
statetype s_angelshoot2         = {ro_no,SPR_ANGEL_SHOOT2,20,NULL,(statefunc)T_Launch,&s_angelshoot3};
statetype s_angelshoot3         = {ro_no,SPR_ANGEL_SHOOT1,10,NULL,(statefunc)A_Relaunch,&s_angelshoot2};

statetype s_angeltired          = {ro_no,SPR_ANGEL_TIRED1,40,NULL,(statefunc)A_Breathing,&s_angeltired2};
statetype s_angeltired2         = {ro_no,SPR_ANGEL_TIRED2,40,NULL,NULL,&s_angeltired3};
statetype s_angeltired3         = {ro_no,SPR_ANGEL_TIRED1,40,NULL,(statefunc)A_Breathing,&s_angeltired4};
statetype s_angeltired4         = {ro_no,SPR_ANGEL_TIRED2,40,NULL,NULL,&s_angeltired5};
statetype s_angeltired5         = {ro_no,SPR_ANGEL_TIRED1,40,NULL,(statefunc)A_Breathing,&s_angeltired6};
statetype s_angeltired6         = {ro_no,SPR_ANGEL_TIRED2,40,NULL,NULL,&s_angeltired7};
statetype s_angeltired7         = {ro_no,SPR_ANGEL_TIRED1,40,NULL,(statefunc)A_Breathing,&s_angelchase1};

statetype s_spark1              = {ro_no,SPR_SPARK1,6,(statefunc)T_Projectile,NULL,&s_spark2};
statetype s_spark2              = {ro_no,SPR_SPARK2,6,(statefunc)T_Projectile,NULL,&s_spark3};
statetype s_spark3              = {ro_no,SPR_SPARK3,6,(statefunc)T_Projectile,NULL,&s_spark4};
statetype s_spark4              = {ro_no,SPR_SPARK4,6,(statefunc)T_Projectile,NULL,&s_spark1};


void A_Slurpie (objtype *)
{
    SD_PlaySound(SLURPIESND);
}

void A_Breathing (objtype *)
{
    SD_PlaySound(ANGELTIREDSND);
}

/*
===============
=
= SpawnAngel
=
===============
*/

void SpawnAngel (int tilex, int tiley)
{
    if (SoundBlasterPresent && DigiMode != sds_Off)
        s_angeldie11.tictime = 105;

    SpawnNewObj (tilex,tiley,&s_angelstand);
    newobj->obclass = angelobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_angel];
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
=================
=
= A_Victory
=
=================
*/

void A_Victory (objtype *)
{
    playstate = ex_victorious;
}


/*
=================
=
= A_StartAttack
=
=================
*/

void A_StartAttack (objtype *ob)
{
    ob->temp1 = 0;
}


/*
=================
=
= A_Relaunch
=
=================
*/

void A_Relaunch (objtype *ob)
{
    if (++ob->temp1 == 3)
    {
        NewState (ob,&s_angeltired);
        return;
    }

    if (US_RndT()&1)
    {
        NewState (ob,&s_angelchase1);
        return;
    }
}




//
// spectre
//
void T_SpectreWait (objtype *ob);
void A_Dormant (objtype *ob);

extern  statetype s_spectrewait1;
extern  statetype s_spectrewait2;
extern  statetype s_spectrewait3;
extern  statetype s_spectrewait4;

extern  statetype s_spectrechase1;
extern  statetype s_spectrechase2;
extern  statetype s_spectrechase3;
extern  statetype s_spectrechase4;

extern  statetype s_spectredie1;
extern  statetype s_spectredie2;
extern  statetype s_spectredie3;
extern  statetype s_spectredie4;

extern  statetype s_spectrewake;

statetype s_spectrewait1        = {ro_no,SPR_SPECTRE_W1,10,(statefunc)T_Stand,NULL,&s_spectrewait2};
statetype s_spectrewait2        = {ro_no,SPR_SPECTRE_W2,10,(statefunc)T_Stand,NULL,&s_spectrewait3};
statetype s_spectrewait3        = {ro_no,SPR_SPECTRE_W3,10,(statefunc)T_Stand,NULL,&s_spectrewait4};
statetype s_spectrewait4        = {ro_no,SPR_SPECTRE_W4,10,(statefunc)T_Stand,NULL,&s_spectrewait1};

statetype s_spectrechase1       = {ro_no,SPR_SPECTRE_W1,10,(statefunc)T_Ghosts,NULL,&s_spectrechase2};
statetype s_spectrechase2       = {ro_no,SPR_SPECTRE_W2,10,(statefunc)T_Ghosts,NULL,&s_spectrechase3};
statetype s_spectrechase3       = {ro_no,SPR_SPECTRE_W3,10,(statefunc)T_Ghosts,NULL,&s_spectrechase4};
statetype s_spectrechase4       = {ro_no,SPR_SPECTRE_W4,10,(statefunc)T_Ghosts,NULL,&s_spectrechase1};

statetype s_spectredie1         = {ro_no,SPR_SPECTRE_F1,10,NULL,NULL,&s_spectredie2};
statetype s_spectredie2         = {ro_no,SPR_SPECTRE_F2,10,NULL,NULL,&s_spectredie3};
statetype s_spectredie3         = {ro_no,SPR_SPECTRE_F3,10,NULL,NULL,&s_spectredie4};
statetype s_spectredie4         = {ro_no,SPR_SPECTRE_F4,300,NULL,NULL,&s_spectrewake};
statetype s_spectrewake         = {ro_no,SPR_SPECTRE_F4,10,NULL,(statefunc)A_Dormant,&s_spectrewake};

/*
===============
=
= SpawnSpectre
=
===============
*/

void SpawnSpectre (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_spectrewait1);
    newobj->obclass = spectreobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_spectre];
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH|FL_BONUS; // |FL_NEVERMARK|FL_NONMARK;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
===============
=
= A_Dormant
=
===============
*/

void A_Dormant (objtype *ob)
{
    int32_t     deltax,deltay;
    int         xl,xh,yl,yh;
    int         x,y;
    uintptr_t   tile;

    deltax = ob->x - player->x;
    if (deltax < -MINACTORDIST || deltax > MINACTORDIST)
        goto moveok;
    deltay = ob->y - player->y;
    if (deltay < -MINACTORDIST || deltay > MINACTORDIST)
        goto moveok;

    return;
moveok:

    xl = (ob->x-MINDIST) >> TILESHIFT;
    xh = (ob->x+MINDIST) >> TILESHIFT;
    yl = (ob->y-MINDIST) >> TILESHIFT;
    yh = (ob->y+MINDIST) >> TILESHIFT;

    for (y=yl ; y<=yh ; y++)
        for (x=xl ; x<=xh ; x++)
        {
            tile = (uintptr_t)actorat[x][y];
            if (!tile)
                continue;
            if (!ISPOINTER(tile))
                return;
            if (((objtype *)tile)->flags&FL_SHOOTABLE)
                return;
        }

        ob->flags |= FL_AMBUSH | FL_SHOOTABLE;
        ob->flags &= ~FL_ATTACKMODE;
        ob->flags &= ~FL_NONMARK;      // stuck bugfix 1
        ob->dir = nodir;
        NewState (ob,&s_spectrewait1);
}


#endif

/*
=============================================================================

                            SCHABBS / GIFT / FAT

=============================================================================
*/

#ifndef SPEAR
/*
===============
=
= SpawnGhosts
=
===============
*/

void SpawnGhosts (int which, int tilex, int tiley)
{
    switch(which)
    {
        case en_blinky:
            SpawnNewObj (tilex,tiley,&s_blinkychase1);
            break;
        case en_clyde:
            SpawnNewObj (tilex,tiley,&s_clydechase1);
            break;
        case en_pinky:
            SpawnNewObj (tilex,tiley,&s_pinkychase1);
            break;
        case en_inky:
            SpawnNewObj (tilex,tiley,&s_inkychase1);
            break;
    }

    newobj->obclass = ghostobj;
    newobj->speed = SPDDOG;

    newobj->dir = east;
    newobj->flags |= FL_AMBUSH;
    if (!loadedgame)
    {
        gamestate.killtotal++;
        gamestate.killcount++;
    }
}



void    T_Gift (objtype *ob);
void    T_GiftThrow (objtype *ob);

void    T_Fat (objtype *ob);
void    T_FatThrow (objtype *ob);

//
// schabb
//
extern  const statetype s_schabbstand;

extern  const statetype s_schabbchase1;
extern  const statetype s_schabbchase1s;
extern  const statetype s_schabbchase2;
extern  const statetype s_schabbchase3;
extern  const statetype s_schabbchase3s;
extern  const statetype s_schabbchase4;

extern  const statetype s_schabbdie1;
extern  const statetype s_schabbdie2;
extern  const statetype s_schabbdie3;
extern  const statetype s_schabbdie4;
extern  const statetype s_schabbdie5;
extern  const statetype s_schabbdie6;

extern  const statetype s_schabbshoot1;
extern  const statetype s_schabbshoot2;

extern  const statetype s_needle1;
extern  const statetype s_needle2;
extern  const statetype s_needle3;
extern  const statetype s_needle4;

extern  const statetype s_schabbdeathcam;


const statetype s_schabbstand         = {ro_no,SPR_SCHABB_W1,0,(statefunc)T_Stand,NULL,&s_schabbstand};

const statetype s_schabbchase1        = {ro_no,SPR_SCHABB_W1,10,(statefunc)T_Schabb,NULL,&s_schabbchase1s};
const statetype s_schabbchase1s       = {ro_no,SPR_SCHABB_W1,3,NULL,NULL,&s_schabbchase2};
const statetype s_schabbchase2        = {ro_no,SPR_SCHABB_W2,8,(statefunc)T_Schabb,NULL,&s_schabbchase3};
const statetype s_schabbchase3        = {ro_no,SPR_SCHABB_W3,10,(statefunc)T_Schabb,NULL,&s_schabbchase3s};
const statetype s_schabbchase3s       = {ro_no,SPR_SCHABB_W3,3,NULL,NULL,&s_schabbchase4};
const statetype s_schabbchase4        = {ro_no,SPR_SCHABB_W4,8,(statefunc)T_Schabb,NULL,&s_schabbchase1};

const statetype s_schabbdeathcam      = {ro_no,SPR_SCHABB_W1,1,NULL,NULL,&s_schabbdie1};

const statetype s_schabbdie1          = {ro_no,SPR_SCHABB_W1,10,NULL,(statefunc)A_DeathScream,&s_schabbdie2};
const statetype s_schabbdie2          = {ro_no,SPR_SCHABB_W1,140,NULL,NULL,&s_schabbdie3};
const statetype s_schabbdie3          = {ro_no,SPR_SCHABB_DIE1,10,NULL,NULL,&s_schabbdie4};
const statetype s_schabbdie4          = {ro_no,SPR_SCHABB_DIE2,10,NULL,NULL,&s_schabbdie5};
const statetype s_schabbdie5          = {ro_no,SPR_SCHABB_DIE3,10,NULL,NULL,&s_schabbdie6};
const statetype s_schabbdie6          = {ro_no,SPR_SCHABB_DEAD,20,NULL,(statefunc)A_StartDeathCam,&s_schabbdie6};

const statetype s_schabbshoot1        = {ro_no,SPR_SCHABB_SHOOT1,30,NULL,NULL,&s_schabbshoot2};
const statetype s_schabbshoot2        = {ro_no,SPR_SCHABB_SHOOT2,10,NULL,(statefunc)T_SchabbThrow,&s_schabbchase1};

const statetype s_needle1             = {ro_no,SPR_HYPO1,6,(statefunc)T_Projectile,NULL,&s_needle2};
const statetype s_needle2             = {ro_no,SPR_HYPO2,6,(statefunc)T_Projectile,NULL,&s_needle3};
const statetype s_needle3             = {ro_no,SPR_HYPO3,6,(statefunc)T_Projectile,NULL,&s_needle4};
const statetype s_needle4             = {ro_no,SPR_HYPO4,6,(statefunc)T_Projectile,NULL,&s_needle1};


//
// gift
//
extern  const statetype s_giftstand;

extern  const statetype s_giftchase1;
extern  const statetype s_giftchase1s;
extern  const statetype s_giftchase2;
extern  const statetype s_giftchase3;
extern  const statetype s_giftchase3s;
extern  const statetype s_giftchase4;

extern  const statetype s_giftdie1;
extern  const statetype s_giftdie2;
extern  const statetype s_giftdie3;
extern  const statetype s_giftdie4;
extern  const statetype s_giftdie5;
extern  const statetype s_giftdie6;

extern  const statetype s_giftshoot1;
extern  const statetype s_giftshoot2;

extern  const statetype s_needle1;
extern  const statetype s_needle2;
extern  const statetype s_needle3;
extern  const statetype s_needle4;

extern  const statetype s_giftdeathcam;

extern  const statetype s_boom1;
extern  const statetype s_boom2;
extern  const statetype s_boom3;


const statetype s_giftstand           = {ro_no,SPR_GIFT_W1,0,(statefunc)T_Stand,NULL,&s_giftstand};

const statetype s_giftchase1          = {ro_no,SPR_GIFT_W1,10,(statefunc)T_Gift,NULL,&s_giftchase1s};
const statetype s_giftchase1s         = {ro_no,SPR_GIFT_W1,3,NULL,NULL,&s_giftchase2};
const statetype s_giftchase2          = {ro_no,SPR_GIFT_W2,8,(statefunc)T_Gift,NULL,&s_giftchase3};
const statetype s_giftchase3          = {ro_no,SPR_GIFT_W3,10,(statefunc)T_Gift,NULL,&s_giftchase3s};
const statetype s_giftchase3s         = {ro_no,SPR_GIFT_W3,3,NULL,NULL,&s_giftchase4};
const statetype s_giftchase4          = {ro_no,SPR_GIFT_W4,8,(statefunc)T_Gift,NULL,&s_giftchase1};

const statetype s_giftdeathcam        = {ro_no,SPR_GIFT_W1,1,NULL,NULL,&s_giftdie1};

const statetype s_giftdie1            = {ro_no,SPR_GIFT_W1,1,NULL,(statefunc)A_DeathScream,&s_giftdie2};
const statetype s_giftdie2            = {ro_no,SPR_GIFT_W1,140,NULL,NULL,&s_giftdie3};
const statetype s_giftdie3            = {ro_no,SPR_GIFT_DIE1,10,NULL,NULL,&s_giftdie4};
const statetype s_giftdie4            = {ro_no,SPR_GIFT_DIE2,10,NULL,NULL,&s_giftdie5};
const statetype s_giftdie5            = {ro_no,SPR_GIFT_DIE3,10,NULL,NULL,&s_giftdie6};
const statetype s_giftdie6            = {ro_no,SPR_GIFT_DEAD,20,NULL,(statefunc)A_StartDeathCam,&s_giftdie6};

const statetype s_giftshoot1          = {ro_no,SPR_GIFT_SHOOT1,30,NULL,NULL,&s_giftshoot2};
const statetype s_giftshoot2          = {ro_no,SPR_GIFT_SHOOT2,10,NULL,(statefunc)T_GiftThrow,&s_giftchase1};


//
// fat
//
extern  const statetype s_fatstand;

extern  const statetype s_fatchase1;
extern  const statetype s_fatchase1s;
extern  const statetype s_fatchase2;
extern  const statetype s_fatchase3;
extern  const statetype s_fatchase3s;
extern  const statetype s_fatchase4;

extern  const statetype s_fatdie1;
extern  const statetype s_fatdie2;
extern  const statetype s_fatdie3;
extern  const statetype s_fatdie4;
extern  const statetype s_fatdie5;
extern  const statetype s_fatdie6;

extern  const statetype s_fatshoot1;
extern  const statetype s_fatshoot2;
extern  const statetype s_fatshoot3;
extern  const statetype s_fatshoot4;
extern  const statetype s_fatshoot5;
extern  const statetype s_fatshoot6;

extern  const statetype s_needle1;
extern  const statetype s_needle2;
extern  const statetype s_needle3;
extern  const statetype s_needle4;

extern  const statetype s_fatdeathcam;


const statetype s_fatstand            = {ro_no,SPR_FAT_W1,0,(statefunc)T_Stand,NULL,&s_fatstand};

const statetype s_fatchase1           = {ro_no,SPR_FAT_W1,10,(statefunc)T_Fat,NULL,&s_fatchase1s};
const statetype s_fatchase1s          = {ro_no,SPR_FAT_W1,3,NULL,NULL,&s_fatchase2};
const statetype s_fatchase2           = {ro_no,SPR_FAT_W2,8,(statefunc)T_Fat,NULL,&s_fatchase3};
const statetype s_fatchase3           = {ro_no,SPR_FAT_W3,10,(statefunc)T_Fat,NULL,&s_fatchase3s};
const statetype s_fatchase3s          = {ro_no,SPR_FAT_W3,3,NULL,NULL,&s_fatchase4};
const statetype s_fatchase4           = {ro_no,SPR_FAT_W4,8,(statefunc)T_Fat,NULL,&s_fatchase1};

const statetype s_fatdeathcam         = {ro_no,SPR_FAT_W1,1,NULL,NULL,&s_fatdie1};

const statetype s_fatdie1             = {ro_no,SPR_FAT_W1,1,NULL,(statefunc)A_DeathScream,&s_fatdie2};
const statetype s_fatdie2             = {ro_no,SPR_FAT_W1,140,NULL,NULL,&s_fatdie3};
const statetype s_fatdie3             = {ro_no,SPR_FAT_DIE1,10,NULL,NULL,&s_fatdie4};
const statetype s_fatdie4             = {ro_no,SPR_FAT_DIE2,10,NULL,NULL,&s_fatdie5};
const statetype s_fatdie5             = {ro_no,SPR_FAT_DIE3,10,NULL,NULL,&s_fatdie6};
const statetype s_fatdie6             = {ro_no,SPR_FAT_DEAD,20,NULL,(statefunc)A_StartDeathCam,&s_fatdie6};

const statetype s_fatshoot1           = {ro_no,SPR_FAT_SHOOT1,30,NULL,NULL,&s_fatshoot2};
const statetype s_fatshoot2           = {ro_no,SPR_FAT_SHOOT2,10,NULL,(statefunc)T_GiftThrow,&s_fatshoot3};
const statetype s_fatshoot3           = {ro_no,SPR_FAT_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_fatshoot4};
const statetype s_fatshoot4           = {ro_no,SPR_FAT_SHOOT4,10,NULL,(statefunc)T_Shoot,&s_fatshoot5};
const statetype s_fatshoot5           = {ro_no,SPR_FAT_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_fatshoot6};
const statetype s_fatshoot6           = {ro_no,SPR_FAT_SHOOT4,10,NULL,(statefunc)T_Shoot,&s_fatchase1};


/*
===============
=
= SpawnSchabbs
=
===============
*/

void SpawnSchabbs (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_schabbstand);
    newobj->speed = SPDPATROL;

    newobj->obclass = schabbobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_schabbs];
    newobj->dir = nodir;
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
===============
=
= SpawnGift
=
===============
*/

void SpawnGift (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_giftstand);
    newobj->speed = SPDPATROL;

    newobj->obclass = giftobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_gift];
    newobj->dir = nodir;
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
===============
=
= SpawnFat
=
===============
*/

void SpawnFat (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_fatstand);
    newobj->speed = SPDPATROL;

    newobj->obclass = fatobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_fat];
    newobj->dir = nodir;
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
=================
=
= T_SchabbThrow
=
=================
*/

void T_SchabbThrow (objtype *ob)
{
    int32_t deltax,deltay;
    float   angle;
    int     iangle;

    deltax = player->x - ob->x;
    deltay = ob->y - player->y;
    angle = (float) atan2((float) deltay, (float) deltax);
    if (angle<0)
        angle = (float) (M_PI*2+angle);
    iangle = (int) (angle/(M_PI*2)*ANGLES);

    GetNewActor ();
    newobj->state = &s_needle1;
    newobj->ticcount = 1;

    newobj->tilex = ob->tilex;
    newobj->tiley = ob->tiley;
    newobj->x = ob->x;
    newobj->y = ob->y;
    newobj->obclass = needleobj;
    newobj->dir = nodir;
    newobj->angle = iangle;
    newobj->speed = 0x2000l;

    newobj->flags = FL_NEVERMARK;
    newobj->active = ac_yes;

    PlaySoundLocActor (SCHABBSTHROWSND,newobj);
}

/*
=================
=
= T_GiftThrow
=
=================
*/

void T_GiftThrow (objtype *ob)
{
    int32_t deltax,deltay;
    float   angle;
    int     iangle;

    deltax = player->x - ob->x;
    deltay = ob->y - player->y;
    angle = (float) atan2((float) deltay, (float) deltax);
    if (angle<0)
        angle = (float) (M_PI*2+angle);
    iangle = (int) (angle/(M_PI*2)*ANGLES);

    GetNewActor ();
    newobj->state = &s_rocket;
    newobj->ticcount = 1;

    newobj->tilex = ob->tilex;
    newobj->tiley = ob->tiley;
    newobj->x = ob->x;
    newobj->y = ob->y;
    newobj->obclass = rocketobj;
    newobj->dir = nodir;
    newobj->angle = iangle;
    newobj->speed = 0x2000l;
    newobj->flags = FL_NEVERMARK;
    newobj->active = ac_yes;

#ifndef APOGEE_1_0          // T_GiftThrow will never be called in shareware v1.0
    PlaySoundLocActor (MISSILEFIRESND,newobj);
#endif
}


/*
=================
=
= T_Schabb
=
=================
*/

void T_Schabb (objtype *ob)
{
    int32_t move;
    int     dx,dy,dist;
    bool dodge;

    dodge = false;
    dx = abs(ob->tilex - player->tilex);
    dy = abs(ob->tiley - player->tiley);
    dist = dx>dy ? dx : dy;

    if (CheckLine(ob))                                              // got a shot at player?
    {
        ob->hidden = false;
        if ( (unsigned) US_RndT() < (tics<<3) && objfreelist)
        {
            //
            // go into attack frame
            //
            NewState (ob,&s_schabbshoot1);
            return;
        }
        dodge = true;
    }
    else
        ob->hidden = true;

    if (ob->dir == nodir)
    {
        if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        if (ob->distance < 0)
        {
            //
            // waiting for a door to open
            //
            OpenDoor (-ob->distance-1);
            if (doorobjlist[-ob->distance-1].action != dr_open)
                return;
            ob->distance = TILEGLOBAL;      // go ahead, the door is now open
            TryWalk(ob);
        }

        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        if (dist <4)
            SelectRunDir (ob);
        else if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }
}


/*
=================
=
= T_Gift
=
=================
*/

void T_Gift (objtype *ob)
{
    int32_t move;
    int     dx,dy,dist;
    bool dodge;

    dodge = false;
    dx = abs(ob->tilex - player->tilex);
    dy = abs(ob->tiley - player->tiley);
    dist = dx>dy ? dx : dy;

    if (CheckLine(ob))                                              // got a shot at player?
    {
        ob->hidden = false;
        if ( (unsigned) US_RndT() < (tics<<3) && objfreelist)
        {
            //
            // go into attack frame
            //
            NewState (ob,&s_giftshoot1);
            return;
        }
        dodge = true;
    }
    else
        ob->hidden = true;

    if (ob->dir == nodir)
    {
        if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        if (ob->distance < 0)
        {
            //
            // waiting for a door to open
            //
            OpenDoor (-ob->distance-1);
            if (doorobjlist[-ob->distance-1].action != dr_open)
                return;
            ob->distance = TILEGLOBAL;      // go ahead, the door is now open
            TryWalk(ob);
        }

        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        if (dist <4)
            SelectRunDir (ob);
        else if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }
}


/*
=================
=
= T_Fat
=
=================
*/

void T_Fat (objtype *ob)
{
    int32_t move;
    int     dx,dy,dist;
    bool dodge;

    dodge = false;
    dx = abs(ob->tilex - player->tilex);
    dy = abs(ob->tiley - player->tiley);
    dist = dx>dy ? dx : dy;

    if (CheckLine(ob))                                              // got a shot at player?
    {
        ob->hidden = false;
        if ( (unsigned) US_RndT() < (tics<<3) && objfreelist)
        {
            //
            // go into attack frame
            //
            NewState (ob,&s_fatshoot1);
            return;
        }
        dodge = true;
    }
    else
        ob->hidden = true;

    if (ob->dir == nodir)
    {
        if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        if (ob->distance < 0)
        {
            //
            // waiting for a door to open
            //
            OpenDoor (-ob->distance-1);
            if (doorobjlist[-ob->distance-1].action != dr_open)
                return;
            ob->distance = TILEGLOBAL;      // go ahead, the door is now open
            TryWalk(ob);
        }

        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        if (dist <4)
            SelectRunDir (ob);
        else if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }
}


/*
=============================================================================

                                    HITLERS

=============================================================================
*/


//
// fake
//
extern  const statetype s_fakestand;

extern  const statetype s_fakechase1;
extern  const statetype s_fakechase1s;
extern  const statetype s_fakechase2;
extern  const statetype s_fakechase3;
extern  const statetype s_fakechase3s;
extern  const statetype s_fakechase4;

extern  const statetype s_fakedie1;
extern  const statetype s_fakedie2;
extern  const statetype s_fakedie3;
extern  const statetype s_fakedie4;
extern  const statetype s_fakedie5;
extern  const statetype s_fakedie6;

extern  const statetype s_fakeshoot1;
extern  const statetype s_fakeshoot2;
extern  const statetype s_fakeshoot3;
extern  const statetype s_fakeshoot4;
extern  const statetype s_fakeshoot5;
extern  const statetype s_fakeshoot6;
extern  const statetype s_fakeshoot7;
extern  const statetype s_fakeshoot8;
extern  const statetype s_fakeshoot9;

extern  const statetype s_fire1;
extern  const statetype s_fire2;

const statetype s_fakestand           = {ro_no,SPR_FAKE_W1,0,(statefunc)T_Stand,NULL,&s_fakestand};

const statetype s_fakechase1          = {ro_no,SPR_FAKE_W1,10,(statefunc)T_Fake,NULL,&s_fakechase1s};
const statetype s_fakechase1s         = {ro_no,SPR_FAKE_W1,3,NULL,NULL,&s_fakechase2};
const statetype s_fakechase2          = {ro_no,SPR_FAKE_W2,8,(statefunc)T_Fake,NULL,&s_fakechase3};
const statetype s_fakechase3          = {ro_no,SPR_FAKE_W3,10,(statefunc)T_Fake,NULL,&s_fakechase3s};
const statetype s_fakechase3s         = {ro_no,SPR_FAKE_W3,3,NULL,NULL,&s_fakechase4};
const statetype s_fakechase4          = {ro_no,SPR_FAKE_W4,8,(statefunc)T_Fake,NULL,&s_fakechase1};

const statetype s_fakedie1            = {ro_no,SPR_FAKE_DIE1,10,NULL,(statefunc)A_DeathScream,&s_fakedie2};
const statetype s_fakedie2            = {ro_no,SPR_FAKE_DIE2,10,NULL,NULL,&s_fakedie3};
const statetype s_fakedie3            = {ro_no,SPR_FAKE_DIE3,10,NULL,NULL,&s_fakedie4};
const statetype s_fakedie4            = {ro_no,SPR_FAKE_DIE4,10,NULL,NULL,&s_fakedie5};
const statetype s_fakedie5            = {ro_no,SPR_FAKE_DIE5,10,NULL,NULL,&s_fakedie6};
const statetype s_fakedie6            = {ro_no,SPR_FAKE_DEAD,0,NULL,NULL,&s_fakedie6};

const statetype s_fakeshoot1          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot2};
const statetype s_fakeshoot2          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot3};
const statetype s_fakeshoot3          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot4};
const statetype s_fakeshoot4          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot5};
const statetype s_fakeshoot5          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot6};
const statetype s_fakeshoot6          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot7};
const statetype s_fakeshoot7          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot8};
const statetype s_fakeshoot8          = {ro_no,SPR_FAKE_SHOOT,8,NULL,(statefunc)T_FakeFire,&s_fakeshoot9};
const statetype s_fakeshoot9          = {ro_no,SPR_FAKE_SHOOT,8,NULL,NULL,&s_fakechase1};

const statetype s_fire1               = {ro_no,SPR_FIRE1,6,NULL,(statefunc)T_Projectile,&s_fire2};
const statetype s_fire2               = {ro_no,SPR_FIRE2,6,NULL,(statefunc)T_Projectile,&s_fire1};

//
// hitler
//
extern  const statetype s_mechachase1;
extern  const statetype s_mechachase1s;
extern  const statetype s_mechachase2;
extern  const statetype s_mechachase3;
extern  const statetype s_mechachase3s;
extern  const statetype s_mechachase4;

extern  const statetype s_mechadie1;
extern  const statetype s_mechadie2;
extern  const statetype s_mechadie3;
extern  const statetype s_mechadie4;

extern  const statetype s_mechashoot1;
extern  const statetype s_mechashoot2;
extern  const statetype s_mechashoot3;
extern  const statetype s_mechashoot4;
extern  const statetype s_mechashoot5;
extern  const statetype s_mechashoot6;


extern  const statetype s_hitlerchase1;
extern  const statetype s_hitlerchase1s;
extern  const statetype s_hitlerchase2;
extern  const statetype s_hitlerchase3;
extern  const statetype s_hitlerchase3s;
extern  const statetype s_hitlerchase4;

extern  const statetype s_hitlerdie1;
extern  const statetype s_hitlerdie2;
extern  const statetype s_hitlerdie3;
extern  const statetype s_hitlerdie4;
extern  const statetype s_hitlerdie5;
extern  const statetype s_hitlerdie6;
extern  const statetype s_hitlerdie7;
extern  const statetype s_hitlerdie8;
extern  const statetype s_hitlerdie9;
extern  const statetype s_hitlerdie10;

extern  const statetype s_hitlershoot1;
extern  const statetype s_hitlershoot2;
extern  const statetype s_hitlershoot3;
extern  const statetype s_hitlershoot4;
extern  const statetype s_hitlershoot5;
extern  const statetype s_hitlershoot6;

extern  const statetype s_hitlerdeathcam;

const statetype s_mechastand          = {ro_no,SPR_MECHA_W1,0,(statefunc)T_Stand,NULL,&s_mechastand};

const statetype s_mechachase1         = {ro_no,SPR_MECHA_W1,10,(statefunc)T_Chase,(statefunc)A_MechaSound,&s_mechachase1s};
const statetype s_mechachase1s        = {ro_no,SPR_MECHA_W1,6,NULL,NULL,&s_mechachase2};
const statetype s_mechachase2         = {ro_no,SPR_MECHA_W2,8,(statefunc)T_Chase,NULL,&s_mechachase3};
const statetype s_mechachase3         = {ro_no,SPR_MECHA_W3,10,(statefunc)T_Chase,(statefunc)A_MechaSound,&s_mechachase3s};
const statetype s_mechachase3s        = {ro_no,SPR_MECHA_W3,6,NULL,NULL,&s_mechachase4};
const statetype s_mechachase4         = {ro_no,SPR_MECHA_W4,8,(statefunc)T_Chase,NULL,&s_mechachase1};

const statetype s_mechadie1           = {ro_no,SPR_MECHA_DIE1,10,NULL,(statefunc)A_DeathScream,&s_mechadie2};
const statetype s_mechadie2           = {ro_no,SPR_MECHA_DIE2,10,NULL,NULL,&s_mechadie3};
const statetype s_mechadie3           = {ro_no,SPR_MECHA_DIE3,10,NULL,(statefunc)A_HitlerMorph,&s_mechadie4};
const statetype s_mechadie4           = {ro_no,SPR_MECHA_DEAD,0,NULL,NULL,&s_mechadie4};

const statetype s_mechashoot1         = {ro_no,SPR_MECHA_SHOOT1,30,NULL,NULL,&s_mechashoot2};
const statetype s_mechashoot2         = {ro_no,SPR_MECHA_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_mechashoot3};
const statetype s_mechashoot3         = {ro_no,SPR_MECHA_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_mechashoot4};
const statetype s_mechashoot4         = {ro_no,SPR_MECHA_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_mechashoot5};
const statetype s_mechashoot5         = {ro_no,SPR_MECHA_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_mechashoot6};
const statetype s_mechashoot6         = {ro_no,SPR_MECHA_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_mechachase1};


const statetype s_hitlerchase1        = {ro_no,SPR_HITLER_W1,6,(statefunc)T_Chase,NULL,&s_hitlerchase1s};
const statetype s_hitlerchase1s       = {ro_no,SPR_HITLER_W1,4,NULL,NULL,&s_hitlerchase2};
const statetype s_hitlerchase2        = {ro_no,SPR_HITLER_W2,2,(statefunc)T_Chase,NULL,&s_hitlerchase3};
const statetype s_hitlerchase3        = {ro_no,SPR_HITLER_W3,6,(statefunc)T_Chase,NULL,&s_hitlerchase3s};
const statetype s_hitlerchase3s       = {ro_no,SPR_HITLER_W3,4,NULL,NULL,&s_hitlerchase4};
const statetype s_hitlerchase4        = {ro_no,SPR_HITLER_W4,2,(statefunc)T_Chase,NULL,&s_hitlerchase1};

const statetype s_hitlerdeathcam      = {ro_no,SPR_HITLER_W1,10,NULL,NULL,&s_hitlerdie1};

const statetype s_hitlerdie1          = {ro_no,SPR_HITLER_W1,1,NULL,(statefunc)A_DeathScream,&s_hitlerdie2};
const statetype s_hitlerdie2          = {ro_no,SPR_HITLER_W1,140,NULL,NULL,&s_hitlerdie3};
const statetype s_hitlerdie3          = {ro_no,SPR_HITLER_DIE1,10,NULL,(statefunc)A_Slurpie,&s_hitlerdie4};
const statetype s_hitlerdie4          = {ro_no,SPR_HITLER_DIE2,10,NULL,NULL,&s_hitlerdie5};
const statetype s_hitlerdie5          = {ro_no,SPR_HITLER_DIE3,10,NULL,NULL,&s_hitlerdie6};
const statetype s_hitlerdie6          = {ro_no,SPR_HITLER_DIE4,10,NULL,NULL,&s_hitlerdie7};
const statetype s_hitlerdie7          = {ro_no,SPR_HITLER_DIE5,10,NULL,NULL,&s_hitlerdie8};
const statetype s_hitlerdie8          = {ro_no,SPR_HITLER_DIE6,10,NULL,NULL,&s_hitlerdie9};
const statetype s_hitlerdie9          = {ro_no,SPR_HITLER_DIE7,10,NULL,NULL,&s_hitlerdie10};
const statetype s_hitlerdie10         = {ro_no,SPR_HITLER_DEAD,20,NULL,(statefunc)A_StartDeathCam,&s_hitlerdie10};

const statetype s_hitlershoot1        = {ro_no,SPR_HITLER_SHOOT1,30,NULL,NULL,&s_hitlershoot2};
const statetype s_hitlershoot2        = {ro_no,SPR_HITLER_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_hitlershoot3};
const statetype s_hitlershoot3        = {ro_no,SPR_HITLER_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_hitlershoot4};
const statetype s_hitlershoot4        = {ro_no,SPR_HITLER_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_hitlershoot5};
const statetype s_hitlershoot5        = {ro_no,SPR_HITLER_SHOOT3,10,NULL,(statefunc)T_Shoot,&s_hitlershoot6};
const statetype s_hitlershoot6        = {ro_no,SPR_HITLER_SHOOT2,10,NULL,(statefunc)T_Shoot,&s_hitlerchase1};



/*
===============
=
= SpawnFakeHitler
=
===============
*/

void SpawnFakeHitler (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_fakestand);
    newobj->speed = SPDPATROL;

    newobj->obclass = fakeobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_fake];
    newobj->dir = nodir;
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
===============
=
= SpawnHitler
=
===============
*/

void SpawnHitler (int tilex, int tiley)
{
    SpawnNewObj (tilex,tiley,&s_mechastand);
    newobj->speed = SPDPATROL;

    newobj->obclass = mechahitlerobj;
    newobj->hitpoints = starthitpoints[gamestate.difficulty][en_hitler];
    newobj->dir = nodir;
    newobj->flags |= FL_SHOOTABLE|FL_AMBUSH;
    if (!loadedgame)
        gamestate.killtotal++;
}


/*
===============
=
= A_HitlerMorph
=
===============
*/

void A_HitlerMorph (objtype *ob)
{
    short hitpoints[4]={500,700,800,900};

    SpawnNewObj (ob->tilex,ob->tiley,&s_hitlerchase1);
    newobj->speed = SPDPATROL*5;

    newobj->x = ob->x;
    newobj->y = ob->y;

    newobj->distance = ob->distance;
    newobj->dir = ob->dir;
    newobj->flags = ob->flags | FL_SHOOTABLE;
    newobj->flags &= ~FL_NONMARK;   // hitler stuck with nodir fix

    newobj->obclass = realhitlerobj;
    newobj->hitpoints = hitpoints[gamestate.difficulty];
}


////////////////////////////////////////////////////////
//
// A_MechaSound
// A_Slurpie
//
////////////////////////////////////////////////////////
void A_MechaSound (objtype *ob)
{
    if (areabyplayer[ob->areanumber])
        PlaySoundLocActor (MECHSTEPSND,ob);
}

void A_Slurpie (objtype *ob)
{
    (void) ob;
    SD_PlaySound(SLURPIESND);
}

/*
=================
=
= T_FakeFire
=
=================
*/

void T_FakeFire (objtype *ob)
{
    int32_t deltax,deltay;
    float   angle;
    int     iangle;

    if (!objfreelist)       // stop shooting if over MAXACTORS
    {
        NewState (ob,&s_fakechase1);
        return;
    }

    deltax = player->x - ob->x;
    deltay = ob->y - player->y;
    angle = (float) atan2((float) deltay, (float) deltax);
    if (angle<0)
        angle = (float)(M_PI*2+angle);
    iangle = (int) (angle/(M_PI*2)*ANGLES);

    GetNewActor ();
    newobj->state = &s_fire1;
    newobj->ticcount = 1;

    newobj->tilex = ob->tilex;
    newobj->tiley = ob->tiley;
    newobj->x = ob->x;
    newobj->y = ob->y;
    newobj->dir = nodir;
    newobj->angle = iangle;
    newobj->obclass = fireobj;
    newobj->speed = 0x1200l;
    newobj->flags = FL_NEVERMARK;
    newobj->active = ac_yes;

    PlaySoundLocActor (FLAMETHROWERSND,newobj);
}



/*
=================
=
= T_Fake
=
=================
*/

void T_Fake (objtype *ob)
{
    int32_t move;

    if (CheckLine(ob))                      // got a shot at player?
    {
        ob->hidden = false;
        if ( (unsigned) US_RndT() < (tics<<1) && objfreelist)
        {
            //
            // go into attack frame
            //
            NewState (ob,&s_fakeshoot1);
            return;
        }
    }
    else
        ob->hidden = true;

    if (ob->dir == nodir)
    {
        SelectDodgeDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        SelectDodgeDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }
}

#endif
/*
============================================================================

STAND

============================================================================
*/


/*
===============
=
= T_Stand
=
===============
*/

void T_Stand (objtype *ob)
{
    SightPlayer (ob);
}


/*
============================================================================

CHASE

============================================================================
*/

/*
=================
=
= T_Chase
=
=================
*/

void T_Chase (objtype *ob)
{
    int32_t move,target;
    int     dx,dy,dist,chance;
    bool dodge;

    if (gamestate.victoryflag)
        return;

    dodge = false;
    if (CheckLine(ob))      // got a shot at player?
    {
        ob->hidden = false;
        dx = abs(ob->tilex - player->tilex);
        dy = abs(ob->tiley - player->tiley);
        dist = dx>dy ? dx : dy;

#ifdef PLAYDEMOLIKEORIGINAL
        if(DEMOCOND_ORIG)
        {
            if(!dist || (dist == 1 && ob->distance < 0x4000))
                chance = 300;
            else
                chance = (tics<<4)/dist;
        }
        else
#endif
        {
            if (dist)
                chance = (tics<<4)/dist;
            else
                chance = 300;

            if (dist == 1)
            {
                target = abs(ob->x - player->x);
                if (target < 0x14000l)
                {
                    target = abs(ob->y - player->y);
                    if (target < 0x14000l)
                        chance = 300;
                }
            }
        }

        if ( US_RndT()<chance)
        {
            //
            // go into attack frame
            //
            switch (ob->obclass)
            {
                case guardobj:
                    NewState (ob,&s_grdshoot1);
                    break;
                case officerobj:
                    NewState (ob,&s_ofcshoot1);
                    break;
                case mutantobj:
                    NewState (ob,&s_mutshoot1);
                    break;
                case ssobj:
                    NewState (ob,&s_ssshoot1);
                    break;
#ifndef SPEAR
                case bossobj:
                    NewState (ob,&s_bossshoot1);
                    break;
                case gretelobj:
                    NewState (ob,&s_gretelshoot1);
                    break;
                case mechahitlerobj:
                    NewState (ob,&s_mechashoot1);
                    break;
                case realhitlerobj:
                    NewState (ob,&s_hitlershoot1);
                    break;
#else
                case angelobj:
                    NewState (ob,&s_angelshoot1);
                    break;
                case transobj:
                    NewState (ob,&s_transshoot1);
                    break;
                case uberobj:
                    NewState (ob,&s_ubershoot1);
                    break;
                case willobj:
                    NewState (ob,&s_willshoot1);
                    break;
                case deathobj:
                    NewState (ob,&s_deathshoot1);
                    break;
#endif
                default:
                    break;
            }
            return;
        }
        dodge = true;
    }
    else
        ob->hidden = true;

    if (ob->dir == nodir)
    {
        if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        if (ob->distance < 0)
        {
            //
            // waiting for a door to open
            //
            OpenDoor (-ob->distance-1);
            if (doorobjlist[-ob->distance-1].action != dr_open)
                return;
            ob->distance = TILEGLOBAL;      // go ahead, the door is now open
            DEMOIF_SDL
            {
                TryWalk(ob);
            }
        }

        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        if (dodge)
            SelectDodgeDir (ob);
        else
            SelectChaseDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }
}


/*
=================
=
= T_Ghosts
=
=================
*/

void T_Ghosts (objtype *ob)
{
    int32_t move;

    if (ob->dir == nodir)
    {
        SelectChaseDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        SelectChaseDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }
}

/*
=================
=
= T_DogChase
=
=================
*/

void T_DogChase (objtype *ob)
{
    int32_t    move;
    int32_t    dx,dy;


    if (ob->dir == nodir)
    {
        SelectDodgeDir (ob);
        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }

    move = ob->speed*tics;

    while (move)
    {
        //
        // check for byte range
        //
        dx = player->x - ob->x;
        if (dx<0)
            dx = -dx;
        dx -= move;
        if (dx <= MINACTORDIST)
        {
            dy = player->y - ob->y;
            if (dy<0)
                dy = -dy;
            dy -= move;
            if (dy <= MINACTORDIST)
            {
                NewState (ob,&s_dogjump1);
                return;
            }
        }

        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        //
        // reached goal tile, so select another one
        //

        //
        // fix position to account for round off during moving
        //
        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

        move -= ob->distance;

        SelectDodgeDir (ob);

        if (ob->dir == nodir)
            return;                                                 // object is blocked in
    }
}



/*
============================================================================

                                    PATH

============================================================================
*/


/*
===============
=
= SelectPathDir
=
===============
*/

void SelectPathDir (objtype *ob)
{
    unsigned spot;

    spot = MAPSPOT(ob->tilex,ob->tiley,1)-ICONARROWS;

    if (spot<8)
    {
        // new direction
        ob->dir = (dirtype)(spot);
    }

    ob->distance = TILEGLOBAL;

    if (!TryWalk (ob))
        ob->dir = nodir;
}


/*
===============
=
= T_Path
=
===============
*/

void T_Path (objtype *ob)
{
    int32_t    move;

    if (SightPlayer (ob))
        return;

    if (ob->dir == nodir)
    {
        SelectPathDir (ob);
        if (ob->dir == nodir)
            return;                                 // all movement is blocked
    }


    move = ob->speed*tics;

    while (move)
    {
        if (ob->distance < 0)
        {
            //
            // waiting for a door to open
            //
            OpenDoor (-ob->distance-1);
            if (doorobjlist[-ob->distance-1].action != dr_open)
                return;
            ob->distance = TILEGLOBAL;      // go ahead, the door is now open
            DEMOIF_SDL
            {
                TryWalk(ob);
            }
        }

        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }

        if (ob->tilex>MAPSIZE || ob->tiley>MAPSIZE)
        {
            char str[80];
            sprintf (str, "T_Path hit a wall at %u,%u, dir %u",
                ob->tilex,ob->tiley,ob->dir);
            Quit (str);
        }

        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;
        move -= ob->distance;

        SelectPathDir (ob);

        if (ob->dir == nodir)
            return;                                 // all movement is blocked
    }
}


/*
=============================================================================

                                    FIGHT

=============================================================================
*/


/*
===============
=
= T_Shoot
=
= Try to damage the player, based on skill level and player's speed
=
===============
*/

void T_Shoot (objtype *ob)
{
    int     dx,dy,dist;
    int     hitchance,damage;

    hitchance = 128;

    if (!areabyplayer[ob->areanumber])
        return;

    if (CheckLine (ob))                    // player is not behind a wall
    {
        dx = abs(ob->tilex - player->tilex);
        dy = abs(ob->tiley - player->tiley);
        dist = dx>dy ? dx:dy;

        if (ob->obclass == ssobj || ob->obclass == bossobj)
            dist = dist*2/3;                                        // ss are better shots

        if (thrustspeed >= RUNSPEED)
        {
            if (ob->flags&FL_VISABLE)
                hitchance = 160-dist*16;                // player can see to dodge
            else
                hitchance = 160-dist*8;
        }
        else
        {
            if (ob->flags&FL_VISABLE)
                hitchance = 256-dist*16;                // player can see to dodge
            else
                hitchance = 256-dist*8;
        }

        // see if the shot was a hit

        if (US_RndT()<hitchance)
        {
            if (dist<2)
                damage = US_RndT()>>2;
            else if (dist<4)
                damage = US_RndT()>>3;
            else
                damage = US_RndT()>>4;

            TakeDamage (damage,ob);
        }
    }

    switch(ob->obclass)
    {
        case ssobj:
            PlaySoundLocActor(SSFIRESND,ob);
            break;
#ifndef SPEAR
#ifndef APOGEE_1_0
        case giftobj:
        case fatobj:
            PlaySoundLocActor(MISSILEFIRESND,ob);
            break;
#endif
        case mechahitlerobj:
        case realhitlerobj:
        case bossobj:
            PlaySoundLocActor(BOSSFIRESND,ob);
            break;
        case schabbobj:
            PlaySoundLocActor(SCHABBSTHROWSND,ob);
            break;
        case fakeobj:
            PlaySoundLocActor(FLAMETHROWERSND,ob);
            break;
#endif
        default:
            PlaySoundLocActor(NAZIFIRESND,ob);
    }
}


/*
===============
=
= T_Bite
=
===============
*/

void T_Bite (objtype *ob)
{
    int32_t    dx,dy;

    PlaySoundLocActor(DOGATTACKSND,ob);     // JAB

    dx = player->x - ob->x;
    if (dx<0)
        dx = -dx;
    dx -= TILEGLOBAL;
    if (dx <= MINACTORDIST)
    {
        dy = player->y - ob->y;
        if (dy<0)
            dy = -dy;
        dy -= TILEGLOBAL;
        if (dy <= MINACTORDIST)
        {
            if (US_RndT()<180)
            {
                TakeDamage (US_RndT()>>4,ob);
                return;
            }
        }
    }
}


#ifndef SPEAR
/*
============================================================================

                                    BJ VICTORY

============================================================================
*/


//
// BJ victory
//

void T_BJRun (objtype *ob);
void T_BJJump (objtype *ob);
void T_BJDone (objtype *ob);
void T_BJYell (objtype *ob);

void T_DeathCam (objtype *ob);

extern  const statetype s_bjrun1;
extern  const statetype s_bjrun1s;
extern  const statetype s_bjrun2;
extern  const statetype s_bjrun3;
extern  const statetype s_bjrun3s;
extern  const statetype s_bjrun4;

extern  const statetype s_bjjump1;
extern  const statetype s_bjjump2;
extern  const statetype s_bjjump3;
extern  const statetype s_bjjump4;


const statetype s_bjrun1              = {ro_no,SPR_BJ_W1,12,(statefunc)T_BJRun,NULL,&s_bjrun1s};
const statetype s_bjrun1s             = {ro_no,SPR_BJ_W1,3, NULL,NULL,&s_bjrun2};
const statetype s_bjrun2              = {ro_no,SPR_BJ_W2,8,(statefunc)T_BJRun,NULL,&s_bjrun3};
const statetype s_bjrun3              = {ro_no,SPR_BJ_W3,12,(statefunc)T_BJRun,NULL,&s_bjrun3s};
const statetype s_bjrun3s             = {ro_no,SPR_BJ_W3,3, NULL,NULL,&s_bjrun4};
const statetype s_bjrun4              = {ro_no,SPR_BJ_W4,8,(statefunc)T_BJRun,NULL,&s_bjrun1};


const statetype s_bjjump1             = {ro_no,SPR_BJ_JUMP1,14,(statefunc)T_BJJump,NULL,&s_bjjump2};
const statetype s_bjjump2             = {ro_no,SPR_BJ_JUMP2,14,(statefunc)T_BJJump,(statefunc)T_BJYell,&s_bjjump3};
const statetype s_bjjump3             = {ro_no,SPR_BJ_JUMP3,14,(statefunc)T_BJJump,NULL,&s_bjjump4};
const statetype s_bjjump4             = {ro_no,SPR_BJ_JUMP4,300,NULL,(statefunc)T_BJDone,&s_bjjump4};


const statetype s_deathcam            = {false,0,0,NULL,NULL,NULL};


/*
===============
=
= SpawnBJVictory
=
===============
*/

void SpawnBJVictory (void)
{
    SpawnNewObj (player->tilex,player->tiley+1,&s_bjrun1);
    newobj->x = player->x;
    newobj->y = player->y;
    newobj->obclass = bjobj;
    newobj->dir = north;
    newobj->temp1 = 6;                      // tiles to run forward
}



/*
===============
=
= T_BJRun
=
===============
*/

void T_BJRun (objtype *ob)
{
    int32_t    move;

    move = BJRUNSPEED*tics;

    while (move)
    {
        if (move < ob->distance)
        {
            MoveObj (ob,move);
            break;
        }


        ob->x = ((int32_t)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
        ob->y = ((int32_t)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;
        move -= ob->distance;

        SelectPathDir (ob);

        if ( !(--ob->temp1) )
        {
            NewState (ob,&s_bjjump1);
            return;
        }
    }
}


/*
===============
=
= T_BJJump
=
===============
*/

void T_BJJump (objtype *ob)
{
    int32_t    move;

    move = BJJUMPSPEED*tics;
    MoveObj (ob,move);
}


/*
===============
=
= T_BJYell
=
===============
*/

void T_BJYell (objtype *ob)
{
    PlaySoundLocActor(YEAHSND,ob);  // JAB
}


/*
===============
=
= T_BJDone
=
===============
*/

void T_BJDone (objtype *ob)
{
    (void) ob;
    playstate = ex_victorious;                              // exit castle tile
}



//===========================================================================


/*
===============
=
= CheckPosition
=
===============
*/

bool CheckPosition (objtype *ob)
{
    int     x,y,xl,yl,xh,yh;
    uint8_t av;

    xl = (ob->x-PLAYERSIZE) >> TILESHIFT;
    yl = (ob->y-PLAYERSIZE) >> TILESHIFT;

    xh = (ob->x+PLAYERSIZE) >> TILESHIFT;
    yh = (ob->y+PLAYERSIZE) >> TILESHIFT;

    //
    // check for solid walls
    //
    for (y=yl;y<=yh;y++)
    {
        for (x=xl;x<=xh;x++)
        {
            av = actorat[x][y];
            if (av && !ACTORAT_IS_OBJLIST_IDX(av))
                return false;
        }
    }

    return true;
}


/*
===============
=
= A_StartDeathCam
=
===============
*/

void    A_StartDeathCam (objtype *ob)
{
    int32_t dx,dy;
    float   fangle;
    int32_t xmove,ymove;
    int32_t dist;

    FinishPaletteShifts ();

    VW_WaitVBL (100);

    if (gamestate.victoryflag)
    {
        playstate = ex_victorious;                              // exit castle tile
        return;
    }

    VH_UpdateScreen();

    gamestate.victoryflag = true;
    unsigned fadeheight = viewsize != 21 ? screenHeight-STATUSLINES : screenHeight;
    VL_Bar (0, 0, screenWidth, fadeheight, bordercol);
    FizzleFade(0, 0, screenWidth, fadeheight, 70, false);

    if (bordercol != VIEWCOLOR)
    {
        fontnumber = 1;
        SETFONTCOLOR(15,bordercol);
        PrintX = 68; PrintY = 45;
        US_Print (STR_SEEAGAIN);
    }
    else
    {
//        CacheLump(LEVELEND_LUMP_START,LEVELEND_LUMP_END);
#ifdef JAPAN
#ifndef JAPDEMO
        CA_CacheScreen(C_LETSSEEPIC);
#endif
#else
        Write(0,7,STR_SEEAGAIN);
#endif
    }

    VW_UpdateScreen ();
    VH_UpdateScreen();

    IN_UserInput(300);

    //
    // line angle up exactly
    //
    NewState (player,&s_deathcam);

    player->x = gamestate.killx;
    player->y = gamestate.killy;

    dx = ob->x - player->x;
    dy = player->y - ob->y;

    fangle = (float) atan2((float) dy, (float) dx);          // returns -pi to pi
    if (fangle<0)
        fangle = (float) (M_PI*2+fangle);

    player->angle = (short) (fangle/(M_PI*2)*ANGLES);

    //
    // try to position as close as possible without being in a wall
    //
    dist = 0x14000l;
    do
    {
        xmove = FixedMul(dist,costable[player->angle]);
        ymove = -FixedMul(dist,sintable[player->angle]);

        player->x = ob->x - xmove;
        player->y = ob->y - ymove;
        dist += 0x1000;

    } while (!CheckPosition (player));
    plux = (uint16_t)(player->x >> UNSIGNEDSHIFT);                      // scale to fit in unsigned
    pluy = (uint16_t)(player->y >> UNSIGNEDSHIFT);
    player->tilex = (uint16_t)(player->x >> TILESHIFT);         // scale to tile values
    player->tiley = (uint16_t)(player->y >> TILESHIFT);

    //
    // go back to the game
    //

    DrawPlayBorder ();

    fizzlein = true;

    switch (ob->obclass)
    {
#ifndef SPEAR
        case schabbobj:
            NewState (ob,&s_schabbdeathcam);
            break;
        case realhitlerobj:
            NewState (ob,&s_hitlerdeathcam);
            break;
        case giftobj:
            NewState (ob,&s_giftdeathcam);
            break;
        case fatobj:
            NewState (ob,&s_fatdeathcam);
            break;
#endif
        default:
            break;
    }
}

#endif
