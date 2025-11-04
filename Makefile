# Makefile for the Compiler

CC = gcc
LEX = flex
YACC = bison
TARGET = compiler

# Source files
LEX_SRC = lexer.l
YACC_SRC = parser.y
AST_SRC = ast.c
SEM_SRCS = types.c symbol_table.c semantic.c
TAC_SRC = tac.c
UTIL_SRCS = list.c  # <-- NEW UTILITY SOURCES
CG_SRCS = mips.c    # <-- NEW CODEGEN SOURCES

# Generated files
LEX_C = lex.yy.c
YACC_C = parser.tab.c
YACC_H = parser.tab.h
COMPILER_EXECUTABLE = compiler.exe

# Flags
CFLAGS = -Wall -Wno-unused-function -g # -g for debugging symbols

.PHONY: all clean run

all: $(TARGET)

# Updated build rule to include all new source files
$(TARGET): $(YACC_C) $(LEX_C) $(AST_SRC) $(SEM_SRCS) $(TAC_SRC) $(UTIL_SRCS) $(CG_SRCS)
	$(CC) $(CFLAGS) $(YACC_C) $(LEX_C) $(AST_SRC) $(SEM_SRCS) $(TAC_SRC) $(UTIL_SRCS) $(CG_SRCS) -o $(TARGET)

$(LEX_C): $(LEX_SRC) $(YACC_H)
	$(LEX) $<

$(YACC_C) $(YACC_H): $(YACC_SRC)
	$(YACC) -d -v $<

run: $(TARGET)
	./$(TARGET) test/test3.c

clean:
ifeq ($(OS),Windows_NT)
	del /f /q $(LEX_C) $(YACC_C) $(YACC_H) $(COMPILER_EXECUTABLE) parser.output output.s
else
	rm -f $(LEX_C) $(YACC_C) $(YACC_H) $(TARGET) $(COMPILER_EXECUTABLE) parser.output output.s
endif