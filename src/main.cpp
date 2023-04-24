#include "constants.h"
#include "bitboard.h"
#include "board.h"
#include "io.h"
#include "movegen.h"

#include "attacks.h"
#include "magics.h"

#include <cstdio>

int main()
{
    AllInit();

    Board board;
    board.ParseFen(STARTING_FEN);

    MoveList list;

    GenerateAllMoves(board, list);

    PrintMoveList(list); 

    return 0;
}