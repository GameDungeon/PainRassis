#include "constants.h"
#include "attacks.h"
#include "magics.h"
#include "bitboard.h"
#include "utils.h"

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

Bitboard MagicBishopAttack(Square sq, Bitboard blockers)
{
    Magic magic = BishopMagics[sq];
    uint64_t index = getMagicIndex(magic, blockers);
    return BishopAttacks[sq][index];
}

Bitboard MagicRookAttack(Square sq, Bitboard blockers)
{
    Magic magic = RookMagics[sq];
    uint64_t index = getMagicIndex(magic, blockers);
    return RookAttacks[sq][index];
}