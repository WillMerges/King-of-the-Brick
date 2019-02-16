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

    newPos->enPassantLoc=NO_ENPASSANT;
    newPos->prevPos=pos;
    newPos->moveNumber+=1;
    newPos->fiftyMoveRule+=1;


    //TODO update zobrist
    newPos->whiteToMove!=pos->whiteToMove;   
    pos=newPos;
}

Position * Board::getCurPos(){
    return pos;
}

void Board::makeMove(Move move){
    PieceType pieceToMove = get_piece_moved(move);
    U8 from = from_sq(move);
    U8 to = to_sq(move);

    Bitboard * moverPieces = pos->whiteToMove ? pos->whitePieces : pos->blackPieces; //getting the pieces of who is moving
    moverPieces[pieceToMove] =  moverPieces[pieceToMove] & ~squareMasks[from]; //setting the bit of where the moving piece came from to 0
    if (get_move_type(move) == PROMOTION){
        
    }
}

bool Board::parseFen(std::string fen){
    
}