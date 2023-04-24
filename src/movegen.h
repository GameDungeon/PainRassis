#pragma once

#include "constants.h"
#include "move.h"
#include "board.h"

template<Color side>
MoveList GenerateAllMoves(const Board &board, MoveList &list);

MoveList GenerateAllMoves(const Board &board, MoveList &list);