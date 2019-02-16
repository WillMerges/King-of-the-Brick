#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include "types.h"
#include "bitboard.h"
#include "move.h"
const int NUM_PIECE_TYPES=6;

struct Position{
    U64 zobrist;

    U16 moveNumber;
    bool whiteToMove;

    U8 enPassantLoc;
    U8 fiftyMoveCount;

    bool canWhiteQueensideCastle;
    bool canWhiteKingsideCastle;
    bool canBlackQueensideCastle;
    bool canBlackKingsideCastle;

    Bitboard whitePieces[NUM_PIECE_TYPES];
    Bitboard blackPieces[NUM_PIECE_TYPES];
    Bitboard WhitePiecesBB;
    Bitboard BlackPiecesBB;
    Bitboard AllPiecesBB;
    //Move lastMove;
    Position * prevPos;
};

class Board{
    public:
        void makeMove(Move move);
        void undoMove();
        void makeNullMove();
        
        bool isDrawn();
        bool isGameOver();
        bool isCheckmate();
        bool isKingInCenter();

        bool parseFen(std::string fen);
        Position * pos;
};

const int MAX_MOVES=2000;
#endif