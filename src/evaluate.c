// evaluate.c

#include "stdio.h"
#include "defs.h"

const int CenterCovered = 10;

const int PawnIsolated = -10;
const int PawnlessPenalty = -30;

const int BackRankKnightPenalty = -10;
const int BackRankBishopPenalty = -5;

const int RookOpenFile = 10;
const int RookSemiOpenFile = 5;
const int QueenOpenFile = 5;
const int QueenSemiOpenFile = 3;

const int BishopPair = 30;

const int CenterSquares[4] = {28, 29, 36, 37};

#define PAWN wP + peiceOffset
#define KNIGHT wN + peiceOffset
#define BISHOP wB + peiceOffset
#define ROOK wR + peiceOffset
#define QUEEN wQ + peiceOffset
#define KING wK + peiceOffset

int MaterialDraw(const S_BOARD *pos) {

	ASSERT(CheckBoard(pos));
	
    if (!pos->pceNum[wR] && !pos->pceNum[bR] && !pos->pceNum[wQ] && !pos->pceNum[bQ]) {
	  if (!pos->pceNum[bB] && !pos->pceNum[wB]) {
	      if (pos->pceNum[wN] < 3 && pos->pceNum[bN] < 3) {  return TRUE; }
	  } else if (!pos->pceNum[wN] && !pos->pceNum[bN]) {
	     if (abs(pos->pceNum[wB] - pos->pceNum[bB]) < 2) { return TRUE; }
	  } else if ((pos->pceNum[wN] < 3 && !pos->pceNum[wB]) || (pos->pceNum[wB] == 1 && !pos->pceNum[wN])) {
	    if ((pos->pceNum[bN] < 3 && !pos->pceNum[bB]) || (pos->pceNum[bB] == 1 && !pos->pceNum[bN]))  { return TRUE; }
	  }
	} else if (!pos->pceNum[wQ] && !pos->pceNum[bQ]) {
        if (pos->pceNum[wR] == 1 && pos->pceNum[bR] == 1) {
            if ((pos->pceNum[wN] + pos->pceNum[wB]) < 2 && (pos->pceNum[bN] + pos->pceNum[bB]) < 2)	{ return TRUE; }
        } else if (pos->pceNum[wR] == 1 && !pos->pceNum[bR]) {
            if ((pos->pceNum[wN] + pos->pceNum[wB] == 0) && (((pos->pceNum[bN] + pos->pceNum[bB]) == 1) || ((pos->pceNum[bN] + pos->pceNum[bB]) == 2))) { return TRUE; }
        } else if (pos->pceNum[bR] == 1 && !pos->pceNum[wR]) {
            if ((pos->pceNum[bN] + pos->pceNum[bB] == 0) && (((pos->pceNum[wN] + pos->pceNum[wB]) == 1) || ((pos->pceNum[wN] + pos->pceNum[wB]) == 2))) { return TRUE; }
        }
    }
  return FALSE;
}

#define ENDGAME_MAT (1 * PieceVal[wR] + 2 * PieceVal[wN] + 2 * PieceVal[wP] + PieceVal[wK])

static int EvalPositionSided(const S_BOARD *pos, int side) {

	ASSERT(CheckBoard(pos));

	int pce;
	int pceNum;
	int sq;

	int opSide = side^1;
	int peiceOffset = side ? 0 : 6;

	int score = pos->material[side];

	for(int i = 0; i < 4; ++i)
	{
		sq = CenterSquares[i];
		if((pos->pieces[SQ120(sq)] >= (peiceOffset + 1)) && (pos->pieces[SQ120(sq)] < (peiceOffset + 7)))
		{
			score += CenterCovered;
			continue;
		}
	
		if(SqAttacked(sq, side, pos)) {
			score += CenterCovered;
		}
	}

	pce = PAWN;	
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];
		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);
		
		if((IsolatedMask[SQ64(sq)] & pos->pawns[side]) == 0) {
			score += PawnIsolated;
		}
	}

	if(pos->pceNum[pce] <= 0)
	{
		score += PawnlessPenalty;
	}
	
	pce = KNIGHT;	
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		if(RanksBrd[sq] == 1 || RanksBrd[sq] == 8)
		{
			score += BackRankKnightPenalty;
		}
	}	
	
	pce = BISHOP;	
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];

		if(RanksBrd[sq] == 1 || RanksBrd[sq] == 8)
		{
			score += BackRankBishopPenalty;
		}
	}	

	pce = ROOK;	
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];
	
		ASSERT(FileRankValid(FilesBrd[sq]));
		
		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score += RookOpenFile;
		} else if(!(pos->pawns[side] & FileBBMask[FilesBrd[sq]])) {
			score += RookSemiOpenFile;
		}
	}
	
	pce = QUEEN;	
	for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
		sq = pos->pList[pce][pceNum];
	
		ASSERT(SqOnBoard(sq));
		ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);
		ASSERT(FileRankValid(FilesBrd[sq]));
	
		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score += QueenOpenFile;
		} else if(!(pos->pawns[side] & FileBBMask[FilesBrd[sq]])) {
			score += QueenSemiOpenFile;
		}
	}	

	pce = KING;
	sq = pos->pList[pce][0];
	ASSERT(SqOnBoard(sq));
	ASSERT(SQ64(sq)>=0 && SQ64(sq)<=63);
	
	if(pos->pceNum[BISHOP] >= 2) score += BishopPair;
	
	return score;
}

int EvalPosition(const S_BOARD *pos, const int LegalMoveCount, const int OpponentLegalMoveCount) {

	ASSERT(CheckBoard(pos));

	int pce;
	int pceNum;
	int sq;

	if(!pos->pceNum[wP] && !pos->pceNum[bP] && MaterialDraw(pos) == TRUE) {
		return 0;
	}

	int score = 0;

	if(OpponentLegalMoveCount != 0)
	{
		score += (LegalMoveCount - OpponentLegalMoveCount);
	}

	score += EvalPositionSided(pos, pos->side) - EvalPositionSided(pos, pos->side^1);

	return score;
}