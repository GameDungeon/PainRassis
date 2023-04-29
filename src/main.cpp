#include "constants.h"
#include "bitboard.h"
#include "board.h"
#include "io.h"
#include "movegen.h"
#include "move.h"
#include "perft.h"

#include <cstdio>

int main()
{
    AllInit();

    Board board;
    board.ParseFen(STARTING_FEN);

    PerftTest(6, board);

    return 0;
}