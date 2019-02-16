#include <stdint.h>
#include <stdbool.h>

typedef uint64_t U64;
typedef int64_t S64;
typedef int8t_t S8;
typedef uint8_t U8


enum PieceTypes{
    PAWN=0,
    KING=1,
    ROOK=2,
    KNIGHT=3,
    BISHOP=4,
    QUEEN=5
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
