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

#include "retroport.h"
#include "embedul.ar/source/core/device/video.h"
#include "embedul.ar/source/core/manager/screen.h"
#include "embedul.ar/source/core/manager/screen/fade.h"
#include "embedul.ar/source/core/manager/screen/tilemap.h"
#include "embedul.ar/source/core/manager/storage.h"
#include "embedul.ar/source/core/manager/storage/cache.h"

#include <assert.h>
#include <string.h>

#include "wl_def.h"
#include "wl_game.h"
#include "id_sd.h"
#include "id_in.h"
#include "resources.h"

#include "assets/source/TILEMAPS_wolf4rom.h"
#include "assets/source/SFX_wolf4rom.h"


#ifndef USE_FMOPL
    opl3_chip           g_opl3;
#endif

static struct SCREEN_FADE   s_fade;
static uint8_t              s_scanlines = 0;
static uint8_t              s_bitops    = 0;


void RETROPORT_Init (void)
{
    // MARQUEE_HueWheel (2000);
}


uint32_t RETROPORT_GetTicks (void)
{
    return (uint32_t) TICKS_Now ();
}


void RETROPORT_Delay (uint32_t msec)
{
    (void) msec;

    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    VIDEO_SwapOverride (C->driver);
    BOARD_Sync ();
}


void RETROPORT_WaitVBL (uint32_t times)
{
    (void) times;

    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    VIDEO_SwapOverride (C->driver);
    BOARD_Sync ();
}


void RETROPORT_FillFromStorage (const uint32_t CachedElement)
{
    SCREEN_BlitCachedElement (SCREEN_Role_Primary, CachedElement);
}


void RETROPORT_UpdateScreen (const enum RETROPORT_UpdateFlags UpdateFlags)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    if (UpdateFlags & RETROPORT_UpdateFlags_CopyFrame)
    {
        VIDEO_CopyFrame (C->driver);
    }

    if (UpdateFlags & RETROPORT_UpdateFlags_SwapOverride)
    {

        VIDEO_SwapOverride (C->driver);
    }

    BOARD_Sync ();
}


void RETROPORT_ClearBack (const uint8_t Color)
{
    SCREEN_ClearBack (SCREEN_Role_Primary, Color);
}


// SEL 4 (so)

void RETROPORT_SetVGAPlaneMode (void)
{    
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    VIDEO_SetWaitVBICount (C->driver, 1);
}


uint8_t * RETROPORT_Frontbuffer (void)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    return C->driver->frontbuffer;
}


uint8_t * RETROPORT_Backbuffer (void)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    return C->driver->backbuffer;
}


uint8_t * RETROPORT_BackbufferXY (const int32_t X, const int32_t Y)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    return SCREEN_Context__backbufferXY (C, X, Y);
}


// Indirectly Called from DrawPlayScreen or from DrawFace.
void RETROPORT_StatusDrawFace (uint32_t pic)
{
    uint16_t anim;
    uint16_t frame;

    if (pic >= FACE1APIC && pic <= FACE7CPIC)
    {
        anim    = TILEMAP_hud_face_INDEX;
        frame   = pic - FACE1APIC;
    }
    else if (pic == FACE8APIC)
    {
        anim    = TILEMAP_hud_dead_INDEX;
        frame   = 0;
    }
    else if (pic == GOTGATLINGPIC)
    {
        anim    = TILEMAP_hud_hehe_INDEX;
        frame   = 0;
    }
    else if (pic == MUTANTBJPIC)
    {
        anim    = TILEMAP_hud_mutant_INDEX;
        frame   = 0;
    }
    else
    {
        return;
    }

    SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, 4, 108,
                         anim, frame,
                         NULL, NULL, NULL);
}


void RETROPORT_StatusDrawNumber (int x, int y, uint8_t number)
{
    SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, x, y,
                         TILEMAP_hud_numbers_INDEX, number,
                         NULL, NULL, NULL);
}


void RETROPORT_StatusDrawWeapon (int pic)
{
    switch (pic)
    {
        case KNIFEPIC:
            SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, 205, 116,
                                 TILEMAP_hud_knife_INDEX, 0,
                                 NULL, NULL, NULL);
            break;

        case GUNPIC:
            SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, 208, 113,
                                 TILEMAP_hud_gun_INDEX, 0,
                                 NULL, NULL, NULL);
            break;

        case MACHINEGUNPIC:
            SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, 205, 119,
                                 TILEMAP_hud_machinegun_INDEX, 0,
                                 NULL, NULL, NULL);
            break;

        case GATLINGGUNPIC:
            SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, 204, 114,
                                 TILEMAP_hud_gatlinggun_INDEX, 0,
                                 NULL, NULL, NULL);
            break;
    }
}


