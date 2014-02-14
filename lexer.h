
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <vector>

using namespace std;

struct Token{
	enum tokenType {VARIABLE, LITERAL, KEYWORD, SYMBOL, OPENPAREN, CLOSEPAREN};
	tokenType type;
	string content;

	Token(tokenType t){
		type = t;
	}
};

