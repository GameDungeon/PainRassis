#pragma once

#include "constants.h"
#include "bitboard.h"
#include <string>

typedef struct {
    Square enPas;

    int move;
    int castlePerm;
    int fiftyMove;

    uint64_t posKey;
} BoardState;

class Board
{
public:
    void Reset();
    uint64_t GeneratePosKey();
    bool ParseFen(const std::string &fen);

    void SetPiece(Piece piece, Square sq, Color color);
    void ClearPiece(Piece piece, Square sq);

    Piece GetPieceBySq(Square sq);

    void PrintBoard();

    Bitboard SidedBitBoard;
    Bitboard AllPieceBitBoard;
    Bitboard PieceBitBoard[6];
    Bitboard SquareAttacked[2];

    Square enPas;

    Color side;
    int fiftyMove;

    int ply;
    int hisPly;

    int castlePerm;

    BoardState history[MAXGAMEHISTORY];

    uint64_t posKey;

    template <Color side> inline Bitboard getSideAllPeices() const
    {
        if constexpr (side == WHITE)
            return AllPieceBitBoard & ~SidedBitBoard;
        else
            return AllPieceBitBoard & SidedBitBoard;
    };

    template <Color side, Piece piece> inline Bitboard getSidePeiceType() const
    {
        if constexpr (side == WHITE)
            return PieceBitBoard[piece] & ~SidedBitBoard;
        else
            return PieceBitBoard[piece] & SidedBitBoard;
    };
};