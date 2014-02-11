#include "rdbms.cpp"

using namespace std;

class ConjunctionNode;
class ComparisonNode;
class OperationNode;
class VariableNode;
class LiteralNode;
class ConditionNode;

class LeafNode{
public:
	virtual Datum getValue(vector<string> &atribNames, vector<Datum> &relation) = 0;
};

class VariableNode : public LeafNode{
	string varName;
	Datum varsDatum;
public:
	VariableNode(string vn);
	Datum getValue(vector<string> &atribNames, vector<Datum> &relation);
};

class LiteralNode : public LeafNode{
	Datum data;
public:
	LiteralNode(string d);
	Datum getValue(vector<string> &atribNames, vector<Datum> &relation);
};

class OperationNode{
	enum OP{eq = 0, neq, ls, leq, gr, geq};
	OP symbol;
	LeafNode* left;
	LeafNode* right;
public:
	OperationNode(OP s, LeafNode* l, LeafNode* r);
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
};

class ComparisonNode{
	OperationNode* operOpOperChild;
	ConditionNode* conditionChild;

public:
	ComparisonNode(OperationNode* n);
	ComparisonNode(ConditionNode* n);
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
};

class ConjunctionNode{
	vector<ComparisonNode*> compNodes;

public:
	ConjunctionNode(vector<ComparisonNode*> cn);
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
};

class ConditionNode{
	vector<ConjunctionNode*> conjNodes;
public:
	ConditionNode(vector<ConjunctionNode*>);
	bool eval(vector<string> &atribNames, vector<Datum> &relation);
};
