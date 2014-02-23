#include <iostream>

#include "artistApp.h"
#include "rdbms.h"
#include "parser.h"
#include "dbConnection.h"

using namespace std;

// string to store tab characters for use in visually displaying menu depth
string tabDepth = "";

// database wrapper needed globally
DBConnection dbCon;

int main()
{
	// load table data
	openAllTables();

	int selection = 0;

	while(true)
	{
		cout << "Enter an entities corresponding number to see options for it." << endl;
		cout << "1 - Artists" << endl;
		cout << "2 - Museums" << endl;
		cout << "3 - Periods" << endl;
		cout << "4 - Works" << endl;
		cout << "5 - To Exit and Save" << endl;
		cout << "6 - To Exit" << endl;

		cout << "Selection: ";
		cin >> selection;
		cout << endl;

		// new tabDepth after selection
		tabDepth = "\t";

		// switch based on user choice
		switch(selection)
		{
			case 1:	// Artists
				artistsSelected();

				break;
			case 2:	// Museums


				break;
			case 3:	// Periods


				break;
			case 4:	// Works


				break;
			case 5:	// To Exit and Save
				closeAndSaveAllTables();
				return 0;
				break;
			case 6:	// To Exit
				return 0;
			default:
				continue;
		}
	}


	getchar();
}

void artistsSelected()
{
	int selection = 0;

	while(true)
	{
		cout << tabDepth << "Enter the number corresponding to what you want to do." << endl;
		cout << tabDepth << "1 - Add artist" << endl;
		cout << tabDepth << "2 - Remove artist" << endl;
		cout << tabDepth << "3 - Show some artist info" << endl;
		cout << tabDepth << "4 - Update an artist's DOD" << endl;
		cout << tabDepth << "5 - Return to previous menu" << endl;

		cout << tabDepth << "Selection: ";
		cin >> selection;
		cout << endl;

		// new tabDepth after selection
		tabDepth = "\t\t";

		// variables needed within switch
		string command;
		string name;
		int birthYear;
		int deathYear;
		string nationality;

		// switch based on user choice
		switch(selection)
		{
			case 1:
				cout << tabDepth << "Adding an artist\n" << endl;
				// get artist info from user
				cout << tabDepth << "Artist name: ";
				cin >> name;
				cout << tabDepth << "Year of birth: ";
				cin >> birthYear;
				cout << tabDepth << "Year of death: ";
				cin >> deathYear;
				cout << tabDepth << "Artist's nationality: ";
				cin >> nationality;

				command = "INSERT INTO Artist VALUES FROM (\"" + name + "\", "
					+ to_string(birthYear) + ", \"" + nationality + "\", "
					+ to_string(deathYear) + ");";
				dbCon.executeCommand(command);

				cout << endl;
				tabDepth = "\t";
				break;
			case 2:
				cout << tabDepth << "Removing an artist\n" << endl;
				cout << tabDepth << "Artist name: ";
				cin >> name;

				command = "DELETE FROM Artist WHERE (name == \"" + name + "\");";
				dbCon.executeCommand(command);
				
				cout << endl;
				tabDepth = "\t";
				break;
			case 3:

				break;
			case 4:

				break;
			case 5:
				return;
			default:
				continue;
		}
	}
}

void openAllTables()
{
	string command = "OPEN Artist;";
	dbCon.executeCommand(command);
	command = "OPEN ArtistWorks;";
	dbCon.executeCommand(command);
	command = "OPEN Museum;"; 
	dbCon.executeCommand(command);
	command = "OPEN MuseumContains;";
	dbCon.executeCommand(command);
	command = "OPEN Period;";
	dbCon.executeCommand(command);
	command = "OPEN PeriodWorks;";
	dbCon.executeCommand(command);
	command = "OPEN Work;";
	dbCon.executeCommand(command);

}

void closeAndSaveAllTables()
{
	string command = "CLOSE Artist;";
	dbCon.executeCommand(command);
	command = "CLOSE ArtistWorks;";
	dbCon.executeCommand(command);
	command = "CLOSE Museum;"; 
	dbCon.executeCommand(command);
	command = "CLOSE MuseumContains;";
	dbCon.executeCommand(command);
	command = "CLOSE Period;";
	dbCon.executeCommand(command);
	command = "CLOSE PeriodWorks;";
	dbCon.executeCommand(command);
	command = "CLOSE Work;";
	dbCon.executeCommand(command);
}

void createTablesForArtistDB()
{
	// Artist table
	string command = "CREATE TABLE Artist (name VARCHAR(30), birthYear INTEGER,"
		" nationality VARCHAR(20), deathYear VARCHAR(20)) PRIMARY KEY (name);";
	dbCon.executeCommand(command);
	command = "WRITE Artist;";
	dbCon.executeCommand(command);

	// Museum table
	command = "CREATE TABLE Museum (name VARCHAR(30), location VARCHAR(30),"
		" yearEstab INTEGER) PRIMARY KEY (name);";
	dbCon.executeCommand(command);
	command = "WRITE Museum;";
	dbCon.executeCommand(command);

	// Period table
	command = "CREATE TABLE Period (name VARCHAR(30), Year INTEGER) PRIMARY"
		" KEY (name);";
	dbCon.executeCommand(command);
	command = "WRITE Period;";
	dbCon.executeCommand(command);

	// Work table
	command = "CREATE TABLE Work (name VARCHAR(40), medium VARCHAR(20), yearMade"
		" INTEGER, currentValue INTEGER) PRIMARY KEY (name);";
	dbCon.executeCommand(command);
	command = "WRITE Work;";
	dbCon.executeCommand(command);

	// MuseumContains table
	command = "CREATE TABLE MuseumContains (museumName VARCHAR(30), workName VARCHAR(40))"
		" PRIMARY KEY (museumName, workName);";
	dbCon.executeCommand(command);
	command = "WRITE MuseumContains;";
	dbCon.executeCommand(command);

	// ArtistWorks table
	command = "CREATE TABLE ArtistWorks (artistName VARCHAR(30), workName VARCHAR(40))"
		" PRIMARY KEY (artistName, workName);";
	dbCon.executeCommand(command);
	command = "WRITE ArtistWorks;";
	dbCon.executeCommand(command);

	// PeriodWorks
	command = "CREATE TABLE PeriodWorks (periodName VARCHAR(30), workName VARCHAR(40))"
		" PRIMARY KEY (periodName, workName);";
	dbCon.executeCommand(command);
	command = "WRITE PeriodWorks;";
	dbCon.executeCommand(command);
}