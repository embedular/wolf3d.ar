#pragma once

#include "wl_game.h"
#include <stdio.h>

/*
=============================================================================

                             WL_MAIN DEFINITIONS

=============================================================================
*/

extern  bool     loadedgame;
extern  fixed    focallength;
extern  int      viewscreenx, viewscreeny;
extern  int      viewwidth;
extern  int      viewheight;
extern  short    centerx;
extern  int32_t  heightnumerator;
extern  fixed    scale;

extern  int      dirangle[9];

extern  int      mouseadjustment;
extern  int      shootdelta;
extern  unsigned screenofs;

extern  bool     startgame;
extern  char     str[80];
//extern  char     configdir[256];
//extern  char     configname[13];

//
// Command line parameter variables
//
extern  bool     param_debugmode;
extern  bool     param_nowait;
extern  int      param_difficulty;
extern  int      param_tedlevel;
extern  int      param_joystickindex;
extern  int      param_joystickhat;
extern  const int      param_samplerate;
extern  const int      param_audiobuffer;
extern  int      param_mission;
extern  bool     param_goodtimes;
extern  bool     param_ignorenumchunks;


void            NewGame (int difficulty,int episode);
void            CalcProjection (int32_t focal);
void            NewViewSize (int width);
bool            SetViewSize (unsigned width, unsigned height);
bool            LoadTheGame(FILE *file,int x,int y);
bool            SaveTheGame(FILE *file,int x,int y);
void            ShowViewSize (int width);
void            ShutdownId (void);
