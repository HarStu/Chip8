# compiler
CC = gcc

# compiler flags
CFLAGS = -g -Wall -o

# the target build executable
TARGET = H_CHIP_8

default: all

all: main.c chip8.o cpu.o
	$(CC) $(CFLAGS) $(TARGET) main.c chip8.o cpu.o

chip8.o: chip8.c
	$(CC) chip8.c -o chip8.o -c

cpu.o: cpu.c
	$(CC) cpu.c -o cpu.o -c


clean: 
	$(RM) $(TARGET) *.o
