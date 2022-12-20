// CS4280 P2 Bryce Paubel 10/16/22
// This program takes in a program string from either standard input or a file
// and determines if it is a valid program as well as generating a parse tree

#include <iostream>
#include <string>

#include "../include/files.h"
#include "../include/compiler.h"
#include "../include/compilerException.h"

using namespace std;

bool checkArguments(int args, char** argv);
bool onlyWS(string);

int main(int args, char** argv) {
    string inputTotal = "";
    string fileName = "";
    string outputFileName = "";
    string input = "";
    string output = "";
    
    // Check arguments
    if (!checkArguments(args, argv)) {
        cout << "Improper arguments, usage is: ./frontEnd [file] or ./frontEnd [< file]" << endl;
        return 1;
    }

        // If no arguments (other than the command), read from the standard input using std::getline
    if (args == 1) {
        cout << "Simulate EOF (CTRL + D or CTRL + Z depending on system)";
        cout << " to send final input if using the keyboard. " << endl;
        cout << "Type input (if keyboard): ";
        
        // Note that in a few cases sending input will require two EOF inputs
        // if using the keyboard in Bash. This is intended behavior from the Bash 
        // and is described here: https://stackoverflow.com/questions/30689930/why-two-eof-needed-as-input

        // Continue getting input until EOF
        while(getline(cin, input)) {
            // I was having errors on Windows with carraige returns messing up newlines,
            // so I had to include this section here to replace carraige returns with space
            if (!input.empty()){
                if ((int)input[input.length() - 1] == 13) {
                    input.pop_back();
                }
            }

            inputTotal += input + "\n";
        }

        // Helpful source was found here:
        // https://stackoverflow.com/questions/2310939/remove-last-character-from-c-string
        if (inputTotal.length() >= 2 && input.length() != 0) {
            inputTotal.pop_back();
        }

        cout << endl;

        outputFileName = "kb.asm";

    // Otherwise, read from the filename argument to get a string    
    } else {
        fileName = argv[1];
        if (!readFromFile(fileName, inputTotal)) {
            std::cout << "Input error in reading from file, file may not exist or be readable!" << std::endl;
            return 1;
        }

        outputFileName = fileName + ".asm";
    }

    // Quick check for empty input
    if (inputTotal == "" || inputTotal.empty()) {
        std::cout << "Input error in reading string, file or input is empty" << std::endl;
        return 1;
    }

    // Quick check for whitespace only input
    if (onlyWS(inputTotal)) {
        std::cout << "Input error. Input contains only whitespace." << std::endl;
        return 1;
    }

    try {
        Compiler UMSLASMCompiler(inputTotal);
        output = UMSLASMCompiler.compile();
    // If error, display message and return 1 
    } catch (CompilerException e) {
        cout << e.message;
        return 1;
    }

    // Output to files
    if (createFile(outputFileName)) {
        if (!writeIntoFile(outputFileName, output)) {
            cout << "Error in writing to " << outputFileName << endl;
            return 1;
        }
    } else {
        cout << "Error in creating " << outputFileName << endl;
        return 1;
    }

    cout << outputFileName << endl;

    return 0;
}

/*
    checkArguments
    Determines if the input arguments are appropriate

    Input: int, char**
    These are the argument inputs

    Output: bool
    Boolean determining if the number of arguments is proper
*/
bool checkArguments(int args, char** argv) {
    if (args > 2 || args < 1) {
        return false;
    } else {
        return true;
    }
}

/*
    onlyWS
    Determines if a given string is only whitespace

    Input: string
    String to be checked

    Output: bool
    Boolean determining if the string is just whitespace or no
*/
bool onlyWS(string input) {
    // Check to make sure the file isn't just full of whitespace
    bool onlyWS = true;
    for (unsigned int i = 0; i < input.length(); i++) {
        if (!isspace(input[i])) {
            onlyWS = false;
        }
    }
    return onlyWS;
}