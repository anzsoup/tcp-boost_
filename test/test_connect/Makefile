DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= ../../medianet/src
INCLUDE	:= ../../medianet/include
LIB		:= ../../medianet/lib

LIBRARIES	:=
EXECUTABLE	:= medianet_test


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp ./main.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) -lboost_system

clean:
	-rm $(BIN)/*
