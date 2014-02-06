/*
RDBMS: Kade Keith, Matthew Saari, Ryan Ledbetter, Victor Gutierrez
*/

// this is my commit VG
// testing RL

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

struct Datum{ 
   string stringData; 
   int numData; 
 
   Datum(int n){
      numData = n;
   }
   
   Datum(string s){
      stringData = s;
   }
};

struct Table{
    //vector<pair<string, vector<Datum> > > attributes;
    vector<vector<Datum> > attributes;
    
       
    Table(){}
};

class Database{
    map<string, Table> allTables;
    
    Database(){}
};


int main(){
  
    //some examples of accessing the data
    Table dummyTable;
    vector<Datum> dummyRow;
    Datum dummyDatum(10);
    dummyRow.push_back(dummyDatum);
    dummyTable.attributes.push_back(dummyRow);
    
    cout << dummyTable.attributes[0][0].numData << endl;
}

