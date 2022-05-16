#include "cell.hpp"

using namespace std;

long readFileLines(string inFileName, vector <string> &lineList);
int createKSSamples(vector <string> const &lineList, vector <CL::Sample*> &samples);
void createSMSample(vector <string> const &lineList, vector <CL::Sample*> &samples);
void displayAllSamples(vector <CL::Sample*> &samples);
void displaySample(CL::Sample *sample);

int main(int argc, char** argv){
    vector <CL::Sample*> KSsamples;
    vector <CL::Sample*> SMsamples;
    vector <string> lineListKS;
    vector <string> lineListSM;
    string inFileNameKS = "test_input_ks.csv";
    string inFileNameSM = "test_input_sm.csv";

    long inKSLength = readFileLines(inFileNameKS, lineListKS);
    cout << "Read KS file. Length: " << inKSLength << std::endl;

    long inSMLength = readFileLines(inFileNameSM, lineListSM) - 1; //-1 accounts for the header line
    lineListSM.erase(lineListSM.begin()); //delete header line
    cout << "Read SM file. Length: " << inSMLength << std::endl;


    int numSamples = createKSSamples(lineListKS, KSsamples);
    cout << "Created KS samples. Number: " << (numSamples+1) << std::endl;

    displayAllSamples(KSsamples);

    createSMSample(lineListSM, SMsamples);
    cout << "Created SM sample." << std::endl;

    displayAllSamples(SMsamples);

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
    for(int i = 0; i < lineList.size(); i++){
        int added;
        CL::cellType *tmpCell = samples.at(numSamples)->parseKSFields(lineList.at(i), added);
        if(i == 0 && (tmpCell->sampleNum).compare("") != 0){ currentSample = tmpCell->sampleNum; samples.at(numSamples)->pushCell(tmpCell);}
        else if(tmpCell->sampleNum.compare(currentSample) == 0){ samples.at(numSamples)->pushCell(tmpCell);}
        else if(tmpCell->sampleNum.compare("") != 0){
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