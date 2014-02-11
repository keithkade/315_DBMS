/*
RDBMS: Kade Keith, Matthew Saari, Ryan Ledbetter, Victor Gutierrez
*/

// this is my commit VG
// testing RL

//#include "rdbms.h"

//#include "conditionTree.cpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

class ConjunctionNode;
class ComparisonNode;
class OperationNode;
class VariableNode;
class LiteralNode;
class ConditionNode;



struct Datum{
	string stringData;
	int numData;

	Datum(int n) : numData(n) {}
	//initialize as -999 so we can check if a datum is a string or an int
	Datum(string s) : stringData(s), numData(-999) {}
	Datum() : numData(-999) {}

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
	LiteralNode(int d);
	Datum getValue(vector<string> &atribNames, vector<Datum> &relation);
};

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




class Database{

public:
	//MOVE BACK
	map<string, Table> allTables;

	Database(){}

	void createTable(string tableName, vector<string> attrNames, vector<string> keys){
		Table newTable(attrNames, keys);
		allTables[tableName] = newTable;
	}

	void dropTable(string tableName){
		allTables.erase(tableName);
	}

	//make sure that there cannot be duplicate entities
	void insertIntoTable(string tableName, vector<Datum> newRow){
		if (!allTables[tableName].duplicateExists(newRow))
			allTables[tableName].data.push_back(newRow);
		else
			cout << "There is already an entry in the table with that key\n";
	}

	void deleteFromTable(string tableName, ConditionNode condition)
	{
		vector<vector<Datum> >::iterator it;
		it = allTables[tableName].data.end();
		while (it != allTables[tableName].data.begin())
		{
			it--;
			if (condition.eval(allTables[tableName].attributeNames, *it))
			{
				allTables[tableName].data.erase(it);
			}
		}
	}

	//make sure that there cannot be duplicate entities
	void updateTable(string tableName, vector<string> attributeName, vector<Datum> newValue, vector<int> keyIndeces){
		vector<Datum> dupRow;   //to check for conflicts
		vector<int> attrIndeces;
		//get the indeces of the attributes to update
		for (int i = 0; i<attributeName.size(); i++){
			for (int j = 0; j<allTables[tableName].attributeNames.size(); j++){
				if (allTables[tableName].attributeNames[j] == attributeName[i])
					attrIndeces.push_back(j);
			}
		}

		//update at the above indeces
		for (int i = 0; i<keyIndeces.size(); i++){
			for (int j = 0; j<attrIndeces.size(); j++){
				dupRow = allTables[tableName].data[keyIndeces[i]];
				dupRow[attrIndeces[j]] = newValue[j];
				if (!allTables[tableName].duplicateExists(dupRow))
					allTables[tableName].data[keyIndeces[i]][attrIndeces[j]] = newValue[j];
				else
					cout << "That update would cause a conflict of primary keys\n";
			}
		}
	}

	Table selectFromTable(string tableName, vector<int> keyIndeces){
		Table retTable(allTables[tableName].attributeNames, allTables[tableName].keyNames);

		for (int i = 0; i<keyIndeces.size(); i++){
			vector<Datum> tempRow;
			tempRow = allTables[tableName].data[keyIndeces[i]];
			retTable.data.push_back(tempRow);
		}
		return retTable;
	}

	Table projectFromTable(string tableName, vector<string> attributeNames){
		Table wholeTable = allTables[tableName];
		Table projectionTable(attributeNames, allTables[tableName].keyNames);

		//get indices in wholeTable of matching attributes
		vector<int> attIndices;
		for (int i = 0; i < attributeNames.size(); ++i){
			for (int j = 0; j < wholeTable.data[0].size(); ++j){
				if (attributeNames[i] == wholeTable.attributeNames[j]){
					attIndices.push_back(j);
					break;
				}
			}
		}

		for (int row = 0; row < wholeTable.data.size(); ++row){
			vector<Datum> newRow;
			for (int column = 0; column < attIndices.size(); ++column){
				Datum d(wholeTable.data[row][attIndices[column]]);
				newRow.push_back(d);
			}
			projectionTable.data.push_back(newRow);
		}
		return projectionTable;
	}

	Table renameAttributes(string tableName, vector<string> attributeNames){
		Table newNamesTable = allTables[tableName];
		newNamesTable.attributeNames = attributeNames;
		return newNamesTable;
	}

	//just see if two tables are union compatible(for setUnion and setDifference)
	bool unionCompatible(string tableName1, string tableName2){
		vector<string> atts1 = allTables[tableName1].attributeNames;
		vector<string> atts2 = allTables[tableName2].attributeNames;

		//if different size then not union compatible
		if (atts1.size() != atts2.size()){
			return false;
		}
		for (int i = 0; i < atts1.size(); ++i){
			//if different names then not union compatible
			if (atts1[i] != atts2[i]){
				return false;
			}
		}
		return true;
	}

