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

    
    for(Square sq = A1; sq < 64; ++sq) {
        printf("Square: %s\n", PrSq(sq));
        RookAttack(sq, board.AllPieceBitBoard).PrintBitBoard();
    }

    GenerateAllMoves(board, list);

    PrintMoveList(list); 

    GenerateMagics();

    return 0;
}