void RETROPORT_StatusDrawKeys (void)
{
    if (gamestate.keys & 1)
    {
        SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, 187, 125,
                             TILEMAP_hud_keygold_INDEX, 0,
                             NULL, NULL, NULL);
    }

    if (gamestate.keys & 2)
    {
        SCREEN_TILEMAP_Draw (SCREEN_Role_Primary, &TILEMAP_hud, 195, 125,
                             TILEMAP_hud_keysilver_INDEX, 0,
                             NULL, NULL, NULL);
    }
}


static int      numreadysamples     = 0;
static int      samplesPerMusicTick;
static uint8_t  * curAlSound        = 0;
static uint8_t  * curAlSoundPtr     = 0;
static uint32_t curAlLengthLeft     = 0;
static int      soundTimeCounter    = 5;


static bool wlProcBGMFunc (struct SOUND *a, uint8_t *buffer, void *procData)
{
    (void) a;
    (void) procData;

    int stereolen = SOUND_MIXER_BUFFER_SIZE >> 1;
    int sampleslen = stereolen >> 1;
    int16_t *stream16 = (int16_t *) (void *) buffer;    // expect correct alignment

    while(1)
    {
        if(numreadysamples)
        {
            if(numreadysamples<sampleslen)
            {
            #ifdef USE_FMOPL
                FMOPL_YM3812_Update (OPL_CHIP, stream16, numreadysamples, true);
            #else
                OPL3_GenerateStream (&g_opl3, stream16, numreadysamples);
            #endif
                stream16 += numreadysamples*2;
                sampleslen -= numreadysamples;
            }
            else
            {
            #ifdef USE_FMOPL
                FMOPL_YM3812_Update (OPL_CHIP, stream16, sampleslen, true);
            #else
                OPL3_GenerateStream (&g_opl3, stream16, sampleslen);
            #endif
                numreadysamples -= sampleslen;
                return true;
            }
        }
        soundTimeCounter--;
        if(!soundTimeCounter)
        {
            soundTimeCounter = 5;
            if(curAlSound != alSound)
            {
                curAlSound = curAlSoundPtr = alSound;
                curAlLengthLeft = alLengthLeft;
            }
            if(curAlSound)
            {
                if(*curAlSoundPtr)
                {
                    alOut(alFreqL, *curAlSoundPtr);
                    alOut(alFreqH, alBlock);
                }
                else alOut(alFreqH, 0);
                curAlSoundPtr++;
                curAlLengthLeft--;
                if(!curAlLengthLeft)
                {
                    curAlSound = alSound = 0;
                    SoundNumber = (soundnames) 0;
                    SoundPriority = 0;
                    alOut(alFreqH, 0);
                }
            }
        }
        if(sqActive)
        {
            do
            {
                if(sqHackTime > alTimeCount) break;
                sqHackTime = alTimeCount + *(sqHackPtr+1);
                alOut(*(uint8_t *) sqHackPtr, *(((uint8_t *) sqHackPtr)+1));
                sqHackPtr += 2;
                sqHackLen -= 4;
            }
            while(sqHackLen>0);
            alTimeCount++;
            if(!sqHackLen)
            {
                sqHackPtr = sqHack;
                sqHackLen = sqHackSeqLen;
                sqHackTime = 0;
                alTimeCount = 0;
            }
        }
        numreadysamples = samplesPerMusicTick;
    }

    return true;
}


void RETROPORT_StartBonusFlash (void)
{
    SCREEN_FADE_Flash (&s_fade, SCREEN_Role_Primary, SCREEN_FADE_Fx_Brighten,
                       SCREEN_FADE_PassFilter_Yellow, 96, 200, 0);
}


void RETROPORT_StartDamageFlash (int damage)
{
    if (damage >= 40)
    {
        damage = 256;
    }
    else
    {
        damage = 128;
    }

    SCREEN_FADE_Flash (&s_fade, SCREEN_Role_Primary, SCREEN_FADE_Fx_Brighten,
                       SCREEN_FADE_PassFilter_Red, (uint8_t)damage, 300, 0);

}


void RETROPORT_UpdateFlash (void)
{
    if (SCREEN_FADE_Pending (&s_fade))
    {
        SCREEN_FADE_Update (&s_fade);
    }
}


void RETROPORT_FinishPaletteShifts (void)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    SCREEN_FADE_Cancel (&s_fade);

    VIDEO_ResetScanlineOp (C->driver, VIDEO_SOP_ShowAnd);
    VIDEO_ResetScanlineOp (C->driver, VIDEO_SOP_ShowOr);
}


