/*
  embedul.ar™ embedded systems framework - http://embedul.ar
  
  ad-hoc porting layer to the embedul.ar framework.

  Copyright 2018-2022 Santiago Germino
  <sgermino@embedul.ar> https://www.linkedin.com/in/royconejo

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "embedul.ar/source/core/device/board.h"


#define USE_FMOPL

#ifdef USE_FMOPL
    #include "embedul.ar/source/core/device/sound/opl/fmopl.h"

    #define OPL_CHIP        0
    #define alOut(n,b)      FMOPL_YM3812_WriteReg (OPL_CHIP, n, b)
#else
    #include "embedul.ar/source/core/sound/nukedopl3.h"

    extern opl3_chip        g_opl3;
    #define alOut(n,b)      OPL3_WriteReg (&g_opl3, n, b)
#endif


#define MIX_CHANNELS                SOUND_SFX_SLOTS
#define MIX_SAMPLERATE              SOUND_MIXER_SAMPLE_RATE

#define RETROPORT_SCREEN_WIDTH      256
#define RETROPORT_SCREEN_HEIGHT     144


enum RETROPORT_UpdateFlags
{
    RETROPORT_UpdateFlags_CopyFrame     = 0x01,
    RETROPORT_UpdateFlags_SwapOverride  = 0x02
};


#define RETROPORT_CACHED_ELEMENT_SIGNON         8
#define RETROPORT_CACHED_ELEMENT_PG13           9
#define RETROPORT_CACHED_ELEMENT_TITLEPIC       10
#define RETROPORT_CACHED_ELEMENT_CREDITS        11


void RETROPORT_Init (void);
uint32_t RETROPORT_GetTicks (void);
void RETROPORT_Delay (uint32_t msec);
void RETROPORT_WaitVBL (uint32_t times);
void RETROPORT_FillFromStorage (uint32_t sector);
void RETROPORT_UpdateScreen (const enum RETROPORT_UpdateFlags UpdateFlags);
void RETROPORT_ClearBack (const uint8_t Color);
void RETROPORT_SetVGAPlaneMode (void);
uint8_t * RETROPORT_Frontbuffer (void);
uint8_t * RETROPORT_Backbuffer (void);
uint8_t * RETROPORT_BackbufferXY (const int32_t X, const int32_t Y);
void RETROPORT_StartBonusFlash (void);
void RETROPORT_StartDamageFlash (int damage);
void RETROPORT_UpdateFlash (void);
void RETROPORT_FinishPaletteShifts (void);
void RETROPORT_FadeOut (void);
void RETROPORT_FadeIn (void);
void RETROPORT_MenuFadeOut (void);
void RETROPORT_MenuFadeIn (void);
void RETROPORT_StatusDrawFace (uint32_t pic);
void RETROPORT_StatusDrawNumber (int x, int y, uint8_t number);
void RETROPORT_StatusDrawWeapon (int pic);
void RETROPORT_StatusDrawKeys (void);
bool RETROPORT_SD_Startup (void);
void RETROPORT_ProcessEvents (void);
void RETROPORT_WaitAndProcessEvents (void);
bool RETROPORT_PlayDigitized (int sound, int *channel);
void RETROPORT_SD_SetPosition (int channel, int leftpos, int rightpos);
void RETROPORT_StopDigitized (void);
