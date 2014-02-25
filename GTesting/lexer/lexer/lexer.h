
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <vector>

using namespace std;

//the lexer takes input and tokenizes, storing the input as well as an enumerated type to help the parser distinguish different kinds of input
struct Token{
	enum tokenType {VARIABLE, LITERAL, KEYWORD, SYMBOL, OPENPAREN, CLOSEPAREN, COMMA}; 
	tokenType type; //the type of he token
	string content; //what the token contains. ex: "JOIN"
	
	//constructor
	Token(tokenType t){
		type = t;
	}
};

//take an input file and lex it
std::vector<std::vector<Token> > lexFile(std::string fileName);

//lex a single command or query
std::vector<Token> lexInputLine(std::string inputLine);