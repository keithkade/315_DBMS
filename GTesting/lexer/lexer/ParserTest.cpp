#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "parser.h"
#include "rdbms.h"
#include "conditionTree.h"
#include "lexer.h"
#include "gtest/gtest.h"
#include <string.h>
#include <vector>
#include <map>

Database dbtest;
Database d;
vector<string> attributes;
vector<string> attributes2;
vector<string> keys;
vector<string> keys2;
vector<Datum> datums;
vector<Datum> datums2;
vector<Token> tokens;
int i = 0;

Parser pars(&dbtest);
Table testTable2;


//write relation
TEST(ParserTest, ParAttList)
{

	//CREATE TABLE species (kind VARCHAR(10)) PRIMARY KEY (kind);
	Token temp(Token::KEYWORD);
	temp.content = "CREATE";
	tokens.push_back(temp);

	Token temp1(Token::KEYWORD);
	temp.content = "TABLE";
	tokens.push_back(temp1);

	Token temp2(Token::VARIABLE);
	temp.content = "species";
	tokens.push_back(temp2);

	Token temp3(Token::OPENPAREN);
	temp.content = "(";
	tokens.push_back(temp3);

	Token temp4(Token::VARIABLE);
	temp.content = "kind";
	tokens.push_back(temp4);

	Token temp5(Token::KEYWORD);
	temp.content = "VARCHAR";
	tokens.push_back(temp5);

	Token temp6(Token::OPENPAREN);
	temp.content = "(";
	tokens.push_back(temp6);

	Token temp7(Token::LITERAL);
	temp.content = "10";
	tokens.push_back(temp7);

	Token temp8(Token::CLOSEPAREN);
	temp.content = ")";
	tokens.push_back(temp8);

	Token temp9(Token::CLOSEPAREN);
	temp.content = ")";
	tokens.push_back(temp9);

	Token temp10(Token::KEYWORD);
	temp.content = "PRIMARY";
	tokens.push_back(temp10);

	Token temp11(Token::KEYWORD);
	temp.content = "KEY";
	tokens.push_back(temp11);

	Token temp12(Token::OPENPAREN);
	temp.content = "(";
	tokens.push_back(temp12);

	Token temp13(Token::VARIABLE);
	temp.content = "kind";
	tokens.push_back(temp13);

	Token temp14(Token::CLOSEPAREN);
	temp.content = ")";
	tokens.push_back(temp14);

	ASSERT_NO_THROW(pars.command(tokens));
}

TEST(ParsTest, ParCloseRelationFile)
{
	//clearing vectors
	attributes.clear();
	keys.clear();
	datums.clear();

	//creating new values
	attributes.push_back("artist");
	attributes.push_back("year");
	attributes.push_back("work");
	attributes.push_back("birth");
	keys.push_back("artist");
	keys.push_back("year");

	dbtest.createTable("RyanTestTable", attributes, keys);
	Datum famousGuy("famous guy");
	Datum year(1999);
	Datum artisticThingie("artisticThingie");
	Datum date("10/10/1010");
	datums.push_back(famousGuy);
	datums.push_back(year);
	datums.push_back(artisticThingie);
	datums.push_back(date);
	dbtest.insertIntoTable("RyanTestTable", datums);

	 
	ASSERT_NO_THROW(pars.closeRelationFile("RyanTestTable"));

}

TEST(ParsTest, ParGetTempTable)
{
	//clearing vectors
	attributes.clear();
	keys.clear();
	datums.clear();

	//creating new values
	attributes.push_back("artist");
	attributes.push_back("year");
	attributes.push_back("work");
	attributes.push_back("birth");
	keys.push_back("artist");
	keys.push_back("year");

	dbtest.createTable("RyanTestTable", attributes, keys);
	Datum famousGuy("famous guy");
	Datum year(1999);
	Datum artisticThingie("artisticThingie");
	Datum date("10/10/1010");
	datums.push_back(famousGuy);
	datums.push_back(year);
	datums.push_back(artisticThingie);
	datums.push_back(date);
	dbtest.insertIntoTable("RyanTestTable", datums);
	

	ASSERT_NO_THROW(pars.getTempTable("RyanTestTable"));
}

TEST(ParsTest, ParOpenRelationFile)
{
	//clearing vectors
	attributes.clear();
	keys.clear();
	datums.clear();

	//creating new values
	attributes.push_back("artist");
	attributes.push_back("year");
	attributes.push_back("work");
	attributes.push_back("birth");

	//attributes 2
	attributes.push_back("artist");
	attributes.push_back("year");
	attributes.push_back("museum");

	keys.push_back("artist");
	keys.push_back("year");
	keys2.push_back("artist");
	keys2.push_back("year");


	dbtest.createTable("RyanTestTable", attributes, keys);
	dbtest.createTable("TestTable", attributes2, keys2);
	//Datums for first table
	Datum famousGuy("famous guy");
	Datum year(1999);
	Datum artisticThingie("artisticThingie");
	Datum date("10/10/1010");
	datums.push_back(famousGuy);
	datums.push_back(year);
	datums.push_back(artisticThingie);
	datums.push_back(date);
	dbtest.insertIntoTable("RyanTestTable", datums);

	//Datums for second table
	Datum famousGuy2("Picasso");
	Datum year2(1800);
	Datum museum("Louvre");
	datums2.push_back(famousGuy);
	datums2.push_back(year);
	datums2.push_back(museum);
	dbtest.insertIntoTable("TestTable", datums2);
	
	testTable2 = dbtest.naturalJoin("RyanTestTable", "TestTable");
	ASSERT_NO_THROW(pars.openRelationFile("testTable2"));
}

TEST(ParsTest, ParSetDatabasePtr)
{
	ASSERT_NO_THROW(pars.setDatabasePtr(&dbtest));
}

TEST(ParsTest, ParWriteRelationFile)
{
	//creating new values
	attributes.push_back("artist");
	attributes.push_back("year");
	attributes.push_back("work");
	attributes.push_back("birth");
	dbtest.createTable("RyanTestTable", attributes, keys);
	//Datums for first table
	Datum famousGuy("famous guy");
	Datum year(1999);
	Datum artisticThingie("artisticThingie");
	Datum date("10/10/1010");
	datums.push_back(famousGuy);
	datums.push_back(year);
	datums.push_back(artisticThingie);
	datums.push_back(date);
	dbtest.insertIntoTable("RyanTestTable", datums);

	pars.writeRelationToFile("RyanTestTable");
	ASSERT_NO_THROW(dbtest.getTable("RyanTestTable"));
}


int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke
}