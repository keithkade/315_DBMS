/*
	The following class will be use to find if a condition is satisfied for the database to then execute a command on a relation.

	It can be used to create any logical expression the parser could need through the parameters stipulated by the problem. 

	Each inner node will contain an eval function and each leaf node will have a getValue to call that will giev the value at the variable.

	To solve the tree you would call the eval on the root condition node. It will recursively call eval on theirs and so forth 
	until a leaf is reached. The leaf will either return the literal or use the parameters entered into the initial function to 
	determine what value belongs. In the end it will return a simple true or false representing if the condition was met or not.
*/

#pragma once
#include <vector>
#include <string>
#include "rdbms.h"


struct Datum;
class ConditionNode;

// A super class so that Variables and Literals can be used interchangably
class LeafNode{
public:
	virtual Datum getValue(std::vector<std::string> &attributeNames, std::vector<Datum> &relation) = 0;
};

// This will be why the atribNames and relations are sent through the getValues
// At this leaf it will find the values
class VariableNode : public LeafNode{
	std::string varName;
	Datum varsDatum;
public:
	VariableNode(std::string vn);
	Datum getValue(std::vector<std::string> &attributeNames, std::vector<Datum> &relation);
};

// a leaf node that just gives a value. 
class LiteralNode : public LeafNode{
	Datum data;
public:
	LiteralNode(std::string d);
	LiteralNode(int d);
	Datum getValue(std::vector<std::string> &attributeNames, std::vector<Datum> &relation);
};

// Uses enum OPs to help clarify what each opperation is.
class OperationNode{
public:
	enum OP{ eq = 0, neq, ls, leq, gr, geq };
	OperationNode(OP s, LeafNode* l, LeafNode* r);
	bool eval(std::vector<std::string> &atribNames, std::vector<Datum> &relation);
private:
	OP symbol;
	LeafNode* left;
	LeafNode* right;
};

// Can either recurse back to condition or continue to opperations
class ComparisonNode{
	OperationNode* operOpOperChild;
	ConditionNode* conditionChild;

public:
	ComparisonNode(OperationNode* n);
	ComparisonNode(ConditionNode* n);
	bool eval(std::vector<std::string> &attributeNames, std::vector<Datum> &relation);
};

// can have any number of Comparisons so will be represented in std::vector
class ConjunctionNode{
	std::vector<ComparisonNode*> compNodes;

public:
	ConjunctionNode(std::vector<ComparisonNode*> cn);
	bool eval(std::vector<std::string> &attributeNames, std::vector<Datum> &relation);
};

// a condition can have any number of conjuctions so will represent with std::vector.
class ConditionNode{
	std::vector<ConjunctionNode*> conjNodes;
public:
	ConditionNode(std::vector<ConjunctionNode*>);
	bool eval(std::vector<std::string> &attributeNames, std::vector<Datum> &relation);
};
