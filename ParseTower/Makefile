# Makefile for ParseTower Compiler

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = parsetower
SOURCES = main.cpp lexer.cpp parser.cpp semantic.cpp ir.cpp optimizer.cpp codegen.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = token.h ast.h lexer.h parser.h semantic.h ir.h optimizer.h codegen.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build successful! Executable: $(TARGET)"

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete."

# Run with example input
test: $(TARGET)
	@echo "Running ParseTower compiler with example input..."
	./$(TARGET) example.td -ir

# Install (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all clean test install uninstall

