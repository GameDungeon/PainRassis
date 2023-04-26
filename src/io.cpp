#include <cstdio>
#include "constants.h"
#include "move.h"

char *PrSq(const int sq) {

    if(sq == NO_SQ) {
        return (char*)"None";
    }

    static char SqStr[3];

    int file = FilesBrd[sq];
    int rank = RanksBrd[sq];

    sprintf(SqStr, "%c%c", ('a'+file), ('1'+rank));

    return SqStr;
}

char *PrMove(const Move move) {

    static char MvStr[6];

    int ff = FilesBrd[move.From];
    int rf = RanksBrd[move.From];
    int ft = FilesBrd[move.To];
    int rt = RanksBrd[move.To];

    int promoted = move.PromotedTo;

    if(promoted != EMPTY) {
        char pchar = 'q';
        if(promoted == KNIGHT) {
            pchar = 'n';
        } else if(promoted == ROOK)  {
            pchar = 'r';
        } else if(promoted == BISHOP)  {
            pchar = 'b';
        }
        sprintf(MvStr, "%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pchar);
    } else {
        sprintf(MvStr, "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
    }

    return MvStr;
}

void PrintMoveList(const MoveList &list)
{
    
    int index = 0;
    Move move;
    printf("Movelist:\n");
    for(index = 0; index < list.count; ++index)
    {
        move = list.moves[index].move;
        printf("Move: %d > %s\n", index+1, PrMove(move));
    }
    printf("MoveList Total %d Moves\n\n", list.count);
}