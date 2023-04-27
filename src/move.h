#pragma once

#include "constants.h"

struct Move {
   Square From : 7;
   Square To : 7;
   Piece Capture : 4;
   Piece PromotedTo : 4;
   bool EnPassant : 1;
   bool Castle : 1;
   bool PawnStart : 1;
};

struct MoveData {
    Move move;
    int score;
};

struct MoveList {
    MoveData moves[256];
    int count = 0;

    void AddMove(Move move);
};

Move GetMove(Square from, Square to);
Move GetMove(Square from, Square to, bool castle, bool pawnStart);
Move GetMove(Square from, Square to, Piece captured);
Move GetMove(Square from, Square to, Piece captured, Piece promotedTo);
Move GetMove(Square from, Square to, Piece captured, Piece promotedTo, 
             bool enPassant, bool castle, bool pawnStart);

Move GetEnPassantMove(Square from, Square to);