// movecount.c

#include "stdio.h"
#include "defs.h"
#include "moves.h"

static int MoveLegal(const S_BOARD *pos, int move, int side)
{
	if (SqAttacked(pos->KingSq[side], side^1, pos))
	{
		return FALSE;
	}

	return TRUE;
}

static int AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, int side)
{
	ASSERT(SqOnBoard(from));
	ASSERT(SqOnBoard(to));

	if (RanksBrd[from] == RANK_7)
	{
		if (MoveLegal(pos, MOVE(from, to, EMPTY, wQ, 0), side))
			return 4;
		return 0;
	}
	else
	{
		if (MoveLegal(pos, MOVE(from, to, EMPTY, EMPTY, 0), side))
			return 1;
		return 0;
	}
}

static int AddBlackPawnMove(const S_BOARD *pos, const int from, const int to, int side)
{

	ASSERT(SqOnBoard(from));
	ASSERT(SqOnBoard(to));

	if (RanksBrd[from] == RANK_2)
	{
		if (MoveLegal(pos, MOVE(from, to, EMPTY, bQ, 0), side))
			return 4;
		return 0;
	}
	else
	{
		if (MoveLegal(pos, MOVE(from, to, EMPTY, EMPTY, 0), side))
			return 1;
		return 0;
	}
}

int CountLegalAllMoves(const S_BOARD *pos, int side)
{
	ASSERT(CheckBoard(pos));

	int legal = 0;

	int pce = EMPTY;
	int sq = 0;
	int t_sq = 0;
	int pceNum = 0;
	int dir = 0;
	int index = 0;
	int pceIndex = 0;

	if (side == WHITE)
	{

		for (pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum)
		{
			sq = pos->pList[wP][pceNum];
			ASSERT(SqOnBoard(sq));

			if (pos->pieces[sq + 10] == EMPTY)
			{
				legal += AddWhitePawnMove(pos, sq, sq + 10, side);
				if (RanksBrd[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY)
				{
					if (MoveLegal(pos, MOVE(sq, (sq + 20), EMPTY, EMPTY, MFLAGPS), side))
						legal += 1;
				}
			}

			if (!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK)
			{
				legal += AddWhitePawnMove(pos, sq, sq + 9, side);
			}
			if (!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK)
			{
				legal += AddWhitePawnMove(pos, sq, sq + 11, side);
			}

			if (pos->enPas != NO_SQ)
			{
				if (sq + 9 == pos->enPas)
				{
					if (MoveLegal(pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), side))
						legal += 1;
				}
				if (sq + 11 == pos->enPas)
				{
					if (MoveLegal(pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), side))
						legal += 1;
				}
			}
		}

		if (pos->castlePerm & WKCA)
		{
			if (pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY)
			{
				if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(F1, BLACK, pos))
				{
					if (MoveLegal(pos, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), side))
						legal += 1;
				}
			}
		}

		if (pos->castlePerm & WQCA)
		{
			if (pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[B1] == EMPTY)
			{
				if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(D1, BLACK, pos))
				{
					if (MoveLegal(pos, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), side))
						legal += 1;
				}
			}
		}
	}
	else
	{

		for (pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum)
		{
			sq = pos->pList[bP][pceNum];
			ASSERT(SqOnBoard(sq));

			if (pos->pieces[sq - 10] == EMPTY)
			{
				legal += AddBlackPawnMove(pos, sq, sq - 10, side);
				if (RanksBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY)
				{
					if (MoveLegal(pos, MOVE(sq, (sq - 20), EMPTY, EMPTY, MFLAGPS), side))
						legal += 1;
				}
			}

			if (!SQOFFBOARD(sq - 9) && PieceCol[pos->pieces[sq - 9]] == WHITE)
			{
				legal += AddBlackPawnMove(pos, sq, sq - 9, side);
			}

			if (!SQOFFBOARD(sq - 11) && PieceCol[pos->pieces[sq - 11]] == WHITE)
			{
				legal += AddBlackPawnMove(pos, sq, sq - 11, side);
			}
			if (pos->enPas != NO_SQ)
			{
				if (sq - 9 == pos->enPas)
				{
					if (MoveLegal(pos, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), side))
						legal += 1;
				}
				if (sq - 11 == pos->enPas)
				{
					if (MoveLegal(pos, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), side))
						legal += 1;
				}
			}
		}

		// castling
		if (pos->castlePerm & BKCA)
		{
			if (pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY)
			{
				if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(F8, WHITE, pos))
				{
					if (MoveLegal(pos, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), side))
						legal += 1;
				}
			}
		}

		if (pos->castlePerm & BQCA)
		{
			if (pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY)
			{
				if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(D8, WHITE, pos))
				{
					if (MoveLegal(pos, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), side))
						legal += 1;
				}
			}
		}
	}

	/* Loop for slide pieces */
	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex++];
	while (pce != 0)
	{
		ASSERT(PieceValid(pce));

		for (pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum)
		{
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			for (index = 0; index < NumDir[pce]; ++index)
			{
				dir = PceDir[pce][index];
				t_sq = sq + dir;

				while (!SQOFFBOARD(t_sq))
				{
					// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
					if (pos->pieces[t_sq] != EMPTY)
					{
						if (PieceCol[pos->pieces[t_sq]] == (side ^ 1))
						{
							if (MoveLegal(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), side))
								legal += 1;
						}
						break;
					}
					if (MoveLegal(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), side))
						legal += 1;
					t_sq += dir;
				}
			}
		}

		pce = LoopSlidePce[pceIndex++];
	}

	/* Loop for non slide */
	pceIndex = LoopNonSlideIndex[side];
	pce = LoopNonSlidePce[pceIndex++];

	while (pce != 0)
	{
		ASSERT(PieceValid(pce));

		for (pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum)
		{
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			for (index = 0; index < NumDir[pce]; ++index)
			{
				dir = PceDir[pce][index];
				t_sq = sq + dir;

				if (SQOFFBOARD(t_sq))
				{
					continue;
				}

				// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
				if (pos->pieces[t_sq] != EMPTY)
				{
					if (PieceCol[pos->pieces[t_sq]] == (side ^ 1))
					{
						if (MoveLegal(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), side))
							legal += 1;
					}
					continue;
				}
				if (MoveLegal(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), side))
					legal += 1;
			}
		}

		pce = LoopNonSlidePce[pceIndex++];
	}

	return legal;
}

