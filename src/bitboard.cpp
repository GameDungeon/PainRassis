#include "bitboard.h"
#include "utils.h"
#include <string>
#include <cstring>
#include <iostream>

void Bitboard::PrintBitboard() const
{
	Bitboard shiftMe = 1ULL;

	int rank = 0;
	int file = 0;
	int sq = 0;

	printf("\n");
	for (rank = RANK_8; rank >= RANK_1; --rank)
	{
		for (file = FILE_A; file <= FILE_H; ++file)
		{
			sq = FileRankToSquare(file, rank);

			if ((shiftMe << sq) & bb)
				printf("X");
			else
				printf("-");
		}
		printf("\n");
	}
	printf("\n\n");
}
