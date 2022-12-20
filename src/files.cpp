// CS4280 Bryce Paubel P0 8/28/22
// Implementation for simple file functions

#include <fstream>
#include <iostream>

#include "../include/files.h"

/*
    createFile
    Creates a file. Will fail if the file already exists

    Input: string
    The filename for the file created

    Output: bool
    A boolean for whether the file creation was successful
*/
bool createFile(std::string fileName) {
    // Use an input file stream to check if the file exists
    std::ifstream inputFile;
    inputFile.open(fileName.c_str());

    // Check if file does not exist
    if (!inputFile) {
        inputFile.close();
        // Use ofstream to create a file
        std::ofstream outputFile;

        // Create the file by opening it
        outputFile.open(fileName.c_str());

        // Check if the opening operation worked
        if (outputFile) {
            outputFile.close();
            return true;
        } else {
            outputFile.close();
            return false;
        }
    } else {
        inputFile.close();
        return false;
    }
}

/*
    writeIntoFile
    Overwrites into a file. If the file exists, it will overwrite it.
    If the file does not exist, this operation will fail

    Input: string, string
    The filename for the file to overwrite to and the string to write

    Output: bool
    A boolean for whether the file writing was successful
*/
bool writeIntoFile(std::string fileName, std::string text) {
    // Use ifstream to check if file exists
    std::ifstream inputFile;
    inputFile.open(fileName.c_str());

    // Check if file exists
    if (inputFile) {
        inputFile.close();
        // Use an output stream to write to file
        std::ofstream outputFile;
        outputFile.open(fileName.c_str());

        // Check if file opened
        if (outputFile) {
            // Write to file
            // and check if successful
            if (outputFile << text) {
                outputFile.close();
                return true;
            } else {
                outputFile.close();
                return false;
            }
        } else {
            outputFile.close();
            return false;
        }
    } else {
        inputFile.close();
        return false;
    }
}

/*
    readFromFile
    Reads data from a file as a string and inserts it into the text parameter

    Input: string, string
    The filename to read from and the string in which to insert the text

    Output: bool
    Returns false if unsuccessful read, true if successful
*/
bool readFromFile(std::string fileName, std::string& text) {
    // Input strings
    std::string input = "";
    std::string totalInput = "";

    // ifstream used to both open and check if file exists
    std::ifstream file;
    file.open(fileName.c_str());

    // Check the file exists
    if (file) {
        // Continue reading all the lines of data
        while (std::getline(file, input)) {
            if (!input.empty()){
                if ((int)input[input.length() - 1] == 13) {
                    input.pop_back();
                }
            }
            totalInput += input + "\n";
        }

        // Remove the extra newline from the input
        // Helpful source was found here:
        // https://stackoverflow.com/questions/2310939/remove-last-character-from-c-string
        if (!totalInput.empty() && input.length() != 0) {
            totalInput.pop_back();
        }

        file.close();
        text = totalInput;
        return true;
    } else {
        text = "";
        file.close();
        return false;
    }
}