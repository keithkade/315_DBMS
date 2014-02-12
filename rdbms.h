#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

class ConditionNode;

struct Datum{
	string stringData;
	int numData;

	// Datums with -999 in the int variable mean it is a stringData type
	Datum();
	Datum(int n);
	Datum(string s);

	//needed to compare rows and remove duplicates
	bool operator!=(const Datum &d);
	bool operator==(const Datum &d);
};

struct Table{

	vector<string> keyNames;
	vector<string> attributeNames;
	vector<vector<Datum> > data;

	//keys are used to check for conflicts. 
	Table(vector<string> attrNames, vector<string> keys);

	Table();
	bool duplicateExists(vector<Datum> newRow);

	//for testing and debugging
	void printTable();
};




class Database{

public:
	//MOVE BACK
	map<string, Table> allTables;

	Database();

	void createTable(string tableName, vector<string> attrNames, vector<string> keys);

	void dropTable(string tableName);

	//make sure that there cannot be duplicate entities
	void insertIntoTable(string tableName, vector<Datum> newRow);

	void deleteFromTable(string tableName, ConditionNode condition);

	//make sure that there cannot be duplicate entities
	void updateTable(string tableName, vector<string> attributeName, vector<Datum> newValue, ConditionNode condition);

	Table selectFromTable(string tableName, ConditionNode condition);

	Table projectFromTable(string tableName, vector<string> attributeNames);

	Table renameAttributes(string tableName, vector<string> attributeNames);

	//just see if two tables are union compatible(for setUnion and setDifference)
	bool unionCompatible(string tableName1, string tableName2);

	Table setUnion(string tableName1, string tableName2);

	//compute minuend - subtrahend = diference
	Table setDifference(string tNameMinuend, string tNameSubtrahend);

	Table crossProduct(string tableName1, string tableName2);
};
