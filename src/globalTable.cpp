// CS4280 Bryce Paubel 11/20/22
// Global table definitions used for labels

#include <string>

#include "../include/globalTable.h"

/* 
    insert
    Inserts a value into the symbol table

    Input: string
    The name of the variable to be inserted

    Output: void
*/
void GlobalTable::insert(std::string input) {
    internalVector.push_back(input);
}

/* 
    Find
    Finds a value in the symbol table

    Input: string
    The name of the variable to be found

    Output: int
    The position of the variable in the internal vector
    -1 is returned if not found
*/
int GlobalTable::find(std::string input) {
    for (unsigned int i = 0; i < internalVector.size(); i++) {
        if (input == internalVector[i]) {
            return i;
        }
    }

    return -1;
}

