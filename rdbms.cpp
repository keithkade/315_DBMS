/*
RDBMS: Kade Keith, Matthew Saari, Ryan Ledbetter, Victor Gutierrez
*/

// this is my commit VG
// testing RL

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

struct Datum{ 
   string stringData; 
   int numData; 
 
   Datum(int n) : numData(n) {}
   //initialize as -999 so we can check if a datum is a string or an int
   Datum(string s) : stringData(s), numData(-999) {} 

   //needed to compare rows and remove duplicates
   bool operator!=(const Datum &d){
	   return !(stringData == d.stringData && numData == d.numData);
   }
   
   bool operator==(const Datum &d){
	   return (stringData == d.stringData && numData == d.numData);
   }
};

struct Table{

    string key1;
    string key2;
    
    vector<string> attributeNames;
    vector<vector<Datum> > data;
    
    //keys are used to check for conflicts. Primary key is combination of key1 and key2
    Table(vector<string> attrNames, string inKey1, string inKey2){
        key1 = inKey1;
        key2 = inKey2;
        attributeNames = attrNames;
    }
    
    Table(){}
    
    bool duplicateExists(vector<Datum> newRow){
        //get indeces of primary keys
        vector<int> keyIndeces;
        int keyIndex1, keyIndex2;
        bool conflictFlag1 = false;
        bool conflictFlag2 = false;  
        bool conflictFound = false;
        for(int i=0; i<attributeNames.size(); i++){
            if (attributeNames[i]==key1)
                keyIndex1=i;
            if (attributeNames[i]==key2)
                keyIndex2=i;
        }
  
        //check for conflicts
        for(int i=0; i<data.size(); i++){
            if (data[i][keyIndex1] == newRow[keyIndex1])
                conflictFlag1 = true;
            if (data[i][keyIndex2] == newRow[keyIndex2])
                conflictFlag2 = true;
            if (conflictFlag1 && conflictFlag2)
                conflictFound=true;
            conflictFlag1 = false;
            conflictFlag2 = false;
        }      
        
        return conflictFound;
    }
    
