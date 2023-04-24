#pragma once

#include <cstdint>
#include "bitboard.h"
#include "constants.h"

Bitboard BishopAttack(Square sq, Bitboard blockers);
Bitboard RookAttack(Square sq, Bitboard blockers);
Bitboard KnightAttack(Square sq);
Bitboard KingAttack(Square sq);

void InitKnightAttacks();
void InitKingAttacks();

void InitBishopMasks();
void InitRookMasks();

Bitboard BishopAttackSlow(Square sq, Bitboard blockers);
Bitboard RookAttackSlow(Square sq, Bitboard blockers);

Bitboard BishopAttackMask(Square sq);
Bitboard RookAttackMask(Square sq);

extern Bitboard KnightAttacks[64];
extern Bitboard KingAttacks[64];

extern Bitboard BishopMasks[64];
extern Bitboard RookMasks[64];