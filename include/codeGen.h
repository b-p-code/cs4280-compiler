// CS4280 P4 Bryce Paubel 11/16/22
// Declaration for the code generation class

#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <string>
#include <vector>

#include "../include/gnode.h"
#include "../include/semanticAnalyzer.h"
#include "../include/semanticStack.h"
#include "../include/globalTable.h"

class CodeGen {
    private:
        std::string input;
        int varCount;
        int labelCount;

        std::string getTempVarName();
        std::string getTempLabelName();
        std::string getUserLabelName(int);

        SemanticStack stack;
        GlobalTable labels;

        void generateCodeAux(GNode*, std::string&);

    public:
        CodeGen(std::string input) {
            this->input = input;
            varCount = 0;
            labelCount = 0;
        }

        std::string generateCode();

        friend class PTree;
};

#endif