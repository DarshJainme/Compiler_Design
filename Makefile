# Makefile for the Compiler

CC = gcc
LEX = flex
YACC = bison
TARGET = compiler

# Source files
LEX_SRC = lexer.l
YACC_SRC = parser.y
AST_SRC = ast.c

# Generated files
LEX_C = lex.yy.c
YACC_C = parser.tab.c
YACC_H = parser.tab.h
COMPILER_EXECUTABLE = compiler.exe

# Flags
CFLAGS = -Wall -Wno-unused-function

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(YACC_C) $(LEX_C) $(AST_SRC)
	$(CC) $(CFLAGS) $(YACC_C) $(LEX_C) $(AST_SRC) -o $(TARGET)

$(LEX_C): $(LEX_SRC)
	$(LEX) $<

$(YACC_C) $(YACC_H): $(YACC_SRC)
	$(YACC) -d -v $<

run: $(TARGET)
	./$(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	del /f /q $(LEX_C) $(YACC_C) $(YACC_H) $(TARGET) ${COMPILER_EXECUTABLE} parser.output
else
	rm -f $(LEX_C) $(YACC_C) $(YACC_H) $(TARGET) ${COMPILER_EXECUTABLE} parser.output
endif
