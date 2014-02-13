//


#include "lexer.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <vector>

using namespace std;

//distinguishes a sequence of character between a keyword and an unknown, the creates appropriate token 
void makeCommVarTok(vector<Token> & afterRetVecter, string in){
	if (in == "project" || in == "rename" || in == "select"){
		Token commandNameTok(Token::tokenType::KEYWORD);
		commandNameTok.content = in;
		afterRetVecter.push_back(commandNameTok);
	}
	//else if the entire thing is numeral make literal
	//check against quotes is to ensure literals do not get lexed twice
	else if (in[0] != '"'){
		Token tableNameTok(Token::tokenType::VARIABLE);
		tableNameTok.content = in;
		afterRetVecter.push_back(tableNameTok);
	}
}

vector<Token> commandLex(string line){
	vector<Token> retRow;
	
	return retRow;
}

//does not yet identify numbers as literals
vector<Token> afterAssignQueryLex(string line){
	vector<Token> afterRetVecter;
	
	string curString=""; //contents of token accumulate on this
	int stringPos=0;

	while (stringPos <= line.size()){
		char curChar = line[stringPos];

		//ignore spaces
		if (curChar == ' '){
			stringPos++;
			continue;
		}

		//needed to get the last token in the string
		if (stringPos == line.size()){
			//if end of string is paren then curstring mught be empty, so this checks for that
			if (curString != "")
				makeCommVarTok(afterRetVecter, curString);
		}

		//if it is an alphanumeric or _ add it to our surrent string
		if (isalnum(curChar) || curChar == '_'){
			curString += curChar;
		}

		if (curChar == '('){
			//if parenthesis are adjacent the curString might be empty, so this checks for that
			if (curString != "")
				makeCommVarTok(afterRetVecter, curString);
			curString = "";
			Token parenTok(Token::tokenType::OPENPAREN);
			parenTok.content = '(';
			afterRetVecter.push_back(parenTok);
		}

		if (curChar == ')'){
			if (curString != "")
				makeCommVarTok(afterRetVecter, curString);
			curString = "";
			Token parenTok(Token::tokenType::CLOSEPAREN);
			parenTok.content = ')';
			afterRetVecter.push_back(parenTok);
		}

		if (curChar == ','){
			makeCommVarTok(afterRetVecter, curString);
			curString = "";
			Token commaTok(Token::tokenType::SYMBOL);
			commaTok.content = ',';
			afterRetVecter.push_back(commaTok);
		}

		if (curChar == '+'){
			makeCommVarTok(afterRetVecter, curString);
			curString = "";
			Token plusTok(Token::tokenType::SYMBOL);
			plusTok.content = '+';
			afterRetVecter.push_back(plusTok);
		}

		if (curChar == '-'){
			makeCommVarTok(afterRetVecter, curString);
			curString = "";
			Token minusTok(Token::tokenType::SYMBOL);
			minusTok.content = '+';
			afterRetVecter.push_back(minusTok);
		}

		if (curChar == '*'){
			makeCommVarTok(afterRetVecter, curString);
			curString = "";
			Token prodTok(Token::tokenType::SYMBOL);
			prodTok.content = '+';
			afterRetVecter.push_back(prodTok);
		}

		if (curChar == '<'){
			makeCommVarTok(afterRetVecter, curString);
			curString = "";
			//can be < or <=
			if (line[stringPos + 1] == '='){
				Token lessEqTok(Token::tokenType::SYMBOL);
				lessEqTok.content = "<=";
				afterRetVecter.push_back(lessEqTok);
			}
			else if (line[stringPos + 1] == ' '){
				Token lessTok(Token::tokenType::SYMBOL);
				lessTok.content = "<";
				afterRetVecter.push_back(lessTok);
			}
			else cout << "ERROR when parsing a <" << endl;
			stringPos++; //skip over next char since we have accounted for it
		}

		if (curChar == '>'){
			makeCommVarTok(afterRetVecter, curString);
			curString = "";
			//can be > or >=
			if (line[stringPos + 1] == '='){
				Token greatEqTok(Token::tokenType::SYMBOL);
				greatEqTok.content = ">=";
				afterRetVecter.push_back(greatEqTok);
			}
			else if (line[stringPos + 1] == ' '){
				Token greatTok(Token::tokenType::SYMBOL);
				greatTok.content = ">";
				afterRetVecter.push_back(greatTok);
			}
			else cout << "ERROR when parsing a >" << endl;
			stringPos++; //skip over next char since we have accounted for it
		}

		if (curChar == '='){
			makeCommVarTok(afterRetVecter, curString);
			curString = "";
			if (line[stringPos + 1] == '='){
				Token eqTok(Token::tokenType::SYMBOL);
				eqTok.content = "==";
				afterRetVecter.push_back(eqTok);
			}
			else cout << "ERROR when parsing a =" << endl;
			stringPos++; //skip over next char since we have accounted for it
		}

		if (curChar == '"'){
			if (isalnum(line[stringPos + 1])){
				int nextQuotePos;
				curString = "";
				nextQuotePos = line.find('\"', stringPos + 1);
				curString = line.substr(stringPos, (nextQuotePos - stringPos) + 1);
				Token litTok(Token::tokenType::LITERAL);
				litTok.content = curString;
				afterRetVecter.push_back(litTok);
				stringPos += (nextQuotePos - stringPos); //skip over the rest of token
			}
			else {
				stringPos++;
				continue;
			}
		}

		stringPos++;
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
			for (int i = 0; i < lexedRow.size(); i++)
				cout << "'" << lexedRow[i].content << "' ";
			cout << endl << endl;
		}
		else{
			lexedRow = commandLex(line);
		}
		lexedInput.push_back(lexedRow);
	}
	int n;
	cin >> n;
}
