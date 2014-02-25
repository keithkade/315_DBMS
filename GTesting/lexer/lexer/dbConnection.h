#pragma once

#include <string>


class Database;
class Parser;


//this class acts as a wrapper to the parser, the lexer, and the database
//this is what will be used by our application to interface a database
//this will also handle the DML command line interface
class DBConnection{
	Database db;
	Parser par;

public:

	DBConnection();

	//function to handle the command line interface for the DML
	void cmdLineInterface();

	//used to execute a command on the database from within a user application
	void executeCommand(std::string command);

	//get a table from the parser temp table vector
	Table getTempTable(const std::string& tableName);

};

