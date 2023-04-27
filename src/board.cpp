#include "constants.h"
#include "board.h"
#include "utils.h"
#include "move.h"
#include "io.h"
#include "attacks.h"

const int CastlePerm[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11,
};

// Remove this. This double define is cursed, and shouldn't exist
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
        return BishopAttack(sq, blockers) |
               RookAttack(sq, blockers);
    else if constexpr (pieceType == KING)
        return KingAttacks[sq];
}

void Board::SetPiece(Piece piece, Square sq, Color color)
{
    posKey ^= PieceKeys[color][piece][sq];

    PieceBitboard[piece].set(sq);
    AllPieceBitboard.set(sq);
    BlackBitboard.setTo(sq, color);
}

template <Color color>
void Board::SetPiece(Piece piece, Square sq)
{
    posKey ^= PieceKeys[color][piece][sq];

    PieceBitboard[piece].set(sq);
    AllPieceBitboard.set(sq);

    if constexpr (color == BLACK)
        BlackBitboard.set(sq);
    else
        BlackBitboard.clear(sq);
}

void Board::ClearPiece(Piece piece, Square sq)
{
    Color color = BlackBitboard.getColor(sq);

    PieceBitboard[piece].clear(sq);
    AllPieceBitboard.clear(sq);
    BlackBitboard.clear(sq);

    posKey ^= PieceKeys[color][piece][sq];
}

void Board::ClearSquare(Square sq)
{
    Piece piece = GetPieceBySq(sq);

    ASSERT(piece != EMPTY)

    ClearPiece(piece, sq);
}

void Board::MovePiece(const Square from, const Square to)
{

    Piece piece = GetPieceBySq(from);
    Color color = BlackBitboard.getColor(from);

    if (piece == EMPTY)
    {
        PrintBoard();
        printf("%s\n", PrSq(from));
        printf("%s\n", PrSq(to));
    }

    ASSERT(piece != EMPTY)

    ClearSquare(from);
    SetPiece(piece, to, color);
}

template <Piece piece, Color color>
void Board::ControlledSquaresByPiece()
{
    Bitboard pce = getSidePieceType<color, piece>();
    while (pce)
    {
        Square sq = pce.popLsb();
        ControlledSquares[color] |= GetPieceMask<piece>(AllPieceBitboard, sq);
    }
}

template <Color color>
void Board::GenerateControlledSquares()
{
    constexpr Direction UP = color == WHITE ? NORTH : SOUTH;

    Bitboard pawns = shift<UP>(getSidePieceType<color, PAWN>());
    Bitboard leftPawnAttack = shift<WEST>(pawns);
    Bitboard rightPawnAttack = shift<EAST>(pawns);

    ControlledSquares[color] = leftPawnAttack | rightPawnAttack;

    ControlledSquaresByPiece<KNIGHT, color>();
    ControlledSquaresByPiece<BISHOP, color>();
    ControlledSquaresByPiece<ROOK, color>();
    ControlledSquaresByPiece<QUEEN, color>();
    ControlledSquaresByPiece<KING, color>();

    ControlledSquares[color] |= getSideAllPieces<color>();
}

void Board::GenerateControlledSquares()
{
    GenerateControlledSquares<WHITE>();
    GenerateControlledSquares<BLACK>();
}

