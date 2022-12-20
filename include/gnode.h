// CS4280 Bryce Paubel 10/20/22
// A header file for a grammar node for a parse tree
// It is called a GNode because it is a 'grammar' node

#ifndef GNODE_H
#define GNODE_H

#include <string>

#include "token.h"

/* MAJOR NOTE - DEALLOCATING ONE GNODE WILL DEALLOCATE ALL NODES BELOW IT */
/* SINCE THIS CLASS IS SEPARATE FROM KTREE, IT MAKES MORE SENSE FOR IT TO HANDLE DEALLOCATING ITSELF */

class GNode {
    public:
        static const int NUM_OF_NODES = 5;      // Maximum number of nodes
        static const int NUM_OF_TOKENS = 5;     // Maximum number of tokens
        std::string nodeType;                   // Function which created the node
        Token* tokens[NUM_OF_NODES];            // Tokens within the node
        GNode* children[NUM_OF_NODES];          // List of children within the node

        GNode(std::string nodeType, Token* token, GNode* child) {
            this->nodeType = nodeType;
            this->tokens[0] = token;
            this->children[0] = child;

            for (int i = 1; i < this->NUM_OF_TOKENS; i++) {
                this->tokens[i] = nullptr;
            }

            for (int i = 1; i < this->NUM_OF_NODES; i++) {
                this->children[i] = nullptr;
            }
        }

        GNode() {
            this->nodeType = "N/A";
            for (int i = 0; i < this->NUM_OF_TOKENS; i++) {
                this->tokens[i] = nullptr;
            }

            for (int i = 0; i < this->NUM_OF_NODES; i++) {
                this->children[i] = nullptr;
            }
        }

        ~GNode() {
            for (int i = 0; i < this->NUM_OF_TOKENS; i++) {
                if (this->tokens[i]) {
                    delete this->tokens[i];
                }
            }

            for (int i = 0; i < this->NUM_OF_NODES; i++) {
                if (this->children[i]) {
                    delete this->children[i];
                }
            }
        }
};

#endif