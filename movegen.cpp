#include "movegen.h"
#include "bitboard.h"
#include "bbmagic.h"
#include "move.h"
//#include "search.h"//for the move ordering heuristics.
Move moveList[MAX_MOVES]={};
U64 kingMoves[] =
			{ 0x303L, 0x707L, 0xe0eL, 0x1c1cL, 0x3838L, 0x7070L, 0xe0e0L, 0xc0c0L, 0x30303L, 0x70707L, 0xe0e0eL, 0x1c1c1cL, 0x383838L, 0x707070L, 0xe0e0e0L, 0xc0c0c0L, 0x3030300L, 0x7070700L, 0xe0e0e00L, 0x1c1c1c00L, 0x38383800L, 0x70707000L, 0xe0e0e000L, 0xc0c0c000L, 0x303030000L, 0x707070000L, 0xe0e0e0000L, 0x1c1c1c0000L, 0x3838380000L, 0x7070700000L, 0xe0e0e00000L, 0xc0c0c00000L, 0x30303000000L, 0x70707000000L, 0xe0e0e000000L, 0x1c1c1c000000L, 0x383838000000L, 0x707070000000L, 0xe0e0e0000000L, 0xc0c0c0000000L, 0x3030300000000L, 0x7070700000000L, 0xe0e0e00000000L, 0x1c1c1c00000000L, 0x38383800000000L, 0x70707000000000L, 0xe0e0e000000000L, 0xc0c0c000000000L, 0x303030000000000L, 0x707070000000000L, 0xe0e0e0000000000L, 0x1c1c1c0000000000L, 0x3838380000000000L, 0x7070700000000000L, 0xe0e0e00000000000L, 0xc0c0c00000000000L, 0x303000000000000L, 0x707000000000000L, 0xe0e000000000000L, 0x1c1c000000000000L, 0x3838000000000000L, 0x7070000000000000L, 0xe0e0000000000000L, 0xc0c0000000000000L };
	
U64 knightMoves[] =
			{ 0x20400L, 0x50800L, 0xa1100L, 0x142200L, 0x284400L, 0x508800L, 0xa01000L, 0x402000L, 0x2040004L, 0x5080008L, 0xa110011L, 0x14220022L, 0x28440044L, 0x50880088L, 0xa0100010L, 0x40200020L, 0x204000402L, 0x508000805L, 0xa1100110aL, 0x1422002214L, 0x2844004428L, 0x5088008850L, 0xa0100010a0L, 0x4020002040L, 0x20400040200L, 0x50800080500L, 0xa1100110a00L, 0x142200221400L, 0x284400442800L, 0x508800885000L, 0xa0100010a000L, 0x402000204000L, 0x2040004020000L, 0x5080008050000L, 0xa1100110a0000L, 0x14220022140000L, 0x28440044280000L, 0x50880088500000L, 0xa0100010a00000L, 0x40200020400000L, 0x204000402000000L, 0x508000805000000L, 0xa1100110a000000L, 0x1422002214000000L, 0x2844004428000000L, 0x5088008850000000L, 0xa0100010a0000000L, 0x4020002040000000L, 0x400040200000000L, 0x800080500000000L, 0x1100110a00000000L, 0x2200221400000000L, 0x4400442800000000L, 0x8800885000000000L, 0x100010a000000000L, 0x2000204000000000L, 0x4020000000000L, 0x8050000000000L, 0x110a0000000000L, 0x22140000000000L, 0x44280000000000L, 0x88500000000000L, 0x10a00000000000L, 0x20400000000000L };

Move * getMoveList(){
	return moveList;	
}

PieceType getBlackPieceOnSquare(Position * pos, U64 mask){
	//Assumes we know there is a blackpiece being captured
	PieceType type = EMPTY;
	if((pos->blackPieces[PAWN] &mask) != 0){
		type=PAWN;
	}else if((pos->blackPieces[KNIGHT] & mask) != 0){
		type = KNIGHT;
	}else if((pos->blackPieces[BISHOP] & mask) != 0){
		type=BISHOP;
	}else if((pos->blackPieces[ROOK] & mask) != 0){
		type=ROOK;
	}else  if((pos->blackPieces[QUEEN] & mask) != 0){
		type=QUEEN;
	}
	return type;
}

