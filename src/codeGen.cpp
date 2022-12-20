// CS4280 P4 Bryce Paubel 11/16/22
// Definition for the code generation class

#include <algorithm>
#include <string>
#include <iostream>

#include "../include/token.h"
#include "../include/codeGen.h"
#include "../include/compilerException.h"
#include "../include/semanticAnalyzer.h"

/*
    NOTE FOR VARIABLE ALLOCATION:
    ALL VARIABLE ARE TREATED AS LOCAL, I.E. ALL VARIABLE ARE PUSHED TO STACK
*/

/*
    getTempVarName
    Returns a new temporary variable name

    Input: void

    Output: string
    The new temporary variable name
*/
std::string CodeGen::getTempVarName() {
    return "T" + std::to_string(this->varCount++);
}

/*
    getTempLabelName
    Returns a new temporary label name

    Input: void

    Output: string
    The new temporary label name
*/
std::string CodeGen::getTempLabelName() {
    return "L" + std::to_string(this->labelCount++);
}

/*
    getUserLabelName
    Returns a the label name for a given label on the stack.
    This scheme avoids any naming collisions, and is global

    Input: int
    A unique position for a given label

    Output: string
    The new temporary label name
*/
std::string CodeGen::getUserLabelName(int stackVal) {
    return "USERLABEL" + std::to_string(stackVal);
}

/*
    generateCode
    Generates out target assembly code based on the seeded input string

    Input: void

    Output: string
    The new assembly program based on translation
*/
std::string CodeGen::generateCode() {
    PTree* parseTree = nullptr;
    try {
        // Get a parse tree through analysis
        SemanticAnalyzer statSem(this->input);
        parseTree = statSem.analyze();

        // Get output from auxiliary code generation
        std::string output = "";
        generateCodeAux(parseTree->root, output);   

        // Delete the parse tree
        if (parseTree) {
            delete parseTree;
        }

        // Return the generated code
        return output;
    } catch (CompilerException e) {
        if (parseTree) {
            delete parseTree;
        }
        throw;
    }
}