void RETROPORT_FadeOut (void)
{
    SCREEN_FADE_Out (&s_fade, SCREEN_Role_Primary, SCREEN_FADE_Fx_Darken, 
                     SCREEN_FADE_PassFilter_All, 100);

    while (SCREEN_FADE_Pending (&s_fade))
    {
        SCREEN_FADE_Update (&s_fade);
        RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);
    }
}


void RETROPORT_FadeIn (void)
{
    SCREEN_FADE_In (&s_fade, SCREEN_Role_Primary, SCREEN_FADE_Fx_Darken,
                    SCREEN_FADE_PassFilter_All, 100);

    while (SCREEN_FADE_Pending (&s_fade))
    {
        SCREEN_FADE_Update (&s_fade);
        RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);
    }
}


void RETROPORT_MenuFadeOut (void)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    VIDEO_SetScanlineOp (C->driver, VIDEO_SOP_ShowAnd, 0b11000101);
    SCREEN_FADE_Out (&s_fade, SCREEN_Role_Primary, SCREEN_FADE_Fx_Brighten,
                     SCREEN_FADE_PassFilter_Red, 70);

    // MARQUEE_Flash (255, 300, 500);

    while (SCREEN_FADE_Pending (&s_fade))
    {
        SCREEN_FADE_Update (&s_fade);
        RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);
    }
}


void RETROPORT_MenuFadeIn (void)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    SCREEN_FADE_In (&s_fade, SCREEN_Role_Primary, SCREEN_FADE_Fx_Brighten,
                    SCREEN_FADE_PassFilter_Red, 70);

    while (SCREEN_FADE_Pending (&s_fade))
    {
        SCREEN_FADE_Update (&s_fade);
        RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);
    }

    VIDEO_ResetScanlineOp (C->driver, VIDEO_SOP_ShowAnd);
}


bool RETROPORT_SD_Startup (void)
{
    // Init music (SDL_t0FastAsmService played at 700Hz)
    samplesPerMusicTick = MIX_SAMPLERATE / 700;

    AdLibPresent = true;
    SoundBlasterPresent = true;

    alTimeCount = 0;

    // Reserved slot for player weapons
    SOUND_ReserveSFX (0);

    SOUND_SetProcBGM (wlProcBGMFunc, 0);

    SOUND_Mute (false);

    #ifdef USE_FMOPL
    if (!FMOPL_YM3812_Init (OPL_CHIP, 3579545, MIX_SAMPLERATE))
    {
        LOG_WarnDebug (NOBJ, "unable to create virtual opl");
        return false;
    }

    FMOPL_YM3812_ResetChip (OPL_CHIP);

    for(uint8_t i = 1; i < 0xf6; i++)
    {
        FMOPL_YM3812_WriteReg (OPL_CHIP, i, 0);
    }

    FMOPL_YM3812_WriteReg (OPL_CHIP, 1, 0x20); // Set WSE=1
//    YM3812Write(0,8,0); // Set CSM=0 & SEL=0		 // already set in for statement
    #else
        OPL3_Reset (&g_opl3, SOUND_MIXER_SAMPLE_RATE);

        for (uint16_t i = 1; i < 0xf6; i++)
        {
            OPL3_WriteReg (&g_opl3, i, 0);
        }

        // Set WSE=1
        OPL3_WriteReg (&g_opl3, 1, 0x20);

        // Set CSM=0 & SEL=0 (already set in for statement)
        OPL3_WriteReg (&g_opl3, 8, 0);
    #endif

    return true;
}


static inline void wlInput (const enum INPUT_PROFILE_Group InProfileType,
                            const uint32_t InB, const IN_Flags InFlags)
{
    if (MIO_GetInputBuffer (InProfileType, IO_Type_Bit, InB))
    {
        InputStatus |= InFlags;
    }
    else
    {
        InputStatus &= ~InFlags;
    }
}


static void processInput (const struct SCREEN_Context *const C)
{
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_Up,
             IN_Flags_KeyUp);
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_Left,
             IN_Flags_KeyLeft);
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_Right,
             IN_Flags_KeyRight);
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_Down,
             IN_Flags_KeyDown);
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_A,
             IN_Flags_KeyAttack);
//  wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_?,
//           IN_Flags_KeyStrafe);
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_Start,
             IN_Flags_KeyUse);
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_B,
             IN_Flags_KeyRun);
    wlInput (INPUT_PROFILE_Group_GP1, INPUT_PROFILE_GP1_Bit_Select,
             IN_Flags_KeyEsc);

