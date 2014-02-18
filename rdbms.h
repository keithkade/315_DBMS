#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

//conditional queries/commands are stored in a tree whose root is a ConditionNode
class ConditionNode;

//each cell in our relations contain one datum object
struct Datum{
	std::string stringData; //if the attribute is a varchar then the info is put into stringData
	int numData;			//if the attribute is a integer then the info is put into numData

	//constructors
	Datum();
	Datum(int n);
	Datum(std::string s);

	//needed to compare rows and remove duplicates
	bool operator!=(const Datum &d);
	bool operator==(const Datum &d);
};


//each relation in the database is one Table object
struct Table{

	std::vector<std::string> keyNames;			//contains all the attributes used as the primary key
	std::vector<std::string> attributeNames;	//contains the names of all of our attributes
	std::vector<std::vector<Datum> > data;		//the relation data

	//constructors
	Table(std::vector<std::string> attrNames, std::vector<std::string> keys);
	Table(); // Blank tables will be input failures.
	
	//needed to compare if Table is a bad table. 
	bool operator==(const Table &t);

	/***relational algebra operations***/

	//return a subset of the attributes in the relation
	Table projectFromTable(const std::vector<std::string>& projectedNames);

	//return a new table with same data but different attribute names
	Table renameAttributes(const std::vector<std::string>& renamedNames);

	//just see if two tables are union compatible(for setUnion and setDifference)
	bool unionCompatibleWith(const Table& paramTable);

	//return the union of two relations
	Table unionWith(const Table& unionTable);

	//compute minuend - subtrahend = diference
	Table differenceWith(const Table& subtrahendTable);

	//return the cross product of two relations
	Table productWith(const Table& paramTable);

	//joins on the common attribute names of two tables and returns a new table
	Table naturalJoinWith(const Table& paramTable);

	//return a new relation that contains all rows that satisfy the condition
	Table selectFromTable(Table selectTable, ConditionNode condition);

	//returns true if there is already a row with the the same primary key as the row argument
	bool duplicateExists(const std::vector<Datum>& newRow);

	//for testing and debugging
	void printTable();
};


//the entire database is an object of this class
class Database{
	std::map<std::string, Table> allTables;		//all the relations in our database

public:

	Database();

	//create new relation in the database
	void createTable(const std::string& tableName, const std::vector<std::string>& attrNames, const std::vector<std::string>& keys);

	//remove a table from the database
	void dropTable(const std::string& tableName);

	//make sure that there cannot be duplicate entities
	void insertIntoTable(const std::string& tableName, const std::vector<Datum>& newRow);

	//remove a row from the table that satisfies the condition tree passed
	void deleteFromTable(const std::string& tableName, ConditionNode condition);

	//changes the attributs in the table in the rows that satisfy the condition
	void updateTable(const std::string& tableName, const std::vector<std::string>& attributeName, const std::vector<Datum>& newValue, ConditionNode condition);

	//return a new relation that contains all rows that satisfy the condition
	Table selectFromTable(const std::string& tableName, ConditionNode condition);

	//return a subset of the attributes in the relation
	Table projectFromTable(const std::string& tableName, const std::vector<std::string>& projectedNames);

	//return a new table with the attributes names changed 
	Table renameAttributes(const std::string& tableName, const std::vector<std::string>& renamedNames);

	//return the union of two relations
	Table setUnion(const std::string& tableName1, const std::string& tableName2);

	//compute minuend - subtrahend = diference
	Table setDifference(const std::string& tNameMinuend, const std::string& tNameSubtrahend);

	//return the cross product of two relations
	Table crossProduct(const std::string& tableName1, const std::string& tableName2);

	//joins on the common attribute names of two tables and returns a new table
	Table naturalJoin(const std::string& tableName1, const std::string& tableName2);

	Table getTable(const std::string& tableName);
};
