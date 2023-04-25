#include "move.h"

void MoveList::AddMove(Move move)
{
    moves[count].move = move;
    moves[count].score = 0;
    count++;
}

Move GetMove(int from, int to, Piece promotedTo)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = false;
    move.PromotedTo = promotedTo;
    move.EnPassant = false;
    move.Castle = false;
    move.PawnStart = false;

    return move;
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

Move GetMove(int from, int to, bool castle, bool pawnStart)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = EMPTY;
    move.PromotedTo = EMPTY;
    move.EnPassant = false;
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

Move GetCaptureMove(int from, int to, bool enpassant)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = true;
    move.PromotedTo = EMPTY;
    move.EnPassant = enpassant;
    move.Castle = false;
    move.PawnStart = false;

    return move;
}

Move GetCaptureMove(int from, int to, Piece promotedTo)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = true;
    move.PromotedTo = promotedTo;
    move.EnPassant = false;
    move.Castle = false;
    move.PawnStart = false;

    return move;
}