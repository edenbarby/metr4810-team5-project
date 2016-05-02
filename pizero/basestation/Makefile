SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o .hpp


PROJECT_NAME = basestation
PROJECT_SOURCE = main.cpp

REPOSITORY_PATH = $(HOME)/uni/metr4810/metr4810-team5-project

LIBRARY_PATH = $(REPOSITORY_PATH)/pizero/lib
LIBRARY_SOURCE = $(wildcard $(LIBRARY_PATH)/*.cpp)

CXX = g++
CXXFLAGS = -g -Wall -I$(LIBRARY_PATH)

RM = rm -fR


.PHONY: clean

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(PROJECT_SOURCE) $(LIBRARY_SOURCE)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(PROJECT_NAME)