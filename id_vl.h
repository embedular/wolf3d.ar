// ID_VL.H

// wolf compatability
#include "retroport.h"

void Quit (const char *error,...);

//===========================================================================

#define CHARWIDTH		2
#define TILEWIDTH		4

#define PALCOLORS       256
#define PALCOMPOS       3
#define PALSIZE         (PALCOLORS * PALCOMPOS)

//===========================================================================

extern  uint16_t    screenWidth, screenHeight;

extern	bool        screenfaded;
extern	uint8_t     bordercolor;

//===========================================================================

//
// VGA hardware routines
//

#define VL_WaitVBL(a) RETROPORT_WaitVBL((a)*8)

void VL_SetVGAPlaneMode (void);
void VL_SetTextMode     (void);
void VL_Shutdown        (void);

void VL_FadeOut         (uint8_t start, uint8_t end, uint8_t red, uint8_t green, uint8_t blue, uint16_t steps);
void VL_FadeIn          (uint8_t start, uint8_t end, const uint8_t *palette, uint16_t steps);

uint8_t VL_GetPixel     (uint16_t x, uint16_t y);
void VL_Plot            (uint16_t x, uint16_t y, uint8_t color);
void VL_Hlin            (uint16_t x, uint16_t y, uint16_t width, uint8_t color);
void VL_Vlin            (uint16_t x, uint16_t y, uint16_t height, uint8_t color);
void VL_Bar             (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
void VL_ClearScreen     (uint8_t color);
void VL_MemToScreen     (const uint8_t *source, uint16_t width, uint16_t height, uint16_t x, uint16_t y);
