CC = gcc
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c99
SOURCE = tests/*.c src/6502.c
OUTPUT = bin/C6502

DEBUG_FLAGS = -g
OUTPUT = bin/tests

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	INCLUDE_PATHS = -I/use/include
	LIBRARY_PATHS = -L/usr/lib
endif
ifeq ($(UNAME_S), Darwin)
	INCLUDE_PATHS = -I/opt/homebrew/Cellar/cunit/2.1-3/include
	LIBRARY_PATHS = -L/opt/homebrew/Cellar/cunit/2.1-3/lib
endif

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(SOURCE) \
		$(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(OUTPUT) -lcunit

debug-build:
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAGS) $(LANG_STD) $(SOURCE) \
		$(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(OUTPUT) -lcunit

run:
	./$(OUTPUT)

test:
	make build && make run

debug:
	make debug-build && make run