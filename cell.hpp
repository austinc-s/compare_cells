#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

namespace CL{
    struct cellType{
        std::string sampleNum;
        std::string index;
        std::string strictType;
        std::string non_strictType;
    };


    class Sample{
        public:
            //constructors/destructors
            Sample();
            ~Sample();

            //public functions
            int parseFields(std::string inLine);

            //getters
            std::vector <CL::cellType*> const &getCells(){return cells;}
        private:
            std::vector <CL::cellType*> cells;

            CL::cellType *makeCell(std::string sampleNum_in, std::string index_in, std::string strictType_in, std::string non_strictType_in);
    };
}