	Table setUnion(string tableName1, string tableName2){
		Table unionTable(allTables[tableName1].attributeNames, allTables[tableName1].keyNames);
		if (!unionCompatible(tableName1, tableName2)){
			cout << "Unable to perform set union on " << tableName1 << " and " << tableName2 << "." << endl;
			return unionTable;	//empty table
		}

		//temp tables to make getting rows easier
		Table t1 = allTables[tableName1];
		Table t2 = allTables[tableName2];

		//inserting ALL rows from both tables into unionTable
		for (int i = 0; i < t1.data.size(); ++i){
			unionTable.data.push_back(t1.data[i]);
		}
		for (int i = 0; i < t2.data.size(); ++i){
			unionTable.data.push_back(t2.data[i]);
		}

		//now remove duplicates from unionTable
		//efficiency is not a concern in our system..
		for (int row = 0; row < unionTable.data.size(); ++row){
			//find duplicate rows
			for (int dupRow = 0; dupRow < unionTable.data.size(); ++dupRow){
				bool isDuplicate = true;
				//compare individual rows
				for (int col = 0; col < unionTable.data[0].size(); ++col){
					if (unionTable.data[row][col] != unionTable.data[dupRow][col]){
						isDuplicate = false;
						break;
					}
				}
				if (isDuplicate){
					unionTable.data.erase(unionTable.data.begin() + dupRow);
					//since vector resizes
					--dupRow;
				}
			}
		}
		return unionTable;
	}

	//compute minuend - subtrahend = diference
	Table setDifference(string tNameMinuend, string tNameSubtrahend){
		//difference is the minuend at beginning
		Table difference = allTables[tNameMinuend];
		Table subtrahend = allTables[tNameSubtrahend];

		if (!unionCompatible(tNameSubtrahend, tNameSubtrahend)){
			cout << "Unable to perform set difference on " << tNameMinuend << " and " << tNameSubtrahend << "." << endl;
			return difference;	//empty table
		}


		//run through all elements to subtract
		for (int sub = 0; sub < subtrahend.data.size(); ++sub){
			//run through all elements to subtract from
			for (int dif = 0; dif < difference.data.size(); ++dif){
				bool isEqual = true;
				//compare rows and delete from difference if equal
				for (int col = 0; col < difference.data[0].size(); ++col){
					if (difference.data[dif][col] != subtrahend.data[sub][col]){
						isEqual = false;
						break;
					}
				}
				if (isEqual){
					difference.data.erase(difference.data.begin() + dif);
					//since we assume no duplicates can be in table
					break;
				}
			}
		}

		return difference;
	}

	Table crossProduct(string tableName1, string tableName2){
		//temp tables to make getting rows easier
		Table t1 = allTables[tableName1];
		Table t2 = allTables[tableName2];

		//need to combine t1 attributes with t2 attributes..
		vector<string> allAttributes = t1.attributeNames;
		for (int i = 0; i < t2.attributeNames.size(); ++i){
			allAttributes.push_back(t2.attributeNames[i]);
		}
		Table productTable(allAttributes, allTables[tableName1].keyNames);

		//compute cross product
		for (int firstT = 0; firstT < t1.data.size(); ++firstT){
			for (int secondT = 0; secondT < t2.data.size(); ++secondT){
				//create newRow by combining t1 and t2 rows
				vector<Datum> newRow = t1.data[firstT];
				for (int col = 0; col < t2.data.size(); ++col){
					newRow.push_back(t2.data[secondT][col]);

				}
				productTable.data.push_back(newRow);
			}
		}

		return productTable;
	}

};







VariableNode::VariableNode(string vn) : varName(vn){}
Datum VariableNode::getValue(vector<string> &atribNames, vector<Datum> &relation)
{
	for (int i = 0; i < atribNames.size(); i++)
	{
		if (atribNames[i] == varName)
		{
			return relation[i];
		}
	}
}


LiteralNode::LiteralNode(string d) : data(d){}
LiteralNode::LiteralNode(int d) : data(d){}
Datum LiteralNode::getValue(vector<string> &atribNames, vector<Datum> &relation)
{
	return data;
}

