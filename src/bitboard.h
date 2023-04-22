#pragma once

#include "constants.h"

#ifdef _MSC_VER
#include <intrin.h>
#include <nmmintrin.h>
#endif

typedef struct Bitboard {

    uint64_t bb = 0;

    void PrintBitBoard();

    constexpr Bitboard(uint64_t value) {
        bb = value;
    }

    inline Bitboard(Square square);

    constexpr Bitboard() = default;

    constexpr bool get(Square sq) const {
        return (bb >> sq) & 1;
    }

    constexpr void set(Square sq) {
        bb |= 1ULL << sq;
    }

    constexpr void clear(Square sq) {
        bb &= ~(1ULL << sq);
    }

    constexpr void setTo(Square sq, bool x) {
        bb ^= (-x ^ bb) & (1ULL << sq);
    }

    constexpr bool pop(Square sq)
    {
        bool x = get(sq);
        clear(sq);
        return x;
    }

    constexpr int Count() const {

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
        return (int) _mm_popcnt_u64(bb);
#else
        return __builtin_popcountll(bb);
#endif
    }

    // Returns the square with the lowest index, that is set to 1.
    constexpr Square lsb() const {

#ifdef __GNUC__
        return Square(__builtin_ctzll(bb));
#elif defined(_MSC_VER)
#ifndef __INTELLISENSE__ // This is a cursed hack
        unsigned long a;
        _BitScanForward64(&a, bb);
        return Square(a);
#endif
#else
#error "Unsupported compiler!"
#endif
    }

    // Clears the square with the lowest index, that is set to 1.
    constexpr Square popLsb() {
        Square square = lsb();
        bb &= bb - 1;
        return square;
    }

    constexpr Bitboard operator*(Bitboard a) const {
        return bb * a.bb;
    }

    constexpr bool operator==(Bitboard a) const {
        return bb == a.bb;
    }

    constexpr bool operator!=(Bitboard a) const {
        return bb != a.bb;
    }

    constexpr Bitboard operator+(Bitboard a) const {
        return bb + a.bb;
    }

    constexpr Bitboard operator-(Bitboard a) const {
        return bb - a.bb;
    }

    constexpr Bitboard operator&(Bitboard a) const {
        return bb & a.bb;
    }

    constexpr Bitboard operator|(Bitboard a) const {
        return bb | a.bb;
    }

    constexpr Bitboard operator^(Bitboard a) const {
        return bb ^ a.bb;
    }

    constexpr Bitboard operator~() const {
        return ~bb;
    }

    constexpr Bitboard operator<<(const unsigned int a) const {
        return bb << a;
    }

    constexpr Bitboard operator>>(const unsigned int a) const {
        return bb >> a;
    }

    constexpr void operator&=(Bitboard a) {
        bb &= a.bb;
    }

    constexpr void operator|=(Bitboard a) {
        bb |= a.bb;
    }

    constexpr void operator^=(Bitboard a) {
        bb ^= a.bb;
    }

    constexpr void operator<<=(const unsigned int a) {
        bb <<= a;
    }

    constexpr void operator>>=(const unsigned int a) {
        bb >>= a;
    }

    constexpr explicit operator bool() const {
        return bb;
    }

    constexpr explicit operator uint64_t() const {
        return bb;
    }
} Bitboard;

extern void PrintBitBoard(Bitboard bb);

constexpr Bitboard fileA = 0x101010101010101ULL;
constexpr Bitboard fileB = fileA << 1;
constexpr Bitboard fileC = fileA << 2;
constexpr Bitboard fileD = fileA << 3;
constexpr Bitboard fileE = fileA << 4;
constexpr Bitboard fileF = fileA << 5;
constexpr Bitboard fileG = fileA << 6;
constexpr Bitboard fileH = fileA << 7;

constexpr Bitboard rank1 = 0xff;
constexpr Bitboard rank2 = rank1 << (1 * 8);
constexpr Bitboard rank3 = rank1 << (2 * 8);
constexpr Bitboard rank4 = rank1 << (3 * 8);
constexpr Bitboard rank5 = rank1 << (4 * 8);
constexpr Bitboard rank6 = rank1 << (5 * 8);
constexpr Bitboard rank7 = rank1 << (6 * 8);
constexpr Bitboard rank8 = rank1 << (7 * 8);

constexpr Bitboard edges = rank1 | rank8 | fileA | fileH;