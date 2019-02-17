#include "eval.h"
#include "search.h"
#include "movegen.h"

int getBestMove(Board * baord, Config * config){

}

int alphaBeta(Board * board, int alpha, int beta, int depth){
    ExtMove moves[MAX_MOVES];
    int moveCount = getAllLegalMoves(board,moves);
    int eval = evaluate(board);
    if(depth <= 0 || moveCount <= 0){
        return eval;
    }
    //Do null move pruning
    int val;
    int adjDepth=depth-1;
    bool whiteToMove=board->pos->whiteToMove;
    for(int i = 0; i < moveCount; i++){
        Move move = moves[i].move;
        board->makeMove(move);
        val = alphaBeta(board,-beta,-alpha,adjDepth);
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

