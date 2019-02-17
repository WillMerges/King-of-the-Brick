#include "eval.h"
#include "search.h"
#include <limits.h>
#include "movegen.h"
#include "eval.h"

bool canDoNullMove;

Move getBestMove(Board * board, Config * config){
    ExtMove moves[MAX_MOVES];
    int moveCount = getAllLegalMoves(board, moves);
    int lowest = INT_MAX;
    int highest = INT_MIN;
    Move move_highest;
    Move move_lowest;
    for(int i=0; i<moveCount; i++) {
        board->makeMove(moves[i].move);
        int eval = alphaBeta(board, INT_MIN, INT_MAX, config->depth-1,NULL);
        board->undoMove();
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

void calculateMovetime(Board * board, Config * config){
    if(config->wtime != 0 || config->btime != 0){
        int time = board->pos->whiteToMove ? config->wtime : config->btime;
        config->movetime = ((400/board->pos->moveNumber)+15) / time;
    }
}

int alphaBeta(Board * board, int alpha, int beta, int depth,LINE * pline){
    ExtMove moves[MAX_MOVES];
    int moveCount = getAllLegalMoves(board,moves);
    int eval = evaluate(board);
    if(depth <= 0 || moveCount <= 0){
        return eval;
    }
    
    //Null pruning
    if(canDoNullMove && evaluate(board) > 1000 && !board->isOwnKingInCheck() && board->pos->moveNumber){
        canDoNullMove=false;
        board->makeNullMove();
        int reduction = depth/4+3;
        int val = -alphaBeta(board, -beta, -alpha, depth-reduction, NULL);
        board->undoMove();
        canDoNullMove=true;
        if(val >= beta){
            return beta;
        }
    }


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
