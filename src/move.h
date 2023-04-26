#pragma once

#include "constants.h"

typedef struct {
   Square From : 7;
   Square To : 7;
   Piece Capture : 4;
   Piece PromotedTo : 4;
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

    void AddMove(Move move);
} MoveList;

Move GetMove(Square from, Square to);
Move GetMove(Square from, Square to, bool castle, bool pawnStart);
Move GetMove(Square from, Square to, Piece captured);
Move GetMove(Square from, Square to, Piece captured, Piece promotedTo);
Move GetMove(Square from, Square to, Piece captured, Piece promotedTo, 
             bool enPassant, bool castle, bool pawnStart);

Move GetEnPassantMove(Square from, Square to);