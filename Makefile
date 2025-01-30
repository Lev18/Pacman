#compiler
CXX=g++

# c++ flags
CXXFLAGS=-std=c++2a -Wall -Wextra -Im -ggdb

# all sfml libraries
SFML_LIBS=-lsfml-graphics -lsfml-window -lsfml-system 

# Directories
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

SRC:=$(wildcard $(SRC_DIR)/*.cpp)
OBJ:=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
EXEC:=$(BIN_DIR)/pacman

all: $(EXEC)

$(EXEC): $(OBJ)
		mkdir -p $(BIN_DIR)
		$(CXX) $(CXXFLAGS) $^ -o $@  $(SFML_LIBS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
		mkdir -p $(OBJ_DIR)
		$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all

