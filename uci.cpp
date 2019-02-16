#include <iostream>
#include <string>
#include <sstream>
#include "bitboard.h"
#include "board.h"
#include "types.h"

Board board;

Config zero_config() {
 	Config c = {0};
	return c;
}

std::string get_tokens() {
	std::string token;
	getline(std::cin, token);
	return token;
}


void position(std::stringstream ss) {
	//have a fenstring from the ss
	//given this fenstring, make these moves ...


	std::string token;
	std::getline(ss, token, ' ');

	std::string boardfen;
	if(!token.compare("startpos")) {
		boardfen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	}else{
		for(int i = 0; i < 6){
			std::getline(ss, token, ' ');
			boardfen.append(token);
			boardfen.append(' ');
		}
	}
	board.parseFen(boardfen);
	
	std::string move;
	while(getline(ss, move, ' ')){
		char* movestr = move.c_str();
		
		//convert move string to move object
		U8 file1 = getFile(movestr[0] - 'a');
		U8 rank1 = getRank(movestr[1] - '1');

		U8 file2 = getFile(movestr[2] - 'a');
		U8 rank2 = getRank(movestr[3] - '1');
		
		//if its a promotion
		

		//create
		//board.makeMove();
	}
	//this is ben's line

	int x;
	(void) x;
	
}


void handle_token(int* running) {
	std::cout << "> ";
	std::string line = get_tokens();

	std::stringstream ss(line);
	std::string token;

	std::getline(ss, token, ' ');


	if(!token.compare("uci")) {
		std::cout << "King of the Brick Chess Engine" << std::endl;
		std::cout << "uciok" << std::endl << std::endl;
	} else if(!token.compare("go")) {

	} else if(!token.compare("position")) {
		//position(ss);
	} else if(!token.compare("perf")) {

	} else if(!token.compare("ucinewgame")) {
		//new stuff
		std::cout << "> ";
	} else if(!token.compare("quit")) {
		std::cout << "goodbye";
		std:: cout << std::endl;
		*running = 0;
	}
}

int main(int argc, char** argv) {
	int running = 1;
	while(running) {
		handle_token(&running);
	}
}
