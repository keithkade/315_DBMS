#include <iostream>
#include <fstream>
#include <Windows.h>

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
	setWindow(150, 50);

	// load table data if they exists
	if (exists("Artist.db")){
		openAllTables();
	}
	//create table data if this is first time running
	else{
		createTablesForArtistDB();
	}

	string selectionStr;
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
		getline(cin, selectionStr); //we use getline for all input because using both getline and cin causes problems
		selection = atoi(selectionStr.c_str());
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
				museumsSelected();

				break;
			case 3:	// Periods
				periodsSelected();

				break;
			case 4:	// Works
				worksSelected();

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

bool isNum(string& input) {
	return all_of(input.begin(), input.end(), ::isdigit);
}

bool exists(string filePath)
{
	ifstream check(filePath.c_str());

	if (check.good()){
		check.close();
		return true;
	}
	else {
		check.close();
		return false;
	}
}

void artistsSelected()
{
	string selectionStr;
	int selection = 0;

	while(true)
	{
		cout << tabDepth << "Enter the number corresponding to what you want to do." << endl;
		cout << tabDepth << "1 - Add artist" << endl;
		cout << tabDepth << "2 - Remove artist" << endl;
		cout << tabDepth << "3 - Show some artist info" << endl;
		cout << tabDepth << "4 - Update an artist's year of death" << endl;
		cout << tabDepth << "5 - Return to previous menu" << endl;

		cout << tabDepth << "Selection: ";
		getline(cin, selectionStr);
		selection = atoi(selectionStr.c_str());
		cout << endl;

		// new tabDepth after selection
		tabDepth = "\t\t";

		// variables needed within switch
		string command;
		string name;
		string birthYear;
		string deathYear;
		string nationality;

		// switch based on user choice
		switch(selection)
		{
			case 1:
				cout << tabDepth << "Adding an artist\n" << endl;

				cout << tabDepth << "Artist name: ";
				getline(cin, name);
				cout << tabDepth << "Year of birth: ";
				getline(cin, birthYear);
				cout << tabDepth << "Year of death: ";
				getline(cin, deathYear);
				cout << tabDepth << "Artist's nationality: ";
				getline(cin, nationality);

				//check that year inputs are numeric
				if (!isNum(birthYear) || !isNum(birthYear)){
					cout << tabDepth << "ERROR: Invalid year" << endl << endl;
					break;
				}

				command = "INSERT INTO Artist VALUES FROM (\"" + name + "\", "
					+ birthYear + ", \"" + nationality + "\", "
					+ deathYear + ");";
				dbCon.executeCommand(command);

				cout << endl;
				tabDepth = "\t";
				break;
			case 2:
				cout << tabDepth << "Removing an artist\n" << endl;

				cout << tabDepth << "Artist name: ";
				getline(cin, name);

				command = "DELETE FROM Artist WHERE (name == \"" + name + "\");";
				dbCon.executeCommand(command);
				
				cout << endl;
				tabDepth = "\t";
				break;
			case 3:
				showForArtists();
				cout << endl;
				tabDepth = "\t";
				break;
			case 4:
				cout << tabDepth << "Updating artist's year of death\n" << endl;

				cout << tabDepth << "Artist name: ";
				getline(cin, name);
				cout << tabDepth << "Year of death: ";
				getline(cin, deathYear);

				if (!isNum(birthYear)){
					cout << tabDepth << "ERROR: Invalid year" << endl << endl;
					break;
				}

				command = "UPDATE Artist SET deathYear = " + deathYear
					+ " WHERE (name == \"" + name + "\");";
				dbCon.executeCommand(command);
				
				cout << endl;
				tabDepth = "\t";
				break;
			case 5:
				return;
			default:
				continue;
		}
	}
}

