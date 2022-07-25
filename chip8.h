#ifndef _CHIP8_H
#define _CHIP8_H

#include <stdio.h>

// struct holding the full state of a Chip-8 system
typedef struct Chip8 {
    unsigned char v[16];          // registers (0x0 - 0xF)
    unsigned short pc;            // progam counter
    unsigned short I;             // index register
    unsigned short stack[16];     // stack
    unsigned short sp;            // stack pointer
    unsigned char mem[4096];      // memory
    unsigned char screen[8][4];   // screen. Every pixel is a bit; 64x32 bits/pixels
} Chip8;

// SETUP FUNCTIONS

// loads data from file into mem
void loadData(Chip8 *c8, int memSize, const char *path);

// loads mem full of empty data
void loadEmptyMem(Chip8 *c8);

// set every pixel on the Chip8 screen to 0
void initChip8Screen(Chip8 *c8);


// OUTPUT FUNCTIONS

// outputs a dump of the data loaded into [mem]
void outputMemDump(Chip8 *c8, FILE *out);

// prints the value of each register, sp, and pc
void statusDump(Chip8 *c8, FILE *out);




#endif // _CHIP8_H