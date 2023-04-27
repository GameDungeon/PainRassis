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
    //board.ParseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    //board.PrintBoard();

    //MoveList list;
    //GenerateAllMoves(board, list);

    //PrintMoveList(list);

    board.ParseFen(STARTING_FEN);

    PerftTest(2, board); // KNIGHT?

    return 0;
}