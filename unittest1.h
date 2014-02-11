#include "stdafx.h"
#include "CppUnitTest.h"

#include "rdbms.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace rdbmsTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		/////////////////////////////////DATABASE FUNCTIONS////////////////////////////////
		TEST_METHOD(RandomTest)
		{
			std::string hi = "Howdy!";
			std::string hello = "Howdy!";
			Assert::AreEqual(hi,hello);
		}
		TEST_METHOD(DatabaseConstructorTest)
		{
			Database database;
			// Assert::IsNotNull(database);
		}
		TEST_METHOD(testDatabaseCreateTable)
		{
			Database database;
			Table* t = new Table();
			Assert::IsTrue(database.createTable(t,"Cool Table"));
		}
		TEST_METHOD(testDatabaseDropTable)
		{
			Database database;
			Table* t = new Table();
			database.
			Assert::IsFalse(database.DropTable("This does not exist"));
		}
		TEST_METHOD(testDatabaseDropTablePass)
		{
			Database database;
			Table* t = new Table();
			database.AddTable(t,"Cool Table");
			Assert::IsTrue(database.DropTable("Cool Table"));
		}
		TEST_METHOD(testDuplicateExistsDatabaseAddTable)
		{
			Database database;
			Table* t = new Table();
			Table* x = new Table();
			database.createTable(t, "Original");
			Assert::IsFalse(database.createTable(x, "Original"));
		}
	
		TEST_METHOD(TestDropTableWithNoTables)
		{
			Database database;
			Assert::AreEqual((int)database.GetTableNames().size(),0);
		}
		TEST_METHOD(testPrintTable)
		{
			Database database;
			Table x;
			Table y;
			Table z;
			Table t;
			database.createTable(&x,"I");
			database.createTable(&y,"am");
			database.createTable(&x,"tired");
			database.createTable(&z,"of");
			database.createTable(&t,"tests");
			Assert::AreEqual(database.printTable().size(), (size_t)5);
		}
		TEST_METHOD(eraseafterDuplicate)
		{
			Database database;
			Table x;
			Table y;
			database.createTable(&x,"Hello");
			database.createTable(&y,"Hello");
			Assert::IsTrue(database.DropTable("Hello"));
		}

		/////////////////TABLE FUNCTIONS///////////////////////////////
		TEST_METHOD(TableDefaultConstructorTest)
		{
			Table table;
			Assert::IsNotNull(&table);
		}
		TEST_METHOD(TableDuplicateExists)
		{
			Table ts1;
			Table a("Name", 0);
			ts1.push_back(a);
   			Table t1(ts1);
   			Assert::IsTrue(t1 == ts1);
		}

		TEST_METHOD(TableGetAttributesTest)
		{
			Table t = *new Table;
			t.push_back(Table("greeting", 0));
			t.push_back(Table("moniker", 0));
			t.push_back(Table("group", 1));
			Table tab = *new Table(t);
			Assert::IsTrue(tab == t);
		}
		TEST_METHOD(UpdateTableRenameTest1)
		{
			Database db;
			Tablet t = *new Tablet;
			t.push_back(Table("greeting", 0));
			t.push_back(Table("moniker", 0));
			t.push_back(Table("group", EntryTypes::INT));
			Table tab = *new Table(t);

			Assert::IsTrue(db.updateTable("moniker","title"));
		}
		TEST_METHOD(TableRenameTest2)
		{
			Database db;
			Table t = *new Table;
			t.push_back(Table("greeting", 0));
			t.push_back(Table("moniker", 0));
			t.push_back(Table("group", 0));
			Table tab = *new Table(t);
			tab.Rename("moniker","title");
			Assert::IsTrue(tab[1].second == 0);
		}
		TEST_METHOD(TableRenameTest3)
		{
			Table t = *new Table;
			t.push_back(Table("greeting", 0));
			t.push_back(Table("moniker", 0));
			t.push_back(Table("group", 0));
			Table tab = *new Table(t);

			Assert::IsFalse(tab.updateTable("FavoriteDate","DateEntered"));
		}
		TEST_METHOD(TableupdateTableTest4)
		{
			Table t = *new Table;
			t.push_back(Table("greeting", 0));
			t.push_back(Table("moniker", 0));
			t.push_back(Table("group", 0));
			Table tab = *new Table(t);

			Assert::IsFalse(tab.updateTable("moniker","group"));
		}
		TEST_METHOD(TableupdateTableTest5)
		{
			Table t = *new Table;
			t.push_back(Table("greeting", 0));
			t.push_back(Table("moniker", 0));
			t.push_back(Table("group", 0));
			Table tab = *new Table(t);

			Assert::IsFalse(tab.updateTable("moniker","moniker"));
		}
		TEST_METHOD(TableUnionTest1)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");

			Database db;
			db.createTable("name", vec1);
			db.createTable("name", vec2);
			db.createTable("name", vec3);
			Table t = *new Table;
			t.push_back(Table("greeting", 0));
			t.push_back(Table("moniker", 0));
			t.push_back(Table("group", 0));
			Table tab = *new Table(t);

		}
		
			
		}

	};
}
