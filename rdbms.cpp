/*
RDBMS: Kade Keith, Matthew Saari, Ryan Ledbetter, Victor Gutierrez
*/

// this is my commit VG
// testing RL


#pragma once
#include "rdbms.h"
#include "ConditionTree.h"

Datum::Datum() : stringData(""), numData(-999){}
Datum::Datum(int n) : numData(n) {}
Datum::Datum(string s) : stringData(s), numData(-999) {}

bool Datum::operator!=(const Datum &d)
{
	return !(stringData == d.stringData && numData == d.numData);
}
bool Datum::operator==(const Datum &d){
	return (stringData == d.stringData && numData == d.numData);
}


Table::Table(){}
Table::Table(vector<string> attrNames, vector<string> keys){
		keyNames = keys;
		attributeNames = attrNames;
	}


bool Table::duplicateExists(vector<Datum> newRow){
	vector<int> keyIndices;
	bool dupFound = false;
	int conflictCounter = 0; //if it is the same as the number of keys there is a conflict
	for (int i = 0; i<attributeNames.size(); i++){
		for (int j = 0; j<keyNames.size(); j++)
			if (attributeNames[i] == keyNames[j])
				keyIndices.push_back(i);
	}

	//check for conflicts
	for (int i = 0; i<data.size(); i++){
		conflictCounter = 0;
		for (int j = 0; j<keyNames.size(); j++){
			if (data[i][j] == newRow[j])
				conflictCounter++;
		}
		if (conflictCounter == keyNames.size())
			dupFound = true;
	}
	return dupFound;
}

	
void Table::printTable(){
	for (int i = 0; i<attributeNames.size(); i++)
		cout << attributeNames[i] << ", ";

	cout << endl;
	for (int i = 0; i<data.size(); i++){
		for (int j = 0; j<attributeNames.size(); j++){
			if (data[i][j].numData == -999)
				cout << data[i][j].stringData << " ";
			else
				cout << data[i][j].numData << " ";
		}
		cout << endl;
	}
	cout << endl;
}




Database::Database(){}

void Database::createTable(string tableName, vector<string> attrNames, vector<string> keys){
	Table newTable(attrNames, keys);
	allTables[tableName] = newTable;
}

void Database::dropTable(string tableName){
	allTables.erase(tableName);
}

void Database::insertIntoTable(string tableName, vector<Datum> newRow){
	if (!allTables[tableName].duplicateExists(newRow))
		allTables[tableName].data.push_back(newRow);
	else
		cout << "There is already an entry in the table with that key\n";
}

void Database::deleteFromTable(string tableName, ConditionNode condition)
{
	// Begin iterating from end so that removes don't change position of any data we have yet to look at.
	vector<vector<Datum> >::iterator it;
	it = allTables[tableName].data.end();
	while (it != allTables[tableName].data.begin())
	{
		it--;
		if (condition.eval(allTables[tableName].attributeNames, *it))
		{
			allTables[tableName].data.erase(it);
		}
	}
}

void Database::updateTable(string tableName, vector<string> attributeName, vector<Datum> newValue, ConditionNode condition){

	vector<Datum> dupRow; //to check for conflicts
	vector<int> attrIndeces;
	//get the indeces of the attributes to update
	for (int i = 0; i<attributeName.size(); i++){
		for (int j = 0; j<allTables[tableName].attributeNames.size(); j++){
			if (allTables[tableName].attributeNames[j] == attributeName[i])
				attrIndeces.push_back(j);
		}
	}

	// Begin iterating from end so that removes don't change position of any data we have yet to look at.
	vector<vector<Datum> >::iterator it;
	it = allTables[tableName].data.end();
	while (it != allTables[tableName].data.begin())
	{
		it--;
		if (condition.eval(allTables[tableName].attributeNames, *it))
		{
			for (int j = 0; j<attrIndeces.size(); j++){
				dupRow = *it;
				dupRow[attrIndeces[j]] = newValue[j];
				if (!allTables[tableName].duplicateExists(dupRow))
					(*it)[attrIndeces[j]] = newValue[j];
				else
					cout << "That update would cause a conflict of primary keys\n";
			}
		}
	}
}

Table Database::selectFromTable(string tableName, ConditionNode condition){
	Table retTable(allTables[tableName].attributeNames, allTables[tableName].keyNames);


	// Begin iterating from end so that removes don't change position of any data we have yet to look at.
	vector<vector<Datum> >::iterator it;
	it = allTables[tableName].data.end();
	while (it != allTables[tableName].data.begin())
	{
		it--;
		if (condition.eval(allTables[tableName].attributeNames, *it))
		{
			retTable.data.push_back(*it);
		}
	}
	return retTable;
}

