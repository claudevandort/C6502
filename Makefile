CC = gcc
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c99
SOURCE = src/*.c
OUTPUT = bin/C6502

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(SOURCE) -o $(OUTPUT)

run:
	./$(OUTPUT)

test:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) \
	tests/*.c \
	src/memory.c \
	src/cpu.c \
	-o bin/tests \
	-I/opt/homebrew/Cellar/cunit/2.1-3/include \
	-L/opt/homebrew/Cellar/cunit/2.1-3/lib \
	-lcunit && ./bin/tests && bin/tests