void showForArtists()
{
	string selectionStr;
	int selection = 0;
	while(true)
	{
		cout << tabDepth << "Enter the number corresponding to what you want to do." << endl;
		cout << tabDepth << "1 - Show all artists" << endl;
		cout << tabDepth << "2 - Show artists in a museum" << endl;
		cout << tabDepth << "3 - Show artists not in a museum" << endl;
		cout << tabDepth << "4 - Show all artists nationalities" << endl;
		cout << tabDepth << "5 - Return to previous menu" << endl;

		cout << tabDepth << "Selection: ";
		getline(cin, selectionStr);
		selection = atoi(selectionStr.c_str());
		cout << endl;

		// new tabDepth after selection
		tabDepth = "\t\t\t";

		// variables needed within switch
		string command;
		string name;

		switch(selection)
		{
			case 1:
				cout << tabDepth << "List of all artists\n" << endl;
				command = "SHOW Artist;";
				dbCon.executeCommand(command);

				tabDepth = "\t\t";
				break;
			case 2:
				cout << tabDepth << "Artists in a museum\n" << endl;

				cout << tabDepth << "Museum name: ";
				getline(cin, name);

				// using cross product here instead of natural join because we have to use it somewhere!
				command = "crossProduct <- MuseumContains * ArtistWorks;";
				dbCon.executeCommand(command);

				command = "correctMuseum <- select (museumName == \"" + name + "\") crossProduct;";
				dbCon.executeCommand(command);

				command = "artistNames <- project (artistName) correctMuseum;";
				dbCon.executeCommand(command);

				command = "SHOW artistNames;";
				dbCon.executeCommand(command);

				tabDepth = "\t\t";
				break;
			case 3:
				cout << tabDepth << "Artists not in a museum\n" << endl;

				cout << tabDepth << "Museum name: ";
				getline(cin, name);

				command = "commonWorkJoin <- MuseumContains JOIN ArtistWorks;";
				dbCon.executeCommand(command);

				command = "correctMuseum <- select (museumName == \"" + name + "\") commonWorkJoin;";
				dbCon.executeCommand(command);

				// these are all the artists in a museum
				command = "artistsIn <- project (artistName) correctMuseum;";
				dbCon.executeCommand(command);

				command = "artistsNotIn <- Artist - artistsIn;";
				dbCon.executeCommand(command);

				cout << tabDepth << "List of artists\n" << endl;
				command = "SHOW artistsNotIN;";
				dbCon.executeCommand(command);

				tabDepth = "\t\t";
				break;
			case 4:
				cout << tabDepth << "List of artist nationalities\n" << endl;

				command = "artistNationalities <- project (name, nationality) Artist;";
				dbCon.executeCommand(command);

				tabDepth = "\t\t";
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

void museumsSelected()
{
	string selectionStr;
	int selection = 0;
	while (true)
	{
		cout << tabDepth << "Enter the number corresponding to what you want to do." << endl;
		cout << tabDepth << "1 - Add museum" << endl;
		cout << tabDepth << "2 - Remove museum" << endl;
		cout << tabDepth << "3 - Museum buys work" << endl;
		cout << tabDepth << "4 - Show works in a museum" << endl;
		cout << tabDepth << "5 - Show museum(s) that have artist" << endl;
		cout << tabDepth << "6 - Return to previous menu" << endl;

		cout << tabDepth << "Selection: ";
		getline(cin, selectionStr); 
		selection = atoi(selectionStr.c_str());
		cout << endl;

		// new tabDepth after selection
		tabDepth = "\t\t";

		// variables needed within switch
		string command;
		string name;
		string location;
		string workName;
		string artistName;
		string yearEstab;

		// switch based on user choice
		switch (selection)
		{
		case 1:
			// get museum info from user
			cout << tabDepth << "Museum name: ";
			getline(cin, name);
			cout << tabDepth << "Location: ";
			getline(cin, location);
			cout << tabDepth << "Year Established: ";
			getline(cin, yearEstab);

			if (!isNum(yearEstab)){
				cout << tabDepth << "ERROR: Invalid year" << endl << endl;
				break;
			}

			command = "INSERT INTO Museum VALUES FROM (\"" + name + "\", \""
				+ location + "\", " + yearEstab + ");";
			dbCon.executeCommand(command);

			break;
		case 2:
			cout << tabDepth << "Museum name: ";
			getline(cin, name);

			command = "DELETE FROM Museum WHERE name == " + name + ";";
			dbCon.executeCommand(command);

			break;
		case 3:
			cout << tabDepth << "Museum name: ";
			getline(cin, name);
			cout << tabDepth << "Work name: "; 
			getline(cin, workName);

			command = "INSERT INTO MuseumContains VALUES FROM (\"" + name + "\", \"" + workName + "\");";
			dbCon.executeCommand(command);

			break;
		case 4:
			cout << tabDepth << "Museum name: ";
			getline(cin, name);

			command = "tempName <- select (museumName == \"" + name + "\") MuseumContains;";
			dbCon.executeCommand(command);
			command = "result <- project (workName) tempName;";
			dbCon.executeCommand(command);
			command = "SHOW result;";
			dbCon.executeCommand(command);

			break;
		case 5:
			cout << tabDepth << "Museum name: ";
			getline(cin, name);
			cout << tabDepth << "Artist name: ";
			getline(cin, artistName);

			command = "tempName <- select (museumName == \"" + name + "\" && artistName == \"" + artistName + "\") "
				"(MuseumContains JOIN ArtistWorks);";
			dbCon.executeCommand(command);
			command = "result <- project (museumName) tempName;";
			dbCon.executeCommand(command);
			command = "SHOW result;";
			dbCon.executeCommand(command);

			break;
		case 6:
			return;
		default:
			continue;
		}
	}
}

void worksSelected()
{
	string selectionStr;
	int selection = 0;

	while (true)
	{
		cout << tabDepth << "Enter the number corresponding to what you want to do." << endl;
		cout << tabDepth << "1 - Add work" << endl;
		cout << tabDepth << "2 - Remove work" << endl;
		cout << tabDepth << "3 - Show works by artist" << endl;
		cout << tabDepth << "4 - Show works by period" << endl;
		cout << tabDepth << "5 - Show works in a museum(s)" << endl;
		cout << tabDepth << "6 - Show works above certain value" << endl;
		cout << tabDepth << "7 - Return to previous menu" << endl;

		cout << tabDepth << "Selection: ";
		getline(cin, selectionStr);
		selection = atoi(selectionStr.c_str());
		cout << endl;

		// new tabDepth after selection
		tabDepth = "\t\t";

		// variables needed within switch
		string command;
		string name;
		string medium;
		string periodName;
		string artistName;
		string museumName;
		string year;
		string value;

		// switch based on user choice
		switch (selection)
		{
		case 1:
			// get museum info from user
			cout << tabDepth << "Work name: ";
			getline(cin, name);
			cout << tabDepth << "Medium: ";
			getline(cin, medium);
			cout << tabDepth << "Year Made: ";
			getline(cin, year);
			cout << tabDepth << "Value: ";
			getline(cin, value);

			if (!isNum(year)){
				cout << tabDepth << "ERROR: Invalid year" << endl << endl;
				break;
			}

			if (!isNum(value)){
				cout << tabDepth << "ERROR: Invalid value" << endl << endl;
				break;
			}

			command = "INSERT INTO Museum VALUES FROM (\"" + name + "\", \""
				+ medium + "\", " + year + ", " + value + ");";
			dbCon.executeCommand(command);

			break;
		case 2:
			cout << tabDepth << "Work name: ";
			getline(cin, name);

			command = "DELETE FROM Museum WHERE name == " + name + ";";
			dbCon.executeCommand(command);

			break;
		case 3:
			cout << tabDepth << "Artist name: ";
			getline(cin, artistName);

			command = "tempName <- select (artistName == \"" + artistName + "\") ArtistWorks;";
			dbCon.executeCommand(command);
			command = "result <- project (workNames) tempName;";
			dbCon.executeCommand(command);
			command = "SHOW result;";
			dbCon.executeCommand(command);

			break;
		case 4:
			cout << tabDepth << "Period name: ";
			getline(cin, periodName);

			command = "tempName <- select (periodName == \"" + periodName + "\") PeriodWorks;";
			dbCon.executeCommand(command);
			command = "result <- project (workNames) tempName;";
			dbCon.executeCommand(command);
			command = "SHOW result;";
			dbCon.executeCommand(command);

			break;
		case 5:
			cout << tabDepth << "Museum name: ";
			getline(cin, periodName);

			command = "tempName <- select (museumName == \"" + periodName + "\") MuseumContains;";
			dbCon.executeCommand(command);
			command = "result <- project (workNames) tempName;";
			dbCon.executeCommand(command);
			command = "SHOW result;";
			dbCon.executeCommand(command);
			
			break;
		case 6:
			cout << tabDepth << "Value: ";
			getline(cin, value);

			command = "tempName <- select (currentValue > " + value + ") Works;";
			dbCon.executeCommand(command);
			command = "result <- project (workNames) tempName;";
			dbCon.executeCommand(command);
			command = "SHOW result;";
			dbCon.executeCommand(command);

			break;
		case 7:
			return;
		default:
			continue;
		}
	}
}

void periodsSelected(){
	//Add
	//Delete
	//Show all

	string selectionStr;
	int selection = 0;

	while (true)
	{
		cout << tabDepth << "Enter the number corresponding to what you want to do." << endl;
		cout << tabDepth << "1 - Add period" << endl;
		cout << tabDepth << "2 - Remove period" << endl;
		cout << tabDepth << "3 - Show all periods" << endl;
		cout << tabDepth << "4 - Return to previous menu" << endl;

		cout << tabDepth << "Selection: ";
		getline(cin, selectionStr);
		selection = atoi(selectionStr.c_str());
		cout << endl;

		// new tabDepth after selection
		tabDepth = "\t\t";

		// variables needed within switch
		string command;
		string name;
		string year;

		// switch based on user choice
		switch (selection)
		{
		case 1:
			cout << tabDepth << "Adding a period\n" << endl;

			cout << tabDepth << "Period name: ";
			getline(cin, name);
			cout << tabDepth << "Year started: ";
			getline(cin, year);

			//check that year inputs are numeric
			if (!isNum(year)){
				cout << tabDepth << "ERROR: Invalid year" << endl << endl;
				break;
			}

			command = "INSERT INTO Period VALUES FROM (\"" + name + "\", "
				+ year + ");";
			dbCon.executeCommand(command);

			cout << endl;
			tabDepth = "\t";
			break;
		case 2:
			cout << tabDepth << "Removing a period\n" << endl;

			cout << tabDepth << "Period name: ";
			getline(cin, name);

			command = "DELETE FROM Period WHERE (name == \"" + name + "\");";
			dbCon.executeCommand(command);

			cout << endl;
			tabDepth = "\t";
			break;
		case 3:
			cout << tabDepth << "List of all Periods\n" << endl;
			command = "SHOW Period;";
			dbCon.executeCommand(command);

			tabDepth = "\t";
			break;
		case 4:
			return;
		default:
			continue;
		}
	}
}

void createTablesForArtistDB()
{
	// Artist table
	string command = "CREATE TABLE Artist (name VARCHAR(30), birthYear INTEGER,"
		" nationality VARCHAR(20), deathYear INTEGER) PRIMARY KEY (name);";
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

void setWindow(int width, int height) 
{ 
    _COORD coord; 
    coord.X = width; 
    coord.Y = height; 

    _SMALL_RECT Rect; 
    Rect.Top = 0; 
    Rect.Left = 0; 
    Rect.Bottom = height - 1; 
    Rect.Right = width - 1; 

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
} 