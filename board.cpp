#include "board.h"
#include <string>
#include "move.h"
#include "types.h"
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include "movegen.h"
#include <stdbool.h>
#include "bbmagic.h"
Position allPos[MAX_MOVES];
const U64 centerSquares = squareMasks[35] | squareMasks[36] | squareMasks[27] | squareMasks[28];

void Board::undoMove(){
    pos=pos->prevPos;
}

bool Board::legal() {
	if (!pos->whiteToMove){
		return !isSquareAttacked(pos, pos->whitePieces[KING], true);
	}
	return !isSquareAttacked(pos, pos->blackPieces[KING], false);
}

bool Board::isCheckmate() {
    ExtMove moves[100];
    int numMoves = 0;
    numMoves = getAllLegalMoves(this, moves);

    return isOwnKingInCheck() && numMoves==0;
}

bool Board::isDrawn() {
    return this->pos->fiftyMoveRule == 50;
}

bool Board::isOwnKingInCheck(){
	if(pos->whiteToMove)
		return isSquareAttacked(pos, pos -> whitePieces[KING],true);
	else
		return isSquareAttacked(pos, pos -> blackPieces[KING],false);
}
void Board::makeNullMove(){
    Position * newPos = &allPos[pos->moveNumber+1];
    memcpy(newPos,pos,sizeof(Position));

    newPos->enPassantLoc=NO_ENPASSANT;
    newPos->prevPos=pos;
    newPos->moveNumber+=1;
    newPos->fiftyMoveRule+=1;


    //TODO update zobrist
    newPos->whiteToMove=!pos->whiteToMove;
    pos=newPos;
}

Position * Board::getCurPos(){
    return pos;
}

void Board::makeMove(Move move){
    //
    Position * newPos = &allPos[pos->moveNumber+1];
    newPos->enPassantLoc=NO_ENPASSANT;
    memcpy(newPos,pos,sizeof(Position));
    newPos->prevPos=pos;
    newPos->moveNumber+=1;
    newPos->fiftyMoveRule+=1;

    //Setting up some local variables to make things easier later on
    PieceType pieceToMove = get_piece_moved(move);
    U8 from = from_sq(move);
    U8 to = to_sq(move);
    Bitboard * moverPieces = newPos->whiteToMove ? newPos->whitePieces : newPos->blackPieces; //getting the pieces of who is moving
    Bitboard * opponentPieces = newPos->whiteToMove ? newPos->blackPieces : newPos->whitePieces; //getting the pieces of who is not moving
    MoveType moveType = get_move_type(move);

    //Setting up the move
    moverPieces[pieceToMove] =  moverPieces[pieceToMove] & ~squareMasks[from]; //setting the bit of where the moving piece came from to 0

    for (U8 pieceTypeInd = 0; pieceTypeInd < 6; pieceTypeInd++){ //looping through types of pieces on the opponent side
        opponentPieces[pieceTypeInd] = opponentPieces[pieceTypeInd] & ~squareMasks[to]; //setting the bit for that square to 0
    }

    //Accounting for move types
    if (moveType == PROMOTION){
            PieceType newPiece = get_promotion_type(move);
            moverPieces[newPiece] =  moverPieces[newPiece] | squareMasks[to];
    }
    else{
        moverPieces[pieceToMove] =  moverPieces[pieceToMove] | squareMasks[to];

        if (moveType == NORMAL){
            //setting possible grid to enpassant
            if (pieceToMove == PAWN && (from==(to+16) || from==(to-16))){
                if (newPos->whiteToMove){
                    newPos->enPassantLoc = to - 8;
                }
                else{
                    newPos->enPassantLoc = to + 8;
                }
            }
        }

        if (moveType == CASTLING){
            if (newPos->whiteToMove){
                if(to == 2){ //If white king is moving to C1
                    moverPieces[ROOK] = moverPieces[ROOK] & ~squareMasks[0];
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[3];
                }
                else { //If white king is moving to G1
                    moverPieces[ROOK] = moverPieces[ROOK] & ~squareMasks[7];
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[5];
                }
            }
            else{
                if(to == 58){ //If black king is moving to C8
                    moverPieces[ROOK] = moverPieces[ROOK] & ~squareMasks[56];
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[59];
                }
                else { //If black king is moving to G8
                    moverPieces[ROOK] = moverPieces[ROOK] & ~squareMasks[63];
                    moverPieces[ROOK] = moverPieces[ROOK] | squareMasks[61];
                }
            }
        }

        if (moveType == ENPASSANT){
            if (newPos->whiteToMove){ //If white pawn enpassants a black pawn, delete a black pawn on the row below
                opponentPieces[PAWN] = opponentPieces[PAWN] & ~squareMasks[to-8];
            }
            else{ //If black pawn enpassants a white pawn, delete a white pawn on the row above
                opponentPieces[PAWN] = opponentPieces[PAWN] & ~squareMasks[to+8];
            }
        }
    }
    pos=newPos;
    for(int i=0; i<NUM_PIECE_TYPES; i++) {
        this->pos->WhitePiecesBB |= this->pos->whitePieces[i];
        this->pos->BlackPiecesBB |= this->pos->blackPieces[i];
    }
    this->pos->AllPiecesBB = this->pos->WhitePiecesBB | this->pos->BlackPiecesBB;
}

