/*
RDBMS: Kade Keith, Matthew Saari, Ryan Ledbetter, Victor Gutierrez
*/

#pragma once
#include "rdbms.h"
#include "ConditionTree.h"
#include <iomanip>	// for setw
#include <locale>	// for toupper

using namespace std;

// Datums with -999 in the int variable mean it is a stringData type
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

bool Table::operator==(const Table &t)
{
	return attributeNames == t.attributeNames;
}

Table Table::projectFromTable(const vector<string>& projectedNames){
	//table to return
	Table projectedTable(projectedNames, keyNames);

	//get the indices of the attributes to be projected
	vector<int> attIndices;
	for (int i = 0; i < projectedNames.size(); ++i){
		for (int j = 0; j < data[0].size(); ++j){
			if (projectedNames[i] == attributeNames[j]){
				attIndices.push_back(j);
				break;
			}
		}
	}

	//create rows to pushback into the new table
	for (int row = 0; row < data.size(); ++row){
		vector<Datum> newRow;
		//keep only the projected columns
		for (int column = 0; column < attIndices.size(); ++column){
			Datum d(data[row][attIndices[column]]);
			newRow.push_back(d);
		}
		projectedTable.data.push_back(newRow);
	}

	return projectedTable;
}

Table Table::renameAttributes(const vector<string>& renamedNames){
	//create new table with new names
	Table renamedTable(renamedNames, keyNames);
	//now set new table's data equal to old table's data
	renamedTable.data = data;
	return renamedTable;
}

bool Table::unionCompatibleWith(const Table& compareTable){
	vector<string> compareAttributes = compareTable.attributeNames;
	//if different size then not union compatible
	if (compareAttributes.size() != attributeNames.size()){
		return false;
	}
	for (int i = 0; i < attributeNames.size(); ++i){
		//if different names then not union compatible
		if (compareAttributes[i] != attributeNames[i]){
			return false;
		}
	}

	return true;
}

