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
#include "uci.h"

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
    pos->whiteToMove = !pos->whiteToMove;
}

char Board::getPieceAtChar(U8 loc){
	U64 square = squareMasks[loc];
	if((pos->WhitePiecesBB & square) != 0){
		if((pos->whitePieces[PAWN] & square) != 0){
			return 'P';
		}else if((pos->whitePieces[KING] & square) != 0){
			return 'K';
		}else if((pos->whitePieces[BISHOP] & square) != 0){
			return 'B';
		}else if((pos->whitePieces[KNIGHT] & square) != 0){
			return 'N';
		}else if((pos->whitePieces[QUEEN] & square) != 0){
			return 'Q';
		}else if((pos->whitePieces[ROOK] & square) != 0){
			return 'R';
		}else{
			return 'W';
		}
	}else if((pos->BlackPiecesBB & square) != 0){
		if((pos->blackPieces[PAWN] & square) != 0){
			return 'p';
		}else if((pos->blackPieces[KING] & square) != 0){
			return 'k';
		}else if((pos->blackPieces[BISHOP] & square) != 0){
			return 'b';
		}else if((pos->blackPieces[KNIGHT] & square) != 0){
			return 'n';
		}else if((pos->blackPieces[QUEEN] & square) != 0){
			return 'q';
		}else if((pos->blackPieces[ROOK] & square) != 0){
			return 'r';
		}else{
			return 'b';
		}
	}else{
		return ' ';
	}
}
std::string Board::getFen(){
  int emptyCnt;
  std::ostringstream ss;
  for (int r = RANK_8; r >= RANK_1; --r)
  {
      for (int f = FILE_A; f <= FILE_H; ++f)
      {
		  U8 loc = getSquare(r,f);
		  for(emptyCnt = 0; f <= FILE_H && ((pos->AllPiecesBB & squareMasks[loc]) == 0);){
			  ++emptyCnt;
			  f++;
			  loc = getSquare(r,f);
		  }
		  if(emptyCnt != 0){
			  ss << emptyCnt;
		  }
		  if(f <= FILE_H){
			  ss << getPieceAtChar(loc);
		  }
	  }
	  if(r > RANK_1){
		  ss << '/';
	  }
  }
  ss << " ";
  ss << ((pos->whiteToMove) ? "w" : "b");
  ss << " ";
  if(pos->whiteKingCastle){
	  ss << 'K';
  }
  if(pos->whiteQueenCastle){
	  ss<< 'Q';
  }
  if(pos->blackKingCastle){
	  ss << 'k';
  }
  if(pos->blackQueenCastle){
	  ss << 'q';
  }
  if(!(pos->blackQueenCastle && pos->blackKingCastle && pos->whiteQueenCastle && pos->whiteKingCastle)){
	  ss<< '-';
  }
  ss << " ";
  ss << getAlgebraicPos1(pos->enPassantLoc);
  ss << " ";
  ss << std::to_string(pos->fiftyMoveRule).c_str();
  ss << " ";
  ss << std::to_string((1 + (pos->moveNumber))/2);
	return ss.str();
}

bool Board::parseFen(std::string fen, Position * p){
    std::string rowstr;
    std::istringstream ss(fen);
    this->pos=p;
    memset(this->pos, 0, sizeof(this->pos));
    int up = 7; //start at corner
    int right;

    while(std::getline(ss, rowstr, '/')) {
        right = 0;
        const char *row = rowstr.c_str();

        while(*row != '\0' && *row != ' ') {
            if(!(*row>'0' && *row <'9')) {
                U64 sq = squareMasks[up*8+right];
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
        if(i==0) { //who's move
            pos->whiteToMove=((*(cstr)=='w'));
        } else if(i==1) { //castling
            this->pos->whiteKingCastle=(strstr(cstr,"K") 	!=0?1:0);
            this->pos->whiteQueenCastle=(strstr(cstr,"Q")!=0?1:0);
            this->pos->blackKingCastle=(strstr(cstr,"k")!=0?1:0);
            this->pos->blackQueenCastle=(strstr(cstr,"q")!=0?1:0);
        } else if(i==2) { //en passant
            this->pos->enPassantLoc=algebraicPosToLoc(cstr);
        } else if(i==3) { //50 move rule
            this->pos->fiftyMoveRule = atoi(cstr);
        } else if(i==4) { //move number
            this->pos->moveNumber = atoi(cstr)*2-1;
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
