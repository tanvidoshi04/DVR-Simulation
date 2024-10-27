# Makefile for Distance Vector Routing Simulation

# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Target executable name
TARGET = dvr_simulation

# Source files
SRC = dvr_simulation.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)
