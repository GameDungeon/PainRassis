#pragma once

#include "constants.h"

typedef struct {
   unsigned short From : 6;
   unsigned short To : 6;
   bool Capture : 1;
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

    void AddMove(Move move);
} MoveList;

Move GetMove(int from, int to);
Move GetMove(int from, int to, bool castle, bool pawnStart);
Move GetMove(int from, int to, Piece promotedTo);
Move GetMove(int from, int to, bool captured, Piece promotedTo, 
             bool enPassant, bool castle, bool pawnStart);

Move GetCaptureMove(int from, int to);
Move GetCaptureMove(int from, int to, Piece promotedTo);
Move GetCaptureMove(int from, int to, bool enpassant);