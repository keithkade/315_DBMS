#pragma once
#include "parser.h"
#include "conditionTree.h"
#include "lexer.h"
#include "rdbms.h"
#include <sstream> //for turning string to int
#include <fstream>	//for file I/O
#include <iostream>

using namespace std;

Parser::Parser(){}

Parser::Parser(Database* db)
{
	rdbms = db;
}

void Parser::setDatabasePtr(Database* dbPtr)
{
	rdbms = dbPtr;
}

Table Parser::getTempTable(const string& tableName)
{
	return tempTables[tableName];
}

void Parser::openRelationFile(const string& relationName)
{
	vector<Token> tokens;
	string command;
	ifstream relationFile(relationName + ".db");
	while (getline(relationFile, command))
	{
		tokens = lexInputLine(command);
		Parser::command(tokens);
	}
}

void Parser::writeRelationToFile(const string& relationName)
{
	ofstream newFile(relationName + ".db");
}

void Parser::closeRelationFile(const string& relationName)
{
	ofstream relationFile(relationName + ".db");
	Table t = rdbms->getTable(relationName);

	vector<Datum> typeSample;
	//need to get types of Datum's in table to construct CREATE command
	if (t.data.size() > 0)
	{
		 typeSample = t.data[0];
	}
	else 
	{
		cout << "No data to save. Save failed." << endl;
		return;
	}

	vector<string> types;
	for (int i = 0; i < typeSample.size(); ++i)
	{
		//indicating this Datum is a string
		if (typeSample[i].numData == -999)
		{
			types.push_back("VARCHAR(20)");
		}
		else
		{
			types.push_back("INTEGER");
		}
	}

	//construct the CREATE command
	string createCmd = "CREATE TABLE " + relationName + " (";
	for (int i = 0; i < types.size(); ++i)
	{
		createCmd += t.attributeNames[i] + " " + types[i];
		if (i + 1 < types.size())
		{
			//need to add a comma
			createCmd += ", ";
		}
		else
		{
			//need to add a parenthesis and pk info
			createCmd += ") PRIMARY KEY (";
			//loop through and add keynames to command
			for (int j = 0; j < t.keyNames.size(); ++j)
			{
				createCmd += t.keyNames[j];
				if (j + 1 < t.keyNames.size())
				{
					//need to add a comma
					createCmd += ", ";
				}
				else
				{
					//need to add closing parenthesis
					createCmd += ");";
				}
			}
		}
	}

	//add to file
	relationFile << createCmd << endl;

	//construct instert commands
	string insertCmd = "";
	//loop through rows in relation data
	for (int row = 0; row < t.data.size(); ++row)
	{
		insertCmd = "INSERT INTO " + relationName + " VALUES FROM (";
		//loop through columns in relation data
		for (int column = 0; column < t.attributeNames.size(); ++column)
		{
			if (t.data[row][column].numData == -999)
			{
				//dealing with string data
				insertCmd += "\"" + t.data[row][column].stringData + "\"";
			}
			else
			{
				//dealing with int data
				insertCmd += to_string(t.data[row][column].numData);
			}

			//determine if need to add comma
			if (column + 1 < t.attributeNames.size())
			{
				insertCmd += ", ";
			}
			else
			{
				//need to add closing parenthesis
				insertCmd += ");";
			}
		}
		relationFile << insertCmd << endl;
	}
}

vector<string> Parser::attributeList(vector<Token>& tokens)
{
	vector<string> attributes;
	vector<Token>::iterator iter = tokens.begin();

	while (true)
	{
		attributes.push_back(iter->content);

		if ((iter + 1) == tokens.end())
		{
			break;
		}
		else{
			iter = iter + 2;
		}
	}
	return attributes;
}

vector<string> Parser::typedAttributeList(vector<Token>& tokens)
{
	vector<string> attributes;
	vector<Token>::iterator iter = tokens.begin();
	while (true)
	{
		attributes.push_back(iter->content);
	
		if ((iter + 1)->content.compare("INTEGER") == 0)
		{
			if ((iter + 2) == tokens.end())
			{
				break;
			}
			else{
				iter = iter + 3;
			}
		}
		else if ((iter + 1)->content.compare("VARCHAR") == 0)
		{
			if ((iter + 5) == tokens.end())
			{
				break;
			}
			else{
				iter = iter + 6;
			}
		}
	}

	return attributes;
}

