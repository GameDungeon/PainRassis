#include "constants.h"
#include "board.h"
#include "utils.h"

void Board::SetPiece(Piece piece, Square sq, Color color)
{
    PieceBitBoard[piece].set(sq);
    SidedBitBoard.setTo(sq, color);
}

void Board::ClearPiece(Piece piece, Square sq)
{
    PieceBitBoard[piece].clear(sq);
    SidedBitBoard.clear(sq);
}

void Board::Reset()
{
    int index = 0;

    SidedBitBoard.bb = 0;

    for(index = 0; index < 6; ++index)
    {
        PieceBitBoard[index].bb = 0;
    }

    side = WHITE;
    enPas = NO_SQ;
    fiftyMove = 0;

    ply = 0;
    hisPly = 0;

    castlePerm = 0;

    posKey = 0ULL;
}

uint64_t Board::GeneratePosKey() {

	int sq = 0;
	uint64_t finalKey = 0;
	
	// pieces
	for(int pieceType = 0; pieceType < 6; ++pieceType) {
		Bitboard pieces(PieceBitBoard[pieceType].bb);
        while (pieces.bb != 0)
        {
            sq = pieces.popLsb();
            finalKey ^= PieceKeys[pieceType + 6 * SidedBitBoard.get(Square(sq))][sq];
        }
	}
	
	if(side == WHITE) {
		finalKey ^= SideKey;
	}
		
	if(enPas != NO_SQ) {
		ASSERT(enPas>=0 && enPas<64);
		ASSERT(RanksBrd[enPas] == RANK_3 || RanksBrd[enPas] == RANK_6);
		finalKey ^= PieceKeys[EMPTY][enPas];
	}
	
	ASSERT(castlePerm>=0 && castlePerm<=15);
	
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

	while(rank >= RANK_1 && index <= fen.length())
    {
	    count = 1;
        char peiceChar = fen[index];
		switch (peiceChar) {
            case 'P': piece = PAWN; side = WHITE; break;
            case 'R': piece = ROOK; side = WHITE; break;
            case 'N': piece = KNIGHT; side = WHITE; break;
            case 'B': piece = BISHOP; side = WHITE; break;
            case 'K': piece = KING; side = WHITE; break;
            case 'Q': piece = QUEEN; side = WHITE; break;
            case 'p': piece = PAWN; side = BLACK; break;
            case 'r': piece = ROOK; side = BLACK; break;
            case 'n': piece = KNIGHT; side = BLACK; break;
            case 'b': piece = BISHOP; side = BLACK; break;
            case 'k': piece = KING; side = BLACK; break;
            case 'q': piece = QUEEN; side = BLACK; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = peiceChar - '0';
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

		for (i = 0; i < count; i++) {
            sq64 = rank * 8 + file;
            if (piece != EMPTY) {
                SetPiece(piece, Square(sq64), side);
            }
			file++;
        }
        index++;
	}

	ASSERT(fen[index] == 'w' || fen[index] == 'b');

	side = (fen[index] == 'w') ? WHITE : BLACK;
	index += 2;

	for (i = 0; i < 4; i++) {
        if (fen[index] == ' ') {
            break;
        }
		switch(fen[index]) {
			case 'K': castlePerm |= WKCA; break;
			case 'Q': castlePerm |= WQCA; break;
			case 'k': castlePerm |= BKCA; break;
			case 'q': castlePerm |= BQCA; break;
			default:	     break;
        }
		index++;
	}
	index++;

	ASSERT(castlePerm>=0 && castlePerm <= 15);

	if (fen[index] != '-') {
		file = fen[index] - 'a';
		rank = fen[index+1] - '1';

		ASSERT(file>=FILE_A && file <= FILE_H);
		ASSERT(rank>=RANK_1 && rank <= RANK_8);

		enPas = FileRankToSquare(file,rank);
    }

	posKey = GeneratePosKey();

	return 0;
}

Piece Board::GetPieceBySq(Square sq)
{
    for(int i = 0; i < 6; ++i)
    {
        if(PieceBitBoard[i].get(sq))
            return Piece(i);
    }
    return EMPTY;
}

void Board::PrintBoard() {

	int file,rank,piece;
    Square sq;
    Color side;

	printf("\nGame Board:\n\n");

	for(rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ",rank+1);
		for(file = FILE_A; file <= FILE_H; file++) {
			sq = FileRankToSquare(file,rank);
			piece = GetPieceBySq(sq);

            if(piece == EMPTY)
            {
                printf("  .");
                continue;
            }
            side = Color(SidedBitBoard.get(sq));

			printf("%3c",PceChar[side][piece]);
		}
		printf("\n");
	}

	printf("\n   ");
	for(file = FILE_A; file <= FILE_H; file++) {
		printf("%3c",'a'+file);
	}
	printf("\n");
	printf("side:%c\n",SideChar[side]);
	printf("enPas:%d\n",enPas);
	printf("castle:%c%c%c%c\n",
			castlePerm & WKCA ? 'K' : '-',
			castlePerm & WQCA ? 'Q' : '-',
			castlePerm & BKCA ? 'k' : '-',
			castlePerm & BQCA ? 'q' : '-'
			);
	printf("PosKey:%llX\n",posKey);
}