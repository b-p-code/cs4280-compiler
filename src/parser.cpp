// CS4280 Bryce Paubel P2 10/14/22
// Implemenations for the parser class

#include <string>

#include "../include/parser.h"
#include "../include/scanner.h"
#include "../include/token.h"

#include "../include/gnode.h"
#include "../include/ptree.h"

#include "../include/compilerException.h"

/* 
    ERROR HANDLING:
    I refactored my error handling from the scanner. The parser will now generate
    a ParserException in the case of a parser error. In the case of an error within
    the scanner, it instead returns a ScannerException, which is a subclass of
    CompilerException. If the parser/scanner generates an exception in the
    parser, it is continually thrown until it reaches whatever called it.

    Help for these exceptions was found here:
    https://www.geeksforgeeks.org/user-defined-custom-exception-with-class-in-c/
    https://stackoverflow.com/questions/2512931/catch-multiple-custom-exceptions-c
*/

/*
    parser
    Parses a string and determines if it is a legal program,
    and returns a parse tree

    Input: string
    The program to be parsed

    Output: PTree
    A tree with five nodes (the parse tree)
*/
PTree* Parser::parse() {
    Scanner scanner(this->input);   // Scanner seeded with input
    PTree* parseTree = new PTree(); // Parse tree with variable amount of nodes
    Token* tk = nullptr;            // Token from the scanner

    try {
        tk = scanner.nextToken();

        parseTree->createTreeFromNode(program(tk, scanner));

        if (tk->tokenID == EOFtk) {
            delete tk;
            tk = nullptr;
        } else {
            std::string errorMsg = error("parser", tk->tokenInstance, "EOF", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
        
        return parseTree;
    } catch (CompilerException &e) {
        delete parseTree; // No need to delete root, since the parse tree contains root
        parseTree = nullptr;
        throw;
    }
}

/***** PRODUCTION METHODS *****/
/* Each function has the same parameters and return type */
/* These parameters and return types are described here */
/* Note that each can possible throw a CompilerException */

/*
    (anyProductionMethod)
    Follows a grammar production

    Input: Token*&, Scanner&, GNode*&
    A reference to the current token we have (so that each function has the current token)
    A reference to the scanner (so that each function has the same scanner)
    A reference to the root node, in case of error cleanup

    Output: GNode*
    A pointer to the node created by the function
*/

/* Production: program */
GNode* Parser::program(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "program";

    try {
        // Add a vars child
        node->children[0] = vars(tk, scanner);

        if (tk->tokenID == PROGRAMtk) {
            // Save the token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            // Add a block child
            node->children[1] = block(tk, scanner);

            // Return the node
            return node;
        } else {
            std::string errorMsg = error("program", tk->tokenInstance, "program", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: block */
GNode* Parser::block(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "block";

    try {
        // Check for the begin token, expand the vars and stats productions
        // and then check for the end token
        if (tk->tokenID == BEGINtk) {
            // Add the token to the node
            // and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            // Add the vars and stats children
            node->children[0] = vars(tk, scanner);
            node->children[1] = stats(tk, scanner);

            if (tk->tokenID == ENDtk) {
                // Add the token to the node and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("block", tk->tokenInstance, "end", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error("block", tk->tokenInstance, "begin", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: vars */
GNode* Parser::vars(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "vars";

    try {
        // Check if the proper tokens are here and then expand with the vars production
        if (tk->tokenID == WHOLEtk) { // Check for whole token
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            if (tk->tokenID == IDtk) {  // Check for an identifier
                // Save token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();
                if (tk->tokenID == CEtk) {  // Check for a colon equals
                    // Save token and get a new one
                    node->tokens[2] = tk;
                    tk = scanner.nextToken();
                    if (tk->tokenID == INTtk) { // Check for an integer
                        // Save token and get a new one
                        node->tokens[3] = tk;
                        tk = scanner.nextToken();
                        if (tk->tokenID == SEMI_COLONtk) {  // Check for a semicolon
                            // Save token and get a new one
                            node->tokens[4] = tk;
                            tk = scanner.nextToken();

                            // Add a child to the node
                            node->children[0] = vars(tk, scanner);

                            return node;
                        } else {
                            std::string errorMsg = error("vars", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                            delete tk;
                            tk = nullptr;
                            throw ParserException(errorMsg);
                        }
                    } else {
                        std::string errorMsg = error("vars", tk->tokenInstance, "integer", tk->lineNum, tk->charNum);
                        delete tk;
                        tk = nullptr;
                        throw ParserException(errorMsg);
                    }
                } else {
                    std::string errorMsg = error("vars", tk->tokenInstance, ":=", tk->lineNum, tk->charNum);
                    delete tk;
                    tk = nullptr;
                    throw ParserException(errorMsg);
                }
            } else {
                std::string errorMsg = error("vars", tk->tokenInstance, "identifier", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else { // Return the empty string (nullptr)
            delete node; // We can simply delete the node, since it will have no children
            node = nullptr;
            return nullptr;
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: expr */
GNode* Parser::expr(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "expr";

    try {
        // Only one option to expand
        // Expand with the N and expr' production
        node->children[0] = N(tk, scanner);
        node->children[1] = expr_(tk, scanner);

        return node;
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: expr' (denoted by _) */
GNode* Parser::expr_(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "expr_ (expr')";

    try {
        if (tk->tokenID == SUBtk) { // Expand with the expr production
            // Save the token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            // Get a new child
            node->children[0] = expr(tk, scanner);

            return node;
        } else { // Return the empty string (nullptr)
            delete node; // We can simply delete the node, since it will not have children
            node = nullptr;
            return nullptr;
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: N */
GNode* Parser::N(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "N";

    try {
        // Only one choice to expand N
        // Expand to A and N'
        node->children[0] = A(tk, scanner);
        node->children[1] = N_(tk, scanner);
        return node;
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: N' (denoted by _) */
GNode* Parser::N_(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "N_ (N')";

    try {
        if (tk->tokenID == ADDtk) { // Exand with the N production
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            node->children[0] = N(tk, scanner);
            return node;
        } else if (tk->tokenID == MULtk) { // Expand with the N production
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            node->children[0] = N(tk, scanner);
            return node;
        } else { // Return empty production (nullptr)
            delete node; // We can simply delete the node, since it will have no children
            node = nullptr;
            return nullptr;
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: A */
GNode* Parser::A(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "A";
    
    try {
        // Only one production choice
        // Expand with M and A'
        node->children[0] = M(tk, scanner);
        node->children[1] = A_(tk, scanner);

        return node;
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: A' (denoted by _) */
GNode* Parser::A_(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "A_ (A')";

    try {
        if (tk->tokenID == DIVtk) { // Expand with M and A'
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            // Add children
            node->children[0] = M(tk, scanner);
            node->children[1] = A_(tk, scanner);

            return node;
        } else { // Return the empty string (nullptr)
            delete node; // We can simply delete the node, since it will have no children
            node = nullptr;
            return nullptr;
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: M */
GNode* Parser::M(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "M";

    try {
        if (tk->tokenID == COLtk) { // Expand with M
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            
            node->children[0] = M(tk, scanner);
            
            return node;
        } else if (
            tk->tokenID == L_PARENtk
            || tk->tokenID == IDtk
            || tk->tokenID == INTtk
            ) { // Expand with R
            node->children[0] = R(tk, scanner);
            return node;
        } else {
            std::string errorMsg = error("M", tk->tokenInstance, ":, (, identifier, or integer", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: R */
GNode* Parser::R(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "R";
    try {
        if (tk->tokenID == L_PARENtk) { // Expand with expr
            // Save the token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            // Add a child
            node->children[0] = expr(tk, scanner);

            if (tk->tokenID == R_PARENtk) {
                // Save the token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();

                return node;
            } else {
                std::string errorMsg = error("R", tk->tokenInstance, ")", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == IDtk) { // Don't expand, consume identifier and return
            // Save the token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            return node;
        } else if (tk->tokenID == INTtk) { // Don't expand, consume integer and return
            // Save the token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            return node;
        } else {
            std::string errorMsg = error("R", tk->tokenInstance, ")", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: stats */
GNode* Parser::stats(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "stats";

    try {
        // Only one choice for stats
        // Expand to stat and mStat
        node->children[0] = stat(tk, scanner);
        node->children[1] = mStat(tk, scanner);

        return node;
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: mStat */
GNode* Parser::mStat(Token*& tk, Scanner& scanner) {
    GNode* node = new GNode();
    node->nodeType = "mStat";

    try {
        if (
            tk->tokenID == INPUTtk
            || tk->tokenID == OUTPUTtk
            || tk->tokenID == BEGINtk
            || tk->tokenID == IFtk
            || tk->tokenID == WHILEtk
            || tk->tokenID == ASSIGNtk
            || tk->tokenID == WARPtk
            || tk->tokenID == LABELtk
            ) { // Expand to stat and mStat
            node->children[0] = stat(tk, scanner);
            node->children[1] = mStat(tk, scanner);
            return node;
        } else { // Return the empty string (nullptr)
            delete node;
            node = nullptr;
            return nullptr;
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: stat */
GNode* Parser::stat(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "stat";

    try {
        if (tk->tokenID == INPUTtk) { // Expand with the in production
            node->children[0] = in(tk, scanner);
            if (tk->tokenID == SEMI_COLONtk) {
                // Save token and get a new one
                node->tokens[0] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("stat", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == OUTPUTtk) { // Expand with the out production
            node->children[0] = out(tk, scanner);
            if (tk->tokenID == SEMI_COLONtk) {
                // Save token and get a new one
                node->tokens[0] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("stat", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == BEGINtk) { // Expand with the out production
            node->children[0] = block(tk, scanner);
            return node;
        } else if (tk->tokenID == IFtk) {
            node->children[0] = iff(tk, scanner);
            if (tk->tokenID == SEMI_COLONtk) {
                // Save token and get a new one
                node->tokens[0] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("stat", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == WHILEtk) { // Expand with the loop production
            node->children[0] = loop(tk, scanner);
            if (tk->tokenID == SEMI_COLONtk) {
                // Save the token and get a new one
                node->tokens[0] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("stat", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == ASSIGNtk) { // Expand with the assign production
            node->children[0] = assign(tk, scanner);
            if (tk->tokenID == SEMI_COLONtk) {
                // Save token and get a new one
                node->tokens[0] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("stat", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == WARPtk) { // Expand with the goto production
            node->children[0] = goTo(tk, scanner);
            if (tk->tokenID == SEMI_COLONtk) {
                // Save the token and get a new one
                node->tokens[0] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("stat", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == LABELtk) { // Expand with the label production
            node->children[0] = label(tk, scanner);
            if (tk->tokenID == SEMI_COLONtk) {
                // Save the token and get a new one
                node->tokens[0] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("stat", tk->tokenInstance, ";", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error(
                "stat", 
                tk->tokenInstance, 
                "input, output, begin, if, while, assign, warp, or label", 
                tk->lineNum, 
                tk->charNum
                );

            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: in */
GNode* Parser::in(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "in";

    try {
        if (tk->tokenID == INPUTtk) { // Consume expected tokens from the in production
            // Save the token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            if (tk->tokenID == IDtk) {
                // Save the token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("in", tk->tokenInstance, "identifier", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error("in", tk->tokenInstance, "input", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: out */
GNode* Parser::out(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "out";

    try {
        if (tk->tokenID == OUTPUTtk) { // Consume expected tokens from the out production
            // Save a new token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            node->children[0] = expr(tk, scanner);

            return node;
        } else {
            std::string errorMsg = error("out", tk->tokenInstance, "output", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: if (denoted by iff here since it is a keyword in C++) */
GNode* Parser::iff(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "iff (if)";

    try {
        if (tk->tokenID == IFtk) { // Expand to the expr, RO, expr, stat prodution
            // Save the token and get a new one
            node->tokens[0]  = tk;
            tk = scanner.nextToken();

            if (tk->tokenID == L_BRACKtk) {
                // Save the token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();

                node->children[0] = expr(tk, scanner);
                node->children[1] = RO(tk, scanner);
                node->children[2] = expr(tk, scanner);

                if (tk->tokenID == R_BRACKtk) {
                    // Save the token and get a new one
                    node->tokens[2] = tk;
                    tk = scanner.nextToken();

                    if (tk->tokenID == THENtk) {
                        // Save the token and get a new one
                        node->tokens[4] = tk;
                        tk = scanner.nextToken();

                        node->children[3] = stat(tk, scanner);
                        node->children[4] = iff_(tk, scanner);

                        return node;
                    } else {
                        std::string errorMsg = error("iff", tk->tokenInstance, "then", tk->lineNum, tk->charNum);
                        delete tk;
                        tk = nullptr;
                        throw ParserException(errorMsg);
                    }
                } else {
                    std::string errorMsg = error("iff", tk->tokenInstance, "]", tk->lineNum, tk->charNum);
                    delete tk;
                    tk = nullptr;
                    throw ParserException(errorMsg);
                }
            } else {
                std::string errorMsg = error("iff", tk->tokenInstance, "[", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error("iff", tk->tokenInstance, "if", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: if' (denoted by _ and iff since if is a keyword in C++) */
GNode* Parser::iff_(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "iff_ (if')";

    try {
        if (tk->tokenID == PICKtk) { // Expand with the stat production
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            node->children[0] = stat(tk, scanner);

            return node;
        } else { // Return the empty string (nullptr)
            delete node; // We can delete the node since it won't have any children
            node = nullptr;
            return nullptr;
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: loop */
GNode* Parser::loop(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "loop";

    try {
        if (tk->tokenID == WHILEtk || tk->tokenID < 0) {   // Expand with the expr, RO, expr, stat production
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            if (tk->tokenID == L_BRACKtk || tk->tokenID < 0) {
                // Save token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();

                node->children[0] = expr(tk, scanner);
                node->children[1] = RO(tk, scanner);
                node->children[2] = expr(tk, scanner);

                if (tk->tokenID == R_BRACKtk || tk->tokenID < 0) {
                    // Save token and get a new one
                    node->tokens[2] = tk;
                    tk = scanner.nextToken();

                    node->children[3] = stat(tk, scanner);

                    return node;
                } else {
                    std::string errorMsg = error("loop", tk->tokenInstance, "]", tk->lineNum, tk->charNum);
                    delete tk;
                    tk = nullptr;
                    throw ParserException(errorMsg);
                }
            } else {
                std::string errorMsg = error("loop", tk->tokenInstance, "[", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error("loop", tk->tokenInstance, "while", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: assign */
GNode* Parser::assign(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "assign";

    try {
        if (tk->tokenID == ASSIGNtk) { // Expand to the expr production
            // Save the token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            if (tk->tokenID == IDtk) {
                // Save the token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();

                if (tk->tokenID == ONEEQtk) {
                    // Save the token and get a new one
                    node->tokens[2] = tk;
                    tk = scanner.nextToken();

                    node->children[0] = expr(tk, scanner);

                    return node;
                } else {
                    std::string errorMsg = error("assign", tk->tokenInstance, "=", tk->lineNum, tk->charNum);
                    delete tk;
                    tk = nullptr;
                    throw ParserException(errorMsg);
                }
            } else {
                std::string errorMsg = error("assign", tk->tokenInstance, "identifier", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error("assign", tk->tokenInstance, "assign", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: RO */
GNode* Parser::RO(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "RO";

    try {
        if (tk->tokenID == LTtk) { // Consume expected tokens
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            return node;
        } else if (tk->tokenID == GTtk) { // Consume expected tokens
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            return node;
        } else if (tk->tokenID == EQtk) { // Consume expected tokens
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            return node;
        } else if (tk->tokenID == L_BRACKtk) { // Consume expected tokens
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            if (tk->tokenID == ONEEQtk) {
                // Save token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();
                if (tk->tokenID == R_BRACKtk) {
                    // Save token and get a new one
                    node->tokens[2] = tk;
                    tk = scanner.nextToken();
                    return node;
                } else {
                    std::string errorMsg = error("RO", tk->tokenInstance, "]", tk->lineNum, tk->charNum);
                    delete tk;
                    tk = nullptr;
                    throw ParserException(errorMsg);
                }
            } else {
                std::string errorMsg = error("RO", tk->tokenInstance, "=", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else if (tk->tokenID == NEQtk) {
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();
            return node;
        } else {
            std::string errorMsg = error("RO", tk->tokenInstance, "relational operator", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: label */
GNode* Parser::label(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "label";
    
    try {
        if (tk->tokenID == LABELtk) { // Consume expected tokens
            // Save token and get a new one
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            if (tk->tokenID == IDtk) {
                // Save token and get a new one
                node->tokens[1] = tk;
                tk = scanner.nextToken();

                return node;
            } else {
                std::string errorMsg = error("label", tk->tokenInstance, "identifier", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error("label", tk->tokenInstance, "label", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/* Production: goto (denoted by goTo since goto is a keyword in C++) */
GNode* Parser::goTo(Token*& tk, Scanner& scanner) {
    // Allocate a new node
    GNode* node = new GNode();
    node->nodeType = "goTo (goto)";
    try {
        if (tk->tokenID == WARPtk) { // Consume expected tokens
            // Save token and get a new node
            node->tokens[0] = tk;
            tk = scanner.nextToken();

            if (tk->tokenID == IDtk) {
                node->tokens[1] = tk;
                tk = scanner.nextToken();
                return node;
            } else {
                std::string errorMsg = error("goTo", tk->tokenInstance, "identifier", tk->lineNum, tk->charNum);
                delete tk;
                tk = nullptr;
                throw ParserException(errorMsg);
            }
        } else {
            std::string errorMsg = error("goTo", tk->tokenInstance, "warp", tk->lineNum, tk->charNum);
            delete tk;
            tk = nullptr;
            throw ParserException(errorMsg);
        }
    } catch (CompilerException &e) {
        delete node;
        node = nullptr;
        throw;
    }
}

/****** ERROR METHOD *****/

/*
    error
    Returns an error message based on the error

    Input: string, string, string, GNode*, int, int
    The rule/production where we got an error, what we got, what was expected,
    the root node of the tree, line number, and char number

    Output: string
    The error message to display
*/
std::string Parser::error(std::string rule, std::string got, std::string expected, int lineNum, int charNum) {
    std::string total = "PARSER ERROR AT RULE: " + rule + "\n";
    total += "Expected: \'" + expected + "\' token" + "\n";
    total += "Got: \'" + got + "\' instead" + "\n";
    total += "Found at line " + std::to_string(lineNum) + ", char " + std::to_string(charNum) + "\n";

    return total;
}