ComparisonNode* Parser::comparison(vector<Token>& tokens)
{
	if (tokens.size() == 3 && (tokens[0].type == Token::VARIABLE || tokens[0].type == Token::LITERAL) )
	{
		LeafNode* leftNode;
		if (tokens[0].type == Token::VARIABLE)
		{
			leftNode = new VariableNode(tokens[0].content);
		}
		else{
			if (tokens[0].content[0] == '\"')
			{
				tokens[0].content.erase(tokens[0].content.begin());
				tokens[0].content.pop_back();
				cout << "1" << tokens[0].content << "\n";
				leftNode = new LiteralNode(tokens[0].content);
			}
			else{
				istringstream buffer(tokens[0].content);
				int numContent;
				buffer >> numContent;

				leftNode = new LiteralNode(numContent);
			}
		}

		OperationNode::OP oper;
		if (tokens[1].content == "==")
		{
			oper = OperationNode::eq;
		}
		else if (tokens[1].content == "!=")
		{
			oper = OperationNode::neq;
		}
		else if (tokens[1].content == "<")
		{
			oper = OperationNode::ls;
		}
		else if (tokens[1].content == ">")
		{
			oper = OperationNode::gr;
		}
		else if (tokens[1].content == "<=")
		{
			oper = OperationNode::leq;
		}
		else if (tokens[1].content == ">=")
		{
			oper = OperationNode::geq;
		}

		LeafNode* rightNode;
		if (tokens[2].type == Token::VARIABLE)
		{
			rightNode = new VariableNode(tokens[2].content);
		}
		else{
			if (tokens[2].content[0] == '\"')
			{
				tokens[2].content.erase(tokens[2].content.begin());
				tokens[2].content.pop_back();
				rightNode = new LiteralNode(tokens[2].content);
			}
			else{
				istringstream buffer(tokens[2].content);
				int numContent;
				buffer >> numContent;

				rightNode = new LiteralNode(numContent);
			}
		}

		OperationNode* operNode = new OperationNode(oper, leftNode, rightNode);
		ComparisonNode* compNode = new ComparisonNode(operNode);
		return compNode;
	}
	else if (tokens[0].type == Token::OPENPAREN)
	{
		tokens.erase(tokens.begin());
		tokens.pop_back();
		ConditionNode* condNode = condition(tokens);
		ComparisonNode* compNode = new ComparisonNode(condNode);
		return compNode;
	}
}

ConjunctionNode* Parser::conjunction(vector<Token>& tokens)
{
	int parenDepth = 0;
	vector<ComparisonNode*> compNodes;
	vector<Token> subset;
	vector<Token>::iterator iter = tokens.begin();
	while (iter != tokens.end())
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}

		if ((parenDepth == 0) && (iter->content.compare("||") == 0))
		{
			ComparisonNode* tempNode = comparison(subset);
			compNodes.push_back(tempNode);
			subset.clear();
		}
		else{
			subset.push_back(*iter);
		}
		iter++;
	}
	ComparisonNode* tempNode = comparison(subset);
	compNodes.push_back(tempNode);

	ConjunctionNode* conjNode = new ConjunctionNode(compNodes);
	return conjNode;
}

ConditionNode* Parser::condition(vector<Token>& tokens)
{
	int parenDepth = 0;
	vector<ConjunctionNode*> conjNodes;
	vector<Token> subset;
	vector<Token>::iterator iter = tokens.begin();
	while (iter != tokens.end())
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}

		if ((parenDepth == 0) && (iter->content.compare("||") == 0))
		{
			ConjunctionNode* tempNode = conjunction(subset);
			conjNodes.push_back(tempNode);
			subset.clear();
		}
		else{
			subset.push_back(*iter); 
		}
		iter++;
	}
	ConjunctionNode* tempNode = conjunction(subset);
	conjNodes.push_back(tempNode);

	ConditionNode* condNode = new ConditionNode(conjNodes);
	return condNode;
}

