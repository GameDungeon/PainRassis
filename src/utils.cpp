#include "utils.h"

inline int FileRankToSquare(int file, int rank)
{
    return rank * 8 + file;
}