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
	Database* rdbms; // Copy of the current database being worked on.
	map<string, Table> tempTables; // Tables only kept while DML is runing. Deleted on finish.

	//Returns a vector of strings that are the attribute names.
	vector<string> attributeList(vector<Token>&);
	vector<string> typedAttributeList(vector<Token>&);

	// Will parse input into a condition tree.
	ConditionNode* condition(vector<Token>&);
	// Called from  condition to make a condition tree.
	ConjunctionNode* conjunction(vector<Token>&);
	// Called from within comparision as subproblem in making condition tree.
	ComparisonNode* comparison(vector<Token>&);
	
	// Distinquishes between atomic-expr, selection, projection, renaming, union, difference, product, natural-join
	// and runs the appropriate function
	Table expression(vector<Token>&);
	// Returns either the table with the specified name, or runs expression on what is in parenthesis
	Table atomExpression(vector<Token>&);
	// Returns the relations from the table in the atomicExpression that satisfy the condition
	Table selection(vector<Token>&);
	// Returns a table with the attributes listed in the attribute-list
	Table projection(vector<Token>&);
	// Returns a table with the attributes renamed with the attribute-list entred
	Table renaming(vector<Token>&);
	// Returns the union of the two tables from the atomExpressions seperated by a +
	Table myUnion(vector<Token>&);
	// Returns the difference of the two tables from the atomExpressions seperated by a -
	Table difference(vector<Token>&);
	// Returns the product of the two tables from the atomExpressions seperated by a *
	Table product(vector<Token>&);
	// Returns the naturalJoin of the two tables from the atomExpressions seperated by a JOIN
	Table naturalJoin(vector<Token>&);

	// Puts the results of a query into a tempTable map 
	void query(vector<Token>&);
	// Calls the Database*'s open in order to open a file and make the corresponding table. 
	void open(vector<Token>&);
	// Calls the Database*'s close in order to save the changes to a databse file.
	void close(vector<Token>&);
	// Calls the Database*'s write in order to create a new database file for a table.
	void write(vector<Token>&);
	// Calls the Database*'s show to print out a table.
	void show(vector<Token>&);
	// Calls the Database*'s create to add a table to the database.
	void create(vector<Token>&);
	// Calls the Database*'s update to change the information in a table based off a condition.
	void update(vector<Token>&);
	// Calls the Database*'s insert to put values from a list of literals or from an expression into a table.
	void insert(vector<Token>&);
	// Calls the Database*'s delete in order to remove attributes from a table.
	void myDelete(vector<Token>&);

public:
	//Constructors.
	Parser();
	Parser(Database*); // Entered database that is called in the commands
	
	// Changes the database you are working on.
	void setDatabasePtr(Database* dbPtr);

	// Called on any line of code that follows the language set by our grammar.
	// Querys are treated as seperate command.
	void command(vector<Token>&);
};