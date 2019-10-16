#!/bin/bash
# Created by nicbauma, Autumn Semester 2018
VERSION="0.1"

# Edit command (executed in current working directory)
IDE_COMMAND='qtcreator CMakeLists.txt &>/dev/null &disown'

# Enable cmake generators for ide project files (not tested)
USE_CODEBLOCKS=false
USE_ECLIPSE=false
USE_KATE=false

# Usage
DOC=$(cat <<'EOF'
install: "Install" the script into user home (including autocomplete for commands after shell restart). After installation, script can be invoked in any directory using the alias 'algolab'.

init: Setup project (creates CMakeLists.txt, algorithm.cpp, build directories (both debug and release) and runs cmake)

clean: Remove build directory and CMakeLists.txt

edit: Execute IDE_COMMAND

debug: Compile in debug mode and open the executable file with gdb (in current working dir)

memcheck: Compile in debug mode (if modified) and run with valgrind

compile: Compiles in release mode

run: Compiles in release mode (if modified) and launches the executable (in current working dir)

test: Compiles in relase mode (if modified) and runs all test cases (compares all .in files with .out files). If a test
fails, mismatching output file lines are highlighted in red (lines are truncated to fit terminal window)

version: Show version number
EOF
)

# Colors
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
NC=$(tput sgr0)

# Used for test output truncation
minlen=7
maxlen=$(($(tput cols)/2-5))
cutlen="-$((maxlen-3))"

if [ -f ".algolab" ]; then
    pname=$(cat ".algolab")
fi
pname=${pname:-algo}
pname=${2:-${pname}}

# File templates

CMAKE_FILE=$(cat <<'EOF'
cmake_minimum_required(VERSION 2.8)
project(pname)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(CGAL REQUIRED COMPONENTS Core)
find_package(Boost REQUIRED)
include_directories(${CGAL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
include( ${CGAL_USE_FILE} )

add_executable(${PROJECT_NAME} "src/algorithm.cpp")
target_link_libraries(${PROJECT_NAME} ${CGAL_LIBRARIES} ${Boost_LIBRARIES} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${THREAD_LIBRARIES})
EOF
)
CMAKE_FILE="${CMAKE_FILE/pname/${pname}}"

CXX_TEMPLATE=$(cat <<'EOF'
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    // TODO
}
EOF
)

COMP_FILE=$(cat <<EOF
#/usr/bin/env bash
alias algolab='~/.algolab.sh'
complete -W "clean init edit debug memcheck run compile test version" algolab
EOF
)

# Helper function
function try_compile() {
    local ok
    local coutput
    coutput=$(make 2>&1)
    ok=$?
    if [ "$ok" != 0 ]; then
        echo "Compiler error!"
        echo ""
        echo "$coutput"
        exit 1
    fi
}

case "$1" in
    version)
        echo "${VERSION}"
        ;;
    install)
        cp "$0" ~/.algolab.sh
        if [ ! -f ~/.bashrc ] || ! grep -q 'source ~/.algolab-bashrc.bash' ~/.bashrc; then
            echo 'source ~/.algolab-bashrc.bash' >> ~/.bashrc
        fi
        echo "$COMP_FILE" > ~/.algolab-bashrc.bash
        exit 0
        ;;
    clean)
    	if [ -f .algolab ]; then
            rm -r build
            rm CMakeLists.txt
            echo "algo" > .algolab
        else
            echo "Not an algolab project directory."
    	fi
        ;;
    init)
        printf "Creating CMake project...\t"
        echo "$pname" > .algolab
        mkdir -p src
        if [ ! -f src/algorithm.cpp ]; then
            echo "$CXX_TEMPLATE" > src/algorithm.cpp
        fi
        echo "$CMAKE_FILE" > CMakeLists.txt
        mkdir -p build
        mkdir -p build/debug
        mkdir -p build/release
        
        cd build/debug
        cmake -DCMAKE_BUILD_TYPE=debug ../..
        echo ""
    	echo "Created Debug Makefile"
    	echo ""
        cd ../release
        cmake -DCMAKE_BUILD_TYPE=release ../..
        echo ""
    	echo "Created Release Makefile"
    	echo ""
        cd ../
        mkdir -p project_files
        cd project_files
        if [ "$USE_CODEBLOCKS" == true ]; then
        	cmake ../.. -G"CodeBlocks - Unix Makefiles"
        	echo ""
        	echo "Created Code::Blocks project"
        	echo ""
        fi
        if [ "$USE_ECLIPSE" == true ]; then
	        cmake ../.. -G"Eclipse CDT4 - Unix Makefiles"
	        echo ""
        	echo "Created Eclipse project"
        	echo ""
        fi
        if [ "$USE_KATE" == true ]; then
	        cmake ../.. -G"Kate - Unix Makefiles"
	        echo ""
        	echo "Created Kate project"
        	echo ""
        fi
        cd ../..
        echo ""
        echo "Finished creating project files."
        echo ""
        ;;
    edit)
        eval "$IDE_COMMAND"
        ;;
    debug)
        cd build/debug
        try_compile
        cd ../..
        gdb --exec="build/debug/${pname}"
        ;;
    memcheck)
        cd build/debug
        try_compile
        cd ../..
        valgrind "build/debug/${pname}"
        ;;
    compile)
        cd build/release
        make
        ;;
    run)
        cd build/release
        try_compile
        cd ../..
        echo "Running ${pname}:"
        echo ""
        "./build/release/${pname}"
        ;;
    test)
        cd build/release
        try_compile
        cd ../..
        echo "Running all tests for ${pname}:"
        echo ""
        find . -name '*.in' | sort | while read line; do
            tname=${line::-3}
            printf "Test '${tname:2}': "
            result=$("./build/release/${pname}" < "$tname.in")
            ok=$?
            if [ "$ok" != 0 ]
            then
                echo "Runtime error!"
            else
                expected=$(cat "$tname.out")
                
                if [ "$result" == "$expected" ]
                then
                    printf "${GREEN}PASSED${NC}\n"
                else
                    # Truncate lines if longer than half terminal
                    len=$(echo "$expected" | awk '(NR==1||length>longest){longest=length} END {print longest}')
                    if [ "$len" -gt "$maxlen" ]; then 
                        len="$maxlen" 
                    fi
                    if [ "$len" -lt "$minlen" ]; then
                        len=$minlen
                    fi
                    len=$((len+3))
                    dashes=$(printf "%-$((len*2))s" "-")
                
                    printf "${RED}FAILED${NC}\n"
                    
                    echo "${dashes// /-}"
                    printf "%-${len}s %-${len}s\n" "Expected" "Was"
                    echo "${dashes// /-}"
                    while read -u3 e; read -u4 w;
                    do
                        equal=0; [ "$e" == "$w" ] && equal=1
                        
                        if [ "${#e}" -gt "$maxlen" ]; then
                            e="$(echo "$e" | cut -c $cutlen)..."
                        fi
                        if [ "${#w}" -gt "$maxlen" ]; then
                            w="$(echo "$w" | cut -c $cutlen)..."
                        fi
                        
                        if [ "$equal" == 0 ]; then
                            printf "${RED}%-${len}s %-${len}s${NC}\n" "$e" "$w"
                        else
                            printf "%-${len}s %-${len}s\n" "$e" "$w"
                        fi
                        
                    done 3< <(echo "$expected") 4< <(echo "$result")
                    echo "${dashes// /-}"
                    echo ""
                fi
            fi
        done
        ;;
    *)
        echo "Usage: algolab (init|clean|edit|debug|memcheck|compile|run|test|version)"
        echo ""
        echo "$DOC"
        echo ""
esac
