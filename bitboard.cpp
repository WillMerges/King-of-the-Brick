#include "bitboard.h"
#include "types.h"

U8 getFile(U8 sq){
    return sq%8;
}

U8 getRank(U8 sq){
    return sq/8;
}

U8 popcnt(U64 bb){
    return __builtin_popcountll(bb);
}

U8 getSquare(U8 rank, U8 file){
    return file+rank*8;
}

