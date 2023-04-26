#pragma once

#include <cstdint>

extern uint64_t PieceKeys[2][12][64];
extern uint64_t SideKey;
extern uint64_t CastleKeys[16];

extern int FilesBrd[64];
extern int RanksBrd[64];

extern void AllInit();