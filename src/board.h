#pragma once

#include "constants.h"
#include "bitboard.h"
#include "move.h"
#include <string>

struct BoardState
{
    Square enPas;

    Move move;
    int castlePerm;
    int fiftyMove;

    uint64_t posKey;
};

class Board
{
public:
    void Reset();
    uint64_t GeneratePosKey();
    bool ParseFen(const std::string &fen);
    Piece GetPieceBySq(Square sq) const;

    void SetPiece(Piece piece, const Square sq, Color color);
    void ClearPiece(Piece piece, const Square sq);
    void ClearSquare(const Square sq);

    template <Color color>
    void SetPiece(Piece piece, const Square sq);

    void MovePiece(const Square from, const Square to);

    bool MakeMove(Move move);
    void TakeMove();

    void PrintBoard();
    template <Color color>
    void GenerateControlledSquares();
    void GenerateControlledSquares();

    // Yes I know this is non standard.
    // At this point I embrace that.
    Bitboard BlackBitboard;    // A board of black piece locations for getting color.
    Bitboard AllPieceBitboard; // All piece locations
    Bitboard PieceBitboard[6]; // Locations by piece type.

    Bitboard ControlledSquares[2];

    Square enPas;

    Color side;
    int fiftyMove;

    int ply;
    int hisPly;

    int castlePerm;

    BoardState history[MAXGAMEHISTORY];

    uint64_t posKey;

    template <Color color>
    inline Bitboard getSideAllPieces() const
    {
        if constexpr (color == WHITE)
            return AllPieceBitboard & ~BlackBitboard;
        else
            return AllPieceBitboard & BlackBitboard;
    };

    template <Color color, Piece piece>
    inline Bitboard getSidePieceType() const
    {
        if constexpr (color == WHITE)
            return PieceBitboard[piece] & ~BlackBitboard;
        else
            return PieceBitboard[piece] & BlackBitboard;
    };

    inline Bitboard getSidePieceType(Color color, Piece piece) const
    {
        if (color == WHITE)
            return PieceBitboard[piece] & ~BlackBitboard;
        else
            return PieceBitboard[piece] & BlackBitboard;
    };

private:
    template <Piece piece, Color color>
    void ControlledSquaresByPiece();
};