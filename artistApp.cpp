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
	createTablesForArtistDB();
	int selection = 0;

	while(true)
	{
		cout << "Enter an entities corresponding number to see options for it." << endl;
		cout << "1 - Artists" << endl;
		cout << "2 - Museums" << endl;
		cout << "3 - Periods" << endl;
		cout << "4 - Works" << endl;
		cout << "5 - To Exit" << endl;

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
			case 5:	// To Exit
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
		string birthDate;
		string deathDate;
		string nationality;

		// switch based on user choice
		switch(selection)
		{
			case 1:
				// get artist info from user
				cout << tabDepth << "Artist name: ";
				cin >> name;
				cout << tabDepth << "Date of birth (mm/dd/yyyy): ";
				cin >> birthDate;
				cout << tabDepth << "Date of death (mm/dd/yyyy): ";
				cin >> deathDate;
				cout << tabDepth << "Artist's nationality: ";
				cin >> nationality;

				command = "INSERT INTO Artist VALUES FROM (\"" + name + "\", \""
					+ birthDate + "\", \"" + nationality + "\", \"" + deathDate + "\");";
				dbCon.executeCommand(command);
				command = "CLOSE Artist;";
				dbCon.executeCommand(command);
				

				break;
			case 2:

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

void createTablesForArtistDB()
{
	// Artist table
	string command = "CREATE TABLE Artist (name VARCHAR(30), birthDate VARCHAR(12),"
		" nationality VARCHAR(20), deathDate VARCHAR(20)) PRIMARY KEY (name);";
	dbCon.executeCommand(command);
	command = "WRITE Artist;";
	dbCon.executeCommand(command);

	// Museum table
	command = "CREATE TABLE Museum (name VARCHAR(30), location VARCHAR(30),"
		" dateEstab VARCHAR(12)) PRIMARY KEY (name);";
	dbCon.executeCommand(command);
	command = "WRITE Museum;";
	dbCon.executeCommand(command);

	// Period table
	command = "CREATE TABLE Period (name VARCHAR(30), date VARCHAR(12)) PRIMARY"
		" KEY (name);";
	dbCon.executeCommand(command);
	command = "WRITE Period;";
	dbCon.executeCommand(command);

	// Work table
	command = "CREATE TABLE Work (name VARCHAR(40), medium VARCHAR(20), dateMade"
		" VARCHAR(12), currentValue INTEGER) PRIMARY KEY (name);";
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