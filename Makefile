include $(LIB_EMBEDULAR_PATH)/embedul.ar/makefiles/system.mk

LIB_EMBEDULAR_CONFIG := NEED_VIDEO=1 \
                        NEED_SOUND=1 \
                        SPLASH_SCREENS=1 \
                        INPUT_ACTION=1

BUILD_LIBS += 3rd_party/fatfs

# CFLAGS += -DSPANISH

# Conditional cflags
TARGET_NAME_retro_ciaa_CFLAGS += -DBOARD_RETRO_CIAA_DISABLE_TCP_SERVER

CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_GRCHUNK_PIC_332='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_MAPSEGS_PLANE_0='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_MAPSEGS_PLANE_1='CC_Section(".rodata.$$$$Flash2")'
#CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_AUDIOSEGS_MUSIC_ADLIB='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_AUDIOSEGS_SOUND_ADLIB='CC_Section(".rodata.$$$$Flash2")'
#CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_TEXTURE_PAGE_332='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_SPRITE_PAGE_332='CC_Section(".rodata.$$$$Flash2")'
#CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_OBJLIST='CC_Section(".bss.$$$$RamAHB_ETB16")'
CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_TILEMAP='CC_Section(".bss.$$$$RamAHB_ETB16")'
CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_ACTORAT='CC_Section(".bss.$$$$RamLoc40")'
CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_STATOBJLIST='CC_Section(".bss.$$$$RamAHB16")'
# CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_VISLIST='CC_Section(".bss.$$$$RamAHB16")'
#CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_AREACONNECT='CC_Section(".bss.$$$$RamAHB16")'
#CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_WALLHEIGHT='CC_Section(".bss.$$$$RamAHB16")'
#CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_DOOROBJLIST='CC_Section(".bss.$$$$RamAHB16")'
#CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_SPOTVIS='CC_Section(".bss.$$$$RamAHB16")'
#CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_ASSETS_TILE_hud='CC_Section(".rodata.$$$$Flash2")'
#CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_ASSETS_TILEMAP_hud='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_ASSETS_SFX_SoundChunks_0='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_ASSETS_SFX_SoundChunks_1='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_ASSETS_SFX_SoundChunks_2='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DBSS_SECTION_BOARD='CC_Section(".bss.$$$$RamAHB16")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_RETROCIAA_SPLASH_THEME='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_ASSETS_TILEMAP_nyancat='CC_Section(".rodata.$$$$Flash2")'
CHIP_lpc4337jbd144_CFLAGS += -DRODATA_SECTION_ASSETS_TILE_nyancat='CC_Section(".rodata.$$$$Flash2")'


# Project files to build
APP_OBJS += \
    ./retroport.o \
    ./retroport_main.o \
    ./id_ca.o \
    ./id_in.o \
    ./id_pm.o \
    ./id_sd.o \
    ./id_us_1.o \
    ./id_vh.o \
    ./id_vl.o \
    ./wl_act1.o \
    ./wl_act2.o \
    ./wl_agent.o \
    ./wl_debug.o \
    ./wl_draw.o \
    ./wl_game.o \
    ./wl_inter.o \
    ./wl_main.o \
    ./wl_menu.o \
    ./wl_play.o \
    ./wl_state.o \
    ./wl_text.o \
    ./wl_def.o \
    ./rom/wl1/rom.o

$(BUILD)
