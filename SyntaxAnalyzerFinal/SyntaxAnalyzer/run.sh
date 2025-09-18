#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Create results directory
mkdir -p results

echo "Running Syntax Analyzer Tests..."
echo "================================="

# Test counters
total_tests=0
passed_tests=0
failed_tests=0

# Function to run a single test
run_test() {
    local test_file=$1
    local test_name=$(basename "$test_file" .c)
    local result_file="results/${test_name}.result"

    echo -e "\n${BLUE}Testing: $test_name${NC}"
    echo "----------------------------------------"

    total_tests=$((total_tests + 1))

    # Run the syntax analyzer with timeout to prevent hanging
    timeout 5s ./bin/syntax_analyzer "$test_file" > "$result_file" 2>&1
    local exit_code=$?

    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}TIMEOUT: Test took too long (possible infinite loop)${NC}"
        failed_tests=$((failed_tests + 1))
        echo "TIMEOUT - Test execution exceeded 5 seconds" >> "$result_file"
    elif [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}PASSED: Syntax analysis completed successfully${NC}"
        passed_tests=$((passed_tests + 1))

        # Check if token table was generated
        if grep -q "Token.*Token_Type" "$result_file"; then
            echo -e "${GREEN}✓ Token table generated${NC}"
        fi

        # Check for AST and symbol table files
        local base_name="${test_file%.*}"
        if [ -f "${base_name}.ast" ]; then
            mv "${base_name}.ast" "results/"
            echo -e "${GREEN}✓ AST file generated${NC}"
        fi
        if [ -f "${base_name}.sym" ]; then
            mv "${base_name}.sym" "results/"
            echo -e "${GREEN}✓ Symbol table file generated${NC}"
        fi
    else
        echo -e "${RED}FAILED: Syntax errors detected (exit code: $exit_code)${NC}"
        failed_tests=$((failed_tests + 1))
    fi

    # Show first few lines of output
    echo "Output preview:"
    head -5 "$result_file" 2>/dev/null || echo "No output generated"
    echo ""
}

# Clean previous results
rm -f results/*.result results/*.ast results/*.sym

# Test valid syntax files first
echo -e "\n${YELLOW}=== Testing Valid Syntax Files ===${NC}"
for test_file in test/test_correct.c test/test1_basic_arithmetic.c test/test2_control_structures.c test/test3_functions_arrays.c; do
    if [ -f "$test_file" ]; then
        run_test "$test_file"
    fi
done

# Test error detection files
echo -e "\n${YELLOW}=== Testing Error Detection ===${NC}"
for test_file in test/*.c; do
    if [ -f "$test_file" ]; then
        run_test "$test_file"
    fi
done

# Summary
echo -e "\n${BLUE}===============================${NC}"
echo -e "${BLUE}Test Summary${NC}"
echo -e "${BLUE}===============================${NC}"
echo -e "Total tests: $total_tests"
echo -e "${GREEN}Passed: $passed_tests${NC}"
echo -e "${RED}Failed: $failed_tests${NC}"

# Show all results in a summary file
summary_file="results/test_summary.txt"
echo "Test Execution Summary - $(date)" > "$summary_file"
echo "=================================" >> "$summary_file"
echo "Total tests: $total_tests" >> "$summary_file"
echo "Passed: $passed_tests" >> "$summary_file"
echo "Failed: $failed_tests" >> "$summary_file"
echo "" >> "$summary_file"

echo -e "\n${BLUE}Detailed results saved in: results/${NC}"
echo -e "${BLUE}Summary saved in: $summary_file${NC}"

if [ $passed_tests -eq $total_tests ]; then
    echo -e "\n${GREEN}All tests completed successfully!${NC}"
    exit 0
else
    echo -e "\n${YELLOW}Some tests failed. Check the results directory for details.${NC}"
    exit 1
fi