#pragma once

#ifdef USE_HIRES
#define PMPageSize 16384
#else
#define PMPageSize 4096
#endif


#ifdef UPLOAD
    #include "rom/wl1/texturedata_wl1.h"
    #include "rom/wl1/spritedata_wl1.h"
#else
    #include "rom/wl6/texturedata_wl6.h"
    #include "rom/wl6/spritedata_wl6.h"
#endif


void PM_Startup(void);
void PM_Shutdown(void);


static inline const uint8_t *PM_GetTexture(int wallpic)
{
    if (!texturedata[wallpic])
    {
        return texturedata[TEXTUREDATA_DEFAULT];
    }

    return texturedata[wallpic];
}


static inline const uint16_t *PM_GetSprite(int shapenum)
{
    if (!spritedata[shapenum])
    {
        return (const uint16_t *) spritedata[SPRITEDATA_NOTFOUND];
    }

    return (const uint16_t *) spritedata[shapenum];

    // correct alignment is enforced by PM_Startup()
    //return (uint16_t *) (void *) PM_GetPage(PMSpriteStart + shapenum);
}
