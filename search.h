#include "board.h"
#include "types.h"
#include "move.h"
#include "movegen.h"

struct LINE{
    Move pastMoves[30];
    int depth;
};
Move getBestMove(Board * board, Config * config);
int alphaBeta(Board * board, int alpha, int beta, int depth, LINE * pline);