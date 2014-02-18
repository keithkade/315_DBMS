#pragma once


//remove anything that isn't needed
struct Datum;
struct Token;
struct Table;
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


};