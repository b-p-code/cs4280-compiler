// CS4180 P1 Bryce Paubel 10/3/22
// Implementation file for token.h

#include "../include/token.h"

// I had to look up how to define these static arrays:
// https://stackoverflow.com/questions/6536805/initializing-a-static-const-array-of-const-strings-in-c

// List of keywords
const std::string Token::keywords[] = {
    "begin",
    "end",
    "do",
    "while",
    "whole",
    "label",
    "return",
    "input",
    "output",
    "program",
    "warp",
    "if",
    "then",
    "pick",
    "declare",
    "assign",
    "func"
};

// List of token names corresponding to their index as i - 1000
const std::string Token::tokenName[] = {
    "IDtk", "INTtk", "ONEEQtk", "LTtk", "GTtk", "EQtk", "NEQtk",
    "COLtk", "CEtk", "ADDtk", "SUBtk", "MULtk", "DIVtk", "POWtk", "PERIODtk",
    "L_PARENtk", "R_PARENtk", "COMMAtk", "L_BRACEtk", "R_BRACEtk", "SEMI_COLONtk",
    "L_BRACKtk", "R_BRACKtk", "ORtk", "ANDtk", "EOFtk", "BEGINtk", "ENDtk", "DOtk",
    "WHILEtk", "WHOLEtk", "LABELtk", "RETURNtk", "INPUTtk", "OUTPUTtk", "PROGRAMtk", 
    "WARPtk", "IFtk", "THENtk", "PICKtk", "DECLAREtk", "ASSIGNtk", "FUNCtk"
};

/*
    isKeyword
    Checks if a lexeme is a keyword

    Input: string
    Lexeme to check if a keyword

    Output: bool
    Whether or not the input was determined to be a keyword
*/
bool isKeyword(std::string lexeme) {
    for (int i = 0; i < Token::NUM_OF_KEYWORDS; i++) {
        if (lexeme == Token::keywords[i]) {
            return true;
        }
    }

    return false;
}

/*
    determineKeyword
    Given a lexeme, determine what keyword it is

    Input: string
    Lexeme to be checked

    Output: int
    Token code for the keyword (-1 if error)
*/
int determineKeyword(std::string input) {
    for (int i = 0; i < Token::NUM_OF_KEYWORDS; i++) {
        if (input == Token::keywords[i]) {
            return 1026 + i;
        }
    }

    // If not a keyword, return a negative value
    return -1;
}
