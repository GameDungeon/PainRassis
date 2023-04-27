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

        printf("%d\n", MoveNum);

        if(list.moves[MoveNum].move.To == C1) {
            board.PrintBoard();
        }
       
        if ( !board.MakeMove(list.moves[MoveNum].move)) {
            continue;
        }
        Perft(depth - 1, board);
        board.TakeMove();
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
        printf("move %d : %s : %ld\n",MoveNum+1,PrMove(move),oldnodes);
    }
	
	printf("\nTest Complete : %ld nodes visited in %dms\n",leafNodes,GetTimeMs() - start);

    return;
}