PieceType getWhitePieceOnSquare(Position * pos, U64 mask){
	//Assumes we know there is a blackpiece being captured
	PieceType type = EMPTY;
	if((pos->whitePieces[PAWN] &mask) != 0){
		type=PAWN;
	}else if((pos->whitePieces[KNIGHT] & mask) != 0){
		type = KNIGHT;
	}else if((pos->whitePieces[BISHOP] & mask) != 0){
		type=BISHOP;
	}else if((pos->whitePieces[ROOK] & mask) != 0){
		type=ROOK;
	}else  if((pos->whitePieces[QUEEN] & mask) != 0){
		type=QUEEN;
	}
	return type;
}
//PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
int mvvlvaValues[] = {1000000,2000000,3000000,4000000,5000000,10000000};
Position * info;//Set me before calling addMove. TODO
void addMove(Move move, ExtMove moves[],int index, bool isWhitePieceMoving, Position * info){
	moves[index].move = move;
}

U64 pseudoLegalKnightMoveDestinations(U8 loc, U64 targets) {
	return knightMoves[loc] & targets;
}
	
U64 pseudoLegalKingMoveDestinations(U8 loc, U64 targets) {
	return kingMoves[loc] & targets;
}

U8 getAllBlackMoves(Position * boardInfo, ExtMove list[]){
	int idx = 0;
	idx+= getBlackBishopMoves(boardInfo, list, idx);
	idx+=getBlackKnightMoves(boardInfo, list, idx);
	idx+=getBlackRookMoves(boardInfo, list, idx);
	idx+=getBlackQueenMoves(boardInfo, list, idx);
	idx+=getBlackPawnMoves(boardInfo, list, idx);
	idx+= getBlackKingMoves(boardInfo, list, idx);

	return idx;
}

U8 getAllWhiteMoves(Position* boardInfo, ExtMove list[]){
	int idx = 0;
	idx+= getWhiteBishopMoves(boardInfo, list, idx);
	idx+=getWhiteKnightMoves(boardInfo, list, idx);
	idx+=getWhiteRookMoves(boardInfo, list, idx);
	idx+=getWhiteQueenMoves(boardInfo, list, idx);
	idx+=getWhitePawnMoves(boardInfo, list, idx);
	idx+= getWhiteKingMoves(boardInfo, list, idx);

	return idx;
}

U8 getAllPseudoLegalMoves(Position * boardInfo, ExtMove list[]){
	if(boardInfo->whiteToMove){
		return getAllWhiteMoves(boardInfo, list);
	}
	return getAllBlackMoves(boardInfo, list);
}
U64 potentiallyPinned[] = {9313761861428380670,180779649147209727,289501704256556799,578721933553179903,1157442771889699071,2314886638996058367,4630054752952049919,9332167099941961983,4693051017133293315,9386102034266586887,325459994840334094,578862399937642268,1157444424410136376,2315169224285290352,4702396038313476064,9404792076610109376,2382695595002233605,4765391190004533002,9530782384287321621,614821794360007722,1157867469642086484,2387511058328678568,4775021017129017424,9550042029946290336,1227517888156599561,2455035776330041874,4910072647893521700,9820426766485563977,1266167049021314194,2460276499726510116,4920271520198053960,9840541936589512848,649930115027110161,1299860234365964834,2600000848492045380,5272058195805358472,10544115296394056209,2641485423861834786,5210912158452303940,10421541742416269448,361412783554236705,722826670915068482,1517430560419759236,3034580745374500872,6068881115283853584,12137481855085847073,5827939256702092354,11583539444389546116,287952221336838465,576187017162015362,1080597919557780484,2089419720071055368,4107063321064181776,8142350518772179232,16212923818971513409,13907045970195547266,18375536441101992321,18376667877509071362,18378650374858081284,18382614274322728968,18390542064695644176,18406396550208102432,18437825145767870784,18428906217826189953};


