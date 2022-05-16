#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

namespace CL{
    struct cellType{
        std::string sampleNum;
        std::string index;
        std::string strictType;
        std::string non_strictType;
    };

    struct cellTotal{
        std::string cellTypeName;
        int count;
    };


    class Sample{
        public:
            //constructors/destructors
            Sample();
            ~Sample();

            //public functions
            CL::cellType* parseKSFields(std::string inLine, int &added);
            CL::cellType* parseSMFields(std::string inLine);

            void countCellsNonS();

            //getters
            std::vector <CL::cellType*> const &getCells(){return cells;}
            std::vector <CL::cellTotal*> const &getCellTotals(){ return cellTotals;}

            //setters
            void pushCell(CL::cellType *cell){ cells.push_back(cell);}
        private:
            std::vector <CL::cellType*> cells;
            std::vector <CL::cellTotal*> cellTotals;

            CL::cellType *makeCell(std::string sampleNum_in, std::string index_in, std::string strictType_in, std::string non_strictType_in);
            std::string extractSampleNum(std::string inNum);
            std::string extractIndex(std::string inIndex);
            int compareNonStrict(std::string non_strict1, std::string non_strict2);
    };
}