SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o .h

EXECUTABLE_EXTENSION = .exe

PROJECT_NAME = test$(EXECUTABLE_EXTENSION)
PROJECT_SOURCE = main.c

REPOSITORY_PATH = $(HOME)/uni/metr4810/metr4810-team5-project

FTDI_LIB_PATH = $(REPOSITORY_PATH)/ftdi/lib

CC = gcc
# CFLAGS = $(FTDI_LIB_PATH)/libftd2xx.a -Wall -Wextra -I$(FTDI_LIB_PATH)
CFLAGS = libftd2xx.a -Wall -Wextra
LDFLAGS = -lpthread -ldl -lrt

RM = rm -fR


.PHONY: clean

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(PROJECT_SOURCE)
#	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
#	gcc $(CFLAGS) $(PROJECT_SOURCE) -o $(PROJECT_NAME) $(LDFLAGS)
	gcc $(PROJECT_SOURCE) -o $(PROJECT_NAME) libftd2xx.a -lpthread -ldl -lrt -Wall -Wextra

clean:
	$(RM) $(PROJECT_NAME)