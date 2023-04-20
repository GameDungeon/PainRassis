#include "constants.h"
#include "board.h"
#include "init.h"

uint64_t GeneratePosKey(const Board& pos) {

	int sq = 0;
	uint64_t finalKey = 0;
	
	// pieces
	for(int pieceType = 0; pieceType < 6; ++pieceType) {
		Bitboard pieces(pos.PieceBitBoard[pieceType].bb);
        while (pieces.bb != 0)
        {
            sq = pieces.popLsb();
            finalKey ^= PieceKeys[pieceType][sq];
        }
	}
	
	if(pos.side == WHITE) {
		finalKey ^= SideKey;
	}
		
	if(pos.enPas != NO_SQ) {
		ASSERT(pos.enPas>=0 && pos.enPas<64);
		ASSERT(RanksBrd[pos.enPas] == RANK_3 || RanksBrd[pos.enPas] == RANK_6);
		finalKey ^= PieceKeys[EMPTY][pos.enPas];
	}
	
	ASSERT(pos.castlePerm>=0 && pos.castlePerm<=15);
	
	finalKey ^= CastleKeys[pos.castlePerm];
	
	return finalKey;
}