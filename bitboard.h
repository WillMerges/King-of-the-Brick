#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED
#include "types.h"

typedef U64 Bitboard;

U8 getFile(U8);
U8 getRank(U8);
U8 popcnt(U64);

U8 getSquare(U8 rank, U8 file);

extern U64 squareMasks[64];
extern U64 rankMasks[8];
extern U64 fileMasks[8];
void initBBHelpers();

int trailingZeroCount(U64 bb);
#endif