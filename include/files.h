// CS4280 Bryce Paubel P0 8/28/22
// Simple functions for file operations

#ifndef FILES_H
#define FILES_H

#include <string>

bool createFile(std::string);
bool readFromFile(std::string, std::string&);
bool writeIntoFile(std::string, std::string);

#endif