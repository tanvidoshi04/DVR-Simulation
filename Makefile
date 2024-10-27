# Compiler and flags
CXX = g++
CXXFLAGS = -Wall

# Targets
TARGETS = Part1 Part2 Part3

# Default target: compile all
all: $(TARGETS)

# Compile each part separately
Part1: Part1.cpp
	$(CXX) $(CXXFLAGS) -o Part1 Part1.cpp

Part2: Part2.cpp
	$(CXX) $(CXXFLAGS) -o Part2 Part2.cpp

Part3: Part3.cpp
	$(CXX) $(CXXFLAGS) -o Part3 Part3.cpp

# Run each part
1: Part1
	./Part1

part1: Part1
	./Part1

2: Part2
	./Part2

part2: Part2
	./Part2

3: Part3
	./Part3

part3: Part3
	./Part3

# Clean up compiled files
clean:
	rm -f $(TARGETS)
