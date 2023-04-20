#pragma once

#include "constants.h"

#ifdef _MSC_VER
#include <intrin.h>
#include <nmmintrin.h>
#endif

struct Bitboard {

    uint64_t bb = 0;

    constexpr Bitboard(uint64_t value) {
        bb = value;
    }

    inline Bitboard(Square square);

    constexpr Bitboard() = default;

    constexpr bool get(Square square) const {
        return (bb >> square) & 1;
    }

    constexpr void set(Square square) {
        bb |= 1ULL << square;
    }

    constexpr void clear(Square square) {
        bb &= ~(1ULL << square);
    }

    constexpr int popCount() const {

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
};

extern void PrintBitBoard(Bitboard bb);