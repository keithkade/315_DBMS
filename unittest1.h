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
		TEST_METHOD(testDatabaseAddTable)
		{
			Database database;
			Table* t = new Table();
			Assert::IsTrue(database.AddTable(t,"Cool Table"));
		}
		TEST_METHOD(testDatabaseDropTableFail)
		{
			Database database;
			Assert::IsFalse(database.DropTable("This does not exist"));
		}
		TEST_METHOD(testDatabaseDropTablePass)
		{
			Database database;
			Table* t = new Table();
			database.AddTable(t,"Cool Table");
			Assert::IsTrue(database.DropTable("Cool Table"));
		}
		TEST_METHOD(testDuplicateNameDatabaseAddTable)
		{
			Database database;
			Table* t = new Table();
			Table* x = new Table();
			database.AddTable(t, "Original");
			Assert::IsFalse(database.AddTable(x, "Original"));
		}
		TEST_METHOD(testGetTables)
		{
			Database database;
			Table x;
			Table y;
			database.AddTable(&x, "Table");
			database.AddTable(&y, "Table2");
			Assert::AreEqual(database.GetTables()[0]->GetSize(),x.GetSize());
		}
		TEST_METHOD(testGetTableNamesWithNoTables)
		{
			Database database;
			Assert::AreEqual((int)database.GetTableNames().size(),0);
		}
		TEST_METHOD(testGetTableNames)
		{
			Database database;
			Table x;
			Table y;
			Table z;
			Table t;
			database.AddTable(&x,"I");
			database.AddTable(&y,"am");
			database.AddTable(&x,"tired");
			database.AddTable(&z,"of");
			database.AddTable(&t,"tests");
			Assert::AreEqual(database.GetTables().size(), (size_t)5);
		}
		TEST_METHOD(eraseafterDuplicate)
		{
			Database database;
			Table x;
			Table y;
			database.AddTable(&x,"Hello");
			database.AddTable(&y,"Hello");
			Assert::IsTrue(database.DropTable("Hello"));
		}
		TEST_METHOD(testGetTableNames2)
		{
			Database database;
			Table t;
			database.AddTable(&t,"Cool Table");
			Table x;
			database.AddTable(&x,"Not Cool Table");
			std::vector<std::string> vec = database.GetTableNames();
			std::string notCool = "Not Cool Table";
			Assert::AreEqual(vec[1],notCool);
		}

		/////////////////TABLE FUNCTIONS///////////////////////////////
		TEST_METHOD(TableDefaultConstructorTest)
		{
			Table table;
			Assert::IsNotNull(&table);
		}
		TEST_METHOD(TableConstructorTest2)
		{
			TableSchema ts1;
			TableAttribute a("Name", EntryTypes::STRING);
			ts1.push_back(a);
   			Table t1(ts1);
   			Assert::IsTrue(t1.GetAttributes() == ts1);
		}
		TEST_METHOD(RecordConstructorTest)
		{
			std::vector<std::string> vec;
			vec.push_back("Hello");
			vec.push_back("Team");
			vec.push_back("25");
			Record* r = new Record(vec);
			Assert::IsNotNull(r);
		}
		TEST_METHOD(TableAddTest1)
		{
			TableSchema schema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));

			Table* tab = new Table(schema);
			Assert::IsTrue(tab->Add("FavoriteDate", EntryTypes::DATE));
		}
		TEST_METHOD(TableAddTest2)
		{
			TableSchema schema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));

			Table* tab = new Table(schema);
			Assert::IsFalse(tab->Add("group", EntryTypes::INT));
		}
		TEST_METHOD(TableAddTest3)
		{
			TableSchema schema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));

			Table* tab = new Table(schema);
			tab->Add("FavoriteDate", EntryTypes::DATE);
			Assert::IsTrue(tab->GetAttributes().size() > 3);
		}
		TEST_METHOD(TableAddTest4)
		{
			TableSchema schema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));

			Table* tab = new Table(schema);
			tab->Add("group", EntryTypes::INT);
			Assert::IsTrue(tab->GetAttributes().size() == 3);
		}
		TEST_METHOD(TableAddTest5)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			tab.Rename("moniker","title");
			tab.Add("moniker", EntryTypes::STRING);
			Assert::AreEqual((int)tab.GetAttributes().size(),4);
		}
		TEST_METHOD(TableAddTest6)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			tab.Rename("moniker","title");
			tab.Add("title", EntryTypes::STRING);
			Assert::AreEqual((int)tab.GetAttributes().size(),3);
		}
		TEST_METHOD(TableDeleteTest1)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			Assert::IsTrue(tab.Delete("moniker"));
		}
		TEST_METHOD(TableDeleteTest2)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			Assert::IsFalse(tab.Delete("FavoriteDate"));
		}
		TEST_METHOD(TableDeleteTest3)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			tab.Delete("moniker");
			Assert::IsTrue(tab.GetAttributes().size() <3);
		}
		TEST_METHOD(TableDeleteTest4)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			tab.Delete("FavoriteDate");
			Assert::IsTrue(tab.GetAttributes().size() == 3);
		}
		TEST_METHOD(TableDeleteTest5)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			tab.Rename("moniker","title");
			Assert::IsTrue(tab.Delete("title"));
		}
		TEST_METHOD(TableDeleteTest6)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			tab.Rename("moniker","title");
			Assert::IsFalse(tab.Delete("moniker"));
		}
		TEST_METHOD(TableInsertTest1)
		{
			std::vector<std::string> vec;
			vec.push_back("Hello");
			vec.push_back("Team");
			vec.push_back("25");
			Record r = *new Record(vec);
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			Assert::IsTrue(tab.Insert(&r));
		}
		TEST_METHOD(TableInsertTest2)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Team");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Justin");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hit, there");
			vec3.push_back("Steven");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
	
			Assert::IsTrue(tab.Insert(&first));	
			Assert::IsTrue(tab.Insert(&second));
			Assert::IsTrue(tab.Insert(&third));
	
		}
		TEST_METHOD(TableInsertTest3)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::IsFalse(tab.Insert(nullptr));
		}
		TEST_METHOD(TableInsertTest4)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::IsFalse(tab.Insert(&first));
		}
		TEST_METHOD(TableGetSizeTest1)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);

			Assert::AreEqual((int)tab.GetSize(), 3);
		}
		TEST_METHOD(TableGetSizeTest2)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			Assert::AreEqual((int)tab.GetSize(), 0);

			tab.Insert(&first);
			Assert::AreEqual((int)tab.GetSize(), 1);

			tab.Insert(&second);
			Assert::AreEqual((int)tab.GetSize(), 2);

			tab.Insert(&third);
			Assert::AreEqual((int)tab.GetSize(), 3);
		}
		TEST_METHOD(TableGetSizeTest3)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			tab.Insert(nullptr);

			Assert::AreEqual((int)tab.GetSize(), 3);
		}
		TEST_METHOD(TableGetAttributesTest)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			Assert::IsTrue(tab.GetAttributes() == schema);
		}
		TEST_METHOD(TableRenameTest1)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			Assert::IsTrue(tab.Rename("moniker","title"));
		}
		TEST_METHOD(TableRenameTest2)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);
			tab.Rename("moniker","title");
			Assert::IsTrue(tab.GetAttributes()[1].second == EntryTypes::STRING);
		}
		TEST_METHOD(TableRenameTest3)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			Assert::IsFalse(tab.Rename("FavoriteDate","DateEntered"));
		}
		TEST_METHOD(TableRenameTest4)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			Assert::IsFalse(tab.Rename("moniker","group"));
		}
		TEST_METHOD(TableRenameTest5)
		{
			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			Assert::IsFalse(tab.Rename("moniker","moniker"));
		}
		TEST_METHOD(TableSumTest1)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::AreEqual(tab.Sum("group"), 136.0);
		}
		TEST_METHOD(TableSumTest2)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			try{
				tab.Sum("moniker");
				Assert::Fail(L"Sum succeeded");
			}
			catch(std::exception e){

			}
		}
		TEST_METHOD(TableSumTest3)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			Table tab = *new Table(schema);
			tab.Insert(&first);

			tab.Add("groups", EntryTypes::INT);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("25");
			Record third = *new Record(vec3);
	
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::AreEqual(tab.Sum("groups"),25.0);
		}
		TEST_METHOD(TableSumTest4)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			Table tab = *new Table(schema);
			tab.Insert(&first);

			tab.Add("groups", EntryTypes::INT);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("25");
			Record third = *new Record(vec3);
	
			tab.Insert(&second);
			tab.Insert(&third);
			try{
				tab.Sum("birthday");
				Assert::Fail(L"Sum Succeeded");
			}
			catch(std::exception e){
				
			}
		}
		TEST_METHOD(TableCountTest1)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			Record* first = new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			Record* second = new Record(vec2);

			TableSchema schema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			Table* tab = new Table(schema);
			tab->Insert(first);

			tab->Add("groups", EntryTypes::INT);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("25");
			Record* third = new Record(vec3);
	
			tab->Insert(second);
			tab->Insert(third);
			Assert::AreEqual(tab->Count("moniker"),(size_t)3);
		}
		TEST_METHOD(TableCountTest2)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			Table tab = *new Table(schema);
			tab.Insert(&first);

			tab.Add("groups", EntryTypes::INT);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("25");
			Record third = *new Record(vec3);
	
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::AreEqual((int)tab.Count("groups"),1);
		}
		TEST_METHOD(TableCountTest3)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			Table tab = *new Table(schema);
			tab.Insert(&first);

			tab.Add("groups", EntryTypes::INT);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("25");
			Record third = *new Record(vec3);
	
			tab.Insert(&second);
			tab.Insert(&third);
			try{
				tab.Sum("birthday");
				Assert::Fail(L"Sum succeeded");
			}
			catch(std::exception e){
				
			}
		}
		TEST_METHOD(TableMinTest1)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			Table tab = *new Table(schema);
			tab.Insert(&first);

			tab.Add("groups", EntryTypes::INT);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("25");
			Record third = *new Record(vec3);
	
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::AreEqual(tab.Min("groups"), std::string("25"));
		}
		TEST_METHOD(TableMinTest2)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::AreEqual(tab.Min("group"), std::string("17"));
		}
		TEST_METHOD(TableMinTest3)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			try{
				tab.Min("moniker");
				Assert::Fail(L"Min succeeded");
			}
			catch(std::exception){
				
			}
		}
		TEST_METHOD(TableMinTest4)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			try{
				tab.Sum("birthday");
				Assert::Fail(L"Sum succeeded");
			}
			catch(std::exception e){
				
			}
		}
		TEST_METHOD(TableMaxTest1)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			Table tab = *new Table(schema);
			tab.Insert(&first);

			tab.Add("groups", EntryTypes::INT);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("25");
			Record third = *new Record(vec3);
	
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::AreEqual(tab.Max("groups"), std::string("25"));
		}
		TEST_METHOD(TableMaxTest2)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			Assert::AreEqual(tab.Max("group"), std::string("94"));
		}
		TEST_METHOD(TableMaxTest3)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			try{
				tab.Min("moniker");
				Assert::Fail(L"Min succeeded");
			}
			catch(std::exception){
				
			}
		}
		TEST_METHOD(TableMaxTest4)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);
			try{
				tab.Max("birthday");
				Assert::Fail(L"Max succeeded");
			}
			catch(std::exception e){

			}
			
		}
		TEST_METHOD(TestCrossJoin1)
		{
			// NOTE: The test written here appears to be for a different type of join than cross join
			// Cross join takes table A with Schema A1 A2 A3 and table B with Schema B1 B2 B3
			// and returns a table C with Schema A1 A2 A3 B1 B2 B3
			// If table A has a records and B has b records, then table C will have c = a*b records.
			// This test will fail because it is incorrect.
			/*
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab1 = *new Table(schema);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			Table tab2 = *new Table(schema);

			tab1.Insert(&first);
			tab1.Insert(&second);
			tab2.Insert(&third);
			Assert::IsTrue(Table::CrossJoin(&tab1, &tab2) != NULL);
			*/
		}
		TEST_METHOD(TestCrossJoin2)
		{
			// NOTE: The test written here appears to be for a different type of join than cross join
			// Cross join takes table A with Schema A1 A2 A3 and table B with Schema B1 B2 B3
			// and returns a table C with Schema A1 A2 A3 B1 B2 B3
			// If table A has a records and B has b records, then table C will have c = a*b records.
			// This test will fail because it is incorrect.
			/*
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Thomas");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Victor");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab1 = *new Table(schema);

			std::vector<std::string> vec3;
			vec3.push_back("Hi, there");
			vec3.push_back("Jason");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			Table tab2 = *new Table(schema);

			tab1.Insert(&first);
			tab1.Insert(&second);
			tab2.Insert(&third);

			Table* tab3 = Table::CrossJoin(&tab1, &tab2);
			Assert::AreEqual((int)tab3->GetSize(), 3);
			*/
		}
		TEST_METHOD(TestGetRecord1)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Team");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Justin");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hit, there");
			vec3.push_back("Steven");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);

			Assert::IsTrue(first.GetEntryAt(0) == tab.GetRecordAt(0)->GetEntryAt(0));
		}

		TEST_METHOD(TestGetRecord2)
		{
			std::vector<std::string> vec1;
			vec1.push_back("Hello");
			vec1.push_back("Team");
			vec1.push_back("25");
			Record first = *new Record(vec1);

			std::vector<std::string> vec2;
			vec2.push_back("Howdy");
			vec2.push_back("Justin");
			vec2.push_back("17");
			Record second = *new Record(vec2);

			std::vector<std::string> vec3;
			vec3.push_back("Hit, there");
			vec3.push_back("Steven");
			vec3.push_back("94");
			Record third = *new Record(vec3);

			TableSchema schema = *new TableSchema;
			schema.push_back(TableAttribute("greeting", EntryTypes::STRING));
			schema.push_back(TableAttribute("moniker", EntryTypes::STRING));
			schema.push_back(TableAttribute("group", EntryTypes::INT));
			Table tab = *new Table(schema);

			tab.Insert(&first);
			tab.Insert(&second);
			tab.Insert(&third);

		Assert::IsFalse(first.GetEntryAt(0) == tab.GetRecordAt(1)->GetEntryAt(0));
		}

		////////////////////RECORD FUNCTIONS///////////////////////////////
		TEST_METHOD(RecordConstructorTest2)
		{
			std::vector<std::string> entries;
			Record record(entries);
			Assert::IsNotNull(&record);
		}
		TEST_METHOD(RecordGetEntryAtTest)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
		entries.push_back("123 Fake St.");
			Record record(entries);
			Assert::AreEqual(record.GetEntryAt(0), std::string("John"));
		}
		TEST_METHOD(RecordGetEntryAtTest2)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
			entries.push_back("123 Fake St.");
			Record record(entries);
			Assert::AreEqual(record.GetEntryAt(1), std::string("Smith"));
		}
		TEST_METHOD(RecordGetEntryAtTest3)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
			entries.push_back("123 Fake St.");
			Record record(entries);
			Assert::AreEqual(record.GetEntryAt(2), std::string("123 Fake St."));
		}
		TEST_METHOD(RecordGetEntryAtTest4)
		{
			std::vector<std::string> entries;
			entries.push_back("Allen");
			Record record(entries);

			try
			{
				record.GetEntryAt(2);
				Assert::Fail(L"Out of bounds was allowed");
			}
			catch(std::exception ex)
			{
			}
		}
		TEST_METHOD(RecordGetEntryAtTest5)
		{
			std::vector<std::string> entries;
			Record record(entries);
			entries.push_back("John");

			try
			{
				record.GetEntryAt(-1);
				Assert::Fail(L"Out of bounds was allowed");
			}
			catch(std::exception ex)
			{
			}
		}
		TEST_METHOD(RecordSetEntryAtTest)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
			entries.push_back("123 Fake St.");
			Record record(entries);
			record.SetEntryAt(0, "Robert");
			Assert::AreEqual(record.GetEntryAt(0), std::string("Robert"));
		}
		TEST_METHOD(RecordSetEntryAtTest2)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
			entries.push_back("123 Fake St.");
			Record record(entries);
			record.SetEntryAt(0, "Robert");
			record.SetEntryAt(1, "Frost");
			Assert::AreEqual(record.GetEntryAt(1), std::string("Frost"));
		}
		TEST_METHOD(RecordSetEntryAtTest3)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
			entries.push_back("123 Fake St.");
			Record record(entries);
			record.SetEntryAt(0, "Robert");
			record.SetEntryAt(1, "Frost");
			record.SetEntryAt(2, "742 Evergreen Tarace");
			Assert::AreEqual(record.GetEntryAt(2), std::string("742 Evergreen Tarace"));
		}
		TEST_METHOD(RecordSetEntryAtTest4)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
			entries.push_back("123 Fake St.");
			Record record(entries);
			record.SetEntryAt(0, "Robert");
			record.SetEntryAt(2, "742 Evergreen Tarace");
			record.SetEntryAt(0, "Steve");
			Assert::AreEqual(record.GetEntryAt(0), std::string("Steve"));
		}
		TEST_METHOD(RecordSetEntryAtTest5)
		{
			std::vector<std::string> entries;
			entries.push_back("John");
			entries.push_back("Smith");
			entries.push_back("123 Fake St.");
			Record record(entries);
			record.SetEntryAt(0, "Robert");
			record.SetEntryAt(2, "742 Evergreen Tarace");
			record.SetEntryAt(0, "Steve");
			Assert::AreEqual(record.GetEntryAt(0), std::string("Steve"));
		}
		TEST_METHOD(RecordSetEntryAtTest6)
		{
			std::vector<std::string> entries;
			Record record(entries);

			try
			{
				record.SetEntryAt(0, "Steve");
				Assert::Fail(L"Out of bounds was allowed");
			}
			catch(std::exception ex)
			{
			}
		}
		TEST_METHOD(RecordSetEntryAtTest7)
		{
			std::vector<std::string> entries;
			Record record(entries);
			entries.push_back("John");
			try{
				record.SetEntryAt(1, "Steve");
				Assert::Fail();
			}
			catch(std::exception e){
				
			}
		}
		TEST_METHOD(RecordSetEntryAtTest8)
		{
			std::vector<std::string> entries;
			Record record(entries);
			entries.push_back("John");
			try{
				record.SetEntryAt(-1, "Steve");
				Assert::Fail();
			}
			catch(std::exception e){
				
			}
		}

		////////////////////////////////Query Testing\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//database function
		TEST_METHOD(QueryTest1)
		{	//tests that query does in fact return a pointer to results
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));

			Table team25(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25.Insert(new Record(entries1));
			team25.Insert(new Record(entries2));
			team25.Insert(new Record(entries3));

			database.AddTable(&team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age > 40";

			Assert::IsNotNull(database.Query(strSelect,strWhere,strFrom));
		}
		TEST_METHOD(QueryTest2)
		{
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table team25(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25.Insert(new Record(entries1));
			team25.Insert(new Record(entries2));
			team25.Insert(new Record(entries3));

			database.AddTable(&team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age > 40";

			Table* results = database.Query(strSelect,strWhere,strFrom);
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Thomas"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Mulholland"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("47"));

			Assert::AreEqual(results->GetRecordAt(1)->GetEntryAt(0), std::string("Jason"));
			Assert::AreEqual(results->GetRecordAt(1)->GetEntryAt(1), std::string("Foura"));
			Assert::AreEqual(results->GetRecordAt(1)->GetEntryAt(2), std::string("71"));
		}

		TEST_METHOD(QueryTest3)
		{
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table team25(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25.Insert(new Record(entries1));
			team25.Insert(new Record(entries2));
			team25.Insert(new Record(entries3));

			database.AddTable(&team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age < 40";

			Table* results = database.Query(strSelect,strWhere,strFrom);
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Victor"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Gutierrez"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("23"));
		}
		TEST_METHOD(QueryTest4)
		{
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table team25(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25.Insert(new Record(entries1));
			team25.Insert(new Record(entries2));
			team25.Insert(new Record(entries3));

			database.AddTable(&team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age <= 23";

			Table* results = database.Query(strSelect,strWhere,strFrom);
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Victor"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Gutierrez"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("23"));
		}
		TEST_METHOD(QueryTest5) 
		{
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table team25(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25.Insert(new Record(entries1));
			team25.Insert(new Record(entries2));
			team25.Insert(new Record(entries3));

			database.AddTable(&team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age >= 23";

			Table* results = database.Query(strSelect,strWhere,strFrom);
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Thomas"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Mulholland"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("47"));
			Assert::AreEqual((int)results->GetSize(),3);
		}
		TEST_METHOD(QueryTest6)
		{
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table team25(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25.Insert(new Record(entries1));
			team25.Insert(new Record(entries2));
			team25.Insert(new Record(entries3));

			database.AddTable(&team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age = 47";

			Table* results = database.Query(strSelect,strWhere,strFrom);
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Thomas"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Mulholland"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("47"));
		}
		TEST_METHOD(QueryTest7)
		{
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table team25(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25.Insert(new Record(entries1));
			team25.Insert(new Record(entries2));
			team25.Insert(new Record(entries3));

			database.AddTable(&team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age != 47";

			Table* results = database.Query(strSelect,strWhere,strFrom);
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Victor"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Gutierrez"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("23"));

			Assert::AreEqual(results->GetRecordAt(1)->GetEntryAt(0), std::string("Jason"));
			Assert::AreEqual(results->GetRecordAt(1)->GetEntryAt(1), std::string("Foura"));
			Assert::AreEqual(results->GetRecordAt(1)->GetEntryAt(2), std::string("71"));
		}
		TEST_METHOD(QueryTest8)
		{
			Database database;
			TableSchema team25schema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table* team25 = new Table(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Thomas");
			entries2.push_back("Rethard");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25->Insert(new Record(entries1));
			team25->Insert(new Record(entries2));
			team25->Insert(new Record(entries3));

			database.AddTable(team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "FirstName = Thomas AND Age = 23";

			Table* results = database.Query(strSelect,strWhere,strFrom);
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Thomas"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Rethard"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("23"));
		}

		TEST_METHOD(QueryTest9)
		{
			Database database;
			TableSchema team25schema = *new TableSchema;
			team25schema.push_back(TableAttribute("FirstName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("LastName", EntryTypes::STRING));
			team25schema.push_back(TableAttribute("Age", EntryTypes::INT));
			Table* team25 = new Table(team25schema);

			std::vector<std::string> entries1;
			entries1.push_back("Thomas");
			entries1.push_back("Mulholland");
			entries1.push_back("47");

			std::vector<std::string> entries2;
			entries2.push_back("Victor");
			entries2.push_back("Gutierrez");
			entries2.push_back("23");

			std::vector<std::string> entries3;
			entries3.push_back("Jason");
			entries3.push_back("Foura");
			entries3.push_back("71");

			team25->Insert(new Record(entries1));
			team25->Insert(new Record(entries2));
			team25->Insert(new Record(entries3));

			database.AddTable(team25, "Team25");

			std::vector<std::string> strSelect;
			strSelect.push_back("FirstName");
			strSelect.push_back("LastName");
			strSelect.push_back("Age");
			std::string strFrom = "Team25";
			std::string strWhere = "Age = 53 OR Age = 71";

			Table* results = database.Query(strSelect,strWhere,strFrom);

			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(0), std::string("Jason"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(1), std::string("Foura"));
			Assert::AreEqual(results->GetRecordAt(0)->GetEntryAt(2), std::string("71"));
			Assert::AreEqual((int)results->GetSize(),1);
		}
	};
}