Table Parser::expression(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	
	if (iter->type == Token::KEYWORD)
	{
		string keyWord = iter->content;
		if (keyWord.compare("select") == 0)
		{
			tokens.erase(iter);
			return selection(tokens);
		}
		else if (keyWord.compare("project") == 0)
		{
			tokens.erase(iter);
			return projection(tokens);
		}
		else if (keyWord.compare("rename") == 0)
		{
			tokens.erase(iter);
			return renaming(tokens);
		}
	}
	
	int parenDepth = 0;
	while (iter != tokens.end())
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
			continue;
		}
		if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
			continue;
		}

		if (parenDepth == 0 && (iter->type == Token::KEYWORD || iter->type == Token::SYMBOL))
		{
			if (iter->content.compare("+") == 0)
			{
				return myUnion(tokens);
			}
			if (iter->content.compare("-") == 0)
			{
				return difference(tokens);
			}
			if (iter->content.compare("*") == 0)
			{
				return product(tokens);
			}
			if (iter->content.compare("JOIN") == 0)
			{
				return naturalJoin(tokens);
			}
		}

		iter++;

		if (iter == tokens.end())
		{
			return atomExpression(tokens);
		}
	}
}

Table Parser::atomExpression(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	if (iter->type == Token::OPENPAREN)
	{
		tokens.erase(iter);
		tokens.pop_back();
		return expression(tokens);
	}
	else{
		if (tokens.size() == 1)
		{
			map<string,Table>::iterator curTableIt = tempTables.find(tokens[0].content);
			if (curTableIt != tempTables.end())
			{
				return curTableIt->second;
			}
			else
			{
				return rdbms->getTable(tokens[0].content);
			}
		}
	}
}

Table Parser::selection(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	iter++;
	
	int parenDepth = 1;
	vector<Token> condTokens;
	while (true)
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}
		
		if (parenDepth == 0)
		{
			iter++;
			break;
		}
		else
		{
			condTokens.push_back(*iter);
			iter++;
		}
	}
	
	vector<Token> atomTokens;
	while (iter != tokens.end())
	{
		atomTokens.push_back(*iter);
		iter++;
	}

	ConditionNode* conditionTree = condition(condTokens);
	Table selectedTable = atomExpression(atomTokens);

	return selectedTable.selectFromTable(*conditionTree);
}

Table Parser::projection(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();	
	iter++;
	
	int parenDepth = 1;
	vector<Token> atribTokens;
	while (true)
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}

		if (parenDepth == 0)
		{
			iter++;
			break;
		}
		else
		{
			atribTokens.push_back(*iter);
			iter++;
		}
	}

	vector<Token> atomTokens;
	while (iter != tokens.end())
	{
		atomTokens.push_back(*iter);
		iter++;
	}

	vector<string> atribNames = attributeList(atribTokens);
	Table selectedTable = atomExpression(atomTokens);

	return selectedTable.projectFromTable(atribNames);
}

Table Parser::renaming(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	iter++;
	
	int parenDepth = 1;
	vector<Token> atribTokens;
	while (true)
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}

		if (parenDepth == 0)
		{
			iter++;
			break;
		}
		else
		{
			atribTokens.push_back(*iter);
			iter++;
		}
	}

	vector<Token> atomTokens;
	while (iter != tokens.end())
	{
		atomTokens.push_back(*iter);
		iter++;
	}

	vector<string> atribNames = attributeList(atribTokens);
	Table selectedTable = atomExpression(atomTokens);

	// return projectFromTable 
	return  selectedTable.renameAttributes(atribNames);
}

Table Parser::myUnion(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();

	int parenDepth = 0;
	vector<Token> atom1Tokens;
	do
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}
	
		atom1Tokens.push_back(*iter);
		iter++;
	}while (parenDepth != 0);

	iter++;
	
	vector<Token> atom2Tokens;
	while (iter != tokens.end())
	{
		atom2Tokens.push_back(*iter);
		iter++;
	}

	Table uniArgTable1 = atomExpression(atom1Tokens);
	Table uniArgTable2 = atomExpression(atom2Tokens);

	return uniArgTable1.unionWith(uniArgTable2);
}

