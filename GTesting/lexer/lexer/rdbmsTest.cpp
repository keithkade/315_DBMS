#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <vector>
#include "rdbms.h"
#include "conditionTree.h"
#include "lexer.h"
#include "gtest/gtest.h"

Database db;
Datum testdata();
Datum dat(10);
Datum dat2(20);
Datum datstring2("hello");
Datum datstring("hello");
std::vector<string> attr;
std::vector<string> attr2;
std::vector<string> key;
vector<Datum>datums3;
vector<Datum>datums4;
string test;

TEST(Datcreate, DatIntCreateTest)
{
	int num = 10;
	ASSERT_EQ(num, dat.numData);
}

TEST(Datcreate, DatStringCreateTest)
{
	std::string s = "hello";
	ASSERT_EQ(s, datstring.stringData);
}

TEST(Datcreate, DatEqualTest)
{
	EXPECT_TRUE(datstring2.stringData == datstring.stringData);
}

TEST(Datcreate, DatNotEqualTest)
{
	EXPECT_TRUE(dat != dat2);
}

//Database tests
TEST(DBTest, DBCreateTable)
{
	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");
	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);
	test = db.getTable("TestTable").attributeNames.at(0);
	ASSERT_EQ("artist", test);
}

TEST(DBTest, DBDRopTable)
{
	//clearing vectors
	attr.clear();
	key.clear();

	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");
	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);
	db.dropTable("TestTable");
	EXPECT_NO_FATAL_FAILURE(db.getTable("TestTable"));
}

TEST(DBTest, DBInstertIntoTable)
{
	//clearing vectors
	attr.clear();
	key.clear();
	
	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");
	
	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);

	Datum famous1("Picasso");
	Datum famous2("DaVinci");
	Datum year(1999);
	Datum year2(2000);
	Datum work("Cubism");
	Datum work2("Mona Lisa");
	Datum date("10/10/1010");
	Datum date2("02/08/1989");

	datums3.push_back(famous1);
	datums3.push_back(year);
	datums3.push_back(work);
	datums3.push_back(date);
	//first insert
	db.insertIntoTable("TestTable", datums3);
	//second insert
	datums4.push_back(famous2);
	datums4.push_back(year2);
	datums4.push_back(work2);
	datums4.push_back(date2);
	ASSERT_NO_THROW(db.insertIntoTable("TestTable", datums4));
}

TEST(DBTest, DBDeleteFromTable)
{
	//clearing vectors
	attr.clear();
	key.clear();
	datums3.clear();
	datums4.clear();
	
	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");

	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);

	Datum famous1("Picasso");
	Datum famous2("DaVinci");
	Datum year(1999);
	Datum year2(2000);
	Datum work("Cubism");
	Datum work2("Mona Lisa");
	Datum date("10/10/1010");
	Datum date2("02/08/1989");

	datums3.push_back(famous1);
	datums3.push_back(year);
	datums3.push_back(work);
	datums3.push_back(date);
	//first insert
	db.insertIntoTable("TestTable", datums3);
	//second insert
	datums4.push_back(famous2);
	datums4.push_back(year2);
	datums4.push_back(work2);
	datums4.push_back(date2);
	db.insertIntoTable("TestTable", datums4);

	//delete from table
	//db.deleteFromTable("TestTable", NULL);
}

TEST(DBTest, DBUpdateTable)
{
	//clearing vectors
	attr.clear();
	key.clear();
	datums3.clear();
	
	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");

	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);

	Datum famous1("Picasso");
	Datum year(1999);;
	Datum work("Cubism");
	Datum date("10/10/1010");

	datums3.push_back(famous1);
	datums3.push_back(year);
	datums3.push_back(work);
	datums3.push_back(date);
	//first insert
	db.insertIntoTable("TestTable", datums3);

	attr.push_back("city");
	Datum city("Paris");
	datums3.push_back(city);
	
	//db.updateTable("TestTable", );
}


TEST(DBTest, DBSelectFromTable)
{
	//clearing vectors
	attr.clear();
	key.clear();
	datums3.clear();
	
	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");

	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);

	Datum famous1("Picasso");
	Datum year(1999);;
	Datum work("Cubism");
	Datum date("10/10/1010");

	datums3.push_back(famous1);
	datums3.push_back(year);
	datums3.push_back(work);
	datums3.push_back(date);
	//first insert
	db.insertIntoTable("TestTable", datums3);
	
	//db.selectFromTable("TestTable", );
}

TEST(DBTest, DBProjectFromTable)
{
	//clearing vectors
	attr.clear();
	key.clear();
	datums3.clear();
	datums4.clear();
	
	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");

	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);
	
	attr.push_back("city");
	db.createTable("TestTable2", attr, key);

	Datum famous1("Picasso");
	Datum famous2("DaVinci");

	Datum year(1999);
	Datum year2(1899);

	Datum work("Cubism");
	Datum work2("Mona Lisa");

	Datum date("10/10/1010");
	Datum date2("02/08/1989");

	Datum city("Paris");

	datums3.push_back(famous1);
	datums3.push_back(year);
	datums3.push_back(work);
	datums3.push_back(date);

	datums4.push_back(famous2);
	datums4.push_back(year2);
	datums4.push_back(work2);
	datums4.push_back(date2);
	datums4.push_back(city);
	//first insert
	db.insertIntoTable("TestTable", datums3);
	db.insertIntoTable("TestTable2", datums4);

	//db.selectFromTable("TestTable", )
}

TEST(DBTest, DBRenameAttribute)
{
	//clearing vectors
	attr.clear();
	key.clear();
	datums3.clear();
	datums4.clear();
	db.dropTable("TestTable");
	
	attr.push_back("artist");
	attr.push_back("year");
	attr.push_back("work");
	attr.push_back("birth");

	key.push_back("artist");
	key.push_back("year");
	db.createTable("TestTable", attr, key);

	Datum famous1("Picasso");
	Datum year(1999);;
	Datum work("Cubism");
	Datum date("10/10/1010");

	datums3.push_back(famous1);
	datums3.push_back(year);
	datums3.push_back(work);
	datums3.push_back(date);
	//first insert
	db.insertIntoTable("TestTable", datums3);

	//renamed attr
	attr2.push_back("I");
	attr2.push_back("Suck");
	attr2.push_back("at");
	attr2.push_back("this");

	db.renameAttributes("TestTable", attr2);
	ASSERT_EQ("artist", db.getTable("TestTable").attributeNames[0]);
}


/*
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke
}
*/