#include "constants.h"
#include "board.h"

void ResetBoard(Board& board)
{
    int index = 0;

    board.SidedBitBoard.bb = 0;

    for(index = 0; index < 6; ++index)
    {
        board.PieceBitBoard[index].bb = 0;
    }

    board.side = BOTH;
    board.enPas = NO_SQ;
    board.fiftyMove = 0;

    board.ply = 0;
    board.hisPly = 0;

    board.castlePerm = 0;

    board.posKey = 0ULL;
}