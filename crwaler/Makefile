# Compiler
CXX = g++
CC = $(CXX)

# Compiler flags
CXXFLAGS = -std=c++17 -pedantic -Wall -Wconversion -Werror -pthread

# Libraries
LIBS = -lcurl -lcurlpp -lxml2 -ltidy -lhtmlcxx $(shell pkg-config --libs libmongocxx) -lssl -lcrypto

# Include flags
CPPFLAGS = -I/usr/include/libxml2 -I/usr/include/htmlcxx $(shell pkg-config --cflags libmongocxx)

# Linker flags
LDFLAGS = -Wl,-rpath,/usr/local/lib

# Directories
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Main target
main: $(OBJS) $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compile main.cpp
$(OBJ_DIR)/main.o: main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Compile all source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Target for running the program
check: main
	./main

# Phony target to clean up generated files
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) main

# Default target
all: main
