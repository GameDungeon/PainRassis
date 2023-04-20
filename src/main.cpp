#include "constants.h"
#include "bitboard.h"
#include "init.h"
#include <stdio.h>

int main()
{
    AllInit();

    Bitboard playBitBoard = 0ULL;

    playBitBoard.set(D2);
    playBitBoard.set(D3);
    playBitBoard.set(D4);

    PrintBitBoard(playBitBoard);

    return 0;
}