#pragma once

#include <cstdint>
#include "bitboard.h"
#include "constants.h"

Bitboard BishopAttackSlow(Square sq, Bitboard blockers);
Bitboard RookAttackSlow(Square sq, Bitboard blockers);

Bitboard BishopAttackMask(Square sq);
Bitboard RookAttackMask(Square sq);

Bitboard MagicBishopAttack(Square sq, Bitboard blockers);
Bitboard MagicRookAttack(Square sq, Bitboard blockers);
