#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED
#include "types.h"
#include "move.h" //For moves
#include "board.h"//For boardinfo

struct ExtMove{
	Move move;
	int score;
};

U64 pseudoLegalKnightMoveDestinations(U8 loc, U64 targets);
U64 pseudoLegalKingMoveDestinations(U8 loc, U64 targets) ;
U8 getWhiteKingMoves(Position * b, ExtMove moves[], int index);
U8 getBlackKingMoves(Position* b, ExtMove moves[], int index);
U8 getWhiteKnightMoves(Position *b, ExtMove moves[], int index);
U8 getBlackKnightMoves(Position *b, ExtMove moves[], int index);
U8 getWhiteBishopMoves(Position *b, ExtMove moves[], int index);
U8 getBlackBishopMoves(Position *b, ExtMove moves[], int index);
U8 getBlackRookMoves(Position *b, ExtMove moves[], int index);
U8 getWhiteRookMoves(Position *b, ExtMove moves[], int index);
U8 getBlackQueenMoves(Position *b, ExtMove moves[], int index);
U8 getWhiteQueenMoves(Position *b, ExtMove moves[], int index);
U8 getWhitePawnMoves(Position *b, ExtMove moves[], int index);
U8 getBlackPawnMoves(Position *b, ExtMove moves[], int index);
void addMove(Move move, ExtMove moves[],int index, bool isWhitePieceMoving, Position * info);
Move* getMoveList();

U8 getAllPseudoLegalMoves(Position * boardInfo, ExtMove list[]);
U8 getAllLegalMoves(Board* boardInfo, ExtMove list[]);

namespace Movegen{
	U8 getAllCaptures(Position * b, ExtMove moves[]);
}
#endif
