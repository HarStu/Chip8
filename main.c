#include <stdio.h>
#include <ncurses.h>
#include "chip8.h"
#include "cpu.h"
#include "display.h"

// declare struct which holds all data about the simulated machine
Chip8 c8;

// create log file
FILE *logfile;

int main() {
	logfile = fopen("./log.txt", "w+");
	startcurses();

	loadData(&c8, 4096, "./test.ch8");
	c8.pc = 0;
	//program will terminate once pc gets too high
	//temporary measure so we don't just end at a seg fault
	while (c8.pc < 32) {
		// fetch, decode, execute opcode
		// increment pc by 2
		cycle(&c8, logfile);

		// update timers
		// TODO

		// if the drawflag is set, draw the screen
		// TODO

		// store key press state
		// TODO
	}
	endcurses();
	fclose(logfile);
	return 0;
}