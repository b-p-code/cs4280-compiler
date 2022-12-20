// CS4280 Bryce Paubel 11/20/22
// Global table declarations used for labels

#ifndef GLOBAL_TABLE_H
#define GLOBAL_TABLE_H

#include <string>
#include <vector>

class GlobalTable {
    private:
        std::vector<std::string> internalVector;
    public:
        void insert(std::string);
        int find(std::string);
};

#endif