#include "bitboard.h"
#include "types.h"
U64 squareMasks[64]={};
U64 rankMasks[8]={};
U64 fileMasks[8]={};
U8 getFile(U8 sq){
    return sq%8;
}
char alge1[10]={};
char * getAlgebraicPos1(U8 loc){
	if(loc == NO_ENPASSANT){
		alge1[0]='-';
		alge1[1]='\0';
		return alge1;
	}
	alge1[0] = (loc % 8)+'a';
	alge1[1]= (loc /8)+'1';
	alge1[2]='\0';
	return alge1;
}	
 U8 algebraicPosToLoc1(const char * pos){
	if(*pos=='-'){
		return NO_ENPASSANT;
	}
	return (pos[0]-'a')+(pos[1]-'0'-1)*8;
}

U8 getRank(U8 sq){
    return sq/8;
}
 U64 lowestOneBit(U64 i){
	return i & -((S64) i);
}

U8 popcnt(U64 bb){
    return __builtin_popcountll(bb);
}

U8 getSquare(U8 rank, U8 file){
    return file+rank*8;
}

void initBBHelpers(){
    for(int i = 0; i < 64; i++){
        squareMasks[i] = (1ULL << i);
    }
    for(int i = 0; i < 64; i++){
        rankMasks[getRank(i)] |= squareMasks[i];
		fileMasks[getFile(i)] |= squareMasks[i];
    }
}

int trailingZeroCount(U64 bb) {
   static const int lookup67[67+1] = {
      64,  0,  1, 39,  2, 15, 40, 23,
       3, 12, 16, 59, 41, 19, 24, 54,
       4, -1, 13, 10, 17, 62, 60, 28,
      42, 30, 20, 51, 25, 44, 55, 47,
       5, 32, -1, 38, 14, 22, 11, 58,
      18, 53, 63,  9, 61, 27, 29, 50,
      43, 46, 31, 37, 21, 57, 52,  8,
      26, 49, 45, 36, 56,  7, 48, 35,
       6, 34, 33, -1 };
   return lookup67[(bb & -bb) % 67];
}
