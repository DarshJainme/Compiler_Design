#!/bin/bash

# Script to compile and run the compiler on all test cases in the 'test' directory.

COMPILER=./compiler
TEST_DIR=./test
OUTPUT_DIR=./output

# Create test and output directories if they don't exist
mkdir -p $TEST_DIR
mkdir -p $OUTPUT_DIR

echo "--- Compiling the compiler ---"
make clean
make
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi
echo "--- Compilation successful ---"
echo ""

# Check if there are any test files
if [ -z "$(ls -A $TEST_DIR)" ]; then
    echo "No test files found in '$TEST_DIR'. Please add some test files."
    echo "Example: echo 'int main() { int x; }' > $TEST_DIR/test1.c"
fi

# Run compiler on each test file
for testfile in $TEST_DIR/*; do
    if [ -f "$testfile" ]; then
        filename=$(basename -- "$testfile")
        output_file="$OUTPUT_DIR/${filename%.*}_symtable.txt"
        
        echo "--- Running test: $testfile ---"
        $COMPILER < "$testfile" > "$output_file"
        cat "$output_file"
        echo "----------------------------------------"
        echo ""
    fi
done

