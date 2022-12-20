Storage=Local

CS4280 P4 Bryce Paubel 11/16/22
This project, written in C++, reads in a string from a file or stdin
and uses a lexical analyzer + parser + static semantic analyzer + code generator
to translate a program in our given language to our target assembly language

Note:
    Since my P3 did no checking on labels for static semantics, there was no label checking done
    for this project as well.

Compilation:
    To compile, use the 'make' command along with the provided Makefile in the P4 directory

Running:
    To run, use ./compfs, since the Makefile will compile to the project directory

    The command either takes in a file argument or text from standard input:
    usage: ./compfs [file] or ./compfs[< file]

Version Control:
    Proof of version control is shown with the .git folder and included HEAD log file. 
    If needed, I can also provide a link to my repository on GitHub, but it is a private repository.

Testing:
    Within the test folder is also a bash script and test files which were used to test
    this project. To run these tests, navigate to the test folder and type the command: 
    bash test.sh. Unfortunately, the majority of the test assembly programs generated
    were having trouble in the assembly executable on the delmar server. So, I tested each
    generated program individually on the online interpreter.

Note:
    Much of P0 + P1 + P2 + P3 were also used to create and test this project. I have not cited it, as it was implied to be
    reused when we were working on P0 + P1 + P2 + P3.

Challenges:
    I actually did not have too much trouble with this project - my code generation actually felt much
    simpler than dealing with my parser, and it ended up being roughly half as big as the parser.

    I tried using the assembly program from the delmar server, but the executable wouldn't interpret valid
    assembly programs. I'm unsure as to why this happened, as the programs were valid and worked flawlessly
    on the online interpreter.