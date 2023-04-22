#pragma once

#include "constants.h"

extern inline Square FileRankToSquare(int file, int rank)
{
    return Square(rank * 8 + file);
}

extern uint64_t random_uint64();