#include "constants.h"
#include "bitboard.h"
#include "board.h"
#include "io.h"
#include "attacks.h"
#include "magics.h"
#include <cstdio>

int main()
{
    AllInit();

    Bitboard blockers = 0;
    blockers.set(C6);
    blockers.set(B4);
    blockers.PrintBitBoard();

    Bitboard attacks = MagicRookAttack(C4, blockers);
    attacks.PrintBitBoard();

    return 0;
}