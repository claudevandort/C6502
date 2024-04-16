CC = gcc
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c99
SOURCE = src/*.c
OUTPUT = bin/C6502

TEST_OUTPUT = bin/tests

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
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(SOURCE) -o $(OUTPUT)

run:
	./$(OUTPUT)

test:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) \
	tests/*.c src/memory.c src/cpu.c \
	$(INCLUDE_PATHS) $(LIBRARY_PATHS) \
	-o $(TEST_OUTPUT) -lcunit && ./$(TEST_OUTPUT)