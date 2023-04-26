#include "move.h"

void MoveList::AddMove(Move move)
{
    moves[count].move = move;
    moves[count].score = 0;
    count++;
}

Move GetMove(Square from, Square to)
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

Move GetMove(Square from, Square to, bool castle, bool pawnStart)
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

Move GetMove(Square from, Square to, Piece captured)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = captured;
    move.PromotedTo = EMPTY;
    move.EnPassant = false;
    move.Castle = false;
    move.PawnStart = false;

    return move;
}

Move GetMove(Square from, Square to, Piece captured, Piece promotedTo)
{
    Move move;
    move.From = from;
    move.To = to;
    move.Capture = captured;
    move.PromotedTo = promotedTo;
    move.EnPassant = false;
    move.Castle = false;
    move.PawnStart = false;

    return move;
}

Move GetMove(Square from, Square to, Piece captured, Piece promotedTo, 
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

Move GetEnPassantMove(Square from, Square to)
{
    ASSERT(RanksBrd[to]==RANK_3 || RanksBrd[to]==RANK_6)

    Move move;
    move.From = from;
    move.To = to;
    move.Capture = PAWN;
    move.PromotedTo = EMPTY;
    move.EnPassant = true;
    move.Castle = false;
    move.PawnStart = false;

    return move;
}