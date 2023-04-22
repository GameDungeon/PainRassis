#include "move.h"

Move GetMove(int from, int to, Piece captured, Piece promotedTo, 
             bool enPassant, bool castle, bool pawnStart)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Captured = captured;
    move.PromotedTo = promotedTo;
    move.EnPassant = enPassant;
    move.Castle = castle;
    move.PawnStart = pawnStart;

    return move;
}