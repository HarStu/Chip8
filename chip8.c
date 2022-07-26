#include "chip8.h"

#include <stdio.h>
#include "cpu.h"


// load data from the file located at 'path' into the memory of the simulated
// machine c8. Must specify the size of c8's memory (in bytes)
void loadData(Chip8 *c8, int memSize, const char *path) {
	FILE *fp;
	fp = fopen(path,"rb");
	while (fread(c8->mem,1,memSize,fp));
	fclose(fp);
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

// set every pixel on the Chip8 screen to 0
void clearScreen(Chip8 *c8) {
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 32; y++)
			c8->screen[x][y] = 0x0;
	}
}

// output the contents of c8->mem in hex
void outputMemDump(Chip8 *c8, FILE *out) {
	printf("\n");
	for (int i = 0; i < 64; i++) {
		if ((i % 8 == 0) && (i != 0)) {
			fprintf(out, "\n");
		}
		fprintf(out," %04x", c8->mem[i]);
	}
	printf("\n");
}

// output the contents of c8's registers, program counter, and stack pointer
void statusDump(Chip8 *c8, FILE *out) {
	printf("\n");
	for (int i = 0x0; i < 0x10; i++) {
		fprintf(out, "v%x: %02x\n", i, c8->v[i]);
	}
	fprintf(out, "program counter (after running): %u\nstack pointer: %u\nindex register:%03x", c8->pc, c8->sp, c8->I);
}


