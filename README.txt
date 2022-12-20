Storage=Local

CS4280 P4 Bryce Paubel 11/16/22
This project, written in C++, reads in a string from a file or stdin
and uses a lexical analyzer + parser + static semantic analyzer + code generator
to translate a program in our given language to our target assembly language

Compilation:
    To compile, use the 'make' command along with the provided Makefile in the P4 directory

Running:
    To run, use ./compfs, since the Makefile will compile to the project directory

    The command either takes in a file argument or text from standard input:
    usage: ./compfs [file] or ./compfs[< file]

Testing:
    Within the test folder is also a bash script and test files which were used to test
    this project. To run these tests, navigate to the test folder and type the command: 
    bash test.sh. Unfortunately, the majority of the test assembly programs generated
    were having trouble in the assembly executable on the delmar server. So, I tested each
    generated program individually on the online interpreter.
