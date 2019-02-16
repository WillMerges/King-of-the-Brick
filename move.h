#include "types.h"

//The move structure looks like the following.
//
//

typedef U32 Move;

enum MoveType{
    NORMAL=0,
    PROMOTION=1,
    CASTLE=2,
    ENPASSANT
}

constexpr Move makeMove(U8 from, U8 to, PieceType pieceMoved){
    return (pieceMoved<<16)+(from<<8)+to;
}

constexpr Move makeMove(U8 from, U8 to, PieceType pieceMoved, MoveType moveType, PieceType promoteTo){
    return moveType<<+(pieceMoved<<16)+(from<<8)+to;
}