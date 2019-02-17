#include "eval.h"
#include "types.h"
#include "bitboard.h"
#include <limits.h>

//Pawn, King, Rook, Knight, Bishop, Queen
int piece_values[6] = {100,0,480,300,315,900};
int centralization[] = {0,0,00,0,0,0,0,0,
						0,2,02,02,02,02,2,0,
						0,2,15,20,20,15,2,0,
						0,2,20,40,40,20,2,0,
						0,2,20,40,40,20,2,0,
						0,2,15,20,20,15,02,0,
						0,2,02,02,02,02,02,0,
						0,0,0,0,0,0,0,0};
int centralizationValue(U64 bb){
	int eval = 0;
	while(bb != 0){
		U64 loc = trailingZeroCount(bb);
		eval+=centralization[loc];
		bb ^= ( U64(1LL << loc));
	}
	return eval;
}

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
    int eval = 0;
    Position *p = board->pos;
    for(int i = 0; i < NUM_PIECE_TYPES; i++){
        U64 white_pieces = p->whitePieces[i];
        U64 black_pieces = p->blackPieces[i];
        eval+=popcnt(white_pieces)*piece_values[i];
        eval-=popcnt(black_pieces)*piece_values[i];
        eval+=centralizationValue(white_pieces);
        eval-=centralizationValue(black_pieces);
    }
    return eval;
}
