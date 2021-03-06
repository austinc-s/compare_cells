#include "cell.hpp"

using namespace std;

/*parseKSFields
* The first three fields always have " " and the first field is not needed
* The next two fields only have " " if not NA type
* Returns 1 if pushed, 0 if not pushed
*/
CL::cellType* CL::Sample::parseKSFields(string inLine, int &added){
    vector <string> ar;
    int fieldNum = 0; //tracks which field we are parsing (0 index)
    bool checkedStrict = false;

    for(int i = 0; i < inLine.size(); i++){ //for each char in the line
        if(fieldNum == 3 && !checkedStrict){
            if(inLine[i+1] != '"'){
                added = 0;
                return (new CL::cellType);
            }
            else checkedStrict = true;
        }
        string token = "";
        bool tokenized = false;
        if(inLine[i] == '"'){
            i++;
            while(inLine[i] != '"'){
                token += inLine[i];
                i++;
            }
            fieldNum++;
            tokenized = true;
        }
        if(tokenized){
            ar.push_back(token);
        }
    }
    CL::cellType* tmp = makeCell(ar.at(1), ar.at(2), ar.at(3), ar.at(4));
    added = 1;
    return tmp;
}

/*parseSMFields
* Parses a scimap generated .csv file
* No need to exclude dropped cells because they are already filtered
* Parse by commas only UNLESS the last comma is immediately followed by a quote; there are 13 fields w no interior commas
*/
CL::cellType* CL::Sample::parseSMFields(std::string inLine){
    vector <string> ar;
    char delimiter = ',';
    string token = "";
    stringstream inlinestream(inLine);
    int fieldNum = 0; //tracks which field we are parsing (0 index)
    bool checkedLast = false;

    while(fieldNum < 13){ //parses fields 0-12 (guaranteed no "" in these fields)
        getline(inlinestream, token, delimiter);
        ar.push_back(token);
        fieldNum++;
    }

    //now need to check the last field
    int numCommas = 0;
    int index = 0;
    while(numCommas < 13){ //loop stops once index is at last comma
        if(inLine[index] == delimiter){ numCommas++;}
        index++;
    }
    if(inLine[index] == '"'){
        index++;
        token = "";
        while(inLine[index] != '"'){ token+=inLine[index]; index++;}
        ar.push_back(token);
    }
    else{
        getline(inlinestream, token, delimiter);
        ar.push_back(token);
    };

    CL::cellType* tmp = makeCell(extractSampleNum(ar.at(12)), extractIndex(ar.at(0)), ar.at(13), ar.at(13));
    return tmp;
}

/*countCells
* Iterates through the cells vector and counts the number of each cell type based on non_strictType
* Creates a struct for each cell type in cellTotals vector
*/
void CL::Sample::countCellsNonS(){
    if(cells.size() == 0){ return;}
    CL::cellTotal* tmp = new CL::cellTotal; //account for first iteration
    tmp->cellTypeName = cells.at(0)->non_strictType;
    tmp->count = 1;
    cellTotals.push_back(tmp);
    for(int i = 1; i < cells.size(); i++){
        bool found = false;
        for(int j = 0; j < cellTotals.size(); j++){
            if(compareNonStrict(cells.at(i)->non_strictType, cellTotals.at(j)->cellTypeName) == 0){
                found = true;
                (cellTotals.at(j)->count)++;
            }
        }
        if(!found){
            CL::cellTotal* tmp2 = new CL::cellTotal;
            tmp2->cellTypeName = cells.at(i)->non_strictType;
            tmp2->count = 1;
            cellTotals.push_back(tmp2);
        }
    }
}

/*makeCell()
* Allocates memory for a cellType struct and initializes fields with the function parameters
* Returns a pointer to the cellType struct
*/
CL::cellType* CL::Sample::makeCell(std::string sampleNum_in, std::string index_in, std::string strictType_in, std::string non_strictType_in){
    CL::cellType* tmp = new CL::cellType;
    tmp->sampleNum = sampleNum_in;
    tmp->index = index_in;
    tmp->strictType = strictType_in;
    tmp->non_strictType = non_strictType_in;
    return tmp;
}

/*extractSampleNum
* Returns string sampleNum (format #-#) from inNum (format #-#-trimmed)
*/
std::string CL::Sample::extractSampleNum(std::string inNum){
    int numDash = 0;
    string token = "";
    int index = 0;
    while(numDash < 2){
        if(inNum[index] == '-'){ numDash++;}
        if(numDash < 2){ token+=inNum[index]; index++;}
    }
    return token;
}

/*extractIndex
* Returns string index from inIndex (format #-#-trimmed_index)
*/
std::string CL::Sample::extractIndex(std::string inIndex){
    int index = 0;
    while(inIndex[index] != '_'){ index++;}
    index++; //increments to the character after _
    return(inIndex.substr(index, inIndex.size()));
}

/*compareNonStrict
* Compares two strings after changing first character of each to lower case
*/
int CL::Sample::compareNonStrict(string non_strict1, string non_strict2){ //this just accounts for "unknown" vs "Unknown"
    string tmp1, tmp2;
    tmp1 = non_strict1; tmp1[0] = tolower(tmp1[0]);
    tmp2 = non_strict2; tmp2[0] = tolower(tmp2[0]);
    return(tmp1.compare(tmp2));
}

/*Sample()
* Default constructor for Sample class
*/
CL::Sample::Sample(){}

/*~Sample()
* Destructor for Sample class
*/
CL::Sample::~Sample(){
    for(int i = 0; i < cells.size(); i++){ delete cells.at(i); cells.at(i) = NULL;}
    for(int i = 0; i < cellTotals.size(); i++){ delete cellTotals.at(i); cellTotals.at(i) = NULL;}
}