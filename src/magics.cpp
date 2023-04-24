#include "magics.h"
#include "constants.h"
#include "bitboard.h"
#include "attacks.h"
#include "utils.h"

#include <iostream>
#include <cstring>
#include <stdio.h>

Bitboard BishopAttacks[64][1024];
Bitboard RookAttacks[64][4096];

Bitboard getBlockersFromIndex(int index, Bitboard mask)
{
    int i, j;
    int bits = mask.Count();

    Bitboard result = 0ULL;
    for (i = 0; i < bits; i++)
    {
        j = mask.popLsb();
        if (index & (1 << i)) result |= (1ULL << j);
    }
    return result;
}

void initRookMagicTable()
{
    for (Square sq = A1; sq < 64; ++sq)
    {
        Magic magic = RookMagics[sq];

        for (int i = 0; i < (1 << magic.shift); i++)
        {
            Bitboard blockers = getBlockersFromIndex(i, magic.mask);
            RookAttacks[sq][getMagicIndex(magic, blockers)] = RookAttackSlow(sq, blockers);
        }
    }
}

void initBishopMagicTable()
{
    for (Square sq = A1; sq < 64; ++sq)
    {
        Magic magic = BishopMagics[sq];

        for (int i = 0; i < (1 << magic.shift); i++)
        {
            Bitboard blockers = getBlockersFromIndex(i, magic.mask);
            BishopAttacks[sq][getMagicIndex(magic, blockers)] = BishopAttackSlow(sq, blockers);
        }
    }
}

Magic findMagic(Square sq, bool bishop, Bitboard mask)
{
    Bitboard blockers[4096];
    Bitboard attack[4096];
    Bitboard used[4096];

    Magic magic;

    magic.mask = mask;
    magic.shift = magic.mask.Count();

    unsigned int length = 0;
    Bitboard occ = 0;
    do {
        blockers[length] = occ;
        attack[length] = bishop ? BishopAttackSlow(sq, occ) : RookAttackSlow(sq, occ);

        length++;
        occ = (occ - magic.mask) & magic.mask;
    } while (occ != 0);

    while (true) {
        magic.magic = random_uint64() & random_uint64() & random_uint64();
        if (((magic.magic * magic.mask) >> 56).Count() < 6)
            continue;

        std::memset(used, 0, sizeof(used));

        bool failed = false;
        for (unsigned int i = 0; i < length; i++) {
            uint64_t index = getMagicIndex(magic, blockers[i]);
            if (used[index] == 0) {
                used[index] = attack[i];
            } else if (used[index] != attack[i]) {
                failed = true;
                break;
            }
        }
        if (!failed) {
            break;
        }
    }

    return magic;
}

void GenerateMagics()
{
    Square sq;
    Bitboard mask;
    Magic magic;
    printf("const Magic RookMagics[64] = {\n");
    for (sq = A1; sq < 64; ++sq)
    {
        mask = RookAttackMask(sq);
        magic = findMagic(sq, false, mask);
        printf("  { 0x%llxULL, 0x%llxULL, %d },\n", magic.mask, magic.magic, magic.shift);
    }
    printf("};\n\n");

    printf("const Magic BishopMagics[64] = {\n");
    for (sq = A1; sq < 64; ++sq)
    {
        mask = BishopAttackMask(sq);
        magic = findMagic(sq, true, mask);
        printf("  { 0x%llxULL, 0x%llxULL, %d },\n", magic.mask, magic.magic, magic.shift);
    }
    printf("};\n\n");
}