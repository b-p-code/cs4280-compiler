#!/bin/bash

# CS4280 P4 Bryce Paubel 11/16/22
# This is a test script to run the program against
# a couple different use cases and to observe outputs

# NOTE THAT THIS WILL TEST INTEGERS AND IDENTIFIERS LONGER
# THAN 8 CHARACTERS - THIS SHOULD NOT IMPACT ACTUAL PERFORMANCE

# LABEL/WARP STATEMENTS ARE ALSO IGNORED, AS ALLOWED
# BY PROFESSOR HAUSCHILD - THIS MEANS THAT SOME TEST
# FILES WILL TECHNICALLY BE ILLEGAL, BUT ONLY BECAUSE
# PROFESSOR HAUSCHILD ALLOWED IT

passed="true"

# Test for improper arguments
echo "Testing for improper arguments..."
if ! ../compfs more than enough arguments
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test empty file
echo "Testing empty file..."
if ! ../compfs empty
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test empty redirection
echo "Testing empty file redirection..."
if ! ../compfs < empty
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test for a nonexistent file
echo "Testing for files that don't exist..."
if ! ../compfs idontexist
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test for an unreadable file
echo "Testing unreadable file..."
chmod u-r unreadableFile
if ! ../compfs unreadableFile
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
fi
chmod u+r unreadableFile

# Test for files full of only whitespace
echo "Testing for files that are full of only whitespace..."
if ! ../compfs onlyWS
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test for files full of only whitespace with redirection
echo "Testing for files that are full of only whitespace with redirection..."
if ! ../compfs < onlyWS
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test for input full of comments
echo "Testing for files full of only comments..."
if ! ../compfs onlyComments
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test for input full of comments with redirection
echo "Testing for files full of only comments with redirection..."
if ! ../compfs < onlyComments
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Test for unclosed comments
echo "Testing for unclosed comments..."
if ! ../compfs unclosedComment
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Testing for illegal characters
echo "Testing for illegal chars..."
if ! ../compfs illegalChar
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Testing for illegal form
echo "Testing for illegal forms..."
if ! ../compfs illegalForm1
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Testing for illegal form
echo "Testing for illegal forms..."
if ! ../compfs illegalForm2
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# Testing for illegal form
echo "Testing for illegal forms..."
if ! ../compfs illegalForm3
then
    echo "**************************Passed**************************"
else
    echo "**************************Failed**************************"
    passed="false"
fi

# HELP FOR CODE FOUND HERE:
# https://www.digitalocean.com/community/tutorials/workflow-loop-through-files-in-a-directory

# Loop through every source program and generate the proper assembly code
echo "Removing original assembly code"
cd src
rm *asm
cd ..
echo "Generating new assembly code"
for FILE in ./src/*
do
    ../compfs $FILE
done

# Final result
echo "Did the tests pass?" $passed