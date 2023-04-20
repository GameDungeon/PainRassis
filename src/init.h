#pragma once

#include "stdint.h"

#define RAND_64 ((uint64_t)rand() | \
                 (uint64_t)rand() << 15 | \
                 (uint64_t)rand() << 30 | \
                 (uint64_t)rand() << 45 | \
                ((uint64_t)rand() & 0xf) << 60 )

extern uint64_t PieceKeys[13][120];
extern uint64_t SideKey;
extern uint64_t CastleKeys[16];

extern int FilesBrd[64];
extern int RanksBrd[64];

extern void AllInit();