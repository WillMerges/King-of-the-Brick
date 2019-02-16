#include "board.h"
#include <string>
#include "move.h"
#include "types.h"

#include <string.h>

Position allPos[MAX_MOVES];

void Board::undoMove(){
    pos=pos->prevPos;
}

void Board::makeNullMove(){
    Position * newPos = &allPos[pos->moveNumber+1];
    memcpy(newPos,pos,sizeof(Position));

    newPos->moveNumber+=1;
}