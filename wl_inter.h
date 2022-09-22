#pragma once

#include <stdint.h>

/*
=============================================================================

                                WL_INTER

=============================================================================
*/

void IntroScreen (void);
void PG13 (void);
void DrawHighScores (void);
void CheckHighScore (int32_t score,uint16_t other);
void Victory (void);
void LevelCompleted (void);
void ClearSplitVWB (void);
void PreloadGraphics (void);
