#pragma once
#include "parser.h"
#include "conditionTree.h"
#include "lexer.h"
#include "rdbms.h"
#include <sstream> //for turning string to int

using namespace std;

Parser::Parser(Database* db)
{
	rdbms = db;
}

// Not complete need info on what to do with errors
vector<string> Parser::attributeList(vector<Token>& tokens)
{
	vector<string> attributes;
	vector<Token>::iterator iter = tokens.begin();
	while (true)
	{
		if (iter->type == Token::VARIABLE)
		{
			attributes.push_back(iter->content);
		}
		else{
			//error
		}

		iter++;
		// Skipping the , if there is one. 
		if (iter != tokens.end())
		{
			if (iter->type == Token::COMMA)
			{
				iter++;
			}
			else
			{
				//error
			}
		}
		else{
			return attributes;
		}
	}
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
			if (isdigit(tokens[0].content[0]))
			{
				istringstream buffer(tokens[0].content);
				int numContent;
				buffer >> numContent;

				leftNode = new LiteralNode(numContent);
			}
			else{
				leftNode = new LiteralNode(tokens[0].content);
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
		else{
			//error
		}
	
		LeafNode* rightNode;
		if (tokens[2].type == Token::VARIABLE)
		{
			rightNode = new VariableNode(tokens[2].content);
		}
		else{
			if (isdigit(tokens[2].content[0]))
			{
				istringstream buffer(tokens[2].content);
				int numContent;
				buffer >> numContent;

				rightNode = new LiteralNode(numContent);
			}
			else{
				rightNode = new LiteralNode(tokens[2].content);
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
	else{
		// error
	}
}

ConjunctionNode* Parser::conjunction(vector<Token>& tokens)
{
	vector<ComparisonNode*> compNodes;
	vector<Token>::iterator iter = tokens.begin();
	while (iter == tokens.end())
	{
		vector<Token> subset;
		if (iter->content.compare("&&") != 0)
		{
			subset.push_back(*iter);
		}
		
		if (iter->content.compare("&&") != 0 || (iter + 1) == tokens.end())
		{
			ComparisonNode* tempNode = comparison(subset);
			compNodes.push_back(tempNode);
			subset.clear();
		}
		iter++;
	}

	ConjunctionNode* conjNode = new ConjunctionNode(compNodes);
	return conjNode;
}

ConditionNode* Parser::condition(vector<Token>& tokens)
{
	vector<ConjunctionNode*> conjNodes;
	vector<Token>::iterator iter = tokens.begin();
	while (iter == tokens.end())
	{
		vector<Token> subset;
		if (iter->content.compare("||") != 0)
		{
			subset.push_back(*iter);
		}

		if (iter->content.compare("||") != 0 || (iter + 1) == tokens.end())
		{
			ConjunctionNode* tempNode = conjunction(subset);
			conjNodes.push_back(tempNode);
			subset.clear();
		}
		iter++;
	}

	ConditionNode* condNode = new ConditionNode(conjNodes);
	return condNode;
}

Table Parser::expression(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	
	if (iter->type == Token::KEYWORD)
	{
		string keyWord = iter->content;
		if (keyWord.compare("select"))
		{
			tokens.erase(iter);
			return selection(tokens);
		}
		else if (keyWord.compare("project"))
		{
			tokens.erase(iter);
			return projection(tokens);
		}
		else if (keyWord.compare("rename"))
		{
			tokens.erase(iter);
			return renaming(tokens);
		}
		else{
			// error
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
	else if(tokens.size() == 1)
	{
		// get table using tokens[0]	
	}
	else{
		// error
	}
}

Table Parser::selection(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	if (iter->type != Token::OPENPAREN)
	{
		//error
	}
	else
	{
		iter++;
	}

	int parenDepth = 1;
	vector<Token> condTokens;
	while (true)
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else 
		{
			if (iter->type == Token::CLOSEPAREN)
			{
				parenDepth--;
			}
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

	//return selectFromTable 
	return selectedTable; //removing error not finished.
}

Table Parser::projection(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	if (iter->type != Token::OPENPAREN)
	{
		//error
	}
	else
	{
		iter++;
	}

	int parenDepth = 1;
	vector<Token> atribTokens;
	while (true)
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else 
		{
			if (iter->type == Token::CLOSEPAREN)
			{
				parenDepth--;
			}
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
	return selectedTable; //removing error not finished.
}

Table Parser::renaming(vector<Token>& tokens)
{
	vector<Token>::iterator iter = tokens.begin();
	if (iter->type != Token::OPENPAREN)
	{
		//error
	}
	else
	{
		iter++;
	}

	int parenDepth = 1;
	vector<Token> atribTokens;
	while (true)
	{
		if (iter->type == Token::OPENPAREN)
		{
			parenDepth++;
		}
		else 
		{
			if (iter->type == Token::CLOSEPAREN)
			{
				parenDepth--;
			}
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
	return selectedTable; //removing error not finished.
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
		else 
		{
			if (iter->type == Token::CLOSEPAREN)
			{
				parenDepth--;
			}
		}
	
		atom1Tokens.push_back(*iter);
		iter++;
	}while (parenDepth != 0);

	if (iter->content[0] != '+')
	{
		// error
	}
	else{
		iter++;
	}

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
		else
		{
			if (iter->type == Token::CLOSEPAREN)
			{
				parenDepth--;
			}
		}

		atom1Tokens.push_back(*iter);
		iter++;
	} while (parenDepth != 0);

	if (iter->content[0] != '-')
	{
		// error
	}
	else{
		iter++;
	}

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
		else
		{
			if (iter->type == Token::CLOSEPAREN)
			{
				parenDepth--;
			}
		}

		atom1Tokens.push_back(*iter);
		iter++;
	} while (parenDepth != 0);

	if (iter->content[0] != '*')
	{
		// error
	}
	else{
		iter++;
	}

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
		else
		{
			if (iter->type == Token::CLOSEPAREN)
			{
				parenDepth--;
			}
		}

		atom1Tokens.push_back(*iter);
		iter++;
	} while (parenDepth != 0);

	if (iter->content.compare("JOIN"))
	{
		// error
	}
	else{
		iter++;
	}

	vector<Token> atom2Tokens;
	while (iter != tokens.end())
	{
		atom2Tokens.push_back(*iter);
		iter++;
	}

	Table prodArgTable1 = atomExpression(atom1Tokens);
	Table prodArgTable2 = atomExpression(atom2Tokens);

	//return prodArgTable1.naturalJoinWith(prodArgTable2);
	return prodArgTable1;
}

