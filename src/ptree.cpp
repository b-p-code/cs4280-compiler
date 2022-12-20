// CS4280 Bryce Paubel 10/20/22
// Implementation file for a tree with can have k children

#include <string>
#include <iostream>

#include "../include/ptree.h"

// Constructors
PTree::PTree() {
    root = nullptr;
}

PTree::PTree(GNode* node) {
    createTreeFromNode(node);
}


// Destructor
PTree::~PTree() {
    destroySubTree(this->root);
}

/*
    destroySubTree
    Destroys a given subtree from a node. If we delete a GNode,
    it will delete all its children as well. So, this function
    merely calls delete, and GNode deallocates itself and its children.

    Input: GNode*
    The node pointer from which we will delete

    Output: void
*/
void PTree::destroySubTree(GNode* node) {
    if (node) {
        delete node;
    }
}

/*
    printPreOrder
    Prints the given tree in preorder traversal

    Input: void

    Output: string
    A string showing the printed tree
*/
std::string PTree::printPreOrder() {
    return printPreOrderAux(root, 0);
}

/*
    insertNodeAtStart
    Takes a node and points it at the root

    Input: GNode*
    The node which will point to the root

    Output: void
*/
void PTree::createTreeFromNode(GNode* newNode) {
    if (root) {
        destroySubTree(root);
        root = newNode;
    } else {
        root = newNode;
    }
}


/*
    printPreOrderAux
    Auxiliary function for printing in preorder

    Input: int
    The depth of recursion (depth of tree)

    Output: string
    Preorder printing for tree
*/
std::string PTree::printPreOrderAux(GNode* node, int depth) {
    std::string total = "";

    if (node) {
        for (int i = 0; i < 2 * depth; i++) {
            total += " ";
        }
        total += node->nodeType + ":";
        for (int i = 0; i < node->NUM_OF_TOKENS; i++) {
            if (node->tokens[i]) {
                total += " " + node->tokens[i]->getTokenName() + " '" + node->tokens[i]->tokenInstance + "'";
            }
        }

        total += "\n";

        for (int i = 0; i < node->NUM_OF_NODES; i++) {
            total += printPreOrderAux(node->children[i], depth + 1);
        }
    }

    return total;
}