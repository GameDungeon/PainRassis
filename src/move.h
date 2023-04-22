#include "constants.h"

typedef struct {
   unsigned short From : 6;
   unsigned short To : 6;
   unsigned short Captured : 4;
   unsigned short PromotedTo : 4;
   bool EnPassant : 1;
   bool Castle : 1;
   bool PawnStart : 1;
} Move;

typedef struct
{
    Move move;
    int score;
} MoveData;

typedef struct
{
    MoveData moves[256];
    int count;
} MoveList;

Move GetMove(int from, int to, Piece captured, Piece promotedTo, 
             bool enPassant, bool castle, bool pawnStart);