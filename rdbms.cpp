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
   Datum(string s) : stringData(s) {}

   //needed to compare rows and remove duplicates
   bool operator!=(const Datum &d){
	   return !(stringData == d.stringData && numData == d.numData);
   }
};

struct Table{

    vector<string> attributeNames;
    vector<vector<Datum> > data;
    
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
        allTables[tableName].data.push_back(newRow);   
    }
    
    void deleteFromTable(string tableName, vector<int> keyIndeces){
        //may need to adjust order of loop if vector auto resizes
        for (int i=0; i<keyIndeces.size(); i++){
            allTables[tableName].data[i].clear();
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
                allTables[tableName].data[keyIndeces[i]][attrIndeces[j]] = newValue[j];
            }
        }
    } 

    Table selectFromTable(string tableName,  vector<int> keyIndeces){
        Table retTable(allTables[tableName].attributeNames);
        
        for (int i=0; i<keyIndeces.size(); i++){
            vector<Datum> tempRow;
            tempRow = allTables[tableName].data[keyIndeces[i]];
            retTable.data.push_back(tempRow);
        }
        return retTable;
    }

	Table projectFromTable(string tableName, vector<string> attributeNames){
		Table wholeTable = allTables[tableName];
		Table projectionTable(attributeNames);

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

	void renameAttributes(string tableName, vector<string> attributeNames){
		allTables[tableName].attributeNames = attributeNames;
	}

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
		Table unionTable(allTables[tableName1].attributeNames);
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
		Table productTable(allAttributes);

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
    cout << dummyTable.data[0][0].stringData << " " << dummyTable.data[0][1].numData << endl;
    
    vector<string> attN;
    attN.push_back("age");
    vector<Datum> newV;
    Datum newAge(30);
    newV.push_back(newAge);

    db.updateTable("artists", attN, newV, zvector);
    dummyTable = db.selectFromTable("artists", zvector);
    cout << dummyTable.data[0][0].stringData << " " << dummyTable.data[0][1].numData << endl;
    
	system("pause");
}

