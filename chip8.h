#ifndef _CHIP8_H
#define _CHIP8_H

// struct holding the full state of a Chip-8 system
typedef struct Chip8 {
    unsigned char v[16];       // registers (0x0 - 0xF)
    unsigned short pc;         // progam counter
    unsigned short I;          // index register
    unsigned short stack[16];  // stack
    unsigned short sp;         // stack pointer
    unsigned char mem[4096];   // memory
} Chip8;

// loads data from file into mem
void loadData(Chip8 *c8, int memSize, const char *path);

// outputs a dump of the data loaded into [mem]
void outputMemDump(Chip8 *c8);

// loads mem full of empty data
void loadEmptyMem(Chip8 *c8);

// prints the value of each register, sp, and pc
void statusDump(Chip8 *c8);

#endif // _CHIP8_H