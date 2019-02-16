#include <iostream>
#include <string>

std::string get_token() {
	std::string token;
	getline(std::cin, token);
	return token;	
}

void handle_token(int* running) {
	std::cout << "> ";
	std::string token = get_token();

	if(!token.compare("uci")) {
		std::cout << "King of the Brick Chess Engine" << std::endl;
		std::cout << "uciok" << std::endl << std::endl;	
	} else if(!token.compare("go")) {
		
	} else if(!token.compare("position")) {
	
	} else if(!token.compare("perf")) {
	
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


