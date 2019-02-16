#include "board.h"
#include <string>
#include "move.h"
#include "types.h"
#include <string.h>
#include <sstream>
#include "bbmagic.h"

Position allPos[MAX_MOVES];

bool Board::isOwnKingInCheck(){
	if(pos->whiteToMove)
		return isSquareAttacked(pos, pos -> whitePieces[KING],true);
	else
		return isSquareAttacked(pos, pos -> blackPieces[KING],false);
}


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
    //Setting up some local variables to make things easier later on
    PieceType pieceToMove = get_piece_moved(move);
    U8 from = from_sq(move);
    U8 to = to_sq(move);
    Bitboard * moverPieces = pos->whiteToMove ? pos->whitePieces : pos->blackPieces; //getting the pieces of who is moving
    Bitboard * opponentPieces = pos->whiteToMove ? pos->blackPieces : pos->whitePieces; //getting the pieces of who is not moving
    MoveType moveType = get_move_type(move);

    //Setting up the move
    moverPieces[pieceToMove] =  moverPieces[pieceToMove] & ~squareMasks[from]; //setting the bit of where the moving piece came from to 0

    for (U8 pieceTypeInd = 0; pieceTypeInd < 6; pieceTypeInd++){ //looping through types of pieces on the opponent side
        opponentPieces[pieceTypeInd] = opponentPieces[pieceToMove] & ~squareMasks[to]; //setting the bit for that square to 0
    }

    switch (moveType)
    {
        case NORMAL:
            moverPieces[pieceToMove] =  moverPieces[pieceToMove] | squareMasks[to];
            break;

        case PROMOTION:
            PieceType newPiece = get_promotion_type(move);
            moverPieces[pieceToMove] =  moverPieces[pieceToMove] | squareMasks[to];
            break;

        case CASTLING:
            if (pos->whiteToMove){
                if(to == 2){ //If white king is moving to C1
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[3];
                }
                else { //If white king is moving to G1
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[5];
                }
            }
            else{
                if(to == 58){ //If black king is moving to C8
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[59];
                }
                else { //If black king is moving to G8
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[61];
                }
            }
            moverPieces[pieceToMove] =  moverPieces[pieceToMove] | squareMasks[to];
            break;
        
        case ENPASSANT:
            if (pos->whiteToMove){
                opponentPieces
            }
            else{

            }
            break;

        default:
            break;
    }
}

bool Board::parseFen(std::string fen){
    std::string token;
    std::istringstream ss(fen);
}
