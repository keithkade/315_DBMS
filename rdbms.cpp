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

    vector<string> attributeNames;
    vector<vector<Datum> > attributes;
    
    Table(vector<string> attrNames){
        attributeNames = attrNames;
    }
    
    Table(){}
};

class Database{
    map<string, Table> allTables;
    
public:
    Database(){}
    
    void createTable(string tableName, vector<string> attrNames){
        Table newTable(attrNames);
        allTables[tableName] = newTable;
    }
    
    void dropTable(string tableName){
        allTables.erase(tableName);
    }
    
    void insertIntoTable(string tableName, vector<Datum> newRow){
        allTables[tableName].attributes.push_back(newRow);   
    }
    
    void deleteFromTable(string tableName, vector<int> keyIndeces){
        //may need to adjust order of loop if vector auto resizes
        for (int i=0; i<keyIndeces.size(); i++){
            allTables[tableName].attributes[i].clear();
        }     
    }
         
    void updateTable(string tableName, vector<string> attributeName, vector<Datum> newValue, vector<int> keyIndeces){
        vector<int> attrIndeces;
        //get the indeces of the attributes to update
        for (int i=0; i<attributeName.size(); i++){
            for (int j=0; j<allTables[tableName].attributeNames.size(); j++){
                if (allTables[tableName].attributeNames[j]==attributeName[i])
                   attrIndeces.push_back(j);     
            }
        }
        
        //update at the above indeces
        for (int i=0; i<keyIndeces.size(); i++){
            for (int j=0; j<attrIndeces.size(); j++){
                allTables[tableName].attributes[keyIndeces[i]][attrIndeces[j]] = newValue[j];
            }
        }
    } 

    Table selectFromTable(string tableName,  vector<int> keyIndeces){
        Table retTable(allTables[tableName].attributeNames);
        
        for (int i=0; i<keyIndeces.size(); i++){
            vector<Datum> tempRow;
            tempRow = allTables[tableName].attributes[keyIndeces[i]];
            retTable.attributes.push_back(tempRow);
        }
        return retTable;
    }
    
};


int main(){
    
    Database db;
    vector<string> attr;
    attr.push_back("name");
    attr.push_back("age");
    db.createTable("artists", attr);
    vector<Datum> pablo;
    Datum pabloname("Picasso");
    Datum pabloage(100);
    pablo.push_back(pabloname);
    pablo.push_back(pabloage);
    db.insertIntoTable("artists", pablo);
    
    vector<Datum> banksy;
    Datum banksyname("Banksy");
    Datum banksyage(40);
    banksy.push_back(banksyname);
    banksy.push_back(banksyage);
    db.insertIntoTable("artists", banksy);
    
    vector<int> zvector;
    zvector.push_back(0); 
    Table dummyTable = db.selectFromTable("artists", zvector);
    cout << dummyTable.attributes[0][0].stringData << " " << dummyTable.attributes[0][1].numData << endl;
    
    vector<string> attN;
    attN.push_back("age");
    vector<Datum> newV;
    Datum newAge(30);
    newV.push_back(newAge);

    db.updateTable("artists", attN, newV, zvector);
    dummyTable = db.selectFromTable("artists", zvector);
    cout << dummyTable.attributes[0][0].stringData << " " << dummyTable.attributes[0][1].numData << endl;
    
}

