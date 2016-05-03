SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o .hpp

EXECUTABLE_EXTENSION = .exe

PROJECT_NAME = rescuedrone$(EXECUTABLE_EXTENSION)
PROJECT_SOURCE = main.cpp

REPOSITORY_PATH = $(HOME)/uni/metr4810/metr4810-team5-project

LIBRARY_PATH = $(REPOSITORY_PATH)/pizero/lib
LIBRARY_SOURCE = $(wildcard $(LIBRARY_PATH)/*.cpp)

CC = g++
CFLAGS = -g -Wall -I$(LIBRARY_PATH)
LDFLAGS = -lboost_system -lboost_thread -lpthread

RM = rm -fR


.PHONY: clean

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(PROJECT_SOURCE) $(LIBRARY_SOURCE)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) $(PROJECT_NAME)