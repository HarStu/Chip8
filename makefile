# compiler
CC = gcc

# compiler flags
CFLAGS = -g -Wall -o

# the target build executable
TARGET = H_CHIP_8

# libraries we're linking against
LINKER_FLAGS = -lSDL2

default: clean all

all: main.c chip8.o cpu.o interface.o
	$(CC) $(CFLAGS) $(TARGET) main.c chip8.o cpu.o interface.o $(LINKER_FLAGS)

chip8.o: chip8.c
	$(CC) chip8.c -o chip8.o -c

cpu.o: cpu.c
	$(CC) cpu.c -o cpu.o -c

interface.o: interface.c
	$(CC) interface.c -o interface.o -c


clean: 
	$(RM) $(TARGET) *.o