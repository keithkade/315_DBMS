#pragma once
#include <vector>
#include <string>
#include "rdbms.h"

using namespace std;

struct Datum;
class ConditionNode;

// A super class so that Variables and Literals can be used interchangably
class LeafNode{
public:
	virtual Datum getValue(vector<string> &atribNames, vector<Datum> &relation) = 0;
};

// This will be why the atribNames and relations are sent through the getValues
// At this leaf it will find the values
class VariableNode : public LeafNode{
	string varName;
	Datum varsDatum;
public:
	VariableNode(string vn);
	Datum getValue(vector<string> &atribNames, vector<Datum> &relation);
};

// a leaf node that just gives a value. 
class LiteralNode : public LeafNode{
	Datum data;
public:
	LiteralNode(string d);
	LiteralNode(int d);
	Datum getValue(vector<string> &atribNames, vector<Datum> &relation);
};

// Uses enum OPs to help clarify what each opperation is.
class OperationNode{
public:
	enum OP{ eq = 0, neq, ls, leq, gr, geq };
	OperationNode(OP s, LeafNode* l, LeafNode* r);
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
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
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
};

// can have any number of Comparisons so will be represented in vector
class ConjunctionNode{
	vector<ComparisonNode*> compNodes;

public:
	ConjunctionNode(vector<ComparisonNode*> cn);
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
};

// a condition can have any number of conjuctions so will represent with vector.
class ConditionNode{
	vector<ConjunctionNode*> conjNodes;
public:
	ConditionNode(vector<ConjunctionNode*>);
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
};
