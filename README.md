# Algolab HS 2018 - 2019

Solutions provided may, or may not, be correct.

## First instance judge
`firstinstance.sh` allows to compile and run all the provided tests. You might need to add permissions to the file.

### Requirements to run the first instance judge
You need to

* Be in the folder `foo/` of the file `foo.cpp` you want to test,
* Have all test cases in a `testsets/` folder,
* Run `../../.firstinstance.sh foo.cpp`

### Output of the first instance judge

#### Compilation
The script stops on an error during the compilation. In case of a successful compilation, it displays all warnings generated.

#### Testing
The script tests the program against every test input it can find, and displays the difference with the correct input (if any).

The first instance judge does **not** test the programs under any time constraints.