int CountAllMoves(const S_BOARD *pos, int side)
{
	ASSERT(CheckBoard(pos));

	int moves = 0;

	int pce = EMPTY;
	int sq = 0;
	int t_sq = 0;
	int pceNum = 0;
	int dir = 0;
	int index = 0;
	int pceIndex = 0;

	if (side == WHITE)
	{

		for (pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum)
		{
			sq = pos->pList[wP][pceNum];
			ASSERT(SqOnBoard(sq));

			if (pos->pieces[sq + 10] == EMPTY)
			{
				moves += AddWhitePawnMove(pos, sq, sq + 10, side);
				if (RanksBrd[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY)
				{
					moves += 1;
				}
			}

			if (!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK)
			{
				moves += AddWhitePawnMove(pos, sq, sq + 9, side);
			}
			if (!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK)
			{
				moves += AddWhitePawnMove(pos, sq, sq + 11, side);
			}

			if (pos->enPas != NO_SQ)
			{
				if (sq + 9 == pos->enPas)
					moves += 1;
				if (sq + 11 == pos->enPas)
					moves += 1;
			}
		}

		if (pos->castlePerm & WKCA)
		{
			if (pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY)
			{
				if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(F1, BLACK, pos))
				{
					moves += 1;
				}
			}
		}

		if (pos->castlePerm & WQCA)
		{
			if (pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[B1] == EMPTY)
			{
				if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(D1, BLACK, pos))
				{
					moves += 1;
				}
			}
		}
	}
	else
	{

		for (pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum)
		{
			sq = pos->pList[bP][pceNum];
			ASSERT(SqOnBoard(sq));

			if (pos->pieces[sq - 10] == EMPTY)
			{
				moves += AddBlackPawnMove(pos, sq, sq - 10, side);
				if (RanksBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY)
					moves += 1;
			}

			if (!SQOFFBOARD(sq - 9) && PieceCol[pos->pieces[sq - 9]] == WHITE)
			{
				moves += AddBlackPawnMove(pos, sq, sq - 9, side);
			}

			if (!SQOFFBOARD(sq - 11) && PieceCol[pos->pieces[sq - 11]] == WHITE)
			{
				moves += AddBlackPawnMove(pos, sq, sq - 11, side);
			}
			if (pos->enPas != NO_SQ)
			{
				if (sq - 9 == pos->enPas)
					moves += 1;
				if (sq - 11 == pos->enPas)
					moves += 1;
			}
		}

		// castling
		if (pos->castlePerm & BKCA)
		{
			if (pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY)
			{
				if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(F8, WHITE, pos))
				{
					moves += 1;
				}
			}
		}

		if (pos->castlePerm & BQCA)
		{
			if (pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY)
			{
				if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(D8, WHITE, pos))
				{
					moves += 1;
				}
			}
		}
	}

	/* Loop for slide pieces */
	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex++];
	while (pce != 0)
	{
		ASSERT(PieceValid(pce));

		for (pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum)
		{
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			for (index = 0; index < NumDir[pce]; ++index)
			{
				dir = PceDir[pce][index];
				t_sq = sq + dir;

				while (!SQOFFBOARD(t_sq))
				{
					// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
					if (pos->pieces[t_sq] != EMPTY)
					{
						if (PieceCol[pos->pieces[t_sq]] == (side ^ 1))
						{
							moves += 1;
						}
						break;
					}
					moves += 1;
					t_sq += dir;
				}
			}
		}

		pce = LoopSlidePce[pceIndex++];
	}

	/* Loop for non slide */
	pceIndex = LoopNonSlideIndex[side];
	pce = LoopNonSlidePce[pceIndex++];

	while (pce != 0)
	{
		ASSERT(PieceValid(pce));

		for (pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum)
		{
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			for (index = 0; index < NumDir[pce]; ++index)
			{
				dir = PceDir[pce][index];
				t_sq = sq + dir;

				if (SQOFFBOARD(t_sq))
				{
					continue;
				}

				// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
				if (pos->pieces[t_sq] != EMPTY)
				{
					if (PieceCol[pos->pieces[t_sq]] == (side ^ 1))
					{
						moves += 1;
					}
					continue;
				}
				moves += 1;
			}
		}

		pce = LoopNonSlidePce[pceIndex++];
	}

	return moves;
}