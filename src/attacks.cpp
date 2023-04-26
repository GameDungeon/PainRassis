#include "constants.h"
#include "attacks.h"
#include "magics.h"
#include "bitboard.h"
#include "utils.h"

Bitboard KnightAttacks[64];
Bitboard KingAttacks[64];

Bitboard BishopMasks[64];
Bitboard RookMasks[64];

Bitboard knightAttackMask(Bitboard knight) {
   Bitboard l1 = (knight >> 1) & 0x7f7f7f7f7f7f7f7f;
   Bitboard l2 = (knight >> 2) & 0x3f3f3f3f3f3f3f3f;
   Bitboard r1 = (knight << 1) & 0xfefefefefefefefe;
   Bitboard r2 = (knight << 2) & 0xfcfcfcfcfcfcfcfc;
   Bitboard h1 = l1 | r1;
   Bitboard h2 = l2 | r2;
   return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

Bitboard kingAttackMask(Bitboard king) {
   Bitboard attacks = shift<EAST>(king) | shift<WEST>(king);
   king       |= attacks;
   attacks    |= shift<NORTH>(king) | shift<SOUTH>(king);
   return attacks;
}

Bitboard RookAttackMask(Square sq) {
    Bitboard result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
    for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
    for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
    for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
    return result;
    }

Bitboard BishopAttackMask(Square sq) {
    Bitboard result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    for(r=rk+1, f=fl+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
    for(r=rk+1, f=fl-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
    for(r=rk-1, f=fl+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
    for(r=rk-1, f=fl-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
    return result;
}


Bitboard RookAttackSlow(Square sq, Bitboard blockers) {
    Bitboard result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    for(r = rk+1; r <= 7; r++) {
        result |= (1ULL << (fl + r*8));
        if(blockers & (1ULL << (fl + r*8))) break;
    }
    for(r = rk-1; r >= 0; r--) {
        result |= (1ULL << (fl + r*8));
        if(blockers & (1ULL << (fl + r*8))) break;
    }
    for(f = fl+1; f <= 7; f++) {
        result |= (1ULL << (f + rk*8));
        if(blockers & (1ULL << (f + rk*8))) break;
    }
    for(f = fl-1; f >= 0; f--) {
        result |= (1ULL << (f + rk*8));
        if(blockers & (1ULL << (f + rk*8))) break;
    }
    return result;
}

Bitboard BishopAttackSlow(Square sq, Bitboard blockers) {
    Bitboard result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
        result |= (1ULL << (f + r*8));
        if(blockers & (1ULL << (f + r * 8))) break;
    }
    for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
        result |= (1ULL << (f + r*8));
        if(blockers & (1ULL << (f + r * 8))) break;
    }
    for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
        result |= (1ULL << (f + r*8));
        if(blockers & (1ULL << (f + r * 8))) break;
    }
    for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
        result |= (1ULL << (f + r*8));
        if(blockers & (1ULL << (f + r * 8))) break;
    }
    return result;
}

void InitBishopMasks()
{
    for(Square sq = A1; sq < 64; ++sq)
    {
        BishopMasks[sq] = BishopAttackMask(sq);
    }
}

void InitRookMasks()
{
    for(Square sq = A1; sq < 64; ++sq)
    {
        RookMasks[sq] = RookAttackMask(sq);
    }
}

void InitKnightAttacks()
{
    for(Square sq = A1; sq < 64; ++sq)
    {
        Bitboard knight = 1ULL << sq;
        KnightAttacks[sq] = knightAttackMask(knight);
    }
}

void InitKingAttacks()
{
    for(Square sq = A1; sq < 64; ++sq)
    {
        Bitboard knight = 1ULL << sq;
        KingAttacks[sq] = kingAttackMask(knight);
    }
}

Bitboard BishopAttack(Square sq, Bitboard blockers)
{
    blockers &= BishopMasks[sq];

    Magic magic = BishopMagics[sq];
    uint64_t index = getMagicIndex(magic, blockers);
    return BishopAttacks[sq][index];
}

Bitboard RookAttack(Square sq, Bitboard blockers)
{
    blockers &= RookMasks[sq];

    Magic magic = RookMagics[sq];
    uint64_t index = getMagicIndex(magic, blockers);
    return RookAttacks[sq][index];
}

inline Bitboard KnightAttack(Square sq)
{
    return KnightAttacks[sq];
}

inline Bitboard KingAttack(Square sq)
{
    return KingAttacks[sq];
}