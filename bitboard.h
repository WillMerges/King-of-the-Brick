#include <types.h>

typedef U64 Bitboard;

U8 getFile(U8);
U8 getRank(U8);
U8 popcnt(U64);

U8 getSquare(U8 rank, U8 file);

char[3] getSquareAsAlgebraicLoc(U8 sq);