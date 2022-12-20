// CS4280 P2 10/25/22 Bryce Paubel
// A generic exception class to handle errors

// Help for these exceptions was found here:
// https://www.geeksforgeeks.org/user-defined-custom-exception-with-class-in-c/
// https://stackoverflow.com/questions/2512931/catch-multiple-custom-exceptions-c

#ifndef COMPILER_EXCEPTION_H
#define COMPILER_EXCEPTION_H

#include <string>

class CompilerException {
    public:
        std::string message;
};

#endif