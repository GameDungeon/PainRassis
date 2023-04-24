#include "move.h"

void MoveList::AddMove(Move move)
{
    moves[count].move = move;
    moves[count].score = 0;
    count++;
}

Move GetMove(int from, int to, bool captured, Piece promotedTo, 
             bool enPassant, bool castle, bool pawnStart)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = captured;
    move.PromotedTo = promotedTo;
    move.EnPassant = enPassant;
    move.Castle = castle;
    move.PawnStart = pawnStart;

    return move;
}

Move GetMove(int from, int to)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = EMPTY;
    move.PromotedTo = EMPTY;
    move.EnPassant = false;
    move.Castle = false;
    move.PawnStart = false;

    return move;
}

Move GetCaptureMove(int from, int to)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = true;
    move.PromotedTo = EMPTY;
    move.EnPassant = false;
    move.Castle = false;
    move.PawnStart = false;

    return move;
}