/*
    generateCodeAux
    Generates out target assembly code based on the seeded input string

    Input: GNode*, string
    The current node, and the current output string

    Output: void
    The new assembly program based on translation
*/
void CodeGen::generateCodeAux(GNode* node, std::string& output) {
    // C++ does not allow strings to be used for switch cases, so an
    // else if is needed
    if (node) {
        /* PROGRAM NODE */
        if (node->nodeType == "program") {
            int scopeVarCount = 0;                  // Number of vars in a given block/program
            GNode* tempNode = node->children[0];    // Temporary node variable to traverse vars nodes

            // Traverse down vars children and allocate memory
            while (tempNode) {
                scopeVarCount++;
                std::string varName = tempNode->tokens[1]->tokenInstance;
                stack.push(varName);
                output += "PUSH\n";
                output += "LOAD " + tempNode->tokens[3]->tokenInstance + "\n"; 
                output += "STACKW " + std::to_string(stack.find(varName)) + "\n";
                tempNode = tempNode->children[0];
            }

            // Traverse children left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }

            // Variable deallocations
            for (int i = 0; i < scopeVarCount; i++) {
                stack.pop();
                output += "POP\n";
            }

            // Storage allocations for the temporaries
            output += "STOP\n";
            for (int i = 0; i < varCount; i++) {
                output += "T" + std::to_string(i) + " 0\n";
            }

            // Remove the last newline on the string
            output.pop_back();

        /* BLOCK NODE */
        } else if (node->nodeType == "block") {
            int scopeVarCount = 0;                  // Number of variables in a given program/block node
            GNode* tempNode = node->children[0];    // Temporary node to travel down vars nodes

            // Storage allocations for local vars on the stack
            while (tempNode) {
                scopeVarCount++;
                std::string varName = tempNode->tokens[1]->tokenInstance;
                stack.push(varName);
                output += "PUSH\n";
                output += "LOAD " + tempNode->tokens[3]->tokenInstance + "\n"; 
                output += "STACKW " + std::to_string(stack.find(varName)) + "\n";
                tempNode = tempNode->children[0];
            }

            // Traverse left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }

            // Deallocate locals
            for (int i = 0; i < scopeVarCount; i++) {
                stack.pop();
                output += "POP\n";
            }

        /* VARS NODE */
        } else if (node->nodeType == "vars") {
            return; // Vars node memory is handled in program/block
        
        /* EXPR NODE */
        } else if (node->nodeType == "expr") {
            // Since '-' is right recursive, we will
            // evaluate the right values first

            // If we have a subtraction child, perform a subtraction
            // (this is done since expr' will not contain the left values)
            if (node->children[1]) {
                std::string tempVar = getTempVarName();

                // Generate the value on the right (since it is right-recursive)
                generateCodeAux(node->children[1], output);
                
                // Store the calculated value
                output += "STORE " + tempVar + "\n";

                // Generate value on the left
                generateCodeAux(node->children[0], output);

                // Subtract
                output += "SUB " + tempVar + "\n";

            } else {
                // Traverse left to right
                for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                    generateCodeAux(node->children[i], output);
                }
            }

        /* EXPR_ (EXPR') NODE */
        } else if (node->nodeType == "expr_ (expr')") {
            // Subtraction is performed earlier in expr, so only follow
            // the expression node to get its value into the accumulator
            generateCodeAux(node->children[0], output);
            
        /* N NODE */    
        } else if (node->nodeType == "N") {
            // Since '+' and '*' are right recursive, we will
            // evaluate them first

            // If we have a multiplication/addition child, perform a subtraction
            // (this is done since N' will not contain the left values)
            if (node->children[1]) {
                std::string tempVar = getTempVarName();

                // Get the right value (since this is right-recursive)
                generateCodeAux(node->children[1], output);
                
                // Store the calculated value
                output += "STORE " + tempVar + "\n";

                // Get the left value into the accumulator
                generateCodeAux(node->children[0], output);

                // Perform the proper operation
                if (node->children[1]->tokens[0]->tokenID == ADDtk) {
                    output += "ADD " + tempVar + "\n";
                } else {
                    output += "MULT " + tempVar + "\n";
                }
            } else {
                // Traverse left to right
                for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                    generateCodeAux(node->children[i], output);
                }
            }

        /* N_ (N') NODE */
        } else if (node->nodeType == "N_ (N')") {
            // Multiplication/addition is performed earlier in N, so only
            // worry about getting value into accumulator
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }
        
        /* A NODE */
        } else if (node->nodeType == "A") {
            // Since '/' is left recursive and division only occurs
            // in the A' child, the code generation must happen there

            // Traverse left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }

        /* A_ (A') NODE */   
        } else if (node->nodeType == "A_ (A')") {
            // Generates code to take the value currently in the
            // accumulator, store it, get the value from the left child,
            // get the value from the right child, and then divide it
            if (node->tokens[0]) {
                std::string leftTempVar = getTempVarName();
                std::string rightTempVar = getTempVarName();
                output += "STORE " + leftTempVar + "\n";
                generateCodeAux(node->children[0], output);
                output += "STORE " + rightTempVar + "\n";
                output += "LOAD " + leftTempVar + "\n";
                output += "DIV " + rightTempVar + "\n";
                generateCodeAux(node->children[1], output);
            }

        /* M NODE */
        } else if (node->nodeType == "M") {
            // If we have the negation operation we must evalue what is in M first
            if (node->tokens[0] && node->tokens[0]->tokenID == COLtk) {
                generateCodeAux(node->children[0], output);
                
                // Negate the output by multiplying it by -1
                output += "MULT -1\n";
            } else {
                generateCodeAux(node->children[0], output);
            }

        /* R NODE */
        } else if (node->nodeType == "R") {
            // Continue traversing if we have an expression node
            if (node->children[0]) {
                generateCodeAux(node->children[0], output);
            } else {
                // Retrieve either the value from an IDtk or an INTtk
                if (node->tokens[0]->tokenID == IDtk) {
                    output += "STACKR ";
                    output += std::to_string(stack.find(node->tokens[0]->tokenInstance));
                    output += "\n";
                } else {
                    output += "LOAD " + node->tokens[0]->tokenInstance + "\n";
                }
            }

        /* STATS NODE */
        } else if (node->nodeType == "stats") {
            // Traverse left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }
        
        /* MSTAT NODE */
        } else if (node->nodeType == "mStat") {
            // Traverse left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }

        /* STAT NODE */
        } else if (node->nodeType == "stat") {
            // Traverse left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }

        /* IN NODE */
        } else if (node->nodeType == "in") {
            // Get a temporary value name and the identifier in the input statement
            std::string tempVar = getTempVarName();
            std::string varName = node->tokens[1]->tokenInstance;

            // Read in the value into a temporary value
            // and then write it into the stack position where
            // the value is stored
            output += "READ " + tempVar + "\n";
            output += "LOAD " + tempVar + "\n";
            output += "STACKW " + std::to_string(stack.find(varName)) + "\n";

            // Traverse left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }

        /* OUT NODE */
        } else if (node->nodeType == "out") {
            std::string tempVar = getTempVarName();

            // Evaluate the expr
            generateCodeAux(node->children[0], output);

            output += "STORE " + tempVar + "\n";
            output += "WRITE " + tempVar + "\n";

        /* IF NODE */
        } else if (node->nodeType == "iff (if)") {
            // Case without pick statement
            if (!node->children[4]) {
                std::string endLabel = getTempLabelName();
                std::string startLabel = getTempLabelName();
                if (node->children[1]->tokens[0]->tokenID == L_BRACKtk) {
                    std::string zeroCheckLabelLeft = getTempLabelName();
                    std::string zeroCheckLabelRight = getTempLabelName();
                    std::string relOpVarLeft = getTempVarName();
                    std::string relOpVarRight = getTempVarName();
                    generateCodeAux(node->children[2], output);
                    output += "STORE " + relOpVarLeft + "\n";
                    generateCodeAux(node->children[0], output);
                    output += "STORE " + relOpVarRight + "\n";
                    output += "MULT " + relOpVarLeft + "\n";

                    // If negative accumulator, signs are not the same
                    output += "BRNEG " + endLabel + "\n";
                    // If positive accumulator, signs are the same
                    output += "BRPOS " + startLabel + "\n";

                    // If we are zero check to make sure
                    // the special cases
                    output += "LOAD " + relOpVarLeft + "\n";
                    output += "BRZERO " + zeroCheckLabelLeft + "\n";

                    // If we aren't negative, check to make sure
                    // the special cases of zero
                    output += "LOAD " + relOpVarRight + "\n";
                    output += "BRZERO " + zeroCheckLabelRight + "\n";

                    // Check zero case
                    output += zeroCheckLabelLeft + ": NOOP\n";
                    output += "LOAD " + relOpVarRight + "\n";
                    output += "BRNEG " + endLabel + "\n";

                    // Check zero case
                    output += zeroCheckLabelRight + ": NOOP\n";
                    output += "LOAD " + relOpVarLeft + "\n";
                    output += "BRNEG " + endLabel + "\n";

                } else {
                    // Get the expression values and subtract
                    std::string relOpVar = getTempVarName();
                    generateCodeAux(node->children[2], output);
                    output += "STORE " + relOpVar + "\n";
                    generateCodeAux(node->children[0], output);
                    output += "SUB " + relOpVar + "\n";

                    // Generate proper branching based on relational operator
                    if (node->children[1]->tokens[0]->tokenID == EQtk) {
                        output += "BRNEG " + endLabel + "\n";
                        output += "BRPOS " + endLabel + "\n";
                    } else if (node->children[1]->tokens[0]->tokenID == NEQtk) {
                        output += "BRZERO " + endLabel + "\n";
                    } else if (node->children[1]->tokens[0]->tokenID == GTtk) {
                        output += "BRZNEG " + endLabel + "\n";
                    } else {
                        output += "BRZPOS " + endLabel + "\n";
                    }
                }

                output += startLabel + ": NOOP\n";
                generateCodeAux(node->children[3], output);
                output += endLabel + ": NOOP\n";

            // With pick statement
            } else {
                std::string endLabel = getTempLabelName();
                std::string pickLabel = getTempLabelName();
                std::string startLabel = getTempLabelName();
                if (node->children[1]->tokens[0]->tokenID == L_BRACKtk) {
                    std::string zeroCheckLabelLeft = getTempLabelName();
                    std::string zeroCheckLabelRight = getTempLabelName();
                    std::string relOpVarLeft = getTempVarName();
                    std::string relOpVarRight = getTempVarName();
                    generateCodeAux(node->children[2], output);
                    output += "STORE " + relOpVarLeft + "\n";
                    generateCodeAux(node->children[0], output);
                    output += "STORE " + relOpVarRight + "\n";
                    output += "MULT " + relOpVarLeft + "\n";

                    // If negative accumulator, signs are not the same
                    output += "BRNEG " + pickLabel + "\n";
                    // If positive accumulator, signs are the same
                    output += "BRPOS " + startLabel + "\n";

                    // If we aren't negative, check to make sure
                    // the special cases of zero
                    output += "LOAD " + relOpVarLeft + "\n";
                    output += "BRZERO " + zeroCheckLabelLeft + "\n";

                    // If we aren't negative, check to make sure
                    // the special cases of zero
                    output += "LOAD " + relOpVarRight + "\n";
                    output += "BRZERO " + zeroCheckLabelRight + "\n";

                    // Check zero case
                    output += zeroCheckLabelLeft + ": NOOP\n";
                    output += "LOAD " + relOpVarRight + "\n";
                    output += "BRNEG " + pickLabel + "\n";

                    // Check zero case
                    output += zeroCheckLabelRight + ": NOOP\n";
                    output += "LOAD " + relOpVarLeft + "\n";
                    output += "BRNEG " + pickLabel + "\n";

                } else {
                    std::string relOpVar = getTempVarName();
                    generateCodeAux(node->children[2], output);
                    output += "STORE " + relOpVar + "\n";
                    generateCodeAux(node->children[0], output);
                    output += "SUB " + relOpVar + "\n";

                    if (node->children[1]->tokens[0]->tokenID == EQtk) {
                        output += "BRNEG " + pickLabel + "\n";
                        output += "BRPOS " + pickLabel + "\n";
                    } else if (node->children[1]->tokens[0]->tokenID == NEQtk) {
                        output += "BRZERO " + pickLabel + "\n";
                    } else if (node->children[1]->tokens[0]->tokenID == GTtk) {
                        output += "BRZNEG " + pickLabel + "\n";
                    } else {
                        output += "BRZPOS " + pickLabel + "\n";
                    }
                }

                output += startLabel + ": NOOP\n";
                generateCodeAux(node->children[3], output);
                output += "BR " + endLabel + "\n";
                output += pickLabel + ": NOOP\n";
                generateCodeAux(node->children[4], output);
                output += endLabel + ": NOOP\n";
            }

        /* IF' NODE */
        } else if (node->nodeType == "iff_ (if')") {
            // Traverse left to right
            for (int i = 0; i < GNode::NUM_OF_NODES; i++) {
                generateCodeAux(node->children[i], output);
            }

        /* LOOP NODE */
        } else if (node->nodeType == "loop") {
            std::string startLabel = getTempLabelName();
            std::string endLabel = getTempLabelName();
            std::string blockLabel = getTempLabelName();

            output += startLabel + ": NOOP\n";

            if (node->children[1]->tokens[0]->tokenID == L_BRACKtk) {
                std::string zeroCheckLabelLeft = getTempLabelName();
                std::string zeroCheckLabelRight = getTempLabelName();
                std::string relOpVarLeft = getTempVarName();
                std::string relOpVarRight = getTempVarName();
                generateCodeAux(node->children[2], output);
                output += "STORE " + relOpVarLeft + "\n";
                generateCodeAux(node->children[0], output);
                output += "STORE " + relOpVarRight + "\n";
                output += "MULT " + relOpVarLeft + "\n";

                // If negative accumulator, signs are not the same
                output += "BRNEG " + endLabel + "\n";

                // If positive accumulator, signs are the same
                output += "BRPOS " + blockLabel + "\n";

                // If we aren't negative or positive, check to make sure
                // the special cases of zero
                output += "LOAD " + relOpVarLeft + "\n";
                output += "BRZERO " + zeroCheckLabelLeft + "\n";

                // If we aren't negative, check to make sure
                // the special cases of zero
                output += "LOAD " + relOpVarRight + "\n";
                output += "BRZERO " + zeroCheckLabelRight + "\n";

                // Check zero case
                output += zeroCheckLabelLeft + ": NOOP\n";
                output += "LOAD " + relOpVarRight + "\n";
                output += "BRNEG " + endLabel + "\n";

                // Check zero case
                output += zeroCheckLabelRight + ": NOOP\n";
                output += "LOAD " + relOpVarLeft + "\n";
                output += "BRNEG " + endLabel + "\n";

            } else {
                std::string relOpVar = getTempVarName();
                generateCodeAux(node->children[2], output);
                output += "STORE " + relOpVar + "\n";
                generateCodeAux(node->children[0], output);
                output += "SUB " + relOpVar + "\n";

                if (node->children[1]->tokens[0]->tokenID == EQtk) {
                    output += "BRNEG " + endLabel + "\n";
                    output += "BRPOS " + endLabel + "\n";
                } else if (node->children[1]->tokens[0]->tokenID == NEQtk) {
                    output += "BRZERO " + endLabel + "\n";
                } else if (node->children[1]->tokens[0]->tokenID == GTtk) {
                    output += "BRZNEG " + endLabel + "\n";
                } else {
                    output += "BRZPOS " + endLabel + "\n";
                }
            }

            output += blockLabel + ": NOOP\n";
            generateCodeAux(node->children[3], output);
            output += "BR " + startLabel + "\n";
            output += endLabel + ": NOOP\n";

        /* ASSIGN NODE */
        } else if (node->nodeType == "assign") {
            generateCodeAux(node->children[0], output);
            output += "STACKW " + std::to_string(stack.find(node->tokens[1]->tokenInstance)) + "\n";

        } else if (node->nodeType == "RO") {
            // Code generation for the relational operator is handled by the if node
            return;
        } else if (node->nodeType == "label") {
            // NO CHECKING IS DONE WITH THESE LABELS, THEY ARE MERELY STORED FOR
            // REFERENCE LATER
            // To handle global labels (which are not checked, due to P3)
            // this stack holds all instances of labels
            // New names are also generated for the labels so errors will not occur
            std::string labelName = node->tokens[1]->tokenInstance;
            labels.insert(labelName);
            output += getUserLabelName(labels.find(labelName)) + ": NOOP\n";

        } else if (node->nodeType == "goTo (goto)") {
            // Jump to the appropriate global label
            std::string labelName = node->tokens[1]->tokenInstance;
            output += "BR " + getUserLabelName(labels.find(labelName)) + "\n";

        } else {
            // Note that this error will not throw an exception
            // this is because the error technically cannot happen
            // and this output was used mainly for debugging purposes
            output += "UNRECOGNIZED NODE, CHECK CODE GENERATION\n";
        }
    } else {
        return;
    }
}