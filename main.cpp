#include "cell.hpp"
#include "disagreements.hpp"

using namespace std;

long readFileLines(string inFileName, vector <string> &lineList);
int createKSSamples(vector <string> const &lineList, vector <CL::Sample*> &samples);
void createSMSample(vector <string> const &lineList, vector <CL::Sample*> &samples);
void compareOutputs(vector <CL::Sample*> &KSsamples, vector <CL::Sample*> &SMsamples, vector <DA::Disagreement*> &difs);
void compareSamples(CL::Sample* KSsample, CL::Sample* SMsample, DA::Disagreement* difs);
int compareCells(CL::cellType *cell1, CL::cellType *cell2);
int compareNonStrict(string non_strict1, string non_strict2);
void countCells(vector <CL::Sample*> &samples);
void displayAllSamples(vector <CL::Sample*> &samples);
void displaySample(CL::Sample *sample);
void displayAllCellCounts(vector <CL::Sample*> &KSsamples, vector <CL::Sample*> &SMsamples);
void displayCellCount(CL::Sample *sample);
void writeAllCellCounts(vector <CL::Sample*> &KSsamples, vector <CL::Sample*> &SMsamples);

int main(int argc, char** argv){
    vector <CL::Sample*> KSsamples;
    vector <CL::Sample*> SMsamples;
    vector <string> lineListKS;
    vector <string> lineListSM;
    //string inFileNameKS = "test_input_ks.csv";
    string inFileNameKS = "ks_celltypes/daphne-cell-types_ks.csv";
    string inFileNameSM = "test_input_sm.csv";

    long inKSLength = readFileLines(inFileNameKS, lineListKS) - 1; //-1 accounts for the header line
    lineListKS.erase(lineListKS.begin()); //delete header line
    cout << "Read KS file. Length: " << inKSLength << std::endl;

    int numSamples = createKSSamples(lineListKS, KSsamples);
    cout << "Created KS samples. Number: " << (numSamples+1) << std::endl;
    //displayAllSamples(KSsamples);

    //TODO make this a loop to read in more than 1 file/create more than 1 SM Sample
    long inSMLength = readFileLines(inFileNameSM, lineListSM) - 1; //-1 accounts for the header line
    lineListSM.erase(lineListSM.begin()); //delete header line
    cout << "Read SM file. Length: " << inSMLength << std::endl;

    createSMSample(lineListSM, SMsamples);
    cout << "Created SM sample." << std::endl;
    //displayAllSamples(SMsamples);

    //vector <DA::Disagreement*> disagreements;
    //compareOutputs(KSsamples, SMsamples, disagreements);

    //disagreements.at(0)->displayDifferences();

    countCells(KSsamples);
    countCells(SMsamples);
    //displayAllCellCounts(KSsamples, SMsamples);
    writeAllCellCounts(KSsamples, SMsamples);

    return 0;
}

long readFileLines(string inFileName, vector <string> &lineList){
    ifstream infile; infile.open(inFileName);
    string tmp = "";
    int fileLength = 0;
    lineList.clear();
    while(getline(infile, tmp)){
        lineList.push_back(tmp);
        fileLength++;
    }
    infile.close();
    return fileLength;
}

int createKSSamples(vector <string> const &lineList, vector <CL::Sample*> &samples){
    //get line from lineList & parse
    //get sample # and compare to current sample #
    //if same sample, push to back of current Sample in samples
    //if different sample, create new Sample and push into back of samples
    CL::Sample *tmp = new CL::Sample();
    samples.push_back(tmp);
    int numSamples = 0; //0 indexed
    string currentSample = "";
    bool anyAdded = false;

    for(int i = 0; i < lineList.size(); i++){
        int added;
        CL::cellType *tmpCell = samples.at(numSamples)->parseKSFields(lineList.at(i), added);
        if(!anyAdded && added == 1){
            currentSample = tmpCell->sampleNum; 
            samples.at(numSamples)->pushCell(tmpCell);
            anyAdded = true;
        }
        else if(anyAdded && tmpCell->sampleNum.compare(currentSample) == 0){ samples.at(numSamples)->pushCell(tmpCell);}
        else if(anyAdded && tmpCell->sampleNum.compare("") != 0){
            if(currentSample.compare(tmpCell->sampleNum) != 0){ 
                currentSample = tmpCell->sampleNum; 
                numSamples++;
                //create new Sample object and push into samples vector
                tmp = new CL::Sample();
                tmp->pushCell(tmpCell);
                samples.push_back(tmp);
            }
        }
        else delete tmpCell;
    }
    return numSamples;
}

void createSMSample(vector <string> const &lineList, vector <CL::Sample*> &samples){
    CL::Sample *tmp = new CL::Sample();
    for(int i = 0; i < lineList.size(); i++){
        CL::cellType *tmpCell = tmp->parseSMFields(lineList.at(i));
        tmp->pushCell(tmpCell);
    }
    samples.push_back(tmp);
}

