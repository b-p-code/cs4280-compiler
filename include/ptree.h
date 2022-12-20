// CS4280 Bryce Paubel 10/20/22
// Header file for a tree for a parse tree with 5 children

// Note that some functions are taken from CS1250/2250's description
// and implementation of a binary tree

#ifndef PTREE_H
#define PTREE_H

#include <string>

#include "gnode.h"

// Note that the class is just declared here
// This is done so that there isn't a circular inclusion
// Source to solve this: https://cplusplus.com/forum/general/56475/
class SemanticAnalyzer;

class PTree {
    private:
        GNode* root;
        void destroySubTree(GNode*);
        std::string printPreOrderAux(GNode*, int);
    public:
        // Source found here: https://stackoverflow.com/questions/5208833/can-two-classes-friend-each-other
        // The SemanticAnalyzer class needs access to the root to perform analysis, as does codeGen
        friend class SemanticAnalyzer;
        friend class CodeGen;

        PTree();
        PTree(GNode*);
        ~PTree();
        void createTreeFromNode(GNode*);
        std::string printPreOrder();
};

#endif