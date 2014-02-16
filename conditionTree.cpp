#pragma once
#include "rdbms.h"
#include "ConditionTree.h"

using namespace std;

VariableNode::VariableNode(string vn) : varName(vn){}
Datum VariableNode::getValue(vector<string> &attributeNames, vector<Datum> &relation)
{
	for (int i = 0; i < attributeNames.size(); i++)
	{
		if (attributeNames[i] == varName)
		{
			return relation[i];
		}
	}

}

LiteralNode::LiteralNode(string d) : data(d){}
LiteralNode::LiteralNode(int d) : data(d){}
Datum LiteralNode::getValue(vector<string> &attributeNames, vector<Datum> &relation)
{
	return data;
}

OperationNode::OperationNode(OP s, LeafNode* l, LeafNode* r) : symbol(s), left(l), right(r) {}
bool OperationNode::eval(vector<string> &attributeNames, vector<Datum> &relation)
{
	if (left->getValue(attributeNames, relation).numData != -999 && right->getValue(attributeNames, relation).numData != -999)
	{	// If both Datum nums are not -999 we know they are both representing ints and therefore can be compared.
		switch (symbol)
		{
		case eq:
			cout << (left->getValue(attributeNames, relation).numData == right->getValue(attributeNames, relation).numData) << "\n";
			return left->getValue(attributeNames, relation).numData == right->getValue(attributeNames, relation).numData; break;
		case neq:
			return left->getValue(attributeNames, relation).numData != right->getValue(attributeNames, relation).numData; break;
		case ls:
			return left->getValue(attributeNames, relation).numData < right->getValue(attributeNames, relation).numData; break;
		case leq:
			return left->getValue(attributeNames, relation).numData <= right->getValue(attributeNames, relation).numData; break;
		case gr:
			return left->getValue(attributeNames, relation).numData > right->getValue(attributeNames, relation).numData; break;
		case geq:
			return left->getValue(attributeNames, relation).numData >= right->getValue(attributeNames, relation).numData;
		}
	}
	else if (left->getValue(attributeNames, relation).numData == -999 && right->getValue(attributeNames, relation).numData == -999)
	{// If both Datum nums are -999 we know they both represent strings and therefore can be compared. However only with == and !=
		switch (symbol)
		{
		case eq:
			return left->getValue(attributeNames, relation).stringData == right->getValue(attributeNames, relation).stringData; break;
		case neq:
			return left->getValue(attributeNames, relation).stringData != right->getValue(attributeNames, relation).stringData; break;
		}
	}
	else{ // If we got to here then the Datums must not be of the same and therefore can not be compared so will return a false.
		return false;
	}
}

ComparisonNode::ComparisonNode(OperationNode* n) : operOpOperChild(n){}
ComparisonNode::ComparisonNode(ConditionNode* n) : conditionChild(n){}
bool ComparisonNode::eval(vector<string> &attributeNames, vector<Datum> &relation){
	if (operOpOperChild != NULL)
	{
		return operOpOperChild->eval(attributeNames, relation);
	}
	else if (conditionChild != NULL)
	{
		return conditionChild->eval(attributeNames, relation);
	}
	else{
		return false;
	}
}

ConjunctionNode::ConjunctionNode(vector<ComparisonNode*> cn) : compNodes(cn) {}
bool ConjunctionNode::eval(vector<string> &attributeNames, vector<Datum> &relation){
	bool result = true;
	for (int i = 0; i < compNodes.size(); i++)
	{
		result = result && compNodes[i]->eval(attributeNames, relation);
	}
	return result;
}


ConditionNode::ConditionNode(vector<ConjunctionNode*> cn) : conjNodes(cn) {}
bool ConditionNode::eval(vector<string> &attributeNames, vector<Datum> &relation){
	bool result = false;
	for (int i = 0; i < conjNodes.size(); i++)
	{
		result = result || conjNodes[i]->eval(attributeNames, relation);
	}
	return result;
}
