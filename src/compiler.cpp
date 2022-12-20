// CS4280 P4 Bryce Paubel 11/16/22
// Definition for compiler class

#include <string>

#include "../include/compiler.h"
#include "../include/compilerException.h"
#include "../include/codeGen.h"

std::string Compiler::compile() {
    try {
        std::string targetString;

        // Create a new code generator and get target code
        CodeGen generator(this->source);
        targetString = generator.generateCode();

        // Return the target string
        return targetString;
    } catch (CompilerException e) {
        throw;
    }
}