OperationNode::OperationNode(OP s, LeafNode* l, LeafNode* r) : symbol(s), left(l), right(r) {}
bool OperationNode::eval(vector<string> &atribNames, vector<Datum> &relation)
{
	if (left->getValue(atribNames, relation).numData != -999 && right->getValue(atribNames, relation).numData != -999)
	{
		switch (symbol)
		{
		case eq:
			return left->getValue(atribNames, relation).numData == right->getValue(atribNames, relation).numData; break;
		case neq:
			return left->getValue(atribNames, relation).numData != right->getValue(atribNames, relation).numData; break;
		case ls:
			return left->getValue(atribNames, relation).numData < right->getValue(atribNames, relation).numData; break;
		case leq:
			return left->getValue(atribNames, relation).numData <= right->getValue(atribNames, relation).numData; break;
		case gr:
			return left->getValue(atribNames, relation).numData > right->getValue(atribNames, relation).numData; break;
		case geq:
			return left->getValue(atribNames, relation).numData >= right->getValue(atribNames, relation).numData;
		}
	}
	else if (left->getValue(atribNames, relation).stringData != "" && right->getValue(atribNames, relation).stringData != "")
	{
		switch (symbol)
		{
		case eq:
			return left->getValue(atribNames, relation).stringData == right->getValue(atribNames, relation).stringData; break;
		case neq:
			return left->getValue(atribNames, relation).stringData != right->getValue(atribNames, relation).stringData; break;
		}
	}
	else{
		return false;
	}
}



ComparisonNode::ComparisonNode(OperationNode* n) : operOpOperChild(n){}
ComparisonNode::ComparisonNode(ConditionNode* n) : conditionChild(n){}
bool ComparisonNode::eval(vector<string> &atribNames, vector<Datum> &relation){
	if (operOpOperChild != NULL)
	{
		return operOpOperChild->eval(atribNames, relation);
	}
	else if (conditionChild != NULL)
	{
		return conditionChild->eval(atribNames, relation);
	}
	else{
		return false;
	}
}

ConjunctionNode::ConjunctionNode(vector<ComparisonNode*> cn) : compNodes(cn) {}
bool ConjunctionNode::eval(vector<string> &atribNames, vector<Datum> &relation){
	bool result = true;
	for (unsigned int i = 0; i < compNodes.size(); i++)
	{
		result = result && compNodes[i]->eval(atribNames, relation);
	}
	return result;
}


ConditionNode::ConditionNode(vector<ConjunctionNode*> cn) : conjNodes(cn) {}
bool ConditionNode::eval(vector<string> &atribNames, vector<Datum> &relation){
	bool result = false;
	for (unsigned int i = 0; i < conjNodes.size(); i++)
	{
		result = result || conjNodes[i]->eval(atribNames, relation);
	}
	return result;
}



int main(){

	Database db;
	vector<string> attr;
	attr.push_back("name");
	attr.push_back("age");
	vector<string> keyNames;
	keyNames.push_back("name");
	keyNames.push_back("age");
	db.createTable("artists", attr, keyNames);
	vector<Datum> pablo;
	Datum pabloname("Picasso");
	Datum pabloage(100);
	pablo.push_back(pabloname);
	pablo.push_back(pabloage);
	db.insertIntoTable("artists", pablo);

	vector<Datum> banksy;
	Datum banksyname("Banksy");
	Datum banksyage(40);
	banksy.push_back(banksyname);
	banksy.push_back(banksyage);
	db.insertIntoTable("artists", banksy);

	db.insertIntoTable("artists", banksy);   //insert conflict test 

	vector<Datum> banksy2;
	Datum banksyname2("Banksy");
	Datum banksyage2(41);
	banksy2.push_back(banksyname2);
	banksy2.push_back(banksyage2);
	db.insertIntoTable("artists", banksy2);

	vector<Datum> newVal;
	Datum newDat(40);
	newVal.push_back(newDat);
	vector<int> newInd;
	int newRowInd(1);
	newInd.push_back(newRowInd);
	vector<string> newAttr;
	string newAttrName = "age";
	newAttr.push_back(newAttrName);
	db.updateTable("artists", newAttr, newVal, newInd); //update conflict test

	vector<int> zvector;
	zvector.push_back(0);
	Table dummyTable = db.selectFromTable("artists", zvector);
	//dummyTable.printTable();

	vector<string> attN;
	attN.push_back("age");
	vector<Datum> newV;
	Datum newAge(30);
	newV.push_back(newAge);

	db.updateTable("artists", attN, newV, zvector);
	dummyTable = db.selectFromTable("artists", zvector);
	//dummyTable.printTable();

	LiteralNode* litN = new LiteralNode("Picasso");
	VariableNode* varN = new VariableNode("name");
	OperationNode* opN = new OperationNode(OperationNode::eq, litN, varN);
	ComparisonNode* compN = new ComparisonNode(opN);
	vector<ComparisonNode*> temp;
	temp.push_back(compN);
	ConjunctionNode* conjN = new ConjunctionNode(temp);
	vector<ConjunctionNode*> temp2;
	ConditionNode* condition = new ConditionNode(temp2);

	vector<string> attr2;
	attr2.push_back("name");
	db.allTables["artists"].printTable();

	cout << "eval result: " << condition->eval(attr2, db.allTables["artists"].data[0]);

	system("pause");
}


