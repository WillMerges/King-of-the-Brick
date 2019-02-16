#include "board.h"
#include <string>
#include "move.h"
#include "types.h"
#include <string.h>
#include <sstream>

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
    switch (moveType)
    {
        case NORMAL:
            moverPieces[pieceToMove] =  moverPieces[pieceToMove] | squareMasks[to];
            //setting possible grid to enpassant
            if (pieceToMove == PAWN && (from==to+16 | from==to-16)){

            }
            break;

        case PROMOTION:
            PieceType newPiece = get_promotion_type(move);
            moverPieces[newPiece] =  moverPieces[newPiece] | squareMasks[to];
            break;

        case CASTLING:
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
            moverPieces[pieceToMove] =  moverPieces[pieceToMove] | squareMasks[to];
            break;

        case ENPASSANT:
            if (newPos->whiteToMove){ //If white pawn enpassants a black pawn, delete a black pawn on the row below
                opponentPieces[PAWN] = opponentPieces[PAWN] & ~squareMasks[to-8];
            }
            else{ //If black pawn enpassants a white pawn, delete a white pawn on the row above
                opponentPieces[PAWN] = opponentPieces[PAWN] & ~squareMasks[to+8];
            }
            break;

        default:
            break;
    }
}

bool Board::parseFen(std::string fen){
    std::string rowstr;
    std::istringstream ss(fen);

    int up = 7; //start at corner
    int right;

    while(std::getline(ss, rowstr, '/')) {
        right = 0;
        const char *row = rowstr.c_str();

        while(*row != '\0') {
            if(*row<'0' && *row >'9') {
                U64 sq = getSquare(right, up);
                if(*row == 'p') {
                    this->pos->blackPieces[PAWN];
                } else if(*row == 'k') {
                    this->pos->blackPieces[KING];
                } else if(*row == 'r') {
                    this->pos->blackPieces[ROOK];
                } else if(*row == 'n') {
                    this->pos->blackPieces[KNIGHT];
                } else if(*row == 'b') {
                    this->pos->blackPieces[BISHOP];
                } else if(*row == 'q') {
                    this->pos->blackPieces[QUEEN];
                } else if(*row == 'P') {
                    this->pos->whitePieces[PAWN];
                } else if(*row == 'K') {
                    this->pos->whitePieces[KING];
                } else if(*row == 'R') {
                    this->pos->whitePieces[ROOK];
                } else if(*row == 'N') {
                    this->pos->whitePieces[KNIGHT];
                } else if(*row == 'B') {
                    this->pos->whitePieces[BISHOP];
                } else if(*row == 'Q') {
                    this->pos->whitePieces[QUEEN];
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
        this->pos->blackPiecesBB |= this->pos->blackPieces[i];
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

        }
    }
}
