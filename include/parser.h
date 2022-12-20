// CS4280 P2 Bryce Paubel 10/14/22
// Definition for Parser class and ParserException class

#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "scanner.h"
#include "token.h"

#include "ptree.h"
#include "gnode.h"

#include "compilerException.h"

class Parser {
    private:
        std::string input;

        GNode* program(Token*&, Scanner&);
        GNode* block(Token*&, Scanner&);
        GNode* vars(Token*&, Scanner&);
        GNode* expr(Token*&, Scanner&);
        GNode* expr_(Token*&, Scanner&);
        GNode* N(Token*&, Scanner&);
        GNode* N_(Token*&, Scanner&);
        GNode* A(Token*&, Scanner&);
        GNode* A_(Token*&, Scanner&);
        GNode* M(Token*&, Scanner&);
        GNode* R(Token*&, Scanner&);
        GNode* stats(Token*&, Scanner&);
        GNode* mStat(Token*&, Scanner&);
        GNode* stat(Token*&, Scanner&);
        GNode* in(Token*&, Scanner&);
        GNode* out(Token*&, Scanner&);
        GNode* iff(Token*&, Scanner&);
        GNode* iff_(Token*&, Scanner&);
        GNode* loop(Token*&, Scanner&);
        GNode* assign(Token*&, Scanner&);
        GNode* RO(Token*&, Scanner&);
        GNode* label(Token*&, Scanner&);
        GNode* goTo(Token*&, Scanner&);

        std::string error(std::string, std::string, std::string, int, int);

    public:
        Parser(std::string input) {
            this->input = input;
        }

        PTree* parse();
};

// Help for these exceptions was found here:
// https://www.geeksforgeeks.org/user-defined-custom-exception-with-class-in-c/
// https://stackoverflow.com/questions/2512931/catch-multiple-custom-exceptions-c

// Exception class
class ParserException : public CompilerException {
    public:
        ParserException(std::string message) {
            this->message = message;
        }
};

#endif