    //for testing and debugging
    void printTable(){
        for (int i=0; i<attributeNames.size(); i++)
            cout << attributeNames[i] << ", ";
        
        cout << endl; 
        for (int i=0; i<data.size(); i++){
            for (int j=0; j<attributeNames.size(); j++){
                if (data[i][j].numData == -999)
                    cout << data[i][j].stringData << " ";
                else
                    cout << data[i][j].numData << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class Database{
    map<string, Table> allTables;
    
public:
    Database(){}
    
    void createTable(string tableName, vector<string> attrNames, string key1, string key2){   
        Table newTable(attrNames, key1, key2);
        allTables[tableName] = newTable;
    }
    
    void dropTable(string tableName){
        allTables.erase(tableName);
    }
    
    //make sure that there cannot be duplicate entities
    void insertIntoTable(string tableName, vector<Datum> newRow){
        if (!allTables[tableName].duplicateExists(newRow))
                allTables[tableName].data.push_back(newRow);
        else
            cout << "There is already an entry in the table with that key\n";
    }
    
    void deleteFromTable(string tableName, vector<int> keyIndeces){
        //may need to adjust order of loop if vector auto resizes
        for (int i=0; i<keyIndeces.size(); i++){
            allTables[tableName].data[i].clear();
        }     
    }
      
    //make sure that there cannot be duplicate entities
    void updateTable(string tableName, vector<string> attributeName, vector<Datum> newValue, vector<int> keyIndeces){     
        vector<Datum> dupRow;   //to check for conflicts
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
                dupRow = allTables[tableName].data[keyIndeces[i]];
                dupRow[attrIndeces[j]] = newValue[j];
                if (!allTables[tableName].duplicateExists(dupRow))
                        allTables[tableName].data[keyIndeces[i]][attrIndeces[j]] = newValue[j];
                else
                    cout << "That update would cause a conflict of primary keys\n";
            }
        }
    } 

    Table selectFromTable(string tableName,  vector<int> keyIndeces){
        Table retTable(allTables[tableName].attributeNames, allTables[tableName].key1, allTables[tableName].key2);
        
        for (int i=0; i<keyIndeces.size(); i++){
            vector<Datum> tempRow;
            tempRow = allTables[tableName].data[keyIndeces[i]];
            retTable.data.push_back(tempRow);
        }
        return retTable;
    }

	Table projectFromTable(string tableName, vector<string> attributeNames){
		Table wholeTable = allTables[tableName];
		Table projectionTable(attributeNames, allTables[tableName].key1, allTables[tableName].key2);

		//get indices in wholeTable of matching attributes
		vector<int> attIndices;
		for(int i = 0; i < attributeNames.size(); ++i){
			for(int j = 0; j < wholeTable.data[0].size(); ++j){
				if(attributeNames[i] == wholeTable.attributeNames[j]){
					attIndices.push_back(j);
					break;
				}
			}
		}

		for(int row = 0; row < wholeTable.data.size(); ++row){
			vector<Datum> newRow;
			for(int column = 0; column < attIndices.size(); ++column){
				Datum d(wholeTable.data[row][attIndices[column]]);
				newRow.push_back(d);
			}
			projectionTable.data.push_back(newRow);
		}
		return projectionTable;
	}

	Table renameAttributes(string tableName, vector<string> attributeNames){
		Table newNamesTable = allTables[tableName];
		newNamesTable.attributeNames = attributeNames;
		return newNamesTable;
	}
	
	//just see if two tables are union compatible(for setUnion and setDifference)
	bool unionCompatible(string tableName1, string tableName2){
		vector<string> atts1 = allTables[tableName1].attributeNames;
		vector<string> atts2 = allTables[tableName2].attributeNames;
		
		//if different size then not union compatible
		if(atts1.size() != atts2.size()){
			return false;
		}
		for(int i = 0; i < atts1.size(); ++i){
			//if different names then not union compatible
			if(atts1[i] != atts2[i]){
				return false;
			}
		}
		return true;
	}

	Table setUnion(string tableName1, string tableName2){
		Table unionTable(allTables[tableName1].attributeNames, allTables[tableName1].key1, allTables[tableName1].key2);
		if(!unionCompatible(tableName1, tableName2)){
			cout<<"Unable to perform set union on "<<tableName1<<" and "<<tableName2<<"."<<endl;
			return unionTable;	//empty table
		}

		//temp tables to make getting rows easier
		Table t1 = allTables[tableName1];
		Table t2 = allTables[tableName2];

		//inserting ALL rows from both tables into unionTable
		for(int i = 0; i < t1.data.size(); ++i){
			unionTable.data.push_back(t1.data[i]);
		}
		for(int i = 0; i < t2.data.size(); ++i){
			unionTable.data.push_back(t2.data[i]);
		}

		//now remove duplicates from unionTable
		//efficiency is not a concern in our system..
		for(int row = 0; row < unionTable.data.size(); ++row){
			//find duplicate rows
			for(int dupRow = 0; dupRow < unionTable.data.size(); ++dupRow){
				bool isDuplicate = true;
				//compare individual rows
				for(int col = 0; col < unionTable.data[0].size(); ++col){
					if(unionTable.data[row][col] != unionTable.data[dupRow][col]){
						isDuplicate = false;
						break;
					}
				}
				if(isDuplicate){
					unionTable.data.erase(unionTable.data.begin()+dupRow);
					//since vector resizes
					--dupRow;
				}
			}
		}
		return unionTable;
	}

	//compute minuend - subtrahend = diference
	Table setDifference(string tNameMinuend, string tNameSubtrahend){
		//difference is the minuend at beginning
		Table difference = allTables[tNameMinuend];
		Table subtrahend = allTables[tNameSubtrahend];

		if(!unionCompatible(tNameSubtrahend, tNameSubtrahend)){
			cout<<"Unable to perform set difference on "<<tNameMinuend<<" and "<<tNameSubtrahend<<"."<<endl;
			return difference;	//empty table
		}


		//run through all elements to subtract
		for(int sub = 0; sub < subtrahend.data.size(); ++sub){
			//run through all elements to subtract from
			for(int dif = 0; dif < difference.data.size(); ++dif){
				bool isEqual = true;
				//compare rows and delete from difference if equal
				for(int col = 0; col < difference.data[0].size(); ++col){
					if(difference.data[dif][col] != subtrahend.data[sub][col]){
						isEqual = false;
						break;
					}
				}
				if(isEqual){
					difference.data.erase(difference.data.begin()+dif);
					//since we assume no duplicates can be in table
					break;
				}
			}
		}

		return difference;
	}
    
	Table crossProduct(string tableName1, string tableName2){
		//temp tables to make getting rows easier
		Table t1 = allTables[tableName1];
		Table t2 = allTables[tableName2];
                
		//need to combine t1 attributes with t2 attributes..
		vector<string> allAttributes = t1.attributeNames;
		for(int i = 0; i < t2.attributeNames.size(); ++i){
			allAttributes.push_back(t2.attributeNames[i]);
		}
		Table productTable(allAttributes, allTables[tableName1].key1, allTables[tableName1].key2);

		//compute cross product
		for(int firstT = 0; firstT < t1.data.size(); ++firstT){
			for(int secondT = 0; secondT < t2.data.size(); ++secondT){
				//create newRow by combining t1 and t2 rows
				vector<Datum> newRow = t1.data[firstT];
				for(int col = 0; col < t2.data.size(); ++col){
					newRow.push_back(t2.data[secondT][col]);

				}
				productTable.data.push_back(newRow);
			}
		}
		
		return productTable;
	}

};


int main(){

    Database db;
    vector<string> attr;
    attr.push_back("name");
    attr.push_back("age");
    db.createTable("artists", attr, "name", "age");
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
  
    db.insertIntoTable("artists", banksy);   //insert conflict test 
    
    vector<Datum> banksy2;
    Datum banksyname2("Banksy");
    Datum banksyage2(41);
    banksy2.push_back(banksyname2);
    banksy2.push_back(banksyage2);
    db.insertIntoTable("artists", banksy2);
    
    vector<Datum> newVal;
    Datum newDat(40);
    newVal.push_back(newDat);
    vector<int> newInd;
    int newRowInd(1);
    newInd.push_back(newRowInd);
    vector<string> newAttr;
    string newAttrName = "age";
    newAttr.push_back(newAttrName);
    db.updateTable("artists", newAttr, newVal, newInd); //update conflict test
    
    vector<int> zvector;
    zvector.push_back(0); 
    Table dummyTable = db.selectFromTable("artists", zvector);
    dummyTable.printTable();
    
    vector<string> attN;
    attN.push_back("age");
    vector<Datum> newV;
    Datum newAge(30);
    newV.push_back(newAge);

    db.updateTable("artists", attN, newV, zvector);
    dummyTable = db.selectFromTable("artists", zvector);
    dummyTable.printTable();
    
    system("pause");
}

