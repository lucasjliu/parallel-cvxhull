CPPFLAGS=-g -Wall -Wextra -std=c++11 -iquote src/ -Wno-sign-compare -fopenmp -DNDEBUG -o3
CXX=/usr/local/bin/g++-6
SRC_DIR=src/pch
BIN_DIR=bin
OBJ_DIR=obj
LDFLAGS=
LIBS=
INCLUDE=src
BIN_NAME=test

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(BIN_DIR) $(BIN_NAME)

test: $(OBJS)
	$(CXX) $(CPPFLAGS) -I$(INCLUDE) $(LIBS) $(OBJS) -o $(BIN_DIR)/test

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) -I$(INCLUDE) $(LIBS) -c $< -o $@

clean:
	rm $(BIN_DIR)/* $(OBJ_DIR)/*.o