Table Database::projectFromTable(string tableName, vector<string> attributeNames){
	Table wholeTable = allTables[tableName];
	Table projectionTable(attributeNames, allTables[tableName].keyNames);
	//get indices in wholeTable of matching attributes
	vector<int> attIndices;
	for (int i = 0; i < attributeNames.size(); ++i){
		for (int j = 0; j < wholeTable.data[0].size(); ++j){
			if (attributeNames[i] == wholeTable.attributeNames[j]){
				attIndices.push_back(j);
				break;
			}
		}
	}

	for (int row = 0; row < wholeTable.data.size(); ++row){
		vector<Datum> newRow;
		for (int column = 0; column < attIndices.size(); ++column){
			Datum d(wholeTable.data[row][attIndices[column]]);
			newRow.push_back(d);
		}
		projectionTable.data.push_back(newRow);
	}
	return projectionTable;
}

Table Database::renameAttributes(string tableName, vector<string> attributeNames){
	Table newNamesTable = allTables[tableName];
	newNamesTable.attributeNames = attributeNames;
	return newNamesTable;
}

	//just see if two tables are union compatible(for setUnion and setDifference)
bool Database::unionCompatible(string tableName1, string tableName2){
	vector<string> atts1 = allTables[tableName1].attributeNames;
	vector<string> atts2 = allTables[tableName2].attributeNames;
	//if different size then not union compatible
	if (atts1.size() != atts2.size()){
		return false;
	}
	for (int i = 0; i < atts1.size(); ++i){
		//if different names then not union compatible
		if (atts1[i] != atts2[i]){
			return false;
		}
	}
	return true;
}

Table Database::setUnion(string tableName1, string tableName2){
	Table unionTable(allTables[tableName1].attributeNames, allTables[tableName1].keyNames);
	if (!unionCompatible(tableName1, tableName2)){
		cout << "Unable to perform set union on " << tableName1 << " and " << tableName2 << "." << endl;
		return unionTable;	//empty table
	}

	//temp tables to make getting rows easier
	Table t1 = allTables[tableName1];
	Table t2 = allTables[tableName2];

	//inserting ALL rows from both tables into unionTable
	for (int i = 0; i < t1.data.size(); ++i){
		unionTable.data.push_back(t1.data[i]);
	}
	for (int i = 0; i < t2.data.size(); ++i){
		unionTable.data.push_back(t2.data[i]);
	}

	//now remove duplicates from unionTable
	//efficiency is not a concern in our system..
	for (int row = 0; row < unionTable.data.size(); ++row){
		//find duplicate rows
		for (int dupRow = 0; dupRow < unionTable.data.size(); ++dupRow){
			bool isDuplicate = true;
			//compare individual rows
			for (int col = 0; col < unionTable.data[0].size(); ++col){
				if (unionTable.data[row][col] != unionTable.data[dupRow][col]){
					isDuplicate = false;
					break;
				}
			}
			if (isDuplicate){
				unionTable.data.erase(unionTable.data.begin() + dupRow);
				//since vector resizes
				--dupRow;
			}
		}
	}
	return unionTable;
}

	//compute minuend - subtrahend = diference
Table Database::setDifference(string tNameMinuend, string tNameSubtrahend){
	//difference is the minuend at beginning
	Table difference = allTables[tNameMinuend];
	Table subtrahend = allTables[tNameSubtrahend];

	if (!unionCompatible(tNameSubtrahend, tNameSubtrahend)){
		cout << "Unable to perform set difference on " << tNameMinuend << " and " << tNameSubtrahend << "." << endl;
		return difference;	//empty table
	}

	//run through all elements to subtract
	for (int sub = 0; sub < subtrahend.data.size(); ++sub){
		//run through all elements to subtract from
		for (int dif = 0; dif < difference.data.size(); ++dif){
			bool isEqual = true;
			//compare rows and delete from difference if equal
			for (int col = 0; col < difference.data[0].size(); ++col){
				if (difference.data[dif][col] != subtrahend.data[sub][col]){
					isEqual = false;
					break;
				}
			}
			if (isEqual){
				difference.data.erase(difference.data.begin() + dif);
				//since we assume no duplicates can be in table
				break;
			}
		}
	}

	return difference;
}

Table Database::crossProduct(string tableName1, string tableName2){
	//temp tables to make getting rows easier
	Table t1 = allTables[tableName1];
	Table t2 = allTables[tableName2];

	//need to combine t1 attributes with t2 attributes..
	vector<string> allAttributes = t1.attributeNames;
	for (int i = 0; i < t2.attributeNames.size(); ++i){
		allAttributes.push_back(t2.attributeNames[i]);
	}
	Table productTable(allAttributes, allTables[tableName1].keyNames);

	//compute cross product
	for (int firstT = 0; firstT < t1.data.size(); ++firstT){
		for (int secondT = 0; secondT < t2.data.size(); ++secondT){
			//create newRow by combining t1 and t2 rows
			vector<Datum> newRow = t1.data[firstT];
			for (int col = 0; col < t2.data.size(); ++col){
				newRow.push_back(t2.data[secondT][col]);
			}

			productTable.data.push_back(newRow);
		}
	}

	return productTable;
}