void compareOutputs(vector <CL::Sample*> &KSsamples, vector <CL::Sample*> &SMsamples, vector <DA::Disagreement*> &difs){
    if(KSsamples.size() != SMsamples.size()){
        cout << "ERROR: Number of samples is not the same.\n";
        return;
    }
    for(int i = 0; i < KSsamples.size(); i++){
        DA::Disagreement *tmp = new DA::Disagreement();
        compareSamples(KSsamples.at(i), SMsamples.at(i), tmp);
        difs.push_back(tmp);
    }
}

void compareSamples(CL::Sample* KSsample, CL::Sample* SMsample, DA::Disagreement* difs){ //helper for compareOutputs
    if(KSsample->getCells().size() != SMsample->getCells().size()){
        cout << "ERROR: Number of cells is not the same.\n";
        return;
    }
    for(int i = 0; i < KSsample->getCells().size(); i++){
        int result = compareCells(KSsample->getCells().at(i), SMsample->getCells().at(i));
        if(result != 0){ difs->pushDifference(KSsample->getCells().at(i), SMsample->getCells().at(i), result);}
    }
}

/*compareCells
* Determines if 2 cells contain the same values
* Returns 0 if same. If not same, returns 1 for dif sample #, 2 for dif index, 3 for dif strictType, 4 for dif non_strictType
*/
int compareCells(CL::cellType *cell1, CL::cellType *cell2){
    if(cell1->sampleNum.compare(cell2->sampleNum) != 0){ return 1;}
    else if(cell1->index.compare(cell2->index) != 0){ return 2;}
    //else if(cell1->strictType.compare(cell2->strictType) != 0){ return 3;} //ignore strict since scimap has no inconc equivalent
    else if(compareNonStrict(cell1->non_strictType, cell2->non_strictType) != 0){ return 4;}
    else return 0;
}

int compareNonStrict(string non_strict1, string non_strict2){ //this just accounts for "unknown" vs "Unknown"
    string tmp1, tmp2;
    tmp1 = non_strict1; tmp1[0] = tolower(tmp1[0]);
    tmp2 = non_strict2; tmp2[0] = tolower(tmp2[0]);
    return(tmp1.compare(tmp2));
}

void countCells(vector <CL::Sample*> &samples){
    for(int i = 0; i < samples.size(); i++){
        samples.at(i)->countCellsNonS();
    }
}

void displayAllSamples(vector <CL::Sample*> &samples){
    for(int i = 0; i < samples.size(); i++){
        displaySample(samples.at(i));
    }
}

void displaySample(CL::Sample *sample){
    for(int i = 0; i < sample->getCells().size(); i++){
        cout << sample->getCells().at(i)->sampleNum << ", " << sample->getCells().at(i)->index << ", " << sample->getCells().at(i)->strictType <<
            ", " << sample->getCells().at(i)->non_strictType << std::endl;
    }
}

void displayAllCellCounts(vector <CL::Sample*> &KSsamples, vector <CL::Sample*> &SMsamples){
    for(int i = 0; i < KSsamples.size(); i++){
        if(KSsamples.at(i)->getCells().size() > 0){ cout << "KS-SAMPLE " << KSsamples.at(i)->getCells().at(0)->sampleNum << std::endl;}
        displayCellCount(KSsamples.at(i));
    }
    for(int i = 0; i < SMsamples.size(); i++){
        if(SMsamples.at(i)->getCells().size() > 0){ cout << "SM-SAMPLE " << SMsamples.at(i)->getCells().at(0)->sampleNum << std::endl;}
        displayCellCount(SMsamples.at(i));
    }
}

void displayCellCount(CL::Sample *sample){
    for(int i = 0; i < sample->getCellTotals().size(); i++){
        cout << sample->getCellTotals().at(i)->cellTypeName << ": " << sample->getCellTotals().at(i)->count << std::endl;
    }
}

void writeAllCellCounts(vector <CL::Sample*> &KSsamples, vector <CL::Sample*> &SMsamples){
    string ofdir = "cell_counts/";
    ofstream ofile;
    for(int i = 0; i < KSsamples.size(); i++){
        string sampleNum;
        if(KSsamples.at(i)->getCells().size() > 0){ 
            sampleNum = KSsamples.at(i)->getCells().at(0)->sampleNum;
            string ofilepath = ofdir + sampleNum + ".csv";
            ofile.open(ofilepath);

            for(int j = 0; j < KSsamples.at(i)->getCellTotals().size(); j++){
                ofile << '"' << KSsamples.at(i)->getCellTotals().at(j)->cellTypeName << '"' << "," << KSsamples.at(i)->getCellTotals().at(j)->count << std::endl;
            }
            //TODO now append the SMsamples numbers
            ofile.close();
        }
    }
}