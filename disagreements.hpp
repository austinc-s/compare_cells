#pragma once
#include "cell.hpp"

namespace DA{
    struct Difference{
        std::string dump;
        int type;
        CL::cellType *KScell;
        CL::cellType *SMcell;
    };

    class Disagreement{
        public:
            //constructors/destructors
            Disagreement();
            ~Disagreement();

            //public functions
            void displayDifferences();

            //getters
            std::vector <DA::Difference*> const &getDiffs(){ return diffs;}

            //setters
            void pushDifference(CL::cellType *KScell, CL::cellType *SMcell, int type);
        private:
            std::vector <DA::Difference*> diffs;

            std::string createDump(CL::cellType *cell1, CL::cellType *cell2, int type);
    };

}