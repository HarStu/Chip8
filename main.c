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

// flag for SDL event loop
bool quit_SDL = false;


int main(int argc, char *argv[]) {
	// open the logfile
	logfile = fopen("./log.txt", "w+");

	// init virtual machine memory and load .ch8 file
	loadData(&c8, 4096, argv[1]);

	// load font data into virtual machine memory
	loadFont(&c8);

	// load a blank screen into the virtual machine
	clearScreen(&c8);

	// start SDL and create window
	startSDL(&scr);

	// SDL event handler
	SDL_Event e;

	// SDL event loop
	while (quit_SDL == false) {
		// clear keypad state before checking event queue
		for (int i = 0; i < 0x10; i++) {
			c8.keypad[i] = 0x00;
		}

		// handling pending events
		while (SDL_PollEvent(&e) != 0) {
			// request to quit SDL
			if (e.type == SDL_QUIT) {
				quit_SDL = true;
			}

			// update virtual machine keypad state
			// move this statement into interface.c updateInput() later
			else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
				updateInput(&c8, e);
			}
		}

		// temporary delay
		SDL_Delay(1000);

		// emulate the CPU cycle (fetch, decode, execute opcode)
		cycle(&c8, logfile);

		// draw the screen
		drawScreen(&c8, &scr);
	}

	// once SDL quits, clean up and end the program
	endSDL();
	fclose(logfile);
	return 0;
}