#if (LIB_EMBEDULAR_CONFIG_INPUT_ACTION == 1)
    if (MIO_CHECK_INPUT_BIT_ACTION (GP2, A, Clicked))
#else 
    if (MIO_GET_INPUT_BIT_BUFFER (GP2, A))
#endif
    {
        ++ s_scanlines;
        if (s_scanlines > 4)
        {
            s_scanlines = 0;
        }

        VIDEO_SetScanlines (C->driver, s_scanlines);
    }

#if (LIB_EMBEDULAR_CONFIG_INPUT_ACTION == 1)
    if (MIO_CHECK_INPUT_BIT_ACTION (GP2, B, Clicked))
#else
    if (MIO_GET_INPUT_BIT_BUFFER (GP2, B))
#endif
    {
        ++ s_bitops;
        if (s_bitops > 3)
        {
            s_bitops = 0;
        }

        switch (s_bitops)
        {
            case 0:
                VIDEO_ResetAllScanlineOps (C->driver);
                break;
            case 1:
                VIDEO_SetScanlineOp (C->driver, VIDEO_SOP_ShowAnd, 0b00011100);
                VIDEO_SetScanlineOp (C->driver, VIDEO_SOP_ShowOr, 0b01000001);
                break;
            case 2:
                VIDEO_SetScanlineOp (C->driver, VIDEO_SOP_ShowAnd, 0b11100000);
                VIDEO_SetScanlineOp (C->driver, VIDEO_SOP_ShowOr, 0b00001001);
                break;
            case 3:
                VIDEO_SetScanlineOp (C->driver, VIDEO_SOP_ShowAnd, 0b00011111);
                VIDEO_SetScanlineOp (C->driver, VIDEO_SOP_ShowOr, 0b00100000);
                break;
        }
    }
}


void RETROPORT_ProcessEvents (void)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    processInput (C);
}


// Always used in a loop until some condition is met
void RETROPORT_WaitAndProcessEvents (void)
{
    const struct SCREEN_Context *const C = 
                                SCREEN_GetContext(SCREEN_Role_Primary);

    processInput (C);

    VIDEO_SwapOverride  (C->driver);
    BOARD_Sync        ();
}


bool RETROPORT_PlayDigitized (int sound, int *channel)
{
    const struct SOUND_FX   *sfx        = NULL;
    SOUND_FX_SlotIndex      slotIndex   = SOUND_FX_SlotIndexNoOverlap;

    if (!channel)
    {
        return false;
    }

    // for reference see wolfdigimap in wl_main.c
    switch (sound)
    {
        case HALTSND:
            sfx = &SFX_SoundChunks_0;
            break;

        case DOGBARKSND:
            sfx = &SFX_SoundChunks_1;
            break;

        case CLOSEDOORSND:
            sfx = &SFX_SoundChunks_2;
            break;

        case OPENDOORSND:
            sfx = &SFX_SoundChunks_3;
            break;

        case ATKMACHINEGUNSND:
            sfx = &SFX_SoundChunks_4;
            slotIndex = 0;
            break;

        case ATKPISTOLSND:
            sfx = &SFX_SoundChunks_5;
            slotIndex = 0;
            break;

        case ATKGATLINGSND:
            sfx = &SFX_SoundChunks_6;
            slotIndex = 0;
            break;

        case SCHUTZADSND:
            sfx = &SFX_SoundChunks_7;
            break;

        case DEATHSCREAM1SND:
            sfx = &SFX_SoundChunks_12;
            break;

        case DEATHSCREAM2SND:
        case DEATHSCREAM3SND:
            sfx = &SFX_SoundChunks_13;
            break;

        case LEBENSND:
            sfx = &SFX_SoundChunks_20;
            break;

        case NAZIFIRESND:
            sfx = &SFX_SoundChunks_21;
            break;

        default:
            break;
    }

    if (!sfx)
    {
        return false;
    }

    if ((slotIndex = SOUND_PlaySFX(sfx, slotIndex)) < SOUND_SFX_SLOTS)
    {
        *channel = (int) slotIndex;
        return true;
    }

    return false;
}


void RETROPORT_SD_SetPosition (int channel, int leftpos, int rightpos)
{
    // left/right pos goes from 0 to 15 being 0 the closest position
    // (max volume)
    SOUND_SetSFXVolume ((SOUND_FX_SlotIndex)channel,
                        (uint8_t)((15-leftpos)*17),
                        (uint8_t)((15-rightpos)*17));
}


void RETROPORT_StopDigitized (void)
{
    SOUND_StopSFX ();
}
