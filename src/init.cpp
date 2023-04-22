#include "constants.h"
#include "utils.h"
#include "attacks.h"
#include "magics.h"

uint64_t PieceKeys[12][64];
uint64_t SideKey;
uint64_t CastleKeys[16];

int FilesBrd[64];
int RanksBrd[64];

void InitHashKeys() {
	int index = 0;
	int index2 = 0;
	for(index = 0; index < 12; ++index) {
		for(index2 = 0; index2 < 64; ++index2) {
			PieceKeys[index][index2] = random_uint64();
		}
	}
	SideKey = random_uint64();
	for(index = 0; index < 16; ++index) {
		CastleKeys[index] = random_uint64();
	}

}

void InitFilesRanksBrd() {
	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;

	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FileRankToSquare(file,rank);
			FilesBrd[sq] = file;
			RanksBrd[sq] = rank;
		}
	}
}

void InitMagicTables() {
	initBishopMagicTable();
	initRookMagicTable();
}

void AllInit() {
	InitHashKeys();
    InitFilesRanksBrd();
	InitMagicTables();
}