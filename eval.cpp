#include "eval.h"
#include "types.h"
#include "bitboard.h"
#include <limits.h>

//Pawn, King, Rook, Knight, Bishop, Queen
int piece_values[6] = {1,0,5,1,1,9};

//if king
int middle_values[4] = {35,36,27,28};


int material_eval(Position *p) {
    int white = 0;
    int black = 0;
    for(int i=0; i<NUM_PIECE_TYPES; i++) {
        U64 white_pieces = p->whitePieces[i];
        U64 black_pieces = p->blackPieces[i];

        white += popcnt(white_pieces);
        black += popcnt(black_pieces);
    }
    return white - black;
}


int evaluate(Board *board) {
    if(board->isDrawn()) {
        return 0;
    } else if(board->isKingInCenter() || board->isCheckmate()) {
        if(board->pos->whiteToMove) {
            //black won?
            return -INT_MAX;
        } else {
            //white won?
            return INT_MIN;
        }
    }

    int material = material_eval(board->pos);
}
