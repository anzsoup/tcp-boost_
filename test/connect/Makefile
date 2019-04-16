DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= ../../tcp-boost/src
INCLUDE	:= ../../tcp-boost/include
LIB		:= ../../tcp-boost/lib

LIBRARIES	:= -lboost_system -lboost_thread
EXECUTABLE	:= tcp_boost_test


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp ./main.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
