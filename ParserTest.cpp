#include "stdafx.h"
#include <iostream>
#include "parser.h"
#include "rdbms.h"
#include "conditionTree.h"
#include "lexer.h"
#include "gtest/gtest.h"
#include <string.h>
#include <vector>
#include <map>

Database dbtest;
vector<string> attributes;
vector<string> keys;
vector<Datum> datums;
vector<Token> tokens;

Parser pars(&dbtest);

//write relation
TEST(ParserAttList, ParAttList)
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

	pars.command(tokens);

}


/*
TEST(ParserconTest, ParCondition)
{

}

TEST(ParserconjTest, ParConjunction)
{

}

TEST(ParsercompTest, ParComparison)
{

}

TEST(ParserexpTest, ParExpression)
{

}

TEST(ParseratexpTest, ParAtomExpression)
{

}

TEST(ParserselTest, ParSelection)
{

}

TEST(ParserprojTest, ParProjection)
{

}

TEST(ParserRenTest, ParRenaming)
{

}

TEST(ParserMyUnionTest, ParmyUnion)
{

}

TEST(ParserDiffTest, ParDifference)
{

}

TEST(ParserProductTest, ParProduct)
{

}

TEST(ParserNatJoinTest, ParNaturalJoin)
{

}

TEST(ParserQueryTest, ParQuery)
{

}

TEST(ParserOpenTest, ParOpen)
{

}

TEST(ParserCloseTest, ParClose)
{

}

TEST(ParserWriteTest, ParWrite)
{

}

TEST(ParserShowTest, ParShow)
{

}

TEST(ParserCreateTest, ParCreate)
{

}

TEST(ParserUpdateTest, ParUpdate)
{

}

TEST(ParserInsertTest, ParInsert)
{

}

TEST(ParserDeleteTest, ParmyDelete)
{

}
*/

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke
}


/*
int main()
{
//tests for file I/O
Database db;
vector<string> attNames;
attNames.push_back("artist");
attNames.push_back("year");
attNames.push_back("work");
attNames.push_back("birth");
vector<string> keys;
keys.push_back("artist");
keys.push_back("year");
db.createTable("RyanTestTable", attNames, keys);
vector<Datum> datums;
Datum famousGuy("famous guy");
Datum year(1999);
Datum artisticThingie("artisticThingie");
Datum date("10/10/1010");
datums.push_back(famousGuy);
datums.push_back(year);
datums.push_back(artisticThingie);
datums.push_back(date);
db.insertIntoTable("RyanTestTable", datums);
Parser p(&db);
p.closeRelationFile("RyanTestTable");

getchar();

}
*/