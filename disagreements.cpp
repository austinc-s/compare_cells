#include "disagreements.hpp"

using namespace std;

void DA::Disagreement::displayDifferences(){
    cout << "Type,KS_sampleNum,KS_index,KS_strict,KS_non_strict,SM_sampleNum,SM_index,SM_strict,SM_non_strict" << std::endl;
    for(int i = 0; i < diffs.size(); i++){
        cout << diffs.at(i)->dump << std::endl;
    }
}

//Pushes next entry of all fields into the 
void DA::Disagreement::pushDifference(CL::cellType *KScell, CL::cellType *SMcell, int type){
    DA::Difference *tmp = new Difference;
    tmp->dump = createDump(KScell, SMcell, type);
    tmp->type = type;
    tmp->KScell = KScell;
    tmp->SMcell = SMcell;
    diffs.push_back(tmp);
}

//Creates a string that summarizes the difference between the cells
std::string DA::Disagreement::createDump(CL::cellType *KScell, CL::cellType *SMcell, int type){
    string token = "";
    token+=(to_string(type)); token+=',';
    token+=KScell->sampleNum; token+=',';
    token+=KScell->index; token+=',';
    token+=KScell->strictType; token+=',';
    token+=KScell->non_strictType; token+=',';
    token+=SMcell->sampleNum; token+=',';
    token+=SMcell->index; token+=',';
    token+=SMcell->strictType; token+=',';
    token+=SMcell->non_strictType;
    return token;
}

DA::Disagreement::Disagreement(){
    
}

DA::Disagreement::~Disagreement(){
    for(int i = 0; i < diffs.size(); i++){ delete diffs.at(i); diffs.at(i) = NULL;}
}