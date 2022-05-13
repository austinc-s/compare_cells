#include "cell.hpp"

using namespace std;

long readFileLines(string inFileName, vector <string> &lineList);
int createSamples(vector <string> const &lineList, vector <CL::Sample*> &samples);
void displayAllSamples(vector <CL::Sample*> &samples);
void displaySample(CL::Sample *sample);

int main(int argc, char** argv){
    vector <CL::Sample*> samples;
    vector <string> lineListKS;
    string inFileNameKS = "test_input_ks.csv";

    long inKSLength = readFileLines(inFileNameKS, lineListKS);
    cout << "Read file. Length: " << inKSLength << std::endl;

    int numSamples = createSamples(lineListKS, samples);
    cout << "Created samples. Number: " << (numSamples+1) << std::endl;

    displayAllSamples(samples);

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

int createSamples(vector <string> const &lineList, vector <CL::Sample*> &samples){
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
        CL::cellType *tmpCell = samples.at(numSamples)->parseFields(lineList.at(i), added);
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

void displayAllSamples(vector <CL::Sample*> &samples){
    for(int i = 0; i < samples.size(); i++){
        cout << "SAMPLE: " << samples.at(i)->getCells().at(0)->sampleNum << std::endl;
        displaySample(samples.at(i));
    }
}

void displaySample(CL::Sample *sample){
    for(int i = 0; i < sample->getCells().size(); i++){
        cout << sample->getCells().at(i)->sampleNum << ", " << sample->getCells().at(i)->index << ", " << sample->getCells().at(i)->strictType <<
            ", " << sample->getCells().at(i)->non_strictType << std::endl;
    }
}