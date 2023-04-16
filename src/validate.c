// validate.c

#include "defs.h"
#include "stdio.h"
#include "string.h"

int MoveListOk(const S_MOVELIST *list,  const S_BOARD *pos) {
	if(list->count < 0 || list->count >= MAXPOSITIONMOVES) {
		return FALSE;
	}

	int MoveNum;
	int from = 0;
	int to = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
		to = TOSQ(list->moves[MoveNum].move);
		from = FROMSQ(list->moves[MoveNum].move);
		if(!SqOnBoard(to) || !SqOnBoard(from)) {
			return FALSE;
		}
		if(!PieceValid(pos->pieces[from])) {
			PrintBoard(pos);
			return FALSE;
		}
	}

	return TRUE;
}

int SqIs120(const int sq) {
	return (sq>=0 && sq<120);
}

int PceValidEmptyOffbrd(const int pce) {
	return (PieceValidEmpty(pce) || pce == OFFBOARD);
}
int SqOnBoard(const int sq) {
	return FilesBrd[sq]==OFFBOARD ? 0 : 1;
}

int SideValid(const int side) {
	return (side==WHITE || side == BLACK) ? 1 : 0;
}

int FileRankValid(const int fr) {
	return (fr >= 0 && fr <= 7) ? 1 : 0;
}

int PieceValidEmpty(const int pce) {
	return (pce >= EMPTY && pce <= bK) ? 1 : 0;
}

int PieceValid(const int pce) {
	return (pce >= wP && pce <= bK) ? 1 : 0;
}

void DebugAnalysisTest(S_BOARD *pos, S_SEARCHINFO *info, S_HASHTABLE *table) {

	FILE *file;
    file = fopen("lct2.epd","r");
    char lineIn [1024];

	info->depth = MAXDEPTH;
	info->timeset = TRUE;
	int time = 1140000;


    if(file == NULL) {
        printf("File Not Found\n");
        return;
    }  else {
        while(fgets (lineIn , 1024 , file) != NULL) {
			info->starttime = GetTimeMs();
			info->stoptime = info->starttime + time;
			ClearHashTable(table);
            ParseFen(lineIn, pos);
            printf("\n%s\n",lineIn);
			printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
				time,info->starttime,info->stoptime,info->depth,info->timeset);
			SearchPosition(pos, info, table);
            memset(&lineIn[0], 0, sizeof(lineIn));
        }
    }
}