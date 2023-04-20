#pragma once

#include "constants.h"
#include "bitboard.h"

typedef struct {
    Square enPas;

    int move;
    int castlePerm;
    int fiftyMove;

    uint64_t posKey;
} BoardState;

class Board
{
public:
    Bitboard SidedBitBoard;
    Bitboard PieceBitBoard[6];

    Square enPas;

    int side;
    int fiftyMove;

    int ply;
    int hisPly;

    int castlePerm;

    BoardState history[MAXGAMEHISTORY];

    uint64_t posKey;
};