#include "magics.h"
#include "constants.h"
#include "bitboard.h"
#include "attacks.h"
#include "utils.h"

#include <iostream>
#include <cstring>
#include <stdio.h>

Bitboard BishopAttacks[64][1024];
Bitboard RookAttacks  [64][4096];

Bitboard index_to_uint64(int index, int bits, Bitboard m) {
  int i, j;
  Bitboard result = 0ULL;
  for(i = 0; i < bits; i++) {
    j = (&m)->popLsb();
    if(index & (1 << i)) result |= (1ULL << j);
  }
  return result;
}

uint64_t transform(Bitboard b, Bitboard magic, int bits) {
  return ((b * magic) >> (64 - bits)).bb;
}

void initRookMagicTable() {
    for (Square sq = A1; sq < 64; ++sq) {
        Magic magic = RookMagics[sq];
        for (int blockerIndex = 0; blockerIndex < (1 << magic.shift); blockerIndex++) {
            Bitboard blockers = index_to_uint64(blockerIndex, magic.shift, magic.mask);
            RookAttacks[sq][getMagicIndex(magic, blockers)] = RookAttackSlow(sq, blockers);
        }
    }
}

void initBishopMagicTable() {
    for (Square sq = A1; sq < 64; ++sq) {
        Magic magic = BishopMagics[sq];
        for (int blockerIndex = 0; blockerIndex < (1 << magic.shift); blockerIndex++) {
            Bitboard blockers = index_to_uint64(blockerIndex, magic.shift, magic.mask);
            BishopAttacks[sq][getMagicIndex(magic, blockers)] = BishopAttackSlow(sq, blockers);
        }
    }
}

uint64_t find_magic(Square sq, int m, int bishop, Bitboard mask) {
    Bitboard blockers[4096];
    Bitboard attack[4096];
    Bitboard used[4096];

    uint64_t magic;
    int k, n, fail, i, j;

    n = mask.Count();

    for(i = 0; i < (1 << n); i++) {
        blockers[i] = index_to_uint64(i, n, mask);
        attack[i] = bishop? BishopAttackSlow(sq, blockers[i]) : RookAttackSlow(sq, blockers[i]);
    }

    for(k = 0; k < 100000000; k++) {
        magic = random_uint64() & random_uint64() & random_uint64();;
        if(((mask * magic) & 0xFF00000000000000ULL).Count() < 6) continue;
        for(i = 0; i < 4096; i++) used[i] = 0ULL;
        for(i = 0, fail = 0; !fail && i < (1 << n); i++) {
          j = transform(blockers[i], magic, m);
          if(used[j] == 0ULL) used[j] = attack[i];
          else if(used[j] != attack[i]) fail = 1;
        }
        if(!fail) return magic;
  }

  printf("***Failed***\n");
  return 0ULL;
}

void GenerateMagics()
{
    Square sq;
    Bitboard mask;
    printf("const Magic RookMagics[64] = {\n");
    for(sq = A1; sq < 64; ++sq)
    {
        mask = RookAttackMask(sq);
        printf("  { 0x%llxULL, 0x%llxULL, %d},\n", mask.bb, find_magic(sq, RBits[sq], 0, mask), RBits[sq]);
    }
    printf("};\n\n");

    printf("const Magic BishopMagics[64] = {\n");
    for(sq = A1; sq < 64; ++sq)
    {
        mask = BishopAttackMask(sq);
        printf("  { 0x%llxULL, 0x%llxULL, %d},\n", mask.bb, find_magic(sq, BBits[sq], 1, mask), BBits[sq]);
    }    
    printf("};\n\n");

}