//Pretty incredible. This is 5x slower than pseudo legal.
U8 getAllLegalMoves(Board* b, ExtMove list[]){
	//Also actually generate by piece rather than calling PseudoLegal.
	//If KING, CHECK.
	//If Bishop of same color, check in check.
	//If knight of same color, check in check.
	//If rook, if ending row ==KING_ROW| ending column==KING_COLUMN
	//Queen, could micro-optimize via rook and bishop
	//Pawn, if near enemy king.
	
	//TODO
	//https://chess.stackexchange.com/questions/16890/efficient-ways-to-go-from-pseduo-legal-to-fully-legal-move-generation?rq=1
	//https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/
	//https://chess.stackexchange.com/questions/15705/c-vs-java-engine-move-generation-performance
	
	//FOR NOW:
	
	
	U8 count = getAllPseudoLegalMoves(b->pos, list);
	bool check = b->isOwnKingInCheck();
	int j = 0;
	U64 mask;
	if(b->pos->whiteToMove){
		mask = potentiallyPinned[trailingZeroCount(b->pos->whitePieces[KING])];
	}else{
		mask = potentiallyPinned[trailingZeroCount(b->pos->blackPieces[KING])];
	}

	if(check){
		for (int i = 0; i < count; i++) {
			b->makeMove(list[i].move);
			if(b->legal()){
				list[j++] = list[i];
			}
			b->undoMove();
		}
	}else{
		for (int i = 0; i < count; i++) {
			if(mask & squareMasks[from_sq(list[i].move)] || get_piece_moved(list[i].move) == KING){
				b->makeMove(list[i].move);
				if(b->legal()){
					list[j++] = list[i];
				}
				b->undoMove();
			}else{
				list[j++] = list[i];
			}
		}
	}
	return j;
}

