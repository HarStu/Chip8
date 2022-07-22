#ifndef _CHIP8_H
#define _CHIP8_H

//loads data from file into mem
void loadData(unsigned char *mem, int memSize, const char *path);

//outputs a dump of the data loaded into [mem]
void outputMemDump();

//loads mem full of empty data
void loadEmptyMem();

//prints the value of each register, sp, and pc
void statusDump();

#endif // _CHIP8_H