#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t U64;
typedef int64_t S64;
typedef int8_t S8;
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

enum PieceType{
    PAWN=0,
    KING=1,
    ROOK=2,
    KNIGHT=3,
    BISHOP=4,
    QUEEN=5,
};

typedef struct {
	int depth;
	int wtime;
	int btime;
	int winc;
	int binc;
	int movestogo;
	int mate;
	int movetime;
	int infinite;
} Config;

#endif