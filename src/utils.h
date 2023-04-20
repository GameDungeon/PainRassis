#pragma once

extern inline int FileRankToSquare(int file, int rank)
{
    return rank * 8 + file;
}