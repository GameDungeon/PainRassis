#include "constants.h"
#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "attacks.h"

template <Color side> 
void GeneratePawnMoves(const Board &board, MoveList &list)
{
    constexpr Direction UP = side == WHITE ? NORTH : SOUTH;
    constexpr Direction DOWN = side == BLACK ? NORTH : SOUTH;
    constexpr Direction DOWN_LEFT = side == BLACK ? NORTH_EAST : SOUTH_WEST;
    constexpr Direction DOWN_RIGHT = side == BLACK ? NORTH_WEST : SOUTH_EAST;

    constexpr Bitboard promoRank = side == WHITE ? rank7 : rank2;
    constexpr Bitboard doublePushRank = side == WHITE ? rank3 : rank6;

    Bitboard empty = ~board.AllPieceBitBoard;
    Bitboard pawns = board.getSidePeiceType<side, PAWN>();
    Bitboard enemy = board.getSideAllPeices<!side>();

    Bitboard upOne = shift<UP>(pawns);

    Bitboard singlePush = upOne & ~promoRank & empty;
    Bitboard doublePush = shift<UP>(singlePush & doublePushRank) & empty;

    Bitboard leftAttack = shift<WEST>(upOne) & ~promoRank & enemy;
    Bitboard rightAttack = shift<EAST>(upOne) & ~promoRank & enemy;

    while (singlePush) {
        Square to = singlePush.popLsb();
        list.AddMove(GetMove(to + DOWN, to));
    }

    while (doublePush) {
        Square to = doublePush.popLsb();
        list.AddMove(GetMove(to + 2 * DOWN, to));
    }

    while (leftAttack) {
        Square to = leftAttack.popLsb();
        list.AddMove(GetCaptureMove(to + DOWN_LEFT, to));
    }

    while (rightAttack) {
        Square to = rightAttack.popLsb();
        list.AddMove(GetCaptureMove(to + DOWN_RIGHT, to));
    }

    if(pawns & promoRank) {
        Bitboard promoteSinglePush = singlePush & promoRank & empty;
        Bitboard promoteLeftAttack = shift<WEST>(upOne) & promoRank & enemy;
        Bitboard promoteRightAttack = shift<EAST>(upOne) & promoRank & enemy;

        while (promoteSinglePush) {
            Square to = promoteSinglePush.popLsb();
            list.AddMove(GetMove(to + DOWN, BISHOP));
            list.AddMove(GetMove(to + DOWN, KNIGHT));
            list.AddMove(GetMove(to + DOWN, ROOK));
            list.AddMove(GetMove(to + DOWN, QUEEN));
        }

        while (promoteLeftAttack) {
            Square to = promoteLeftAttack.popLsb();
            list.AddMove(GetCaptureMove(to + DOWN_LEFT, BISHOP));
            list.AddMove(GetCaptureMove(to + DOWN_LEFT, KNIGHT));
            list.AddMove(GetCaptureMove(to + DOWN_LEFT, ROOK));
            list.AddMove(GetCaptureMove(to + DOWN_LEFT, QUEEN));
        }

        while (promoteRightAttack) {
            Square to = promoteRightAttack.popLsb();
            list.AddMove(GetCaptureMove(to + DOWN_RIGHT, BISHOP));
            list.AddMove(GetCaptureMove(to + DOWN_RIGHT, KNIGHT));
            list.AddMove(GetCaptureMove(to + DOWN_RIGHT, ROOK));
            list.AddMove(GetCaptureMove(to + DOWN_RIGHT, QUEEN));
        }
    }

    if (board.enPas != NO_SQ) {
        Square attackingPawnLeft = Square(board.enPas + int(DOWN_LEFT));
        Square attackingPawnRight = Square(board.enPas + int(DOWN_RIGHT));

        if (pawns.get(attackingPawnLeft)) {
            list.AddMove(GetCaptureMove(attackingPawnLeft, board.enPas));
        }

        if (pawns.get(attackingPawnRight)) {
           list.AddMove(GetCaptureMove(attackingPawnRight, board.enPas));
        }
    } 
}

template <Piece pieceType>
Bitboard GetPieceMask(const Board &board, Square sq)
{
    if constexpr(pieceType == KNIGHT)
        return KnightAttacks[sq];
    else if constexpr(pieceType == BISHOP)
        return BishopAttack(sq, board.AllPieceBitBoard);
    else if constexpr(pieceType == ROOK)
        return RookAttack(sq, board.AllPieceBitBoard);
    else if constexpr(pieceType == QUEEN)
        return BishopAttack(sq, board.AllPieceBitBoard) ||
               RookAttack(sq, board.AllPieceBitBoard);
    else if constexpr(pieceType == KING)
        return KingAttacks[sq];
}

template <Color side, Piece pieceType>
void GeneratePieceMoves(const Board &board, MoveList &list)
{
    Bitboard allPieces = board.AllPieceBitBoard;
    Bitboard empty = ~allPieces;
    Bitboard enemy = board.getSideAllPeices<!side>();
    Bitboard ally = board.getSideAllPeices<side>();

    Bitboard piece = board.getSidePeiceType<side, pieceType>();
    while (piece)
    {
        Square sq = piece.popLsb();
        Bitboard pieceMask = GetPieceMask<pieceType>(board, sq) & ~ally;
        Bitboard quite = pieceMask & empty;
        Bitboard captures = pieceMask & enemy;

        while (quite) {
            list.AddMove(GetMove(sq, quite.popLsb()));
        }

        while (captures) {
            list.AddMove(GetCaptureMove(sq, captures.popLsb()));
        }
    }
}

template<Color side>
MoveList GenerateAllMoves(const Board &board, MoveList &list)
{
    GeneratePawnMoves<side>(board, list);
    GeneratePieceMoves<side, KNIGHT>(board, list);
    GeneratePieceMoves<side, BISHOP>(board, list);
    GeneratePieceMoves<side, ROOK>(board, list);
    GeneratePieceMoves<side, QUEEN>(board, list);
    GeneratePieceMoves<side, KING>(board, list);

    return list;
}

MoveList GenerateAllMoves(const Board &board, MoveList &list)
{
    if(board.side == WHITE)
        return GenerateAllMoves<WHITE>(board, list);
    else
        return GenerateAllMoves<BLACK>(board, list);
}
