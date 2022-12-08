/*
  embedul.ar™ embedded systems framework - http://embedul.ar
  
  entry point to the embedul.ar framework using RetrOS armv7 real-time
  preemtive operating system.

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

#include "embedul.ar/source/core/main.h"
#include "embedul.ar/source/core/mempool.h"
#include "embedul.ar/source/core/retros/api.h"


_Alignas(8) static uint8_t  s_Heap[4608];
static struct MEMPOOL       s_MempoolHeap;
static const char           * s_TaskMain    = "MAIN";
static const char           * s_TaskAudio   = "AUDIO";


// defined in wl_main.c
int wl_main (int argc, char *argv[]);


OS_TaskRetVal taskMain (OS_TaskParam param)
{
    (void) param;

    return (OS_TaskRetVal) wl_main (0, NULL);
}


OS_TaskRetVal taskAudio (OS_TaskParam param)
{
    (void) param;

    OS_TaskPeriodicDelay (0);
    while (1)
    {
        SOUND_Mix ();

        OS_TaskYield ();

        //OS_TaskPeriodicDelay (2);
    }

    return 0;
}


OS_TaskRetVal taskBoot (OS_TaskParam param)
{
    (void) param;

    // Seems to work "fine" with 1024. VBI sync not working.
    void *taskMainBuffer = MEMPOOL_Block (&s_MempoolHeap,
                                          OS_TaskBufferSize(1024),
                                          s_TaskMain);
    BOARD_AssertState (taskMainBuffer);

    void *taskAudioBuffer = MEMPOOL_Block (&s_MempoolHeap,
                                           MEMPOOL_BLOCKSIZE_REMAINS,
                                           s_TaskAudio);
    BOARD_AssertState (taskAudioBuffer);

    OS_TaskStart (taskMainBuffer, MEMPOOL_BlockSize(taskMainBuffer),
                  taskMain, NULL, OS_TaskPriority_Kernel2, s_TaskMain);

    OS_TaskStart (taskAudioBuffer, MEMPOOL_BlockSize(taskAudioBuffer),
                  taskAudio, NULL, OS_TaskPriority_Kernel2, s_TaskAudio);

    return 0;
}


void EMBEDULAR_Main (void *param)
{
    (void) param;

    MEMPOOL_Init (&s_MempoolHeap, (uintptr_t)s_Heap, sizeof(s_Heap));

    _Alignas(8) uint8_t retrosInitBuffer[OS_InitBufferSize()];

    const enum OS_Result OsInitResult = OS_Init (retrosInitBuffer);

    BOARD_AssertState (OsInitResult == OS_Result_OK);

    const enum OS_Result OsStartResult = OS_Start (taskBoot, NULL);

    BOARD_AssertState (OsStartResult == OS_Result_OK);
}