bool Board::parseFen(std::string fen, Position * p){
    std::string rowstr;
    std::istringstream ss(fen);
    this->pos=p;
    int up = 7; //start at corner
    int right;

    while(std::getline(ss, rowstr, '/')) {
        memset(this->pos, 0, sizeof(this->pos));
        right = 0;
        const char *row = rowstr.c_str();

        while(*row != '\0') {
            if(!(*row>'0' && *row <'9')) {
                U64 sq = squareMasks[right*8+up];
                if(*row == 'p') {
                    this->pos->blackPieces[PAWN] |= sq;
                } else if(*row == 'k') {
                    this->pos->blackPieces[KING] |= sq;
                } else if(*row == 'r') {
                    this->pos->blackPieces[ROOK] |= sq;
                } else if(*row == 'n') {
                    this->pos->blackPieces[KNIGHT] |= sq;
                } else if(*row == 'b') {
                    this->pos->blackPieces[BISHOP] |= sq;
                } else if(*row == 'q') {
                    this->pos->blackPieces[QUEEN] |= sq;
                } else if(*row == 'P') {
                    this->pos->whitePieces[PAWN] |= sq;
                } else if(*row == 'K') {
                    this->pos->whitePieces[KING] |= sq;
                } else if(*row == 'R') {
                    this->pos->whitePieces[ROOK] |= sq;
                } else if(*row == 'N') {
                    this->pos->whitePieces[KNIGHT] |= sq;
                } else if(*row == 'B') {
                    this->pos->whitePieces[BISHOP] |= sq;
                } else if(*row == 'Q') {
                    this->pos->whitePieces[QUEEN] |= sq;
                } //else it's empty probably
                right++;
            } else {
                right += *row-'0'; //move over
            }
            row++; //increment the row
        }
        up--; //move down
    }

    for(int i=0; i<NUM_PIECE_TYPES; i++) {
        this->pos->WhitePiecesBB |= this->pos->whitePieces[i];
        this->pos->BlackPiecesBB |= this->pos->blackPieces[i];
    }
    this->pos->AllPiecesBB = this->pos->WhitePiecesBB | this->pos->BlackPiecesBB;


    std::istringstream endstr(rowstr);
    std::string substr;
    std::getline(endstr, substr, ' ');


    int i = 0;
    while(std::getline(endstr, substr, ' ')) {
        const char* cstr = substr.c_str();
        if(i==0) {
            pos->whiteToMove=((*(cstr)=='w'));
        } else if(i==1) {
            this->pos->whiteKingCastle=(strstr(cstr,"K") 	!=0?1:0);
            this->pos->whiteQueenCastle=(strstr(cstr,"Q")!=0?1:0);
            this->pos->blackKingCastle=(strstr(cstr,"k")!=0?1:0);
            this->pos->blackQueenCastle=(strstr(cstr,"q")!=0?1:0);
        } else if(i==3) {
            this->pos->fiftyMoveRule = atoi(substr.c_str());
        } else if(i==4) {
            this->pos->moveNumber = atoi(substr.c_str())*2-1;
            if(!this->pos->whiteToMove) {
                this->pos->moveNumber++;
            }
        }
        i++;
    }
    if(this->pos->moveNumber > MAX_MOVES) {
        this->pos->moveNumber = 1;
    }
    allPos[this->pos->moveNumber]=*pos;
    return true;
}

bool Board::isKingInCenter(){
    Bitboard * moverPieces = pos->whiteToMove ? pos->whitePieces : pos->blackPieces; //getting the pieces of who is moving
    return (moverPieces[KING] & centerSquares) != 1;
}
