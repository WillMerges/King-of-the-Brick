#include "eval.h"
#include "types.h"
#include <limits.h>

//Pawn, King, Ring, Knight, Bishop, Queen
int piece_values[6] = {0,0,0,0,0,0};

//if king
int middle_values[4] = {35,36,27,28};

int evaluate(Board *board) {
    Board b = *board;
    if(b.isDrawn()) {
        return 0;
    } else if(b.isKingInCenter() || b.isCheckmate()) {
        if(b::pos->whiteToMove()) {
            //black won?
            return -INT_MAX;
        } else {
            //white won?
            return INT_MIN;
        }
    }

    int material = material_eval(b.prevPos);
}

int material_eval(Position *p) {
    int white = 0;
    int black = 0;
    for(int i=0; i<NUM_PIECE_TYPES; i++) {
        U64 white_pieces = p->whitePieces[i];
        U64 black_pieces = p->blackPieces[i];
        for(int j=0; j<64; j++) {
            int piecew = white_pieces % 10;
            int pieceb = black_pieces % 10;
            if(piecew) {
                white += piece_values[i];
            } if(pieceb) {
                black += piece_values[i];
            }
            whitePieces /= 10;
        }
    }
    return white - black;
}
