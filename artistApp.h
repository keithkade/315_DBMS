#include <string>

// Handle Artist selection from command line interface
void artistsSelected();


// Open (create and load) all database tables when application starts
void openAllTables();

// Save table data at program end
void closeAndSaveAllTables();

// Only need to use this function one time to create tables initially
// After one use it should be commented out otherwise it will overwrite table data
void createTablesForArtistDB();

void museumsSelected();

void worksSelected();

// Only need to use this function one time to create tables initially
// After one use it should be commented out otherwise it will overwrite table data
void createTablesForArtistDB();

//returns whether a file exists
bool exists(std::string filePath);