bool Board::MakeMove(Move move)
{
    history[hisPly].posKey = posKey;

    if (!AllPieceBitboard.get(move.From) || (move.Capture != EMPTY && GetPieceBySq(move.To) == EMPTY))
    {
        printf("\n******\nMove: %s\nCapture: %d\n******\n", PrMove(move), move.Capture);
        PrintBoard();
    }

    if (move.EnPassant)
    {
        if (side == WHITE)
        {
            ASSERT(GetPieceBySq(Square(move.To + SOUTH)) != EMPTY)
            ClearSquare(Square(move.To + SOUTH));
        }
        else
        {
            ASSERT(GetPieceBySq(Square(move.To + NORTH)) != EMPTY)
            ClearSquare(Square(move.To + NORTH));
        }
    }
    else if (move.Castle)
    {
        switch (move.To)
        {
        case C1:
            MovePiece(A1, D1);
            break;
        case C8:
            MovePiece(A8, D8);
            break;
        case G1:
            MovePiece(H1, F1);
            break;
        case G8:
            MovePiece(H8, F8);
            break;
        default:
            ASSERT(false);
            break;
        }
    }

    if (enPas != NO_SQ)
        posKey ^= PieceKeys[WHITE][EMPTY][enPas];
    posKey ^= CastleKeys[castlePerm];

    history[hisPly].move = move;
    history[hisPly].fiftyMove = fiftyMove;
    history[hisPly].enPas = enPas;
    history[hisPly].castlePerm = castlePerm;

    castlePerm &= CastlePerm[move.From];
    castlePerm &= CastlePerm[move.To];
    enPas = NO_SQ;

    posKey ^= (CastleKeys[(castlePerm)]);

    fiftyMove++;

    if (move.Capture != EMPTY)
    {
        ClearSquare(move.To);
        fiftyMove = 0;
    }

    hisPly++;
    ply++;

    if (PieceBitboard[PAWN].get(move.From))
    {
        fiftyMove = 0;
        if (move.PawnStart)
        {
            if (side == WHITE)
            {
                enPas = move.From + NORTH;
                ASSERT(RanksBrd[enPas] == RANK_3);
            }
            else
            {
                enPas = move.From + SOUTH;
                ASSERT(RanksBrd[enPas] == RANK_6);
            }
            posKey ^= PieceKeys[WHITE][EMPTY][(enPas)];
        }
    }

    MovePiece(move.From, move.To);

    if (move.PromotedTo != EMPTY)
    {
        ASSERT(GetPieceBySq(move.To) != EMPTY)
        ClearSquare(move.To);
        SetPiece(move.PromotedTo, move.To, side);
    }

    side ^= 1;
    posKey ^= SideKey;

    GenerateControlledSquares();

    if (getSidePieceType(!side, KING) & ControlledSquares[side])
    {
        TakeMove();
        return false;
    }

    if (move.Castle)
    {
        bool attacked = false;
        switch (move.To)
        {
        case C1:
            attacked = ControlledSquares[side].get(E1) ||
                       ControlledSquares[side].get(D1);
            break;
        case C8:
            attacked = ControlledSquares[side].get(E8) ||
                       ControlledSquares[side].get(D8);
            break;
        case G1:
            attacked = ControlledSquares[side].get(E1) ||
                       ControlledSquares[side].get(F1);
            break;
        case G8:
            attacked = ControlledSquares[side].get(E8) ||
                       ControlledSquares[side].get(F8);
            break;
        default:
            ASSERT(false);
            break;
        }

        if (attacked)
        {
            TakeMove();
            return false;
        }
    }

    return true;
}

void Board::TakeMove()
{
    hisPly--;
    ply--;

    Move move = history[hisPly].move;

    if (enPas != NO_SQ)
        posKey ^= PieceKeys[WHITE][EMPTY][enPas];
    posKey ^= (CastleKeys[(castlePerm)]);

    castlePerm = history[hisPly].castlePerm;
    fiftyMove = history[hisPly].fiftyMove;
    enPas = history[hisPly].enPas;

    if (enPas != NO_SQ)
        posKey ^= PieceKeys[WHITE][EMPTY][(enPas)];
    posKey ^= (CastleKeys[(castlePerm)]);

    side ^= 1;
    posKey ^= PieceKeys[WHITE][EMPTY][(enPas)];

    if (move.EnPassant)
    {
        if (side == WHITE)
        {
            SetPiece(PAWN, move.To + SOUTH, WHITE);
        }
        else
        {
            SetPiece(PAWN, move.To + NORTH, BLACK);
        }
    }
    else if (move.Castle)
    {
        switch (move.To)
        {
        case C1:
            MovePiece(D1, A1);
            break;
        case C8:
            MovePiece(D8, A8);
            break;
        case G1:
            MovePiece(F1, H1);
            break;
        case G8:
            MovePiece(F8, H8);
            break;
        default:
            ASSERT(false);
            break;
        }
    }

    MovePiece(move.To, move.From);

    if (move.Capture != EMPTY)
    {
        SetPiece(move.Capture, move.To, side);
    }

    if (move.PromotedTo != EMPTY)
    {
        ClearSquare(move.From);
        SetPiece(PAWN, move.From, BlackBitboard.getColor(move.From));
    }
}

void Board::Reset()
{
    int index = 0;

    BlackBitboard.bb = 0;
    AllPieceBitboard.bb = 0;

    for (index = 0; index < 6; ++index)
    {
        PieceBitboard[index].bb = 0;
    }

    ControlledSquares[WHITE] = 0;
    ControlledSquares[BLACK] = 0;

    side = WHITE;
    enPas = NO_SQ;
    fiftyMove = 0;

    ply = 0;
    hisPly = 0;

    castlePerm = 0;

    posKey = 0ULL;
}

uint64_t Board::GeneratePosKey()
{

    Square sq;
    uint64_t finalKey = 0;

    // pieces
    for (int pieceType = 0; pieceType < 6; ++pieceType)
    {
        Bitboard pieces(PieceBitboard[pieceType].bb);
        while (pieces.bb != 0)
        {
            sq = pieces.popLsb();
            Color color = BlackBitboard.getColor(sq);
            finalKey ^= PieceKeys[color][pieceType + 6 * BlackBitboard.get(Square(sq))][sq];
        }
    }

    if (side == WHITE)
    {
        finalKey ^= SideKey;
    }

    if (enPas != NO_SQ)
    {
        ASSERT(enPas >= 0 && enPas < 64);
        ASSERT(RanksBrd[enPas] == RANK_3 || RanksBrd[enPas] == RANK_6);
        finalKey ^= PieceKeys[WHITE][EMPTY][enPas];
    }

    ASSERT(castlePerm >= 0 && castlePerm <= 15);

    finalKey ^= CastleKeys[castlePerm];

    return finalKey;
}

