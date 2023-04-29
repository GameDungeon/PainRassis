// perft.c

#include "constants.h"
#include "board.h"
#include "movegen.h"
#include "move.h"
#include "io.h"
#include "utils.h"

#include <cstdio>

long leafNodes;

void Perft(int depth, Board board) {

	if(depth == 0) {
        leafNodes++;
        return;
    }	

    MoveList list;
    GenerateAllMoves(board, list);
      
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list.count; ++MoveNum) {	     

#ifdef DEBUG
        const Board oldBoard = board;
#endif

        if ( !board.MakeMove(list.moves[MoveNum].move)) {
            continue;
        }
        Perft(depth - 1, board);
        board.TakeMove();

#ifdef DEBUG
        if(oldBoard != board) {
            printf("\n*******\n\nOld Board:\n");
            oldBoard.PrintBoard();
            printf("New Board:\n");
            board.PrintBoard();

            board.MakeMove(list.moves[MoveNum].move);
            printf("Move %s:\n", PrMove(list.moves[MoveNum].move));
            board.PrintBoard();
        }
        ASSERT(oldBoard == board);
#endif
    }

    return;
}

void PerftTest(int depth, Board board) {
	board.PrintBoard();
	printf("\nStarting Test To Depth: %d\n",depth);	
	leafNodes = 0;
	int start = GetTimeMs();
    MoveList list;
    GenerateAllMoves(board, list);
    
    Move move;	    
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list.count; ++MoveNum) {
        move = list.moves[MoveNum].move;
        if ( !board.MakeMove(move))  {
            continue;
        }
        long cumnodes = leafNodes;
        Perft(depth - 1, board);
        board.TakeMove();        
        long oldnodes = leafNodes - cumnodes;
        printf("%s : %ld\n",PrMove(move),oldnodes);
    }
	
	printf("\nTest Complete : %ld nodes visited in %dms\nSpeed: %.2fM nps\n",leafNodes,GetTimeMs() - start, (float)leafNodes / (float)(GetTimeMs() - start) / 1000.0f);

    return;
}
