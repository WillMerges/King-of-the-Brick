#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED
#include "types.h"

//The move structure looks like the following.
//bits 0-5  (move start)
//bits 6-11 (move dest)
//bits 12-14 (PieceType moved)
//bits 15-16 (MoveType type)
//bits 17-19 (PieceType promote)


typedef U32 Move;

enum MoveType{
    NORMAL=0,
    PROMOTION=1,
    CASTLE=2,
    ENPASSANT=3
};

constexpr Move createMove(U8 from, U8 to, PieceType pieceMoved){
    return (pieceMoved<<12)+(from<<6)+to;
}

constexpr Move createMove(U8 from, U8 to, PieceType pieceMoved, MoveType moveType, PieceType promoteTo){
    return (promoteTo<<17)+(moveType<<15)+(pieceMoved<<16)+(from<<8)+to;
}

constexpr U8 dest_sq(Move move){
    return (move>>6)&7;
}

constexpr U8 to_sq(Move move){
    return move & 7;
}

constexpr PieceType get_piece_moved(Move move){
    return PieceType((move>>12)&7);
}

constexpr MoveType get_move_type(Move move){
    return MoveType((move>>15)&3);
}

constexpr PieceType get_promotion_type(Move move){
    return PieceType((move>>17)&7);
}

#endif