#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

struct Datum{
	string stringData;
	int numData;

	Datum(){}
	Datum(int n) : numData(n) {}
	//initialize as -999 so we can check if a datum is a string or an int
	Datum(string s) : stringData(s), numData(-999) {}

	//needed to compare rows and remove duplicates
	bool operator!=(const Datum &d){
		return !(stringData == d.stringData && numData == d.numData);
	}

	bool operator==(const Datum &d){
		return (stringData == d.stringData && numData == d.numData);
	}
};

struct Table{

	vector<string> keyNames;
	string key1;
	string key2;

	vector<string> attributeNames;
	vector<vector<Datum> > data;

	//keys are used to check for conflicts. Primary key is combination of key1 and key2
	Table(vector<string> attrNames, vector<string> keys){
		keyNames = keys;
		attributeNames = attrNames;
	}

	Table(){}

	bool duplicateExists(vector<Datum> newRow){
		//get indeces of primary keys
		vector<int> keyIndices;
		bool dupFound = false;
		int conflictCounter = 0; //if it is the same as the number of keys there is a conflict
		for (int i = 0; i<attributeNames.size(); i++){
			for (int j = 0; j<keyNames.size(); j++)
			if (attributeNames[i] == keyNames[j])
				keyIndices.push_back(i);
		}

		//check for conflicts
		for (int i = 0; i<data.size(); i++){
			conflictCounter = 0;
			for (int j = 0; j<keyNames.size(); j++){
				if (data[i][j] == newRow[j])
					conflictCounter++;
			}
			if (conflictCounter == keyNames.size())
				dupFound = true;
		}

		return dupFound;
	}

	//for testing and debugging
	void printTable(){
		for (int i = 0; i<attributeNames.size(); i++)
			cout << attributeNames[i] << ", ";

		cout << endl;
		for (int i = 0; i<data.size(); i++){
			for (int j = 0; j<attributeNames.size(); j++){
				if (data[i][j].numData == -999)
					cout << data[i][j].stringData << " ";
				else
					cout << data[i][j].numData << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

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
