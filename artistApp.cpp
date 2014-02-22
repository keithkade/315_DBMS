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

		// switch based on user choice
		switch(selection)
		{
			case 1:

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
	string command = "CREATE TABLE Artists (name VARCHAR(30), birthDate VARCHAR(12),"
		" nationality VARCHAR(20), deathDate VARCHAR(20)) PRIMARY KEY (name);";
	dbCon.executeCommand(command);
	
	command = "WRITE Atists;";
	dbCon.executeCommand(command);
}