Table Table::unionWith(const Table& paramTable){
	Table unionTable(paramTable.attributeNames, paramTable.keyNames);
	if (!unionCompatibleWith(paramTable)){
		cout << "Unable to perform set union. Tables must be union compatible." << endl;
		return unionTable;	//empty table
	}

	//inserting ALL rows from both tables into unionTable
	for (int i = 0; i < data.size(); ++i){
		unionTable.data.push_back(data[i]);
	}
	for (int i = 0; i < paramTable.data.size(); ++i){
		unionTable.data.push_back(paramTable.data[i]);
	}

	//now remove duplicates from unionTable
	//efficiency is not a concern in our system..
	bool isDuplicate;
	for (int row = 0; row < unionTable.data.size(); ++row){
		//find duplicate rows
		for (int dupRow = 0; dupRow < unionTable.data.size(); ++dupRow){
			isDuplicate = true;
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

Table Table::differenceWith(const Table& subtrahendTable){
	//difference is the minuend (this) at the beginning
	Table difference(attributeNames, keyNames);
	difference.data = data;

	if (!unionCompatibleWith(subtrahendTable)){
		cout << "Unable to perform set difference. Tables must be union compatible." << endl;
		return difference;	//empty table
	}

	//run through all elements to subtract
	for (int sub = 0; sub < subtrahendTable.data.size(); ++sub){
		//run through all elements being subtracted from
		for (int dif = 0; dif < difference.data.size(); ++dif){
			bool isEqual = true;
			//compare rows and delete from difference if equal
			for (int col = 0; col < difference.data[0].size(); ++col){
				if (difference.data[dif][col] != subtrahendTable.data[sub][col]){
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

Table Table::productWith(const Table& paramTable){
	//make temp table for this table to make compution clearer
	Table thisTable(attributeNames, keyNames);
	thisTable.data = data;

	//need to combine thisTable attributes with paramTable attributes..
	vector<string> allAttributes = thisTable.attributeNames;
	for (int i = 0; i < paramTable.attributeNames.size(); ++i){
		allAttributes.push_back(paramTable.attributeNames[i]);
	}
	Table productTable(allAttributes, keyNames);

	//compute cross product
	for (int firstT = 0; firstT < thisTable.data.size(); ++firstT){
		for (int secondT = 0; secondT < paramTable.data.size(); ++secondT){
			//create newRow by combining thisTable and paramTable rows
			vector<Datum> newRow = thisTable.data[firstT];
			for (int col = 0; col < paramTable.data.size(); ++col){
				newRow.push_back(paramTable.data[secondT][col]);
			}

			productTable.data.push_back(newRow);
		}
	}

	return productTable;
}

Table Table::naturalJoinWith(const Table& paramTable){

	//indices that this table has in common with paramTable
	vector<int> thisAttIndices;
	//indices that paramTable has in common with this table
	vector<int> paramAttIndices;
	for (int param_index = 0; param_index < paramTable.attributeNames.size(); ++param_index){
		for (int this_index = 0; this_index < data[0].size(); ++this_index){
			if (paramTable.attributeNames[param_index] == attributeNames[this_index]){
				thisAttIndices.push_back(this_index);
				paramAttIndices.push_back(param_index);
				//since we wont find another match (tables can't have attributes with the same name),
				//we break the inside loop if we find a match
				break;
			}
		}
	}

	//need to get all attribute names that will be in joined table
	vector<string> combinedAttNames = attributeNames;

	//get indices of attributes in paramTable that aren't in this table
	//we need this so we can have all attributes from both tables (minus duplicates) for the join
	vector<int> notCommonAtts;

	//combinedAttNames has all names from this table,
	//now add attributes from paramTable but skip duplicates
	for (int i = 0; i < paramTable.attributeNames.size(); ++i){
		if (find(combinedAttNames.begin(), combinedAttNames.end(), paramTable.attributeNames[i]) == combinedAttNames.end()){
			//this means that combinedAttNames does not contain paramTable.attributesNames[i]
			//which means we need to add paramTable.attributeNames[i] to combinedAttNames
			combinedAttNames.push_back(paramTable.attributeNames[i]);

			//it also means we need to add the index of the attribute name from paramTable
			//to the notCommonAtts vector
			notCommonAtts.push_back(i);
		}
	}

	//now we can create a table with the combinedAttNames
	//given the same keys as this table
	Table joinedTable(combinedAttNames, keyNames);

	//loop through this table data to find indices of matching rows
	bool allCommonAttsEqual;
	for (int this_row = 0; this_row < data.size(); ++this_row){
		//loop through paramTable data to find indices of matching rows
		for (int param_row = 0; param_row < paramTable.attributeNames.size(); ++param_row){
			allCommonAttsEqual = true;
			//loop through columns in data to compare attributes with same name for equality
			for (int column = 0; column < thisAttIndices.size(); ++column) {
				//check if equal on common attributes
				if (data[this_row][thisAttIndices[column]] != paramTable.data[param_row][paramAttIndices[column]]){
					allCommonAttsEqual = false;
					break;
				}
			}

			if (allCommonAttsEqual){
				//need to create a new row for joinedTable
				vector<Datum> newRow = data[this_row];
				//now we just add the notCommonAtts from paramTable
				for (int i = 0; i < notCommonAtts.size(); ++i){
					newRow.push_back(paramTable.data[param_row][notCommonAtts[i]]);
				}
				joinedTable.data.push_back(newRow);
			}
		}
	}

	return joinedTable;
}

Table Table::selectFromTable(Table selectTable, ConditionNode condition){
	Table retTable(selectTable.attributeNames, selectTable.keyNames);


	// Begin iterating from end so that removes don't change position of any data we have yet to look at.
	vector<vector<Datum> >::iterator it;
	it = selectTable.data.end();
	while (it != selectTable.data.begin()) {
		it--;
		if (condition.eval(selectTable.attributeNames, *it)) {
			retTable.data.push_back(*it);
		}
	}
	return retTable;
}

bool Table::duplicateExists(const vector<Datum>& newRow){
	vector<int> keyIndices;
	bool duplicateFound = false;
	int conflictCounter = 0; //if it is the same as the number of keys there is a conflict
	for (int i = 0; i<attributeNames.size(); i++){
		for (int j = 0; j < keyNames.size(); j++){
			if (attributeNames[i] == keyNames[j]){
				keyIndices.push_back(i);
			}
		}
	}

	//check for conflicts
	for (int i = 0; i<data.size(); i++){
		conflictCounter = 0;
		for (int j = 0; j<keyNames.size(); j++){
			if (data[i][j] == newRow[j]){
				conflictCounter++;
			}
		}
		if (conflictCounter == keyNames.size()){
			duplicateFound = true;
		}
	}
	return duplicateFound;
}
	
void Table::printTable(){
	cout << left;
	cout << "---------------------------------------------------------" 
		"----------------------------------------" << endl;
	string tempAtt = "";
	for (int i = 0; i < attributeNames.size(); i++){
		tempAtt = attributeNames[i];
		transform(tempAtt.begin(), tempAtt.end(), tempAtt.begin(), toupper);
		cout << setw(30) << tempAtt;
	}

	cout << endl;
	for (int i = 0; i<data.size(); i++){
		for (int j = 0; j<attributeNames.size(); j++){
			if (data[i][j].numData == -999){
				cout << setw(30) << data[i][j].stringData;
			}
			else{
				cout << setw(30) << data[i][j].numData;
			}
		}
		cout << endl;
	}
	cout << "---------------------------------------------------------"
		"----------------------------------------\n" << endl;
}



Database::Database(){}

void Database::createTable(const string& tableName, const vector<string>& attrNames, const vector<string>& keys){
	Table newTable(attrNames, keys);
	allTables[tableName] = newTable;
}

void Database::dropTable(const string& tableName){
	allTables.erase(tableName);
}

void Database::insertIntoTable(const string& tableName, const vector<Datum>& newRow){
	if (!allTables[tableName].duplicateExists(newRow)){
		allTables[tableName].data.push_back(newRow);
	}
	else {
		cout << "There is already an entry in the table with that key\n";
	}
}

void Database::deleteFromTable(const string& tableName, ConditionNode condition)
{
	// Begin iterating from end so that removes don't change position of any data we have yet to look at.
	vector<vector<Datum> >::iterator it;
	it = allTables[tableName].data.begin();
	while (it != allTables[tableName].data.end()) {
		if (condition.eval(allTables[tableName].attributeNames, *it)) {
			it = allTables[tableName].data.erase(it);
		}
		else{
			it++;
		}
	}
}

void Database::updateTable(const string& tableName, const vector<string>& attributeName, const vector<Datum>& newValue, ConditionNode condition){

	vector<Datum> dupRow; //to check for conflicts
	vector<int> attrIndeces;
	//get the indeces of the attributes to update
	for (int i = 0; i<attributeName.size(); i++){
		for (int j = 0; j<allTables[tableName].attributeNames.size(); j++){
			if (allTables[tableName].attributeNames[j] == attributeName[i])
				attrIndeces.push_back(j);
		}
	}
	
	bool isPrimaryModified = false; // set to let us know if we are updating primary keys or not
	for (int i = 0; i < allTables[tableName].keyNames.size(); i++){
		for (int j = 0; j < attributeName.size(); j++){
			if (allTables[tableName].keyNames[i] == attributeName[j]){
				isPrimaryModified = true;
			}
		}
	}
	
	// Begin iterating from end so that removes don't change position of any data we have yet to look at.
	vector<vector<Datum> >::iterator it;
	it = allTables[tableName].data.end();
	while (it != allTables[tableName].data.begin()) {
		it--;
		if (condition.eval(allTables[tableName].attributeNames, *it)) {
			for (int j = 0; j<attrIndeces.size(); j++){
				dupRow = *it;
				dupRow[attrIndeces[j]] = newValue[j];	
				if (allTables[tableName].duplicateExists(dupRow) && isPrimaryModified){
					cout << "That update would cause a conflict of primary keys\n";				
				}
				else {
					(*it)[attrIndeces[j]] = newValue[j];
				}
			}
		}
	}
}

Table Database::selectFromTable(const string& tableName, ConditionNode condition){
	Table retTable(allTables[tableName].attributeNames, allTables[tableName].keyNames);


	// Begin iterating from end so that removes don't change position of any data we have yet to look at.
	vector<vector<Datum> >::iterator it;
	it = allTables[tableName].data.end();
	while (it != allTables[tableName].data.begin()) {
		it--;
		if (condition.eval(allTables[tableName].attributeNames, *it)) {
			retTable.data.push_back(*it);
		}
	}
	return retTable;
}

Table Database::projectFromTable(const string& tableName, const vector<string>& projectedNames){
	return allTables[tableName].projectFromTable(projectedNames);
}

Table Database::renameAttributes(const string& tableName, const vector<string>& renamedNames){
	return allTables[tableName].renameAttributes(renamedNames);
}

Table Database::setUnion(const string& tableName1, const string& tableName2){
	//allTables returns a table, just call unionWith on those tables
	return allTables[tableName1].unionWith(allTables[tableName2]);
}

Table Database::setDifference(const string& tNameMinuend, const string& tNameSubtrahend){
	//allTables returns a table, jsut call differeneWith on those tables
	return allTables[tNameMinuend].differenceWith(allTables[tNameMinuend]);
}

Table Database::crossProduct(const string& tableName1, const string& tableName2){
	//allTables returns a table, jsut call productWith on those tables
	return allTables[tableName1].productWith(allTables[tableName2]);
}

Table Database::naturalJoin(const string& tableName1, const string& tableName2){
	//allTables returns a table, jsut call productWith on those tables
	Table t;
	return t;
}

Table Database::getTable(const string& tableName){
	return allTables[tableName];
}