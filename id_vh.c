#include "id_vh.h"
#include "id_in.h"
#include "id_sd.h"      // GetTimeCount
#include "retroport.h"
#include <string.h>


const pictabletype  *pictable;


int	    px,py;
uint8_t	fontcolor,backcolor;
int	    fontnumber;

//==========================================================================
void VWB_DrawPropString(const char* string)
{
	const fontstruct  *font;
	int		    width, step, height;
	uint8_t	    *source, *dest;
	uint8_t	    ch;
	int i;

	uint8_t *vbuf = RETROPORT_Backbuffer ();

	font = (const fontstruct *) fontdata[fontnumber];
	height = font->height;
	dest = vbuf + (py * RETROPORT_SCREEN_WIDTH + px);

	while ((ch = (uint8_t)*string++)!=0)
	{
		width = step = font->width[ch];
		source = ((uint8_t *)font)+font->location[ch];
		while (width--)
		{
			for(i=0; i<height; i++)
			{
				if(source[i * step])
				{
                    dest[i * RETROPORT_SCREEN_WIDTH] = fontcolor;
				}
			}

			source++;
			px++;
			dest++;
		}
    }
}


void VWL_MeasureString (const char *string, uint16_t *width, uint16_t *height, const fontstruct *font)
{
	*height = font->height;
	for (*width = 0;*string;string++)
		*width += font->width[*((uint8_t *)string)];	// proportional width
}


void VW_MeasurePropString (const char *string, uint16_t *width, uint16_t *height)
{
	VWL_MeasureString(string,width,height,(const fontstruct *)fontdata[fontnumber]);
}

/*
=============================================================================

				Double buffer management routines

=============================================================================
*/

void VH_UpdateScreen(void)
{
    RETROPORT_UpdateScreen (0);
}

void VW_UpdateScreen(void)
{
    // Partial updates on menu screens
    RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_CopyFrame);
}

void VWB_DrawTile8 (int x, int y, int tile)
{
    LatchDrawChar (x,y,tile);
}


void VWB_DrawPic (int x, int y, int chunknum)
{
	int	picnum = chunknum - STARTPICS;
	int16_t width,height;

	width = pictable[picnum].width;
	height = pictable[picnum].height;

    VL_MemToScreen (pictdata[picnum],width,height,x,y);
}


/*
=============================================================================

						WOLFENSTEIN STUFF

=============================================================================
*/

void LatchDrawChar (unsigned x, unsigned y, unsigned charnum)
{
    VL_MemToScreen (tiledata[charnum], 8, 8, x*8, y);
}


void LatchDrawPic (unsigned x, unsigned y, unsigned picnum)
{
    int16_t width  = pictable[picnum-STARTPICS].width;
    int16_t height = pictable[picnum-STARTPICS].height;

	VL_MemToScreen (pictdata[picnum-STARTPICS], width, height, x*8, y);
}

//==========================================================================

/*
===================
=
= FizzleFade
=
= returns true if aborted
=
= It uses maximum-length Linear Feedback Shift Registers (LFSR) counters.
= You can find a list of them with lengths from 3 to 168 at:
= http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
= Many thanks to Xilinx for this list!!!
=
===================
*/

// XOR masks for the pseudo-random number sequence starting with n=17 bits
static const uint32_t rndmasks[] = {
                    // n    XNOR from (starting at 1, not 0 as usual)
    0x00012000,     // 17   17,14
    0x00020400,     // 18   18,11
    0x00040023,     // 19   19,6,2,1
    0x00090000,     // 20   20,17
    0x00140000,     // 21   21,19
    0x00300000,     // 22   22,21
    0x00420000,     // 23   23,18
    0x00e10000,     // 24   24,23,22,17
    0x01200000,     // 25   25,22      (this is enough for 8191x4095)
};

static unsigned int rndbits_y;
static unsigned int rndmask;

// Returns the number of bits needed to represent the given value
static int log2_ceil(uint32_t x)
{
    int n = 0;
    uint32_t v = 1;
    while(v < x)
    {
        n++;
        v <<= 1;
    }
    return n;
}

void VH_Startup(void)
{
    int rndbits_x = log2_ceil(screenWidth);
    rndbits_y = log2_ceil(screenHeight);

    int rndbits = rndbits_x + rndbits_y;
    if(rndbits < 17)
        rndbits = 17;       // no problem, just a bit slower
    else if(rndbits > 25)
        rndbits = 25;       // fizzle fade will not fill whole screen

    rndmask = rndmasks[rndbits - 17];
}

bool FizzleFade (int x1, int y1,
    unsigned width, unsigned height, unsigned frames, bool abortable)
{
    unsigned x, y, frame, pixperframe;
    int32_t  rndval, lastrndval;
    int      first = 1;

    rndval = 0;
    lastrndval = 0;
    pixperframe = width * height / frames;

    IN_StartAck ();

    frame = GetTimeCount();
    uint8_t *srcptr = RETROPORT_Backbuffer (); // source

    do
    {
        IN_ProcessEvents();

        if(abortable && IN_CheckAck ())
        {
            RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);
            return true;
        }

        uint8_t *destptr = RETROPORT_Frontbuffer (); //VL_LockSurface(screen);

        if(destptr != NULL)
        {
            rndval = lastrndval;

            // When using double buffering, we have to copy the pixels of the last AND the current frame.
            // Only for the first frame, there is no "last frame"
            for(int i = first; i < 2; i++)
            {
                for(unsigned p = 0; p < pixperframe; p++)
                {
                    //
                    // seperate random value into x/y pair
                    //

                    x = rndval >> rndbits_y;
                    y = rndval & ((1 << rndbits_y) - 1);

                    //
                    // advance to next random element
                    //

                    rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : rndmask);

                    if(x >= width || y >= height)
                    {
                        if(rndval == 0)     // entire sequence has been completed
                            goto finished;
                        p--;
                        continue;
                    }

                    //
                    // copy one pixel
                    //

                    *(destptr + (y1 + y) * RETROPORT_SCREEN_WIDTH + x1 + x)
                        = *(srcptr + (y1 + y) * RETROPORT_SCREEN_WIDTH + x1 + x);

                    if(rndval == 0)		// entire sequence has been completed
                        goto finished;
                }

                if(!i || first) lastrndval = rndval;
            }

            RETROPORT_UpdateScreen (RETROPORT_UpdateFlags_SwapOverride);
        }
        else
        {
            // No surface, so only enhance rndval
            for(int i = first; i < 2; i++)
            {
                for(unsigned p = 0; p < pixperframe; p++)
                {
                    rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : rndmask);
                    if(rndval == 0)
                        goto finished;
                }
            }
        }

        frame++;
        Delay (frame - GetTimeCount());        // don't go too fast
    } while (1);

finished:
    RETROPORT_UpdateScreen (0);
    return false;
}
