#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include "types.h"
#include "bitboard.h"
#include "move.h"
#include <string>
constexpr int NUM_PIECE_TYPES=6;
constexpr U8 NO_ENPASSANT=199;//Shouldn't matter
struct Position{
    U64 zobrist;

    U16 moveNumber;
    bool whiteToMove;

    U8 enPassantLoc;
    U8 fiftyMoveRule;

    bool whiteQueenCastle;
    bool whiteKingCastle;
    bool blackQueenCastle;
    bool blackKingCastle;

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

        bool legal();


        bool isOwnKingInCheck();

        bool parseFen(std::string fen, Position * pos);
        Position * getCurPos();
        Position * pos;
        std::string getFen();
};

#endif



