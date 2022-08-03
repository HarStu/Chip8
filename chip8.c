#include "chip8.h"

#include <stdio.h>
#include "cpu.h"


// load data from the file located at 'path' into the memory of 
// the virtual machine. Must specify the size of c8's memory (in bytes)
// also sets the timers to 0
void loadData(Chip8 *c8, int memSize, const char *path) {
	FILE *fp;
	fp = fopen(path,"rb");
	while (fread((c8->mem + 0x0200),1,memSize,fp)); // doing a little pointer arithmetic 
	fclose(fp);

	// set timers to 0 since we're loading data in here anyway
	c8->dt = 0;
	c8->st = 0;
}

// fill c8's memory and registers with empty values
void loadEmptyMem(Chip8 *c8) {
	for (int i = 0; i < 4097; i++) {
		c8->mem[i] = 0x00;
	}
	for (int i = 0; i < 17; i++) {
		c8->v[i] = 0x0000;
	}
}

// load font data into virtual machine memory
void loadFont(Chip8 *c8) {
	unsigned char fontData[] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0 located at 0x50
		0x20, 0x60, 0x20, 0x20, 0x70, // 1 located at 0x55
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2 located at 0x5a
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3 located at 0x5f
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4 located at 0x64
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5 located at 0x69
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6 located at 0x6e
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7 located at 0x73
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8 located at 0x78
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9 located at 0x7d
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A located at 0x82
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B located at 0x87
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C located at 0x8c
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D located at 0x91
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E located at 0x96
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F located at 0x9b
	};

	for (int i = 0; i < 80; i++) {
		c8->mem[(i + 0x50)] = fontData[i];
	}
}

// set every pixel on the Chip8 screen to 0
void clearScreen(Chip8 *c8) {
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 32; y++)
			c8->screen[x][y] = 0x0;
	}
}

// output the contents of c8->mem in hex
// output is formatted as 2-byte opcodes
void outputMemDump(Chip8 *c8, FILE *out) {
	printf("\n");
	for (int i = 0; i < 4096; i += 2) {
		if ((i % 16 == 0) && (i != 0)) {
			fprintf(out, "\n");
		}
		fprintf(out,"%02x%02x ", c8->mem[i], c8->mem[i+1]);
	}
	fprintf(out, "\n\n");
}

// output the contents of c8's registers, program counter, and stack pointer
void statusDump(Chip8 *c8, FILE *out) {
	printf("\n");
	for (int i = 0x0; i < 0x10; i++) {
		fprintf(out, "v%x: %02x\n", i, c8->v[i]);
	}
	fprintf(out, "program counter (after running): %03x\nstack pointer: %x\nindex register: %03x", c8->pc, c8->sp, c8->I);
}


