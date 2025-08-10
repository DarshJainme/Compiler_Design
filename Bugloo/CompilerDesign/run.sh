#!/bin/bash

# Script to run lexical analyzer on all test cases

EXECUTABLE="./lexical_analyzer"
TEST_DIR="test"
OUTPUT_FILE="test_results.txt"

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable '$EXECUTABLE' not found. Please run 'make' first."
    exit 1
fi

# Check if test directory exists
if [ ! -d "$TEST_DIR" ]; then
    echo "Error: Test directory '$TEST_DIR' not found."
    exit 1
fi

# Clear previous output file
> "$OUTPUT_FILE"

echo "Running Lexical Analyzer on all test cases..."
echo "Output will be saved to $OUTPUT_FILE"
echo "=================================================="

# Also output to console and file simultaneously
{
    echo "Running Lexical Analyzer on all test cases..."
    echo "=================================================="

    # Run tests on all .c files in test directory
    for test_file in "$TEST_DIR"/*.c; do
        if [ -f "$test_file" ]; then
            echo
            echo "Testing: $(basename "$test_file")"
            echo "----------------------------------------"
            "$EXECUTABLE" "$test_file"
            echo
        fi
    done

    echo "=================================================="
    echo "All tests completed."
} | tee "$OUTPUT_FILE"