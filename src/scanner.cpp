// CS4280 P1 Bryce Paubel 9/26/22
// Implementation for the scanner class
// Contains the main driver for the scanner, nextToken

#include <ctype.h>

#include "../include/scanner.h"
#include "../include/token.h"

const bool DEBUG_MSG = false;

/*
    ERROR HANDLING:
    I refactored my error handling from the earlier version of this project.
    The scanner can now possibly throw a ScannerException, which is a subclass
    of the CompilerException class.
*/

/*
    nextToken
    Returns the next token in the lines of code

    Input: void

    Output: Token*
    A pointer to the next token found (EOF if we have reached the end and error EOF if error)
*/
Token* Scanner::nextToken() {
    int currentState = 0;   // Initial current state
    int nextState;          // The next state
    int commentChar = 0;
    int commentLine = 0;

    // Return an error token if we are in an error state
    if (this->errorState < 0) {
        throw ScannerException(
            new Token(this->errorState, "ERROR", this->errorLinePos, this->errorCharPos),
            error(this->errorState, this->errorChar, this->errorLinePos, this->errorCharPos)
        );
    }

    // If continued calls are made to the scanner after reaching EOF
    // then it just returns the EOF token again at the same position
    if (this->reachedEOF) {
        return new Token(EOFtk, "EOF", this->linePos + 1, this->lineCharPos);
    }

    // Lookahead char
    char nextChar;

    // If we have hit the end, then we send an EOF token
    // This is done in case a string does not actually have some EOF character
    if ((unsigned int)this->charPos == this->lines.length()) {
        // Special case when the last character is a newline
        if (this->lines[this->charPos] == '\n') {
            this->linePos++;
            this->lineCharPos = 0;
        }
        nextChar = EOF;
    } else {
        // Seed the next char if we are not at the end
        nextChar = this->lines[this->charPos];
    }

    std::string lexeme = ""; // Empty lexeme to start with

    // Iterate through the string
    while ((unsigned int)this->charPos < this->lines.length() + 1) {
        // Get the next state from the driver table (while also mapping the char to a column)
        nextState = this->STATE_TRANSITIONS[currentState][column(nextChar)];

        // If we enter a comment, save the information for later
        // in case of an error
        if (nextState == 5 && currentState != 5) {
            commentLine = this->linePos + 1;
            commentChar = this->lineCharPos + 1;
        }

        // If we have an error state at our next character, output and exit
        if (nextState < 0) {
            this->errorState = nextState;
            // If we have an error inside a comment, then it is a comment error
            if (commentLine > 0) {
                this->errorLinePos = commentLine;
                this->errorCharPos = commentChar;
                this->errorChar = nextChar;
                throw ScannerException(
                    new Token(this->errorState, "ERROR", this->errorLinePos, this->errorCharPos),
                    error(nextState, nextChar, this->errorLinePos, this->errorCharPos)
                );
            // Otherwise, it's a normal error
            } else {
                this->errorLinePos = this->linePos + 1;
                this->errorCharPos = this->lineCharPos + 1;
                this->errorChar = nextChar;
                throw ScannerException(
                    new Token(this->errorState, "ERROR", this->errorLinePos, this->errorCharPos),
                    error(nextState, nextChar, this->errorLinePos, this->errorCharPos)
                );
            }

            // Return an error token if there is an error
            throw ScannerException(
                new Token(this->errorState, "ERROR", this->errorLinePos, this->errorCharPos),
                error(nextState, nextChar, this->errorLinePos, this->errorCharPos)
            );

        // Prep the input if we have hit the end of a comment
        } else if (nextState == 2000) {
            commentLine = 0;
            commentChar = 0;
            nextState = 0;
            nextChar = ' ';
            lexeme = "";
        }

        // If we have a state greater than 1000, we have hit a token
        if (nextState > 999) {
            if (nextState == IDtk) {
                if (isKeyword(lexeme)) {
                    nextState = determineKeyword(lexeme);
                }
            }

            if (lexeme.length() != 0) {
                this->onlyComments = false;
                return new Token(nextState, lexeme, this->linePos + 1, this->lineCharPos - lexeme.length() + 1);
            // Position of EOF is slightly different, it is at very end
            } else {
                // Set the EOF flag to true
                // and set the EOF position to be one character past the end of input
                this->reachedEOF = true;
                this->lineCharPos++;

                if (this->onlyComments) {
                    // Handle errors
                    this->errorLinePos = this->linePos + 1;
                    this->errorCharPos = this->lineCharPos;
                    this->errorChar = nextChar;
                    this->errorState = -7; // Save the error code (which is actually not the state)

                    throw ScannerException(
                        new Token(-7, "ERROR", this->linePos + 1, this->lineCharPos),
                        error(-7, this->errorChar, this->errorLinePos, this->errorCharPos)
                    );
                }

                return new Token(nextState, "EOF", this->linePos + 1, this->lineCharPos);
            }
        // Otherwise, continue on finding other characters
        } else {
            /***** EMBEDDED FILTER PORTION *****/
            // If we are at the end of a line, treat it as a newline
            if (nextChar == '\n' || nextChar == '\r') {
                this->linePos++;
                this->lineCharPos = -1;
            }

            // If we aren't whitespace, add to token
            if (!isspace(nextChar)) {
                lexeme += nextChar;
            }
            /***** EMBEDDED FILTER PORTION *****/

            this->charPos++;
            this->lineCharPos++;
            if ((unsigned int)this->charPos == this->lines.length()) {
                nextChar = EOF;
            } else {
                nextChar = this->lines[this->charPos];
            }
            
            if (DEBUG_MSG) {
                std::cout << "Total string length: " << lines.length() << std::endl;
                std::cout << "Char position: " << this->charPos << std::endl;
                std::cout << "Character Code: " << (int)nextChar << std::endl;
                std::cout << "At: " << this->linePos + 1 << " " << this->lineCharPos << std::endl;
                std::cout << lexeme << std::endl;
            }

            currentState = nextState;
        }
    }

    // If we somehow reach this point in execution, return
    // an error code of -8
    return new Token(-8, "ERROR", -1, -1);
}

