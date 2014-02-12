
#include <string>

using namespace std;

struct Token{
	enum tokenType {ATTRIBUTE, VARIABLE, LITERAL, KEYWORD, SYMBOL, TABLE};
	tokenType type;
	string content;

	Token(tokenType t){
		type = t;
	}
};

