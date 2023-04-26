#include "constants.h"
#include "movegen.h"
#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "attacks.h"

template <Color side>
static void GeneratePawnMoves(const Board &board, MoveList &list)
{
    constexpr Direction UP = side == WHITE ? NORTH : SOUTH;
    constexpr Direction DOWN = side == BLACK ? NORTH : SOUTH;
    constexpr Direction DOWN_LEFT = side == BLACK ? NORTH_EAST : SOUTH_WEST;
    constexpr Direction DOWN_RIGHT = side == BLACK ? NORTH_WEST : SOUTH_EAST;

    constexpr Bitboard promoRank = side == WHITE ? rank7 : rank2;
    constexpr Bitboard doublePushRank = side == WHITE ? rank3 : rank6;

    Bitboard empty = ~board.AllPieceBitboard;
    Bitboard pawns = board.getSidePeiceType<side, PAWN>();
    Bitboard enemy = board.getSideAllPeices<!side>();

    Bitboard upOne = shift<UP>(pawns);

    Bitboard singlePush = upOne & ~promoRank & empty;
    Bitboard doublePush = shift<UP>(singlePush & doublePushRank) & empty;

    Bitboard leftAttack = shift<WEST>(upOne) & ~promoRank & enemy;
    Bitboard rightAttack = shift<EAST>(upOne) & ~promoRank & enemy;

    while (singlePush)
    {
        Square to = singlePush.popLsb();
        list.AddMove(GetMove(to + DOWN, to));
    }

    while (doublePush)
    {
        Square to = doublePush.popLsb();
        list.AddMove(GetMove(to + 2 * DOWN, to, false, true));
    }

    while (leftAttack)
    {
        Square to = leftAttack.popLsb();
        Piece pce = board.GetPieceBySq(to);
        list.AddMove(GetMove(to + DOWN_LEFT, to, pce));
    }

    while (rightAttack)
    {
        Square to = rightAttack.popLsb();
        Piece pce = board.GetPieceBySq(to);
        list.AddMove(GetMove(to + DOWN_RIGHT, to, pce));
    }

    if (pawns & promoRank)
    {
        Bitboard promoteSinglePush = singlePush & promoRank & empty;
        Bitboard promoteLeftAttack = shift<WEST>(upOne) & promoRank & enemy;
        Bitboard promoteRightAttack = shift<EAST>(upOne) & promoRank & enemy;

        while (promoteSinglePush)
        {
            Square to = promoteSinglePush.popLsb();
            list.AddMove(GetMove(to + DOWN, to, BISHOP));
            list.AddMove(GetMove(to + DOWN, to, KNIGHT));
            list.AddMove(GetMove(to + DOWN, to, ROOK));
            list.AddMove(GetMove(to + DOWN, to, QUEEN));
        }

        while (promoteLeftAttack)
        {
            Square to = promoteLeftAttack.popLsb();
            Piece pce = board.GetPieceBySq(to);
            list.AddMove(GetMove(to + DOWN_LEFT, to, pce, BISHOP));
            list.AddMove(GetMove(to + DOWN_LEFT, to, pce, KNIGHT));
            list.AddMove(GetMove(to + DOWN_LEFT, to, pce, ROOK));
            list.AddMove(GetMove(to + DOWN_LEFT, to, pce, QUEEN));
        }

        while (promoteRightAttack)
        {
            Square to = promoteRightAttack.popLsb();
            Piece pce = board.GetPieceBySq(to);
            list.AddMove(GetMove(to + DOWN_RIGHT, to, pce, BISHOP));
            list.AddMove(GetMove(to + DOWN_RIGHT, to, pce, KNIGHT));
            list.AddMove(GetMove(to + DOWN_RIGHT, to, pce, ROOK));
            list.AddMove(GetMove(to + DOWN_RIGHT, to, pce, QUEEN));
        }
    }

    if (board.enPas != NO_SQ)
    {
        Square attackingPawnLeft = Square(board.enPas + int(DOWN_LEFT));
        Square attackingPawnRight = Square(board.enPas + int(DOWN_RIGHT));

        if (pawns.get(attackingPawnLeft))
        {
            list.AddMove(GetEnPassantMove(attackingPawnLeft, board.enPas));
        }

        if (pawns.get(attackingPawnRight))
        {
            list.AddMove(GetEnPassantMove(attackingPawnRight, board.enPas));
        }
    }
}

template <Piece pieceType>
Bitboard GetPieceMask(Bitboard blockers, Square sq)
{
    if constexpr (pieceType == KNIGHT)
        return KnightAttacks[sq];
    else if constexpr (pieceType == BISHOP)
        return BishopAttack(sq, blockers);
    else if constexpr (pieceType == ROOK)
        return RookAttack(sq, blockers);
    else if constexpr (pieceType == QUEEN)
        return BishopAttack(sq, blockers) ||
               RookAttack(sq, blockers);
    else if constexpr (pieceType == KING)
        return KingAttacks[sq];
}

template <Color side, Piece pieceType>
static void GeneratePieceMoves(const Board &board, MoveList &list)
{
    Bitboard allPieces = board.AllPieceBitboard;
    Bitboard empty = ~allPieces;
    Bitboard enemy = board.getSideAllPeices<!side>();
    Bitboard ally = board.getSideAllPeices<side>();

    Bitboard piece = board.getSidePeiceType<side, pieceType>();
    while (piece)
    {
        Square sq = piece.popLsb();
        Bitboard pieceMask = GetPieceMask<pieceType>(allPieces, sq) & ~ally;
        Bitboard quite = pieceMask & empty;
        Bitboard captures = pieceMask & enemy;

        while (quite)
        {
            list.AddMove(GetMove(sq, quite.popLsb()));
        }

        while (captures)
        {
            Piece pce = board.GetPieceBySq(sq);
            list.AddMove(GetMove(sq, captures.popLsb(), pce));
        }
    }
}

template <Color side>
MoveList GenerateAllMoves(const Board &board, MoveList &list)
{
    constexpr int kingCastle = side ? BKCA : WKCA;
    constexpr int queenCastle = side ? BQCA : WQCA;

    GeneratePawnMoves<side>(board, list);
    GeneratePieceMoves<side, KNIGHT>(board, list);
    GeneratePieceMoves<side, BISHOP>(board, list);
    GeneratePieceMoves<side, ROOK>(board, list);
    GeneratePieceMoves<side, QUEEN>(board, list);
    GeneratePieceMoves<side, KING>(board, list);

    if(board.castlePerm & kingCastle) {
        if (!(board.AllPieceBitboard.get(FlipToSide<side>(F1)) || 
              board.AllPieceBitboard.get(FlipToSide<side>(G1)))) {
            list.AddMove(GetMove(FlipToSide<side>(E1), FlipToSide<side>(G1), true, false));
        }
    }

    if(board.castlePerm & queenCastle) {
        if(!(board.AllPieceBitboard.get(FlipToSide<side>(B1)) || 
             board.AllPieceBitboard.get(FlipToSide<side>(C1)) || 
             board.AllPieceBitboard.get(FlipToSide<side>(D1)))) {
            list.AddMove(GetMove(FlipToSide<side>(E1), FlipToSide<side>(C1), true, false));
        }
    }

    return list;
}

MoveList GenerateAllMoves(const Board &board, MoveList &list)
{
    if (board.side == WHITE)
        return GenerateAllMoves<WHITE>(board, list);
    else
        return GenerateAllMoves<BLACK>(board, list);
}
