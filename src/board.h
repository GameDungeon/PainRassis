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
    Bitboard PieceBitBoard[6];

    Square enPas;

    int side;
    int fiftyMove;

    int ply;
    int hisPly;

    int castlePerm;

    BoardState history[MAXGAMEHISTORY];

    uint64_t posKey;
};