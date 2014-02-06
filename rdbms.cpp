/*
RDBMS: Kade Keith, Matthew Saari, Ryan Ledbetter, Victor Gutierrez
*/

// this is my commit VG
// testing RL

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

struct Datum{ 
   string stringData; 
   int numData; 
 
   Datum(int n){
      numData = n;
   }
   
   Datum(string s){
      numData = n
   }
};

struct Table{
    vector<pair<string, vector<Datum> > > attributes;
       
    Table(){}
};

int main(){
  
    //some examples of accessing the data
    Table dummyTable;
    vector<Datum> dummyColumn;
    Datum dummyDatum;
    dummyDatum.numData=10;
    dummyColumn.push_back(dummyDatum);
    pair<string, vector<Datum> > dummyPair;
    dummyPair.first = "attributeName";
    dummyPair.second = dummyColumn;
    dummyTable.attributes.push_back(dummyPair);
    
    cout << dummyTable.attributes[0].second[0].numData << endl;
}

