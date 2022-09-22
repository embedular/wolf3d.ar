#pragma once
//
//      ID Engine
//      ID_SD.h - Sound Manager Header
//      Version for Wolfenstein
//      By Jason Blochowiak
//

#include "wl_def.h"
#include "resources.h"
#include "retroport.h"


#define TickBase        70      // 70Hz per tick - used as a base for timer 0


typedef enum
{
    sdm_Off,
    sdm_AdLib,
} SDMode;

typedef enum
{
    smm_Off,
    smm_AdLib
} SMMode;

typedef enum
{
    sds_Off,
    sds_SoundBlaster
} SDSMode;


// sgermino: adlib sound data are structured around
//           a SIX byte header, therefore no padding must be added.
typedef struct
{
    uint32_t            length;
    uint16_t            priority;
} __attribute__((packed)) SoundCommon;

#define ORIG_SOUNDCOMMON_SIZE 6

//      PC Sound stuff
#define pcTimer         0x42
#define pcTAccess       0x43
#define pcSpeaker       0x61

#define pcSpkBits       3

typedef struct
{
    SoundCommon         common;
    uint8_t             data[1];
} PCSound;

//      Register addresses
// Operator stuff
#define alChar          0x20
#define alScale         0x40
#define alAttack        0x60
#define alSus           0x80
#define alWave          0xe0
// Channel stuff
#define alFreqL         0xa0
#define alFreqH         0xb0
#define alFeedCon       0xc0
// Global stuff
#define alEffects       0xbd

typedef struct
{
    uint8_t mChar,cChar,
            mScale,cScale,
            mAttack,cAttack,
            mSus,cSus,
            mWave,cWave,
            nConn,

            // These are only for Muse - these bytes are really unused
            voice,
            mode;
    uint8_t unused[3];
} Instrument;

#define ORIG_INSTRUMENT_SIZE 16

typedef struct
{
    SoundCommon     common;
    Instrument      inst;
    uint8_t         block;
    uint8_t         data[1];
} AdLibSound;

#define ORIG_ADLIBSOUND_SIZE (ORIG_SOUNDCOMMON_SIZE + ORIG_INSTRUMENT_SIZE + 2)

//
//      Sequencing stuff
//
#define sqMaxTracks     10

typedef struct
{
    uint16_t    length;
    uint16_t    values[1];
} MusicGroup;

typedef struct
{
    int valid;
    fixed globalsoundx, globalsoundy;
} globalsoundpos;

extern globalsoundpos   channelSoundPos[];

// Global variables
extern  bool            AdLibPresent,
                        SoundBlasterPresent,
                        SoundPositioned;
extern  SDMode          SoundMode;
extern  SDSMode         DigiMode;
extern  SMMode          MusicMode;

//      "internal" variables
extern soundnames       SoundNumber;
extern uint16_t         SoundPriority;

//      AdLib variables
extern uint8_t          *alSound;
extern uint8_t          alBlock;
extern uint32_t         alLengthLeft;
extern uint32_t         alTimeCount;
extern Instrument       alZeroInst;

//      Sequencer variables
extern bool             sqActive;
extern uint16_t         *sqHack;
extern uint16_t         *sqHackPtr;
extern int              sqHackLen;
extern int              sqHackSeqLen;
extern uint32_t         sqHackTime;


#define GetTimeCount()  ((RETROPORT_GetTicks()*7)/100)

void Delay(int wolfticks);

// Function prototypes
extern  void        SD_Startup(void),
                    SD_Shutdown(void);

extern  void        SD_PositionSound(int leftvol,int rightvol);
extern  int         SD_PlaySound(soundnames sound);
extern  void        SD_SetPosition(int channel, int leftvol,int rightvol);
extern  void        SD_StopSound(void),
                    SD_WaitSoundDone(void);

extern  void        SD_StartMusic(int chunk);
extern  void        SD_ContinueMusic(int chunk, int startoffs);
extern  void        SD_MusicOn(void),
                    SD_FadeOutMusic(void);
extern  int         SD_MusicOff(void);

extern  bool        SD_MusicPlaying(void);
extern  bool        SD_SetSoundMode(SDMode mode);
extern  bool        SD_SetMusicMode(SMMode mode);
extern  uint16_t    SD_SoundPlaying(void);

extern  void        SD_ChannelFinished(int channel);

extern  void        SD_SetDigiDevice(SDSMode);
extern  int         SD_PlayDigitized(uint16_t which,int leftpos,int rightpos);
extern  void        SD_StopDigitized(void);
