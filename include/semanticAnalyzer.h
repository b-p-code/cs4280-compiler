// CS4280 P3 Bryce Paubel 11/5/22
// The header for the static semantic analyzer

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <string>

#include "compilerException.h"
#include "ptree.h"
#include "gnode.h"
#include "semanticStack.h"

class SemanticAnalyzer {
    private:
        SemanticStack stack;
        std::string source;
        std::string error(std::string, std::string,  int, int);
    public:
        // Analyze will need access to the PTree's data
        // Source found at: https://stackoverflow.com/questions/5208833/can-two-classes-friend-each-other
        friend class PTree;
        
        SemanticAnalyzer(std::string input) {
            source = input;
        }

        // Note that I would prefer analyze to return a boolean
        // but in reality I would like to keep all errors handled the same way
        // so I decided to just have it throw an exception in the case of an error
        PTree* analyze();

        void analyzeAux(GNode*);
};

class SemanticException : public CompilerException {
    public:
        SemanticException(std::string errorMsg) {
            message = errorMsg;
        }
};

#endif