#include "cell.hpp"

using namespace std;

/*parseFields
* The first three fields always have " " and the first field is not needed
* The next two fields only have " " if not NA type
* Returns 1 if pushed, 0 if not pushed
*/
CL::cellType* CL::Sample::parseFields(string inLine, int &added){
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
    //cells.push_back(tmp);
    added = 1;
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