// CS4280 P4 Bryce Paubel 11/16/22
// Declaration for compiler class

#ifndef COMPILER_H
#define COMPILER_H

#include <string>

class Compiler {
    private:
        std::string source;
    public:
        Compiler(std::string source) {
            this->source = source;
        }
        std::string compile();
};

#endif