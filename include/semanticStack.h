// CS4280 P3 Bryce Paubel 11/5/22
// The declaration for a simple stack

#ifndef SEMANTIC_STACK_H
#define SEMANTIC_STACK_H

#include <vector>
#include <string>

class SemanticStack {
    private:
        std::vector<std::string> internalStack;
    public:
        void push(std::string);
        void pop();
        int find(std::string);
        void display();
        bool empty();
        int size();
};

#endif