Table Parser::difference(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();

	int parenDepth = 0;
	vector<Token> atom1Tokens;
	do
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}

		atom1Tokens.push_back(*iter);
		iter++;
	} while (parenDepth != 0);

	iter++;

	vector<Token> atom2Tokens;
	while (iter != tokens.end())
	{
		atom2Tokens.push_back(*iter);
		iter++;
	}

	Table difArgTable1 = atomExpression(atom1Tokens);
	Table difArgTable2 = atomExpression(atom2Tokens);

	return difArgTable1.differenceWith(difArgTable2);
}

Table Parser::product(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();

	int parenDepth = 0;
	vector<Token> atom1Tokens;
	do
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}
		
		atom1Tokens.push_back(*iter);
		iter++;
	} while (parenDepth != 0);
	
	iter++;
	
	vector<Token> atom2Tokens;
	while (iter != tokens.end())
	{
		atom2Tokens.push_back(*iter);
		iter++;
	}

	Table prodArgTable1 = atomExpression(atom1Tokens);
	Table prodArgTable2 = atomExpression(atom2Tokens);

	return prodArgTable1.productWith(prodArgTable2);
}

Table Parser::naturalJoin(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();

	int parenDepth = 0;
	vector<Token> atom1Tokens;
	do
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			parenDepth--;
		}
		
		atom1Tokens.push_back(*iter);
		iter++;
	} while (parenDepth != 0);

	iter++;

	vector<Token> atom2Tokens;
	while (iter != tokens.end())
	{
		atom2Tokens.push_back(*iter);
		iter++;
	}

	Table natJoinArgTable1 = atomExpression(atom1Tokens);
	Table natJoinArgTable2 = atomExpression(atom2Tokens);

	return natJoinArgTable1.naturalJoinWith(natJoinArgTable2);
}

void Parser::query(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	string queryTableName = iter->content;
	
	vector<Token> exprTokens;
	iter = iter + 2;
	while (iter != tokens.end())
	{
		exprTokens.push_back(*iter);
		iter++;
	}

	Table resultTable = expression(exprTokens);

	map<string, Table>::iterator nameTaken = tempTables.find(queryTableName);
	if (nameTaken == tempTables.end())
	{
		tempTables.insert(pair<string, Table>(queryTableName, resultTable));
	}
	else{
		tempTables.erase(nameTaken);
		tempTables.insert(pair<string, Table>(queryTableName, resultTable));
	}
}

void Parser::open(vector<Token>& tokens)
{
	string tableName = tokens[1].content;
	openRelationFile(tableName);
}

void Parser::close(vector<Token>& tokens)
{
	string tableName = tokens[1].content;
	closeRelationFile(tableName);
}

void Parser::write(vector<Token>& tokens)
{
	string tableName = tokens[1].content;
	writeRelationToFile(tableName);
}

void Parser::show(vector<Token>& tokens)
{
	vector<Token> atomExprTokens;
	vector<Token>::iterator iter = tokens.begin() + 1;
	while (iter != tokens.end())
	{
		atomExprTokens.push_back(*iter);
		iter++;
	}

	Table resultTable = atomExpression(atomExprTokens);
	resultTable.printTable();
}

void Parser::create(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	string tableName = (iter + 2)->content;

	int depth = 1;
	iter = iter + 4;
	vector<Token> typedAtribTokens;
	while (depth != 0)
	{
		if (iter->type == Token::OPENPAREN)
		{
			depth++;
		}
		else if (iter->type == Token::CLOSEPAREN)
		{
			depth--;
		}

		typedAtribTokens.push_back(*iter);
		iter++;
	}
	typedAtribTokens.pop_back(); // removes trailing closeparen

	iter = iter + 3;
	vector<Token> atribTokens;
	while (iter->type != Token::CLOSEPAREN)
	{
		atribTokens.push_back(*iter);
		iter++;
	}

	//printTokenList(typedAtribTokens);
	//printTokenList(atribTokens);

	vector<string> typedAtribs = typedAttributeList(typedAtribTokens);
	vector<string> primaryKeys = attributeList(atribTokens);
	/*
	for (int i = 0; i < typedAtribs.size(); i++)
		cout << typedAtribs[i] << " ";
	cout << "\n";


	for (int i = 0; i < primaryKeys.size(); i++)
		cout << primaryKeys[i] << " ";
	cout << "\n";
	*/
	rdbms->createTable(tableName, typedAtribs, primaryKeys);
}

