#include "board.h"
#include <string>
#include "types.h"
#include "moves.h"
#include <string.h>

Position allPos[MAX_MOVES];

void Board::undoMove(){
    pos=pos->prevPos;
}

void Board::makeNullMove(){
    Position * newPos = &allPos[pos->moveNumber+1];
    memcpy(newPos,pos,sizeof(Position));
}