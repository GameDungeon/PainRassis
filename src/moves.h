#ifndef MOVES_H
#define MOVES_H

#include "defs.h"

#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)]==OFFBOARD)

const int LoopSlidePce[8];

const int LoopNonSlidePce[6];
const int LoopSlideIndex[2];
const int LoopNonSlideIndex[2];

const int PceDir[13][8];

const int NumDir[13];

#endif