U8 getWhiteKingMoves(Position * b, ExtMove moves[], int index) {
	U64 king = b->whitePieces[KING];
	
	int num_moves_generated = 0;
	
	U8 from_loc = trailingZeroCount(king);
	U64 movelocs = pseudoLegalKingMoveDestinations(from_loc, ~b->WhitePiecesBB);
	
	while (movelocs != 0L) {
		U64 to = lowestOneBit(movelocs);
		U8 to_loc = trailingZeroCount(to);
		Move move =createMove(from_loc, to_loc, KING);
		addMove(move,moves,index+num_moves_generated,true,b);
		num_moves_generated++;
		movelocs &= ~to;
	}
	
	if (b->whiteKingCastle) {
		if ( (king << 1 & b->AllPiecesBB) == 0L
				&& (king << 2 & b->AllPiecesBB) == 0L) {
			if ( (king << 3 & b->whitePieces[ROOK]) != 0L) {
				if (!isSquareAttacked(b, king, true)
						&& !isSquareAttacked(b, king << 1,
								true)
						&& !isSquareAttacked(b, king << 2,
								true)) {
					addMove(createMove(from_loc, from_loc + 2, KING, KING, CASTLING),moves,index+num_moves_generated,true,b);
					num_moves_generated++;
				}
			}
		}
	}
	
	if (b->whiteQueenCastle) {
		if ( (king >> 1 & b->AllPiecesBB) == 0L
				&& (king>> 2 & b->AllPiecesBB) == 0L
				&& (king>> 3 & b->AllPiecesBB) == 0L) {
			if ( (king>> 4 & b->whitePieces[ROOK]) != 0L) {
				if ( !isSquareAttacked(b, king, true)
						&& !isSquareAttacked(b, king >> 1,
								true)
						&& !isSquareAttacked(b, king >> 2,
								true)) {
					addMove(createMove(from_loc, from_loc - 2, KING, KING, CASTLING),moves,index+num_moves_generated,true,b);
					num_moves_generated++;
				}
			}
		}
	}
	
	return num_moves_generated;
}
U8 getBlackKingMoves(Position* b, ExtMove moves[], int index) {
	U64 king = b->blackPieces[KING];
	
	int num_moves_generated = 0;
	
	U8 from_loc = trailingZeroCount(king);
	U64 movelocs = pseudoLegalKingMoveDestinations(from_loc, ~b->BlackPiecesBB);
	
	while (movelocs != 0L) {
		U64 to = lowestOneBit(movelocs);
		U8 to_loc = trailingZeroCount(to);
		Move move = createMove(from_loc, to_loc, KING);
		addMove(move,moves,index+num_moves_generated,false,b);
		num_moves_generated++;
		movelocs &= ~to;
	}
	
	if (b->blackKingCastle) {
		if ( (b->blackPieces[KING] << 1 & b->AllPiecesBB) == 0L
				&& (b->blackPieces[KING] << 2 & b->AllPiecesBB) == 0L) {
			if ( (b->blackPieces[KING] << 3 & b->blackPieces[ROOK]) != 0L) {
				if ( !isSquareAttacked(b, b->blackPieces[KING], false)
						&& !isSquareAttacked(b, b->blackPieces[KING] << 1,
								false)
						&& !isSquareAttacked(b, b->blackPieces[KING] << 2,
								false)) {
					addMove(createMove(from_loc, from_loc + 2, KING, KING, CASTLING),moves,index+num_moves_generated,false,b);
					num_moves_generated++;
				}
			}
		}
	}
	
	if (b->blackQueenCastle) {
		if ( (b->blackPieces[KING] >> 1 & b->AllPiecesBB) == 0L
				&& (b->blackPieces[KING] >> 2 & b->AllPiecesBB) == 0L
				&& (b->blackPieces[KING] >> 3 & b->AllPiecesBB) == 0L) {
			if ( (b->blackPieces[KING] >> 4 & b->blackPieces[ROOK]) != 0L) {
				if ( !isSquareAttacked(b, b->blackPieces[KING], false)
						&& !isSquareAttacked(b, b->blackPieces[KING] >> 1,
								false)
						&& !isSquareAttacked(b, b->blackPieces[KING] >> 2,
								false)) {
					addMove(createMove(from_loc, from_loc - 2, KING,KING, CASTLING),moves,index+num_moves_generated,false,b);
					num_moves_generated++;
				}
			}
		}
	}
	
	return num_moves_generated;
}

U8 getWhiteKnightMoves(Position *b, ExtMove moves[], int index) {
	U64 knights = b->whitePieces[KNIGHT];
	U8 num_moves_generated = 0;
	
	while (knights != 0L) {
		U64 from = lowestOneBit(knights);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs = pseudoLegalKnightMoveDestinations(from_loc, ~b->WhitePiecesBB);
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, KNIGHT);
			addMove(move,moves,index+num_moves_generated,true,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		knights &= ~from;
	}
	return num_moves_generated;
}


