#pragma once

#include "constants.h"
#include "move.h"
#include "board.h"

template <Piece pieceType>
Bitboard GetPieceMask(Bitboard blockers, Square sq);

template<Color side>
MoveList GenerateAllMoves(const Board &board, MoveList &list);
MoveList GenerateAllMoves(const Board &board, MoveList &list);
