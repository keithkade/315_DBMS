//


#include "lexer.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <vector>

using namespace std;

bool isComm(string in){
	if (in == "project" || in == "rename" || in == "select")
		return true;
	return false;
}


vector<Token> commandLex(string line){
	vector<Token> retRow;
	
	return retRow;
}


vector<Token> afterAssignQueryLex(string line){
	vector<Token> afterRetVecter;
	
	string nextTokStr = line.substr(0, line.find(' '));
	if (isComm(nextTokStr)){
		Token commandNameTok(Token::tokenType::KEYWORD);
		commandNameTok.content = nextTokStr;
		afterRetVecter.push_back(commandNameTok);
	}
	else{
		Token tableNameTok(Token::tokenType::TABLE);
		tableNameTok.content = nextTokStr;
		afterRetVecter.push_back(tableNameTok);
	}

	return afterRetVecter;
}


vector<Token> queryLex(string line){
	vector<Token> retRow;
	vector<Token> afterAssignRow;

	//trim possible newline characters
	if (line[0] == '\n')
		line.erase(line.begin());
	if (line[0] == '\n')
		line.erase(line.begin());

	//name of variable we return to
	Token varTok(Token::tokenType::VARIABLE);
	varTok.content = line.substr(0, line.find(' '));
	retRow.push_back(varTok);

	Token assignTok(Token::tokenType::SYMBOL);
	assignTok.content = "<-";
	retRow.push_back(assignTok);

	string afterAssign(line);
	afterAssign.erase(0, afterAssign.find("<-")+3);

	//lex rest of line
	afterAssignRow = afterAssignQueryLex(afterAssign);

	//concatonate the two vectors
	retRow.insert(retRow.end(), afterAssignRow.begin(), afterAssignRow.end());

	return retRow;
}


int main(){
	string line;
	vector<vector<Token>> lexedInput;
	int i;
	ifstream inFile("parser_milestone_good_inputs.txt");
	while (getline(inFile, line, ';')) {
		vector<Token> lexedRow;
		if (line.find("<-") != std::string::npos){
			lexedRow = queryLex(line);
		}
		else{
			lexedRow = commandLex(line);
		}
		lexedInput.push_back(lexedRow);
	}
	int n;
	cin >> n;
}
