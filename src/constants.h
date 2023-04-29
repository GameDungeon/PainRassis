#pragma once

#include <cstdint>
#include <string>

#include "init.h"

//#define DEBUG

inline const std::string NAME = "Pain Rassis";
inline const std::string VERSION = "0.1";

inline const int MAXGAMEHISTORY = 2048;

const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

const char* const PceChar[] =
{
    "PNBRQK",
    "pnbrqk",
};

enum Color : bool
{
    WHITE, 
    BLACK
};

constexpr Color operator!(const Color &side) {
    return Color(side ^ 1);
}

constexpr Color operator^(const Color &a, int b) {
    return Color(bool(a) ^ b);
}

constexpr Color operator^=(Color &a, int b) {
    return a = Color(bool(a) ^ b);
}


enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

enum Piece : int
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    EMPTY
};

enum Direction
{
    NORTH = 8,
    WEST = -1,
    SOUTH = -8,
    EAST = 1,
    NORTH_EAST = 9,
    NORTH_WEST = 7,
    SOUTH_WEST = -9,
    SOUTH_EAST = -7
};

enum Square : unsigned short
{
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

inline Square operator+(const Square &a, int b) {
    return Square(int(a) + b);
}

inline Square operator-(const Square &a, int b) {
    return Square(int(a) - b);
}

inline Square operator+=(Square &a, int b) {
    return a = a + b;
}

inline Square operator-=(Square &a, int b) {
    return a = a - b;
}

inline Square flipSquare(const Square &sq) {
    return Square(int(sq) ^ 56);
}

inline Square operator++(Square &sq) {
    return sq += 1;
}

template<Color side>
inline Square FlipToSide(const Square &sq) {
    if constexpr(side == WHITE)
        return sq;
    else
        return flipSquare(sq);
}

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

const char SideChar[] = "wb-";
const char RankChar[] = "12345678";
const char FileChar[] = "abcdefgh";

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed",#n); \
printf("On %s ",__DATE__); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);}
#endif