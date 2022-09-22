#pragma once
//
//	ID Engine
//	ID_US.h - Header file for the User Manager
//	v1.0d1
//	By Jason Blochowiak
//

#include "wl_def.h"
#include "retroport.h"

#ifdef	__DEBUG__
#define	__DEBUG_UserMgr__
#endif

//#define	HELPTEXTLINKED

#define	MaxX    RETROPORT_SCREEN_WIDTH
#define	MaxY	RETROPORT_SCREEN_HEIGHT

#define	MaxHelpLines	500

#define	MaxHighName	57
#define	MaxScores	4

typedef	struct
{
    char        name[MaxHighName + 1];
    int32_t     score;
    uint16_t	completed,episode;
} HighScore;

#define	MaxGameName		32
#define	MaxSaveGames	6
typedef	struct
{
    char        signature[4];
    uint16_t	*oldtest;
    bool        present;
    char        name[MaxGameName + 1];
} SaveGame;

#define	MaxString	128	// Maximum input string size

typedef	struct
{
    int	x,y,
        w,h,
        px,py;
} WindowRec;	// Record used to save & restore screen windows


extern	uint16_t	PrintX,PrintY;	// Current printing location in the window
extern	uint16_t	WindowX,WindowY,// Current location of window
					WindowW,WindowH;// Current size of window

extern	void		(*USL_MeasureString)(const char *,uint16_t *,uint16_t *);
extern void			(*USL_DrawString)(const char *);

extern	bool		(*USL_SaveGame)(int),(*USL_LoadGame)(int);
extern	void		(*USL_ResetGame)(void);
//extern	SaveGame	Games[MaxSaveGames];
extern	HighScore	Scores[];

#define	US_HomeWindow()	{PrintX = WindowX; PrintY = WindowY;}

void            US_Startup(void);
void            US_Shutdown(void);
void			US_TextScreen(void),
				US_UpdateTextScreen(void),
				US_FinishTextScreen(void);
void			US_DrawWindow(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void			US_CenterWindow(uint16_t,uint16_t);
void			US_SaveWindow(WindowRec *win),
				US_RestoreWindow(WindowRec *win);
void 			US_ClearWindow(void);
void			US_SetPrintRoutines(void (*measure)(const char *,uint16_t *,uint16_t *),
									void (*print)(const char *));
void			US_PrintCentered(const char *s),
				US_CPrint(const char *s),
				US_CPrintLine(const char *s),
				US_Print(const char *s);
void			US_Printf(const char *formatStr, ...);
void			US_CPrintf(const char *formatStr, ...);

void			US_PrintUnsigned(uint32_t n);
void			US_PrintSigned(int32_t n);
void			US_StartCursor(void),
				US_ShutCursor(void);
void			US_CheckHighScore(int32_t score,uint16_t other);
void			US_DisplayHighScores(int which);
extern	bool	US_UpdateCursor(void);
bool            US_LineInput(int x,int y,char *buf,const char *def,bool escok,
                             int maxchars,int maxwidth);

void	        USL_PrintInCenter(const char *s,Rect r);
char 	        *USL_GiveSaveName(uint16_t game);

void            US_InitRndT(int randomize);
int             US_RndT(void);
