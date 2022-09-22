#pragma once

#include "wl_def.h"

/*
=============================================================================

                             WL_ACT2 DEFINITIONS

=============================================================================
*/

#define s_nakedbody s_static10

extern  const statetype s_grddie1;
extern  const statetype s_dogdie1;
extern  const statetype s_ofcdie1;
extern  const statetype s_mutdie1;
extern  const statetype s_ssdie1;
extern  const statetype s_bossdie1;
extern  const statetype s_schabbdie1;
extern  const statetype s_fakedie1;
extern  const statetype s_mechadie1;
extern  const statetype s_hitlerdie1;
extern  const statetype s_greteldie1;
extern  const statetype s_giftdie1;
extern  const statetype s_fatdie1;

extern  const statetype s_spectredie1;
extern  const statetype s_angeldie1;
extern  const statetype s_transdie0;
extern  const statetype s_uberdie0;
extern  const statetype s_willdie1;
extern  const statetype s_deathdie1;


extern  const statetype s_grdchase1;
extern  const statetype s_dogchase1;
extern  const statetype s_ofcchase1;
extern  const statetype s_sschase1;
extern  const statetype s_mutchase1;
extern  const statetype s_bosschase1;
extern  const statetype s_schabbchase1;
extern  const statetype s_fakechase1;
extern  const statetype s_mechachase1;
extern  const statetype s_gretelchase1;
extern  const statetype s_giftchase1;
extern  const statetype s_fatchase1;

extern  const statetype s_spectrechase1;
extern  const statetype s_angelchase1;
extern  const statetype s_transchase1;
extern  const statetype s_uberchase1;
extern  const statetype s_willchase1;
extern  const statetype s_deathchase1;

extern  const statetype s_blinkychase1;
extern  const statetype s_hitlerchase1;

extern  const statetype s_grdpain;
extern  const statetype s_grdpain1;
extern  const statetype s_ofcpain;
extern  const statetype s_ofcpain1;
extern  const statetype s_sspain;
extern  const statetype s_sspain1;
extern  const statetype s_mutpain;
extern  const statetype s_mutpain1;

extern  const statetype s_deathcam;

extern  const statetype s_schabbdeathcam2;
extern  const statetype s_hitlerdeathcam2;
extern  const statetype s_giftdeathcam2;
extern  const statetype s_fatdeathcam2;

void SpawnStand (enemy_t which, int tilex, int tiley, int dir);
void SpawnPatrol (enemy_t which, int tilex, int tiley, int dir);
void KillActor (objtype *ob);

void SpawnDeadGuard (int tilex, int tiley);
void SpawnBoss (int tilex, int tiley);
void SpawnGretel (int tilex, int tiley);
void SpawnTrans (int tilex, int tiley);
void SpawnUber (int tilex, int tiley);
void SpawnWill (int tilex, int tiley);
void SpawnDeath (int tilex, int tiley);
void SpawnAngel (int tilex, int tiley);
void SpawnSpectre (int tilex, int tiley);
void SpawnGhosts (int which, int tilex, int tiley);
void SpawnSchabbs (int tilex, int tiley);
void SpawnGift (int tilex, int tiley);
void SpawnFat (int tilex, int tiley);
void SpawnFakeHitler (int tilex, int tiley);
void SpawnHitler (int tilex, int tiley);

void A_DeathScream (objtype *ob);
void SpawnBJVictory (void);
