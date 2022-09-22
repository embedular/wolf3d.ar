//
//	ID Engine
//	ID_IN.c - Input Manager
//	v1.0d1
//	By Jason Blochowiak
//

//
//	This module handles dealing with the various input devices
//
//	Depends on: Memory Mgr (for demo recording), Sound Mgr (for timing stuff),
//				User Mgr (for command line parms)
//
//	Globals:
//		LastScan - The keyboard scan code of the last key pressed
//		LastASCII - The ASCII value of the last key pressed
//	DEBUG - there are more globals
//

#include "id_in.h"
#include "id_sd.h"      // GetTimeCount()
#include "retroport.h"

/*
=============================================================================

					GLOBAL VARIABLES

=============================================================================
*/


// 	Global variables
         IN_Flags   InputStatus;
static   IN_Flags   StartAckInputStatus;
volatile bool       Paused;


/*
=============================================================================

					LOCAL VARIABLES

=============================================================================
*/

static	bool            IN_Started;

static const Direction8 DirTable[] =		// Quick lookup for total direction
{
    dir_NorthWest,	dir_North,	dir_NorthEast,
    dir_West,		dir_None,	dir_East,
    dir_SouthWest,	dir_South,	dir_SouthEast
};


void IN_WaitAndProcessEvents(void)
{
    RETROPORT_WaitAndProcessEvents ();
}

void IN_ProcessEvents(void)
{
    RETROPORT_ProcessEvents ();
}


bool IN_KeyDown (IN_Flags flags)
{
    return (InputStatus & flags);
}

void IN_ClearKey (IN_Flags flags)
{
    InputStatus &= ~flags;
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_Startup() - Starts up the Input Mgr
//
///////////////////////////////////////////////////////////////////////////
void
IN_Startup(void)
{
    if (IN_Started)
        return;

    IN_Started = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_Shutdown() - Shuts down the Input Mgr
//
///////////////////////////////////////////////////////////////////////////
void
IN_Shutdown(void)
{
	if (!IN_Started)
		return;

    IN_Started = false;
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_ReadControl() - Reads the device associated with the specified
//		player and fills in the control info struct
//
///////////////////////////////////////////////////////////////////////////
void
IN_ReadControl(ControlInfo *info)
{
	IN_ProcessEvents();

    const Motion8 My = (InputStatus & IN_Flags_KeyUp)? motion_Up :
                        (InputStatus & IN_Flags_KeyDown)? motion_Down :
                                                            motion_None;

    const Motion8 Mx = (InputStatus & IN_Flags_KeyLeft)? motion_Left :
                        (InputStatus & IN_Flags_KeyRight)? motion_Right :
                                                             motion_None;

    info->btAttack  = (InputStatus & IN_Flags_KeyAttack)? 1 : 0;
    info->btEsc     = (InputStatus & IN_Flags_KeyEsc)? 1 : 0;
    info->btUse     = (InputStatus & IN_Flags_KeyUse)? 1 : 0;
    info->btRun     = (InputStatus & IN_Flags_KeyRun)? 1 : 0;

    const int16_t Dx = Mx * 127;
    const int16_t Dy = My * 127;

	info->x         = Dx;
	info->xaxis     = Mx;
	info->y         = Dy;
	info->yaxis     = My;
	info->dir       = DirTable[((My + 1) * 3) + (Mx + 1)];
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_Ack() - waits for a button or key press.  If a button is down, upon
// calling, it must be released for it to be recognized
//
// sgermino: Functions reimplemented to handle direct gamepad status
//           stored on InputStatus.
///////////////////////////////////////////////////////////////////////////

void IN_StartAck (void)
{
    IN_ProcessEvents ();
    StartAckInputStatus = InputStatus;
}


bool IN_CheckAck (void)
{
    IN_ProcessEvents ();

    // A key not pressed in StartAck is now being pressed
    if (~StartAckInputStatus & InputStatus)
    {
        return true;
    }

    // An already pressed key is now being released
    if ((StartAckInputStatus & InputStatus) != StartAckInputStatus)
    {
        // Store released key(s) as the initial StartAck status
        StartAckInputStatus = InputStatus;
    }

	return false;
}


void IN_Ack (void)
{
	IN_StartAck ();

    do
    {
        IN_WaitAndProcessEvents ();
    }
	while (!IN_CheckAck ());
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_UserInput() - Waits for the specified delay time (in ticks) or the
//		user pressing a key or a mouse button. If the clear flag is set, it
//		then either clears the key or waits for the user to let the mouse
//		button up.
//
///////////////////////////////////////////////////////////////////////////
bool IN_UserInput(uint32_t delay)
{
	uint32_t	lasttime;

	lasttime = GetTimeCount();
	IN_StartAck ();
	do
	{
        IN_WaitAndProcessEvents ();
		if (IN_CheckAck ())
        {
			return true;
        }
	}
    while (GetTimeCount() - lasttime < delay);

	return false;
}

//===========================================================================

