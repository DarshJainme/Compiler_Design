# Compiler and tools
CXX = g++
FLEX = flex
CXXFLAGS = -std=c++11 -Wall

# Executable name
TARGET = lexical_analyzer

# Source and object files
SOURCES = src/main.cpp src/lexer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile C++ source files into object files
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to generate lexer.cpp from lexer.l
src/lexer.cpp: src/lexer.l src/tokens.h
	$(FLEX) -o src/lexer.cpp src/lexer.l

# Rule to compile the generated lexer.cpp
src/lexer.o: src/lexer.cpp src/tokens.h
	$(CXX) $(CXXFLAGS) -c src/lexer.cpp -o src/lexer.o

# Phony target for cleaning up the directory
clean:
	rm -f $(OBJECTS) $(TARGET) src/lexer.cpp test_results.txt

.PHONY: all clean