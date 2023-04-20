#include "constants.h"
#include "bitboard.h"
#include "board.h"
#include "init.h"
#include <stdio.h>

int main()
{
    AllInit();

    Board gameBoard;
    gameBoard.ParseFen(STARTING_FEN);
    gameBoard.PrintBoard();

    return 0;
}