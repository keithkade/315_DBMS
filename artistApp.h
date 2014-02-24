#include <string>

// Handle Artist selection from command line interface
void artistsSelected();

// Handle selction protion of artist menu
void showForArtists();

// Handle Museum selection from command line interface
void museumsSelected();

// Handle Work selection from command line interface
void worksSelected();

// Handle Period selection from command line interface
void periodsSelected();

// Open (create and load) all database tables when application starts
void openAllTables();

// Save table data at program end
void closeAndSaveAllTables();

// Only need to use this function one time to create tables initially
// After one use it should be commented out otherwise it will overwrite table data
void createTablesForArtistDB();

// Only need to use this function one time to create tables initially
// After one use it should be commented out otherwise it will overwrite table data
void createTablesForArtistDB();

//check whether a file exists
bool exists(std::string filePath);

//check if a string is numeric
bool isNum(std::string& input);

void setWindow(int width, int height);