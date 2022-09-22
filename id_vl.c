// ID_VL.C

#include "id_vl.h"
#include "retroport.h"
#include <string.h>
#include <assert.h>

// Uncomment the following line, if you get destination out of bounds
// assertion errors and want to ignore them during debugging
//#define IGNORE_BAD_DEST

#ifdef IGNORE_BAD_DEST
#undef assert
#define assert(x) if(!(x)) return
#define assert_ret(x) if(!(x)) return 0
#else
#define assert_ret(x) assert(x)
#endif

uint16_t screenWidth = RETROPORT_SCREEN_WIDTH;
uint16_t screenHeight = RETROPORT_SCREEN_HEIGHT;

bool	 screenfaded;
uint8_t  bordercolor;

//===========================================================================


/*
=======================
=
= VL_Shutdown
=
=======================
*/

void	VL_Shutdown (void)
{
	//VL_SetTextMode ();
}


/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode (void)
{
    RETROPORT_SetVGAPlaneMode ();
}

/*
=============================================================================

						PALETTE OPS

		To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/

//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut (uint8_t start, uint8_t end, uint8_t red, uint8_t green,
                 uint8_t blue, uint16_t steps)
{
    (void) start;
    (void) end;
    (void) red;
    (void) green;
    (void) blue;
    (void) steps;

    RETROPORT_FadeOut ();
	screenfaded = true;
}


/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn (uint8_t start, uint8_t end, const uint8_t *palette, uint16_t steps)
{
    (void) start;
    (void) end;
    (void) palette;
    (void) steps;

    RETROPORT_FadeIn ();
	screenfaded = false;
}

/*
=============================================================================

							PIXEL OPS

=============================================================================
*/

/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot (uint16_t x, uint16_t y, uint8_t color)
{
    BOARD_Assert (x < screenWidth && y < screenHeight,
                "Pixel out of bounds");

    *VIDEO_BackbufferXY(x, y) = color;
}

/*
=================
=
= VL_GetPixel
=
=================
*/

uint8_t VL_GetPixel (uint16_t x, uint16_t y)
{
    BOARD_Assert (x < screenWidth && y < screenHeight,
                "Pixel out of bounds");

    return *VIDEO_BackbufferXY(x, y);
}


/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin (uint16_t x, uint16_t y, uint16_t width, uint8_t color)
{
    BOARD_Assert (x + width <= screenWidth && y < screenHeight,
                "Destination rectangle out of bounds");

    uint8_t *ptr = VIDEO_BackbufferXY (x, y);
    memset (ptr, color, width);
}


/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin (uint16_t x, uint16_t y, uint16_t height, uint8_t color)
{
	BOARD_Assert (x < screenWidth && y + height <= screenHeight,
                "Destination rectangle out of bounds");

	uint8_t *ptr = VIDEO_BackbufferXY (x, y);

	while (height--)
	{
		*ptr = color;
		ptr += VIDEO_Width ();
	}
}


/*
=================
=
= VL_Bar
=
=================
*/

void VL_Bar (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color)
{
	BOARD_Assert (x + width <= screenWidth && y + height <= screenHeight,
                "Destination rectangle out of bounds");

	uint8_t *ptr = VIDEO_BackbufferXY (x, y);

	while (height--)
	{
		memset (ptr, color, width);
		ptr += VIDEO_Width ();
	}
}

/*
============================================================================

							MEMORY OPS

============================================================================
*/

/*
=================
=
= VL_ScreenToScreen
=
=================
*/

void VL_ClearScreen (uint8_t color)
{
    VIDEO_ClearBack (color);
}


void VL_MemToScreen (const uint8_t *source, uint16_t width, uint16_t height, uint16_t x, uint16_t y)
{
    uint8_t *vbuf = VIDEO_BackbufferXY (x, y);

    BOARD_Assert (source, "Invalid source");

    BOARD_Assert (x + width <= VIDEO_Width()
                && y + height <= VIDEO_Height(),
                "Destination rectangle out of bounds");

    for (int h = 0; h < height; ++h)
    {
        memcpy (vbuf, source, width);
        vbuf += VIDEO_Width ();
        source += width;
    }
}
