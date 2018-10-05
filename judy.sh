#!/bin/bash

fail_colour=$'\x1b[31m'
pass_colour=$'\x1b[32m'
warn_colour=$'\x1b[33m'
colour_end=$'\x1b[0m'

if [ ! -f "Makefile" ]; then
    cgal_create_cmake_script
    cmake .
fi

eval make

if [ $? -ne 0 ]; then
    exit 1
fi

printf "Retrieving test cases...\n"
if [ ! -d testsets/ ]; then
    printf "[ %sERROR%s ] No testsets folder found.\nEnd of the testing process.\n" "$fail_colour" "$colour_end"
    return
fi

for test in testsets/*; do
    extension=${test##*.}

    if [ $extension = "in" ]; then
        testin=${test##*/} # Just the name of the test - only used to indicate what is being tested, and for the temporary out file.
        testnoext=${test%.*}
        testout=$testnoext".out" # Correct test output.

        printf "Testing %s... " "$testin"

        command="./$executable < $test > $testin.out"
        eval ${command}

        command="diff $testin.out $testout > $testin.diff"
        eval ${command}

        if [ -s $testin".diff" ]; then
            printf "\t[ %sERROR%s ]\n" "$fail_colour" "$colour_end"
            printf "< : What you had.\n> : What they had.\n"
            command="cat $testin.diff"
            eval ${command}
        else
            printf "\t[ %sOK%s ]\n" "$pass_colour" "$colour_end"
        fi

        rm $testin".diff"
        rm $testin".out"
    fi
    # No need to treat the case where the extension is out, since we perform the tests in the in case directly.
done
