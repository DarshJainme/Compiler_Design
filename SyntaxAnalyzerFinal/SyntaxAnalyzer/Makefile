# Makefile for YACC/BISON C-like Language Syntax Analyzer

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -Wno-implicit-function-declaration -Wno-unused-parameter -Wno-incompatible-pointer-types
LDFLAGS = 

# Directories
SRCDIR = src
TESTDIR = test
OBJDIR = obj
BINDIR = bin

# Tools
YACC = bison
LEX = flex
YACCFLAGS = -d -v --report=all
LEXFLAGS = 

# Source files
LEX_SRC = $(SRCDIR)/lexer.l
YACC_SRC = $(SRCDIR)/grammar.y
C_SOURCES = $(SRCDIR)/ast.c $(SRCDIR)/symbol_table.c $(SRCDIR)/main.c
HEADERS = $(SRCDIR)/ast.h $(SRCDIR)/symbol_table.h

# Generated files
LEX_OUTPUT = $(SRCDIR)/lex.yy.c
YACC_OUTPUT = $(SRCDIR)/grammar.tab.c
YACC_HEADER = $(SRCDIR)/grammar.tab.h
YACC_REPORT = $(SRCDIR)/grammar.output

# Object files
OBJECTS = $(OBJDIR)/lex.yy.o $(OBJDIR)/grammar.tab.o $(OBJDIR)/ast.o $(OBJDIR)/symbol_table.o $(OBJDIR)/main.o

# Executable
TARGET = $(BINDIR)/syntax_analyzer

# Test files
TEST_FILES = $(wildcard $(TESTDIR)/*.c)
TEST_RESULTS = $(TEST_FILES:.c=.result)

# Default target
all: $(TARGET)

# Create directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Generate lexer
$(LEX_OUTPUT): $(LEX_SRC) $(YACC_HEADER)
	$(LEX) $(LEXFLAGS) -o $@ $<

# Generate parser
$(YACC_OUTPUT) $(YACC_HEADER): $(YACC_SRC)
	$(YACC) $(YACCFLAGS) -o $(YACC_OUTPUT) $<

# Compile object files
$(OBJDIR)/lex.yy.o: $(LEX_OUTPUT) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/grammar.tab.o: $(YACC_OUTPUT) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/ast.o: $(SRCDIR)/ast.c $(SRCDIR)/ast.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/symbol_table.o: $(SRCDIR)/symbol_table.c $(SRCDIR)/symbol_table.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(HEADERS) $(YACC_HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Test targets
test: $(TARGET)
	@echo "Running syntax analyzer tests..."
	@chmod +x run.sh
	@./run.sh

test-verbose: $(TARGET)
	@echo "Running syntax analyzer tests in verbose mode..."
	@chmod +x run.sh
	@./run.sh -v

# Individual test file execution
test-file: $(TARGET)
	@if [ "$(FILE)" = "" ]; then \
	        echo "Usage: make test-file FILE=test/filename.c"; \
	else \
	        echo "Testing $(FILE)..."; \
	        $(TARGET) $(FILE); \
	fi

# Generate parser report
report: $(YACC_SRC)
	$(YACC) $(YACCFLAGS) --report=all -o /tmp/grammar.tab.c $<
	@echo "Parser report generated in $(YACC_REPORT)"
	@if [ -f $(YACC_REPORT) ]; then \
	        echo "=== Grammar Report Summary ==="; \
	        grep -E "(conflict|error|warning)" $(YACC_REPORT) || echo "No conflicts found"; \
	fi

# Debug target
debug: CFLAGS += -DDEBUG -DYYDEBUG
debug: clean $(TARGET)

# Clean targets
clean:
	rm -f src/lex.yy.c src/grammar.tab.c src/grammar.tab.h src/grammar.output
	rm -rf obj bin
	rm -f test/*.result test/*.ast test/*.sym
	rm -rf results
	rm -f *.ast *.sym *.result
	rm -f core *.core test_results.log
	rm -f test_summary.txt

clean-generated:
	rm -f $(LEX_OUTPUT) $(YACC_OUTPUT) $(YACC_HEADER) $(YACC_REPORT)

clean-objects:
	rm -rf $(OBJDIR)

clean-tests:
	rm -f $(TEST_RESULTS)

# Install target (optional)
install: $(TARGET)
	@echo "Installing syntax analyzer..."
	cp $(TARGET) /usr/local/bin/
	chmod +x /usr/local/bin/syntax_analyzer

# Uninstall target (optional)
uninstall:
	rm -f /usr/local/bin/syntax_analyzer

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the syntax analyzer (default)"
	@echo "  test         - Run all test cases"
	@echo "  test-verbose - Run tests with verbose output"
	@echo "  test-file    - Test specific file (usage: make test-file FILE=path/to/file.c)"
	@echo "  debug        - Build with debug flags"
	@echo "  report       - Generate parser analysis report"
	@echo "  clean        - Remove all generated files"
	@echo "  clean-generated - Remove only generated lexer/parser files"
	@echo "  clean-objects - Remove only object files"
	@echo "  clean-tests  - Remove test result files"
	@echo "  install      - Install to /usr/local/bin"
	@echo "  uninstall    - Remove from /usr/local/bin"
	@echo "  help         - Show this help message"

# Performance test
perf-test: $(TARGET)
	@echo "Running performance tests..."
	@time $(TARGET) test/test1_basic_arithmetic.c > /dev/null
	@time $(TARGET) test/test2_control_structures.c > /dev/null
	@time $(TARGET) test/test3_functions_arrays.c > /dev/null
	@time $(TARGET) test/test4_advanced_features.c > /dev/null

# Memory check (requires valgrind)
memcheck: $(TARGET)
	@echo "Running memory check on test files..."
	@if command -v valgrind > /dev/null; then \
	        valgrind --leak-check=full --show-leak-kinds=all $(TARGET) test/test1_basic_arithmetic.c; \
	else \
	        echo "Valgrind not found. Please install valgrind for memory checking."; \
	fi

# Code quality check
quality:
	@echo "Running code quality checks..."
	@if command -v cppcheck > /dev/null; then \
	        cppcheck --enable=all --suppress=missingIncludeSystem $(SRCDIR)/*.c $(SRCDIR)/*.h; \
	else \
	        echo "cppcheck not found. Skipping quality check."; \
	fi

# Documentation generation (if doxygen is available)
docs:
	@if command -v doxygen > /dev/null; then \
	        doxygen Doxyfile 2>/dev/null || echo "Doxyfile not found, skipping documentation generation"; \
	else \
	        echo "Doxygen not found. Please install doxygen for documentation generation."; \
	fi

# Show parser states and conflicts
show-conflicts: $(YACC_OUTPUT)
	@if [ -f $(YACC_REPORT) ]; then \
	        echo "=== Parser Conflicts ==="; \
	        grep -A 5 -B 5 "conflict" $(YACC_REPORT) || echo "No conflicts found"; \
	else \
	        echo "Parser report not found. Run 'make report' first."; \
	fi

# Dependency tracking
-include $(OBJECTS:.o=.d)

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

# Phony targets
.PHONY: all test test-verbose test-file debug clean clean-generated clean-objects clean-tests install uninstall help report perf-test memcheck quality docs show-conflicts

# Special variables
.DEFAULT_GOAL := all
.SUFFIXES:
MAKEFLAGS += --no-builtin-rules