void Parser::update(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	string tableName = (iter + 1)->content;

	iter = iter + 3;

	vector<string> varNames;
	vector<Datum> newVals;
	while (true)
	{
		varNames.push_back(iter->content);
		Datum newVal;
		if ((iter + 2)->content[0] == '\"')
		{
			(iter + 2)->content.erase((iter + 2)->content.begin());
			(iter + 2)->content.pop_back();
			newVals.push_back(Datum((iter + 2)->content));
		}
		else
		{
			istringstream buffer((iter + 2)->content);
			int numContent;
			buffer >> numContent;

			newVals.push_back(Datum(numContent));
		}

		if ((iter + 3)->content.compare("WHERE") == 0)
		{
			iter = iter + 4;
			break;
		}
		
		iter = iter + 4;
	}

	vector<Token> condTokens;
	while (iter != tokens.end())
	{
		condTokens.push_back(*iter);
		iter++;
	}

	ConditionNode* cond = condition(condTokens);

	rdbms->updateTable(tableName, varNames, newVals, *cond);
}

void Parser::insert(vector<Token>& tokens)
{
	//printTokenList(tokens);
	vector<Token>::iterator iter = tokens.begin();
	string tableName = (iter + 2)->content;
	iter = iter + 5;

	if (iter->type == Token::OPENPAREN)
	{
		iter++;
		vector<Datum> info;
		while (iter != tokens.end())
		{
			if (iter->content[0] == '\"')
			{
				iter->content.erase(iter->content.begin());
				iter->content.pop_back();
				info.push_back(Datum(iter->content));
			}
			else{
				istringstream buffer(iter->content);
				int numContent;
				buffer >> numContent;
				
				info.push_back(Datum(numContent));
			}
			iter = iter + 2;
		}
		rdbms->insertIntoTable(tableName, info);
	}
	else{
		vector<Token> exprTokens;
		while (iter != tokens.end())
		{
			exprTokens.push_back(*iter);
			iter++;
		}
	}

}

void Parser::myDelete(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	iter = iter + 2;
	string tableName = iter->content;

	iter = iter + 3;
	vector<Token> exprTokens;
	while (iter->type != Token::CLOSEPAREN)
	{
		exprTokens.push_back(*iter);
		iter++;
	}

	ConditionNode* cond = condition(exprTokens);

	rdbms->deleteFromTable(tableName, *cond);
}

void Parser::command(vector<Token>& tokens)
{
	if (tokens[0].content.compare("OPEN") == 0)
	{
		open(tokens);
	}
	else if (tokens[0].content.compare("CLOSE") == 0)
	{
		close(tokens);
	}
	else if (tokens[0].content.compare("WRITE") == 0)
	{
		//cout << "\nIn Write \n";
		write(tokens);
	}
	else if (tokens[0].content.compare("SHOW") == 0)
	{
		show(tokens);
	}
	else if (tokens[0].content.compare("CREATE") == 0)
	{
		create(tokens);
	}
	else if (tokens[0].content.compare("UPDATE") == 0)
	{
		update(tokens);
	}
	else if (tokens[0].content.compare("INSERT") == 0)
	{
		insert(tokens);
	}
	else if (tokens[0].content.compare("DELETE") == 0)
	{
		myDelete(tokens);
	}
	else if (tokens[1].content.compare("<-") == 0)
	{
		query(tokens);
	}
}

bool Parser::isNum(string input)
{
	for (int i = 0; i < input.length(); i++)
	{
		if (!isdigit(input[i]))
			return false;
	}
	
	return true;
}

void Parser::printTokenList(vector<Token>& tokens)
{
	cout << "\n";
	for (int i = 0; i < tokens.size(); i++)
	{
		cout << tokens[i].content << " ";
	}
	cout << "\n";
}