// CS4280 P3 Bryce Paubel 11/5/22
// The definitions for the static semantic analyzer

#include <iostream>

#include "../include/parser.h"
#include "../include/semanticAnalyzer.h"
#include "../include/ptree.h"
#include "../include/gnode.h"
#include "../include/token.h"

// To view the stack growth and the checking of variables
// set this value to true
const bool SEMANTIC_DEBUG_MESSAGES = false;

/*
    analyze
    Analyzes the static semantics of the source attribute in the class

    Input: void

    Output: PTree*
    A valid parse tree that has been analyzed (if invalid, exception is thrown)
*/
PTree* SemanticAnalyzer::analyze() {
    PTree* tree = nullptr;
    try {
        Parser parser(source);
        tree = parser.parse();
        analyzeAux(tree->root);
        return tree;
    } catch (CompilerException e) {
        if (tree) {
            delete tree;
        }
        throw;
    }
}

/*
    analyzeAux
    Auxiliary function to analyze the nodes in a given tree

    Input: GNode*
    The node to analyze

    Output: void
*/
void SemanticAnalyzer::analyzeAux(GNode* node) {
    // Try block to handle semantic exceptions
    try {

        // Check if there are too many variables
        if (stack.size() > 100) {
            throw SemanticException(
                "SEMANTIC ERROR: Too many variables! Only up to 100 variables are allowed in any given program\n"
            );
        }

        // Only check node if it contains information
        // We also have to be sure that we don't check the ID tokens in the warp/label statements
        // THIS WAS ALLOWED BY PROFESSOR HAUSCHILD
        if (node != nullptr && node->nodeType != "label" && node->nodeType != "goTo (goto)") {
            // If we have a block or a program node, we could potentionally have variables
            if (node->nodeType == "block" || node->nodeType == "program") {
                int varCount = 0;

                // Check vars nodes (variable declarations)
                GNode* tempNode = node->children[0];
                while (tempNode) {
                    // If a variable already exists in a new definition, throw an error
                    if (stack.find(tempNode->tokens[1]->tokenInstance) != -1) {
                        throw SemanticException(
                            error(tempNode->tokens[1]->tokenInstance, "is redefined", tempNode->tokens[1]->lineNum, tempNode->tokens[1]->charNum)
                        );
                    // Otherwise, iterate the variable count and store the variable
                    } else {
                        stack.push(tempNode->tokens[1]->tokenInstance);

                        // Debug messages for interactive viewing of stack growth
                        if (SEMANTIC_DEBUG_MESSAGES) {
                            std::cout << varCount + 1 << std::endl; 
                            stack.display();
                            std::cout << std::endl;
                        }
                        varCount++;
                    }

                    // Check if this vars node has any other vars nodes
                    tempNode = tempNode->children[0];
                }

                // Recursively check children
                for (int i = 0; i < node->NUM_OF_NODES; i++) {
                    analyzeAux(node->children[i]);
                }

                // Remove any variables within this scope afterwards
                for (int i = 0; i < varCount; i++) {
                    stack.pop();

                    // Debug messages for viewing of stack growth
                    if (SEMANTIC_DEBUG_MESSAGES) {
                        std::cout << varCount << std::endl;
                        if (stack.empty()) {
                            std::cout << "empty" << std::endl;
                        } else {
                            stack.display();
                        }
                        std::cout << std::endl;
                    }

                }

            } else {
                // If we aren't in a block, we must check if there are any ID tokens
                // used in the other nodes. Vars will be traversed here, but only after
                // being found in a block or program node, so it will still be valid
                for (int i = 0; i < node->NUM_OF_TOKENS; i++) {
                    // Check if any stored tokens are ID tokens
                    if (node->tokens[i] != nullptr && node->tokens[i]->tokenID == IDtk) {
                        // If ID token is not on the stack (i.e. not defined), throw an error
                        if (stack.find(node->tokens[i]->tokenInstance) == -1) {
                            throw SemanticException(
                                error(node->tokens[i]->tokenInstance, "is not defined", node->tokens[i]->lineNum, node->tokens[i]->charNum)
                            );
                        }

                        // Semantic debug messages to view which variables are being found
                        if (SEMANTIC_DEBUG_MESSAGES) {
                            std::cout << "Variable " << node->tokens[i]->tokenInstance << " is valid in ";
                            std::cout << node->nodeType << std::endl << std::endl;
                        }
                    }
                }

                // Recursively check children
                for (int i = 0; i < node->NUM_OF_NODES; i++) {
                    analyzeAux(node->children[i]);
                }
            }
        }
    
    // Handle and rethrow any exceptions
    } catch (CompilerException e) {
        throw;
    }
}

/*
    error
    Generates error messages

    Input: string, string, int, int
    The name of the variable, the error that occurred, the line number, and the character number

    Output: string
    The generated error message
*/
std::string SemanticAnalyzer::error(std::string varName, std::string specificError, int lineNum, int charNum) {
    return "SEMANTIC ERROR: " + varName + " " + specificError + ". Found at line " + std::to_string(lineNum) + ", char " + std::to_string(charNum) + "\n";
}