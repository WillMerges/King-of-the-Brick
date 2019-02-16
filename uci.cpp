#include <iostream>
#include <string>
#include <sstream>
#include "bitboard.h"
#include "board.h"
#include "types.h"



Config zero_config() {
	c = Config = {0};
	return c;
}

std::string get_tokens() {
	std::string token;
	getline(std::cin, token);
	return token;
}


void position(stringstream ss) {
	std::string token;
	std::getline(ss, token, ' ');

	if(!token.compare("fen")) {
		//do stuff
	}
}


void handle_token(int* running) {
	std::cout << "> ";
	std::string line = get_tokens();

	std::istringstream ss(line);
	std::string token;

	std::getline(ss, token, ' ');


	if(!token.compare("uci")) {
		std::cout << "King of the Brick Chess Engine" << std::endl;
		std::cout << "uciok" << std::endl << std::endl;
	} else if(!token.compare("go")) {

	} else if(!token.compare("position")) {
		position(ss);
	} else if(!token.compare("perf")) {

	} else if (!token.compare("ucinewgame") {
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
