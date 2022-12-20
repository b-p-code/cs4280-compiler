// CS4280 P1 Bryce Paubel 9/19/22
// A token type for tokens

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Enumerate the token IDs, starting at 1000
enum tokenIDs {
    IDtk = 1000, INTtk, ONEEQtk, LTtk, GTtk, EQtk, NEQtk, 
    COLtk, CEtk, ADDtk, SUBtk, MULtk, DIVtk, POWtk, PERIODtk, 
    L_PARENtk, R_PARENtk, COMMAtk, L_BRACEtk, R_BRACEtk, SEMI_COLONtk, 
    L_BRACKtk, R_BRACKtk, ORtk, ANDtk, EOFtk, BEGINtk, ENDtk, DOtk, 
    WHILEtk, WHOLEtk, LABELtk, RETURNtk, INPUTtk, OUTPUTtk, PROGRAMtk, 
    WARPtk, IFtk, THENtk, PICKtk, DECLAREtk, ASSIGNtk, FUNCtk
};

class Token {
    public:
        static const int NUM_OF_KEYWORDS = 17;
        static const int NUM_OF_TOKENS = 26 + NUM_OF_KEYWORDS;
        static const std::string keywords[];
        static const std::string tokenName[];

        int tokenID;
        std::string tokenInstance;
        int lineNum;
        int charNum;

        Token(int tokenID, std::string tokenInstance, int lineNum, int charNum) {
            this->tokenID = tokenID;
            this->tokenInstance = tokenInstance;
            this->lineNum = lineNum;
            this->charNum = charNum;
        } 

        std::string getTokenName() {
            if (tokenID < 1000) {
                return "Invalid TokenID";
            } else {
                return tokenName[this->tokenID - 1000];
            }
        }
};

bool isKeyword(std::string);
int determineKeyword(std::string);

#endif