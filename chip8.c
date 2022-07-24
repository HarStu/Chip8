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

// output the contents of c8->mem in hex
void outputMemDump(Chip8 *c8) {
	printf("\n");
	for (int i = 0; i < 64; i++) {
		if ((i % 8 == 0) && (i != 0)) {
			printf("\n");
		}
		printf(" %04x", c8->mem[i]);
	}
	printf("\n");
}

// output the contents of c8's registers, program counter, and stack pointer
void statusDump(Chip8 *c8) {
	printf("\n");
	for (int i = 0x0; i < 0x10; i++) {
		printf("v%x: %02x\n", i, c8->v[i]);
	}
	printf("program counter (after running): %04x\nstack pointer: %d", c8->pc, c8->sp);
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
