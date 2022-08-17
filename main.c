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

// struct holding SDL window and surface
Screen scr;

// delay to control the number of cycles which should be executed each second
struct timespec cycleDelay = {0, 25000000};
struct timespec cycleDelayRemaining = {0, 0};

// timeval structs used to decrement timers accurately
struct timespec start, end; 

int main(int argc, char *argv[]) {
	// open the logfile
	logfile = fopen("./log.txt", "w+");

	// load the .ch8 file into virtual machine memory
	loadData(&c8, 4096, argv[1]);

	// load font data into virtual machine memory
	loadFont(&c8);

	// load a blank screen into the virtual machine
	clearScreen(&c8);

	// init timeval structs
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	// start SDL and create window
	startSDL(&scr);

	c8.pc = 0x0200;
	//program will terminate once pc reaches the end of memory
	while (c8.pc < 4096) {
		// Delay the program to control the number of cycles per second
		nanosleep(&cycleDelay, &cycleDelayRemaining);

		// fetch input
		updateInput(&c8);

		// fetch, decode, execute opcode
		// increment pc by 2
		cycle(&c8, logfile);

		// TODO Draw the screen

		// update timers
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		long microsecondDelta = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_nsec + start.tv_nsec);
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		uint64_t timerUnitsDelta = microsecondDelta / 16666;
		updateTimers(&c8, timerUnitsDelta);
	}
	endSDL();
	fclose(logfile);
	return 0;
}