bool Board::ParseFen(const std::string &fen)
{
    int rank = RANK_8;
    int file = FILE_A;
    Piece piece;
    Color side;
    int count = 0;
    int i = 0;
    int index = 0;
    int sq64 = 0;

    Reset();

    while (rank >= RANK_1 && index <= int(fen.length()))
    {
        count = 1;
        char pieceChar = fen[index];
        switch (pieceChar)
        {
        case 'P':
            piece = PAWN;
            side = WHITE;
            break;
        case 'R':
            piece = ROOK;
            side = WHITE;
            break;
        case 'N':
            piece = KNIGHT;
            side = WHITE;
            break;
        case 'B':
            piece = BISHOP;
            side = WHITE;
            break;
        case 'K':
            piece = KING;
            side = WHITE;
            break;
        case 'Q':
            piece = QUEEN;
            side = WHITE;
            break;
        case 'p':
            piece = PAWN;
            side = BLACK;
            break;
        case 'r':
            piece = ROOK;
            side = BLACK;
            break;
        case 'n':
            piece = KNIGHT;
            side = BLACK;
            break;
        case 'b':
            piece = BISHOP;
            side = BLACK;
            break;
        case 'k':
            piece = KING;
            side = BLACK;
            break;
        case 'q':
            piece = QUEEN;
            side = BLACK;
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            piece = EMPTY;
            count = pieceChar - '0';
            break;

        case '/':
        case ' ':
            rank--;
            index++;
            file = FILE_A;
            continue;

        default:
            printf("FEN error \n");
            return -1;
        }

        for (i = 0; i < count; i++)
        {
            sq64 = rank * 8 + file;
            if (piece != EMPTY)
            {
                SetPiece(piece, Square(sq64), side);
            }
            file++;
        }
        index++;
    }

    ASSERT(fen[index] == 'w' || fen[index] == 'b');

    side = (fen[index] == 'w') ? WHITE : BLACK;
    index += 2;

    for (i = 0; i < 4; i++)
    {
        if (fen[index] == ' ')
        {
            break;
        }
        switch (fen[index])
        {
        case 'K':
            castlePerm |= WKCA;
            break;
        case 'Q':
            castlePerm |= WQCA;
            break;
        case 'k':
            castlePerm |= BKCA;
            break;
        case 'q':
            castlePerm |= BQCA;
            break;
        default:
            break;
        }
        index++;
    }
    index++;

    ASSERT(castlePerm >= 0 && castlePerm <= 15);

    if (fen[index] != '-')
    {
        file = fen[index] - 'a';
        rank = fen[index + 1] - '1';

        ASSERT(file >= FILE_A && file <= FILE_H);
        ASSERT(rank >= RANK_1 && rank <= RANK_8);

        enPas = FileRankToSquare(file, rank);
    }

    posKey = GeneratePosKey();

    return 0;
}

Piece Board::GetPieceBySq(Square sq) const
{
    for (int i = 0; i < 6; ++i)
    {
        if (PieceBitboard[i].get(sq))
            return Piece(i);
    }
    return EMPTY;
}

void Board::PrintBoard()
{

    int file, rank, piece;
    Square sq;
    Color color;

    printf("\nGame Board:\n\n");

    for (rank = RANK_8; rank >= RANK_1; rank--)
    {
        printf("%d  ", rank + 1);
        for (file = FILE_A; file <= FILE_H; file++)
        {
            sq = FileRankToSquare(file, rank);
            piece = GetPieceBySq(sq);

            if (piece == EMPTY)
            {
                printf("  .");
                continue;
            }
            color = Color(BlackBitboard.get(sq));

            printf("%3c", PceChar[color][piece]);
        }
        printf("\n");
    }

    printf("\n   ");
    for (file = FILE_A; file <= FILE_H; file++)
    {
        printf("%3c", 'a' + file);
    }
    printf("\n");
    printf("side:%c\n", SideChar[side]);
    printf("enPas:%s\n", PrSq(enPas));
    printf("castle:%c%c%c%c\n",
           castlePerm & WKCA ? 'K' : '-',
           castlePerm & WQCA ? 'Q' : '-',
           castlePerm & BKCA ? 'k' : '-',
           castlePerm & BQCA ? 'q' : '-');
    printf("PosKey:%llX\n", posKey);
}