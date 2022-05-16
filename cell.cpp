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

/*Sample()
* Default constructor for Sample class
*/
CL::Sample::Sample(){}

/*~Sample()
* Destructor for Sample class
*/
CL::Sample::~Sample(){
    for(int i = 0; i < cells.size(); i++){ delete cells.at(i); cells.at(i) = NULL;}
}