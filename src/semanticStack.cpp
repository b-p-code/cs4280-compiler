// CS4280 P3 Bryce Paubel 11/5/22
// The implementations for a simple stack

#include <iostream>
#include <string>
#include <vector>

#include "../include/semanticStack.h"

/*
    push
    Pushes an item onto the stack

    Input: string
    String to be pushed onto the stack

    Output: void
*/
void SemanticStack::push(std::string input) {
    internalStack.push_back(input);
}

/*
    pop
    pops an item from the stack

    Input: void

    Output: void
*/
void SemanticStack::pop() {
    if (!internalStack.empty()) {
        internalStack.pop_back();
    }
}


/*
    find
    finds an item on the stack

    Input: string
    The string to find

    Output: int
    The string's distance from the top of the stack. -1 if not found
*/
int SemanticStack::find(std::string stringToFind) {
    if (!internalStack.empty()) {
        for (int i = internalStack.size() - 1, j = 0; i >= 0; i--, j++) {
            if (stringToFind == internalStack[i]) {
                return j;
            }
        }
    }

    return -1;
}


/*
    display
    Displays the stack from top to bottom

    Input: void

    Output: void
*/
void SemanticStack::display() {
        if (!internalStack.empty()) {
        for (int i = internalStack.size() - 1; i >= 0; i--) {
            std::cout << internalStack[i] << std::endl;
        }
    }
}

/*
    empty
    Checks if the stack is empty

    Input: void

    Output: bool
    True if empty, false if not empty
*/
bool SemanticStack::empty() {
    return internalStack.empty();
}

/*
    size
    Returns the stack size

    Input: void

    Output: int
    Size of stack
*/
int SemanticStack::size() {
    return internalStack.size();
}