U8 getBlackKnightMoves(Position * b,ExtMove moves[], int index) {
	U64 knights = b->blackPieces[KNIGHT];
	int num_moves_generated = 0;
	
	while (knights != 0L) {
		U64 from = lowestOneBit(knights);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs = pseudoLegalKnightMoveDestinations(from_loc, ~b->BlackPiecesBB);
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, KNIGHT);
			addMove(move,moves,index+num_moves_generated,false,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		knights &= ~from;
	}
	return num_moves_generated;
}
U8 getWhitePawnMoves(Position * b, ExtMove moves[], int listIdx){
	U64 pawns = b->whitePieces[PAWN];
	int moveGenCount = 0;
	
	while(pawns != 0){
		U64 fromBB = lowestOneBit(pawns);
		U8 fromLoc = trailingZeroCount(fromBB);
		
		//If pawn is on seventh rank
		if((fromBB & rankMasks[RANK_7]) != 0){
			//If pawn is not on A file, and there is a piece to capture upper left 
			if(((fromBB & fileMasks[FILE_A]) == 0) &&  (((fromBB << 7 ) & (b->BlackPiecesBB)) != 0)){
				U8 destLoc = trailingZeroCount(fromBB << 7);
				Move m = createMove(fromLoc, destLoc, PAWN, KNIGHT, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, QUEEN, PROMOTION);
				//list[listIdx+moveGenCount].move = m;
				addMove(m,moves,listIdx+moveGenCount,true,b);

				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, BISHOP, PROMOTION);
				//list[listIdx+moveGenCount].move = m;
				addMove(m,moves,listIdx+moveGenCount,true,b);

				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, ROOK, PROMOTION);
				//list[listIdx+moveGenCount].move = m;
				addMove(m,moves,listIdx+moveGenCount,true,b);
				moveGenCount++;
			}
			//If pawn not on H file, and there is a piece to capture upper right
			if(((fromBB & fileMasks[FILE_H]) ==0) && (((fromBB <<9) & (b->BlackPiecesBB)) != 0)){
				U8 destLoc = trailingZeroCount(fromBB << 9);
				Move m = createMove(fromLoc, destLoc, PAWN, KNIGHT, PROMOTION);
				//list[listIdx+moveGenCount].move = m;
				addMove(m,moves,listIdx+moveGenCount,true,b);

				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, QUEEN, PROMOTION);
				//list[listIdx+moveGenCount].move = m;
				addMove(m,moves,listIdx+moveGenCount,true,b);
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, BISHOP, PROMOTION);
				//list[listIdx+moveGenCount].move = m;
				addMove(m,moves,listIdx+moveGenCount,true,b);
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, ROOK, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
			}
			//If next square clear
			if(((fromBB << 8) & (b->AllPiecesBB)) == 0){
				U8 destLoc = trailingZeroCount(fromBB << 8);
				Move m = createMove(fromLoc, destLoc, PAWN, KNIGHT, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, QUEEN, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, BISHOP, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, ROOK, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
			}
		}else{
			//Pawn not on seventh rank
			
			//Leftwards captures.
			if(((fromBB & fileMasks[FILE_A]) == 0) && ((((fromBB << 7) & (b->BlackPiecesBB)) != 0) || (trailingZeroCount(fromBB << 7) == (b->enPassantLoc)))){
				U8 destLoc = trailingZeroCount(fromBB << 7);
				Move m;
				if(destLoc == (b->enPassantLoc)){
					m = createMove(fromLoc, destLoc, PAWN, KING, ENPASSANT);
				}else{
					m = createMove(fromLoc, destLoc, PAWN);
				}
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;			
				moveGenCount++;
			}
			
			//Rightwards captures
			if(((fromBB & fileMasks[FILE_H]) == 0) && ((((fromBB << 9) & (b->BlackPiecesBB)) != 0) || (trailingZeroCount(fromBB << 9) == (b->enPassantLoc)))){
				U8 destLoc = trailingZeroCount(fromBB << 9);
				Move m;
				if(destLoc == (b->enPassantLoc)){
					m = createMove(fromLoc, destLoc, PAWN, KING, ENPASSANT);
				}else{
					m = createMove(fromLoc, destLoc, PAWN);
				}
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;			
				moveGenCount++;
			}
			Move m;
			//Normal move
			bool nextSquareClear = false;
			if(((fromBB << 8) & (b->AllPiecesBB)) == 0){
				nextSquareClear=true;
				U8 destLoc = trailingZeroCount(fromBB << 8);
				m = createMove(fromLoc, destLoc, PAWN);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;			
				moveGenCount++;
			}
			//2 Squares
			if(((fromBB & rankMasks[RANK_2]) != 0) && nextSquareClear && (((fromBB << 16) & (b->AllPiecesBB)) == 0)){
				U8 destLoc = trailingZeroCount(fromBB << 16);
				m = createMove(fromLoc, destLoc, PAWN);
				addMove(m,moves,listIdx+moveGenCount,true,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
			}
		}
		pawns = pawns &~fromBB;
	}
	return moveGenCount;
}


