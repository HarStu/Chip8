#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include "chip8.h"
#include "cpu.h"
#include "interface.h"

// declare struct which holds all data about the simulated machine
Chip8 c8;

// create log file
FILE *logfile;

// delay to control the number of cycles which should be executed each second
struct timespec cycleDelay = {0, 50000000};
struct timespec cycleDelayRemaining = {0, 0};

int main(int argc, char *argv[]) {
	// open the logfile
	logfile = fopen("./log.txt", "w+");

	// load the .ch8 file into the virtual machine's memory
	loadData(&c8, 4096, argv[1]);

	// load a blank screen into the virtual machine
	clearScreen(&c8);

	// start ncurses
	startcurses();

	c8.pc = 0x0200;
	//program will terminate once pc reaches the end of memory
	while (c8.pc < 4096) {
		// check for a keypress
		// TODO

		// Delay the program to control the number of cycles per second
		nanosleep(&cycleDelay, &cycleDelayRemaining);

		// fetch, decode, execute opcode
		// increment pc by 2
		cycle(&c8, logfile);

		// write the state of the virtual machine's screen to the ncurses buffer
		writeScreenBuffer(c8);

		// write information about the state of the virtual machine to the ncurses output
		writeStateBuffer(c8);

		// draw the ncurses buffer to the screen;
		refresh();

		// update timers
		// TODO

		// if the drawflag is set, draw the screen
		// TODO
	}
	endcurses();
	fclose(logfile);
	return 0;
}