/*
    error
    Returns an error message based on an error code

    Input: int
    The error code determining the type of error

    Ouput: string
    The error message
*/
std::string Scanner::error(int errorCode, char c, int lineNum, int charNum) {
    std::string total = "SCANNER ERROR: ";
    switch (errorCode) {
        case -1: 
            total += "Table entry unfilled";
            break;
        case -2:
            total += "Illegal character: \'" + std::string(1, c) + "\' ASCII Code: " + std::to_string((int)c) + ".";
            break;
        case -3:
            total += "Comment not closed!";
            break;
        case -4:
            total += "\'!\' must be followed by \'=\'.";
            break;
        case -5:
            total += "\'|\' must be followed by \'|\'.";
            break;
        case -6:
            total += "\'&\' must be followed by \'&\'.";
            break;
        case -7:
            total += "Input contains only comments! Reached EOF without token.";
            break;
        case -8:
            total += "Reached unreachable point in execution";
            break;
        default:
            total += "Unknown error code.";
    }

    total += " Found at line " + std::to_string(lineNum) + ", char " + std::to_string(charNum) + "\n";
    return total;
}

/*
    column
    Maps characters to columns for the associated FSA table

    Input: char
    The character whose column we need

    Output: int
    The column associated with the input character
*/
int Scanner::column(char c) {
    if (isalpha(c)) {
        return 0;
    }

    if (isdigit(c)) {
        return 1;
    }

    if (isspace(c)) {
        return 25;
    }

    // EOF represented by -1
    if (c < 0) {
        return 2;
    }

    switch (c) {
        case '=': return 3;
        case '<': return 4;
        case '>': return 5;
        case '!': return 6;
        case ':': return 7;
        case '+': return 8;
        case '-': return 9;
        case '*': return 10;
        case '/': return 11;
        case '^': return 12;
        case '.': return 13;
        case '(': return 14;
        case ')': return 15;
        case ',': return 16;
        case '{': return 17;
        case '}': return 18;
        case ';': return 19;
        case '[': return 20;
        case ']': return 21;
        case '|': return 22;
        case '&': return 23;
        case '_': return 24;
        case '#': return 26;
        default: return 27;
    }
}