U8 getBlackPawnMoves(Position * b, ExtMove moves[], int listIdx){
	U64 pawns = b->blackPieces[PAWN];
	int moveGenCount = 0;
	
	while(pawns != 0){
		U64 fromBB = lowestOneBit(pawns);
		U8 fromLoc = trailingZeroCount(fromBB);
		
		//If pawn is on second rank
		if((fromBB & rankMasks[RANK_2]) != 0){
			//If pawn is not on A file, and there is a piece to capture upper left 
			if(((fromBB & fileMasks[FILE_H]) == 0) &&  (((fromBB >> 7) & (b->WhitePiecesBB)) != 0)){
				U8 destLoc = trailingZeroCount(fromBB >> 7);
				Move m = createMove(fromLoc, destLoc, PAWN, KNIGHT, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, QUEEN, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, BISHOP, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, ROOK, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
			}
			//If pawn not on H file, and there is a piece to capture upper right
			if(((fromBB & fileMasks[FILE_A]) ==0) && (((fromBB >>9) & (b->WhitePiecesBB)) != 0)){
				U8 destLoc = trailingZeroCount(fromBB >>9);
				Move m = createMove(fromLoc, destLoc, PAWN, KNIGHT, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, QUEEN, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, BISHOP, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, ROOK, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
			}
			//If next square clear
			if(((fromBB >>8) & (b->AllPiecesBB)) == 0){
				U8 destLoc = trailingZeroCount(fromBB >> 8);
				Move m = createMove(fromLoc, destLoc, PAWN, KNIGHT, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, QUEEN, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, BISHOP, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
				m = createMove(fromLoc, destLoc, PAWN, ROOK, PROMOTION);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
			}
		}else{
			//Pawn not on seventh rank
			
			//Leftwards captures.
			if(((fromBB & fileMasks[FILE_H]) == 0) && ((((fromBB >> 7) & (b->WhitePiecesBB)) != 0) || (trailingZeroCount(fromBB >> 7) == (b->enPassantLoc)))){
				U8 destLoc = trailingZeroCount(fromBB >> 7);
				Move m;
				if(destLoc == (b->enPassantLoc)){
					m = createMove(fromLoc, destLoc, PAWN, KING, ENPASSANT);
				}else{
					m = createMove(fromLoc, destLoc, PAWN);
				}
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;			
				moveGenCount++;
			}
			
			//Rightwards captures
			if(((fromBB & fileMasks[FILE_A]) == 0) && ((((fromBB >> 9) & (b->WhitePiecesBB)) != 0) || (trailingZeroCount(fromBB >> 9) == (b->enPassantLoc)))){
				U8 destLoc = trailingZeroCount(fromBB >> 9);
				Move m;
				if(destLoc == (b->enPassantLoc)){
					m = createMove(fromLoc, destLoc, PAWN, KING, ENPASSANT);
				}else{
					m = createMove(fromLoc, destLoc, PAWN);
				}
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;			
				moveGenCount++;
			}
			//Normal move
			bool nextSquareClear = false;
			if(((fromBB >> 8) & (b->AllPiecesBB)) == 0){
				nextSquareClear=true;
				Move m;
				U8 destLoc = trailingZeroCount(fromBB >> 8);
				m = createMove(fromLoc, destLoc, PAWN);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;			
				moveGenCount++;
			}
			//2 Squares
			if(((fromBB & rankMasks[RANK_7]) != 0) && nextSquareClear && (((fromBB >> 16) & (b->AllPiecesBB)) == 0)){
				Move m;
				U8 destLoc = trailingZeroCount(fromBB >> 16);
				m = createMove(fromLoc, destLoc, PAWN);
				addMove(m,moves,listIdx+moveGenCount,false,b);
				//list[listIdx+moveGenCount].move = m;
				moveGenCount++;
			}
		}
		pawns = pawns &~fromBB;
	}
	return moveGenCount;
}

U8 getWhiteBishopMoves(Position * b, ExtMove moves[], int index) {
	U64 bishops = b->whitePieces[BISHOP];
	U8 num_moves_generated = 0;
	
	while (bishops != 0L) {
		U64 from = lowestOneBit(bishops);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs =
				getBishopAttacks(from_loc, b->AllPiecesBB & ~from);
		movelocs &= ~b->WhitePiecesBB;
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, BISHOP);
			addMove(move,moves,index+num_moves_generated,true,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		
		bishops &= ~from;
	}
	return num_moves_generated;
}
U8 getBlackBishopMoves(Position* b,ExtMove moves[], int index) {
	U64 bishops = b->blackPieces[BISHOP];
	U8 num_moves_generated = 0;
	
	while (bishops != 0L) {
		U64 from = lowestOneBit(bishops);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs =
				getBishopAttacks(from_loc, b->AllPiecesBB & ~from);
		movelocs &= ~b->BlackPiecesBB;
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, BISHOP);
			addMove(move,moves,index+num_moves_generated,false,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		
		bishops &= ~from;
	}
	return num_moves_generated;
}
U8 getWhiteRookMoves(Position* b, ExtMove moves[], int index) {
	U64 rooks = b->whitePieces[ROOK];
	int num_moves_generated = 0;
	
	while (rooks != 0L) {
		U64 from = lowestOneBit(rooks);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs = getRookAttacks(from_loc, b->AllPiecesBB & ~from);
		movelocs &= ~b->WhitePiecesBB;
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, ROOK);
			addMove(move,moves,index+num_moves_generated,true,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		
		rooks &= ~from;
	}
	return num_moves_generated;
}

U8 getBlackRookMoves(Position *b, ExtMove moves[], int index) {
	U64 rooks = b->blackPieces[ROOK];
	U8 num_moves_generated = 0;
	
	while (rooks != 0L) {
		U64 from = lowestOneBit(rooks);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs = getRookAttacks(from_loc, b->AllPiecesBB & ~from);
		movelocs &= ~b->BlackPiecesBB;
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, ROOK);
			addMove(move,moves,index+num_moves_generated,false,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		
		rooks &= ~from;
	}
	return num_moves_generated;
}


U8 getWhiteQueenMoves(Position* b, ExtMove moves[], int index) {
	U64 queens = b->whitePieces[QUEEN];
	int num_moves_generated = 0;
	
	while (queens != 0L) {
		U64 from = lowestOneBit(queens);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs =
				getQueenAttacks(from_loc, b->AllPiecesBB & ~from);
		
		movelocs &= ~b->WhitePiecesBB;
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, QUEEN);
			addMove(move,moves,index+num_moves_generated,true,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		
		queens &= ~from;
	}
	return num_moves_generated;
}

U8 getBlackQueenMoves(Position * b, ExtMove moves[], int index) {
	U64 queens = b->blackPieces[QUEEN];
	U8 num_moves_generated = 0;
	
	while (queens != 0L) {
		U64 from = lowestOneBit(queens);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs =
				getQueenAttacks(from_loc, b->AllPiecesBB & ~from);
		movelocs &= ~b->BlackPiecesBB;
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, QUEEN);
			addMove(move,moves,index+num_moves_generated,false,b);
			num_moves_generated++;
			movelocs &= ~to;
		}
		
		queens &= ~from;
	}
	return num_moves_generated;
}

/*
U8 getBlackQueenCaptures(BoardInfo * b, ExtMove moves[], int index) {
	U64 queens = b->BlackQueenBB;
	U8 num_moves_generated = 0;
	
	while (queens != 0L) {
		U64 from = lowestOneBit(queens);
		U8 from_loc = trailingZeroCount(from);
		U64 movelocs =
				getQueenAttacks(from_loc, b->AllPiecesBB & ~from);
		movelocs &= ~b->BlackPiecesBB;
		
		while (movelocs != 0L) {
			U64 to = lowestOneBit(movelocs);
			U8 to_loc = trailingZeroCount(to);
			Move move =
					createMove(from_loc, to_loc, QUEEN);
			moves[index + num_moves_generated].move = move;
			num_moves_generated++;
			movelocs &= ~to;
		}
		
		queens &= ~from;
	}
	return num_moves_generated;
}*/