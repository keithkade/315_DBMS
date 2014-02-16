#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iterator>

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

//each relation in the table is one Table object
struct Table{

	std::vector<std::string> keyNames;			//contains all the attributes used as the primary key
	std::vector<std::string> attributeNames;	//contains the names of all of our attributes
	std::vector<std::vector<Datum> > data;		//the relation itself

	//constructors
	Table(std::vector<std::string> attrNames, std::vector<std::string> keys);
	Table();
	
	//returns true if there is already a row with the the same primary key as the row argument
	bool duplicateExists(std::vector<Datum> newRow);

	//for testing and debugging
	void printTable();
};

//the entire database is an object of this class
class Database{
	std::map<std::string, Table> allTables;		//all the relations in our database

public:

	Database();

	//create new relation in the database
	void createTable(std::string tableName, std::vector<std::string> attrNames, std::vector<std::string> keys);

	//remove a table from the database
	void dropTable(std::string tableName);

	//make sure that there cannot be duplicate entities
	void insertIntoTable(std::string tableName, std::vector<Datum> newRow);

	//remove a row from the table that satisfies the condition tree passed
	void deleteFromTable(std::string tableName, ConditionNode condition);

	//changes the attributs in the table in the rows that satisfy the condition
	void updateTable(std::string tableName, std::vector<std::string> attributeName, std::vector<Datum> newValue, ConditionNode condition);

	//return a new relation that contains all rows that satisfy the condition
	Table selectFromTable(std::string tableName, ConditionNode condition);

	//return a subset of the attributes in the relation
	Table projectFromTable(std::string tableName, std::vector<std::string> attributeNames);

	//returna new table with the attributes names changed 
	Table renameAttributes(std::string tableName, std::vector<std::string> attributeNames);

	//just see if two tables are union compatible(for setUnion and setDifference)
	bool unionCompatible(std::string tableName1, std::string tableName2);

	//return the union of two relations
	Table setUnion(std::string tableName1, std::string tableName2);

	//compute minuend - subtrahend = diference
	Table setDifference(std::string tNameMinuend, std::string tNameSubtrahend);

	//return the cross product of two relations
	Table crossProduct(std::string tableName1, std::string tableName2);
};
