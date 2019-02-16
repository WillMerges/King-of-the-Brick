#ifdef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include "types.h"
#include "bitboard.h"
typedef struct{
    U64 zobrist;

    U16 moveNumber;
    bool whiteToMove;

    U8 enPassantLoc;
    U8 fiftyMoveCount;

    Bitboard whitePieces[NUM_PIECE_TYPES];
    Bitboard blackPieces[NUM_PIECE_TYPES];
    Bitboard WhitePiecesBB;
    Bitboard BlackPiecesBB;
    Bitboard AllPiecesBB;
    //Move lastMove;
} Position;

#endif