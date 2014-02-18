#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Datum;
struct Token;
struct Table;
class Database;
class ConditionNode;
class ConjunctionNode;
class ComparisonNode;

class Parser{
	Database* rdbms;
	map<string, Table> tempTables;	// Tables only kept while DML is runing. Deleted on finish.
	// whenever inserting new temp table into map, store name here so it can be accessed after the command
	string lastInsertedTableName;

public:
	Parser();
	Parser(Database*);

	//set the parser's database pointer
	void setDatabasePtr(Database* dbPtr);

	//return one of the temp table stored within the parser
	//if tableName == "" then the last inserted table will be returned
	Table getTempTable(const std::string& tableName);

	//Returns a vector of strings that are the attribute names.
	//Returns an empty vector if failed.
	vector<string> attributeList(vector<Token>&);

	ConditionNode* condition(vector<Token>&);
	ConjunctionNode* conjunction(vector<Token>&);
	ComparisonNode* comparison(vector<Token>&);
	Table expression(vector<Token>&);
	Table atomExpression(vector<Token>&);
	Table selection(vector<Token>&);
	Table projection(vector<Token>&);
	Table renaming(vector<Token>&);
	Table myUnion(vector<Token>&);
	Table difference(vector<Token>&);
	Table product(vector<Token>&);
	Table naturalJoin(vector<Token>&);

	void query(vector<Token>&);
	void open(vector<Token>&);
	void close(vector<Token>&);
	void write(vector<Token>&);
	void show(vector<Token>&);
	void create(vector<Token>&);
	void update(vector<Token>&);
	void insert(vector<Token>&);
	void myDelete(vector<Token>&);
	void exit();
};