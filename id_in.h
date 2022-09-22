#pragma once
//
//	ID Engine
//	ID_IN.h - Header file for Input Manager
//	v1.0d1
//	By Jason Blochowiak
//

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    IN_Flags_None           = 0x0000,
    IN_Flags_KeyUp          = 0x0001,
    IN_Flags_KeyLeft        = 0x0002,
    IN_Flags_KeyRight       = 0x0004,
    IN_Flags_KeyDown        = 0x0008,
    IN_Flags_KeyAttack      = 0x0010,
    IN_Flags_KeyStrafe      = 0x0020,
    IN_Flags_KeyUse         = 0x0040,
    IN_Flags_KeyRun         = 0x0080,
    IN_Flags_KeyStrafeLeft  = 0x0100,
    IN_Flags_KeyStrafeRight = 0x0200,
    IN_Flags_KeyEsc         = 0x0400,
    IN_Flags_KeyPause       = 0x0800,
    IN_Flags_KeyPrevWeapon  = 0x1000,
    IN_Flags_KeyNextWeapon  = 0x2000
} IN_Flags;


typedef	enum
{
    demo_Off,demo_Record,demo_Playback,demo_PlayDone
}
Demo;


typedef	enum
{
    motion_Left = -1,motion_Up = -1,
    motion_None = 0,
    motion_Right = 1,motion_Down = 1
}
Motion;
typedef int8_t Motion8;


typedef	enum
{
    dir_North,dir_NorthEast,
    dir_East,dir_SouthEast,
    dir_South,dir_SouthWest,
    dir_West,dir_NorthWest,
    dir_None
}
Direction;
typedef uint8_t Direction8;


typedef	struct
{
    union {
        uint8_t button0;
        uint8_t btAttack;
    };
    union {
        uint8_t button1;
        uint8_t btEsc;
    };
    union {
        uint8_t button2;
        uint8_t btUse;
    };
    union {
        uint8_t button3;
        uint8_t btRun;
    };

    int16_t		x,y;
    Motion8		xaxis,yaxis;
    Direction8	dir;
}
CursorInfo;
typedef	CursorInfo	ControlInfo;


// Global variables
extern           IN_Flags   InputStatus;
extern  volatile bool       Paused;

// Function prototypes
bool    IN_KeyDown (IN_Flags flags);
void    IN_ClearKey (IN_Flags flags);

void	IN_Startup(void);
void    IN_Shutdown(void);
void	IN_ReadControl(ControlInfo *);
void	IN_StopDemo(void);
void    IN_FreeDemoBuffer(void);
void    IN_Ack(void);
bool    IN_UserInput(uint32_t delay);

void    IN_WaitAndProcessEvents(void);
void    IN_ProcessEvents(void);

void    IN_StartAck(void);
bool    IN_CheckAck(void);
