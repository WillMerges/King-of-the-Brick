#include <string>
#include <iostream>
#include <sstream>
#include "board.h"
#include "movegen.h"
#include "bitboard.h"
#include "uci.h"
#include "eval.h"
#include "search.h"
#include "bbmagic.h"
#include <stdio.h>
#include "string.h"
#include "bitboard.h"
#include "move.h"

using namespace std;
std::string StartPositionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
char alge[10]={};	
char * getAlgebraicPos(U8 loc){
	if(loc == NO_ENPASSANT){
		alge[0]='-';
		alge[1]='\0';
		return alge;
	}
	alge[0] = (loc % 8)+'a';
	alge[1]= (loc /8)+'1';
	alge[2]='\0';
	return alge;
}	
 U8 algebraicPosToLoc(const char * pos){
	if(*pos=='-'){
		return NO_ENPASSANT;
	}
	return (pos[0]-'a')+(pos[1]-'0'-1)*8;
}

#include <time.h>
#include <sys/time.h>
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}


char* UCI::getMoveString(Move m, char* ret){
	char from[3], to[3];
	char* arr = getAlgebraicPos(from_sq(m));
	from[0] = arr[0];from[1]=arr[1];from[2]=arr[2];from[3]=arr[3];
	
	arr = getAlgebraicPos(to_sq(m));

	to[0] = arr[0];to[1]=arr[1];to[2]=arr[2];to[3]=arr[3];
	if(get_promotion_type(m)!=PAWN && get_promotion_type(m) != KING){
		switch(get_promotion_type(m)){
			case KNIGHT:
				snprintf(ret,10,"%s%sn",from,to);
				break;
			case QUEEN:
				snprintf(ret,10,"%s%sq",from,to);
				break;
			case ROOK:
				snprintf(ret,10,"%s%sr",from,to);
				break;
			case BISHOP:
				snprintf(ret,10,"%s%sb",from,to);
				break;
			default:
				snprintf(ret,10,"%s%su",from,to);
				break;
			}
	}else{
		snprintf(ret,10,"%s%s",from,to);
	}
	return ret;
}
Move UCI::toMove(Board * board, std::string move){
	//printf("%s\n", move.c_str());
	char curMove[100];
	ExtMove moves[MAX_MOVES];
	U8 count = getAllLegalMoves(board,moves);
	for(int i = 0; i < count; i++){
		memset(&curMove[0], 0, sizeof(curMove));
		Move move1 = moves[i].move;
		//printf("Compare %s : %s\n", move.c_str(), getMoveString(move1,curMove)); 
		if(strcmp(move.c_str(),getMoveString(move1,curMove)) == 0){
			//printf("RET\n");
			return move1;
		}
	}
	return -1;//BRITTLE. Checked in SetPosition. Only currently works because Moves use few enough bits the top can't be negative
}


void UCI::setPosition(Board * board, Position* info, istringstream* parser){
	std::string token, fen;
	(*parser) >> token;
	if(token=="startpos"){
		fen=StartPositionFEN;
		(*parser) >> token;
	}else if(token=="fen"){
		 while ((*parser) >> token && token != "moves"){
            fen += token + " ";
		 }
	}else{
		return;
	}
	board->parseFen(fen);
	while((*parser) >> token){
		Move m = toMove(board, token);
		if(m != -1){
			board->makeMove(m);
		}else{
			break;
		}
	}
}

void UCI::go(Board * board, istringstream *parser){
		char buffer[100];
	string token;
	Config cfg;
	while((*parser) >> token){
		if(token == "depth"){
			(*parser) >> cfg.depth;
		}else if(token =="wtime"){
			(*parser) >> cfg.wtime;
		}else if(token=="btime"){
			(*parser) >> cfg.btime;
		}else if(token == "winc"){
			(*parser) >> cfg.winc;
		}else if(token == "binc"){
			(*parser) >> cfg.binc;
		}else if(token == "movetime"){
			(*parser) >> cfg.movetime;
		}else if(token == "infinite"){
			cfg.depth = 20;
		}
	}
	//Search::setConfig(&cfg);
	//Move bestMove = Search::getBestMove(board);
	//printf("bestmove %s\n", getMoveString(bestMove,buffer));
}
int UCI::perft(Board* b, int depth) {
	if (depth == 0){
		return 1;
	}
	ExtMove moveList[MAX_MOVES]={};
	int num_moves = getAllLegalMoves(b, moveList);
	int count = 0;	
	for (int i = 0; i < num_moves; i++) {
		b->makeMove(moveList[i].move);
		count += perft(b, depth - 1);
		b->undoMove();
	}
	
	return count;
}

int UCI::divide(Board* b, int depth) {
	ExtMove moveList[MAX_MOVES]={};
	int num_moves = getAllLegalMoves(b, moveList);
	int count = 0;
	char buffer[100];
	for (int i = 0; i < num_moves; i++) {
		b->makeMove(moveList[i].move);
		int countc = perft(b, depth - 1);
		count+= countc;
		printf("%s : %i\n", getMoveString(moveList[i].move,buffer),countc);
		b->undoMove();
	}
	return count;
}

void UCI::perft(Board * board, istringstream * parser){
	int depth;
	(*parser) >> depth;
	double time = get_wall_time();
	int numNodes = divide(board, depth);
	double newTime = get_wall_time();
	printf("---------------\n");
	printf("Nodes: %i\n" , numNodes);
	printf("Took %fs. nps: %f.\n",newTime-time,numNodes/(newTime-time));
}
bool UCI::loop(){
	Board b;
	Position info;
	std::string token, cmd;
	
	b.parseFen(StartPositionFEN);
	std::cout << ("King of the Brick") << std::endl;
	while(true){
		getline(cin,cmd);
		istringstream parser(cmd);
		parser >> skipws >> token;
		if(token == "quit"){
			return false;
		}
		if(token == "uci"){
			std::cout << "id name King of the Brick\n";
			std::cout << "uciok" << std::endl;
		}else if(token == "ucinewgame"){
			b.parseFen(StartPositionFEN);
		}else if(token == "isready"){
			std::cout << "readyok" << std::endl;
		}else if(token == "position"){
			setPosition(&b,&info,&parser);
		}else if(token=="setoption"){

		}else if(token == "go"){
			go(&b,&parser);
		}else if(token == "perft"){
			perft(&b,&parser);
		}else if(token == "eval"){
			printf("Current Position: %i\n",Eval::evaluate(&b));
		}else if(token == "d"){
			
			//char buffer[100];
			//printf("FEN: %s\nIs checkmate: %i\nIs draw: %i\nZobrist: %llx\nEval %i\nHash Move: %s\nPinned Pieces %llx\n", b.getFen().c_str(), b.isCheckmate(), b.isDraw(), b.currentBoard()->zobrist, Eval::evaluate(&b), UCI::getMoveString(bestMove,buffer),b.currentBoard()->pinnedPieces);
		}
	}
	return true;

}

int main(){
	UCI::loop();
}