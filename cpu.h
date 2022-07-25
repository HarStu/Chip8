#ifndef _CPU_H
#define _CPU_H

#include "chip8.h"

// fetch, decode, and execute a chip-8 opcode
// increment pc by 2 bytes after
void cycle(Chip8 *c8,  FILE *out);

#endif // _CPU_H