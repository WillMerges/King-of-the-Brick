#include "eval.h"
#include "search.h"
#include <limits.h>
#include "movegen.h"


Move getBestMove(Board * board, Config * config){
    ExtMove moves[MAX_MOVES];
    int moveCount = getAllLegalMoves(board, moves);
    int lowest = INT_MAX;
    int highest = INT_MIN;
    Move move_highest;
    Move move_lowest;
    for(int i=0; i<moveCount; i++) {
        int eval = alphaBeta(board, INT_MIN, INT_MAX, 1,NULL);
        if(eval > highest) {
            highest = eval;
            move_highest = moves[i].move;
        } if(eval < lowest) {
            lowest = eval;
            move_lowest = moves[i].move;
        }
        
    }
    if(board->pos->whiteToMove) {
        return move_highest;
    } else {
        return move_lowest;
    }
}

int alphaBeta(Board * board, int alpha, int beta, int depth,LINE * pline){
    ExtMove moves[MAX_MOVES];
    int moveCount = getAllLegalMoves(board,moves);
    int eval = evaluate(board);
    if(depth <= 0 || moveCount <= 0){
        return eval;
    }
    //Do null move pruning
    int adjDepth=depth-1;
    bool whiteToMove=board->pos->whiteToMove;
    for(int i = 0; i < moveCount; i++){
        Move move = moves[i].move;
        board->makeMove(move);
        int val = -alphaBeta(board,-beta,-alpha,adjDepth,NULL);
        board->undoMove();
        if(val > alpha){
            alpha=val;
            if(val >= beta){
                return beta;
            }
        }
    }
    return alpha;
}

//get all legal moves
//evaluate
