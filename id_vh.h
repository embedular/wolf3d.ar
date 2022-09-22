#pragma once
// ID_VH.H

#include "id_vl.h"
#include "resources.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define WHITE			255 // 15			// graphics mode independant colors
#define BLACK			0
#define FIRSTCOLOR		2   // 1
#define SECONDCOLOR		233 // 12
#define F_WHITE			WHITE           // 15
#define F_BLACK			BLACK           // 0
#define F_FIRSTCOLOR	FIRSTCOLOR      // 1
#define F_SECONDCOLOR	F_SECONDCOLOR   // 12

//===========================================================================

#define MAXSHIFTS	1

extern const uint8_t *  tiledata[NUMTILE8];
extern const uint8_t *  fontdata[NUMFONT];
extern const uint8_t *  pictdata[NUMPICS];


typedef struct
{
	int16_t width,height;
} pictabletype;


typedef struct
{
	int16_t height;
	int16_t location[256];
	int8_t width[256];
} fontstruct;


//===========================================================================


extern	const pictabletype *    pictable;
extern	pictabletype *          picmtable;

extern  uint8_t                 fontcolor,backcolor;
extern	int                     fontnumber;
extern	int                     px,py;

#define SETFONTCOLOR(f,b) fontcolor=f;backcolor=b;

//
// mode independant routines
// coordinates in pixels, rounded to best screen res
// regions marked in double buffer
//

void VWB_DrawPropString	 (const char *string);


void VWB_DrawTile8 (int x, int y, int tile);
#if 0
void VWB_DrawTile8M (int x, int y, int tile);
void VWB_DrawTile16 (int x, int y, int tile);
void VWB_DrawTile16M (int x, int y, int tile);
#endif
void VWB_DrawPic (int x, int y, int chunknum);
void VWB_DrawMPic(int x, int y, int chunknum);

#define VWB_Bar VW_Bar
#define VWB_Plot VW_Plot
#define VWB_PlotScaledCoord VW_Plot
#define VWB_Hlin VW_Hlin
#define VWB_Vlin VW_Vlin
#define VWB_HlinScaledCoord VW_Hlin
#define VWB_VlinScaledCoord VW_Vlin

void VH_UpdateScreen(void);
void VW_UpdateScreen(void);

//#define VW_UpdateScreen VH_UpdateScreen

//
// wolfenstein EGA compatability stuff
//


#define VW_Shutdown		    VL_Shutdown
#define VW_Bar			    VL_Bar
#define VW_Plot			    VL_Plot
#define VW_Hlin(x,z,y,c)	VL_Hlin(x,y,(z)-(x)+1,c)
#define VW_Vlin(y,z,x,c)	VL_Vlin(x,y,(z)-(y)+1,c)
#define VW_DrawPic		    VH_DrawPic
#define VW_WaitVBL		    VL_WaitVBL
#define VW_FadeIn()		    VL_FadeIn(0,255,NULL,30);
#define VW_FadeOut()	    VL_FadeOut(0,255,0,0,0,30);
#define VW_ScreenToScreen	VL_ScreenToScreen

void	VW_MeasurePropString (const char* string, uint16_t* width, uint16_t* height);

//#define LatchDrawTile(x,y,p)    VWB_DrawTile8(x,y,p)

void    LatchDrawChar   (unsigned x, unsigned y, unsigned charnum); // 8x8, latchpics[0][charnum] ->latchchars
void    LatchDrawPic    (unsigned x, unsigned y, unsigned picnum);

void    VH_Startup      (void);
bool    FizzleFade      (int x1, int y1, unsigned width, unsigned height,
                         unsigned frames, bool abortable);
