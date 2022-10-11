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

// delay to control the number of cycles which should be executed each second
struct timespec cycleDelay = {0, 25000000};
struct timespec cycleDelayRemaining = {0, 0};

// timeval structs used to decrement timers accurately
struct timespec start, end; 

int main(int argc, char *argv[]) {
	// open the logfile
	logfile = fopen("./log.txt", "w+");

	// init virtual machine memory and load .ch8 file
	loadData(&c8, 4096, argv[1]);

	// load font data into virtual machine memory
	loadFont(&c8);

	// load a blank screen into the virtual machine
	clearScreen(&c8);

	// init timeval structs
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

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

			// key is pressed down
			else if (e.type == SDL_KEYDOWN) {
				// update virtual machine keypad state based on key pressed
				// move this switch statement into interface.c updateInput() later
				switch(e.key.keysym.sym) {
					case SDLK_1:
						c8.keypad[0x00] = 0x01;
					case SDLK_2:
						c8.keypad[0x01] = 0x01;
					case SDLK_3:
						c8.keypad[0x02] = 0x01;
					case SDLK_4:
						c8.keypad[0x03] = 0x01;
					case SDLK_q:
						c8.keypad[0x04] = 0x01;
					case SDLK_w:
						c8.keypad[0x05] = 0x01;
					case SDLK_e:
						c8.keypad[0x06] = 0x01;
					case SDLK_r:
      					c8.keypad[0x07] = 0x01;
					case SDLK_a:                  					
       					c8.keypad[0x08] = 0x01;
					case SDLK_s:
      					c8.keypad[0x09] = 0x01;
					case SDLK_d:
      					c8.keypad[0x0a] = 0x01;				
					case SDLK_f:
						c8.keypad[0x0b] = 0x01;
					case SDLK_z:
						c8.keypad[0x0c] = 0x01;
					case SDLK_x:
						c8.keypad[0x0d] = 0x01;
					case SDLK_c:
						c8.keypad[0x0e] = 0x01;
					case SDLK_v:
						c8.keypad[0x0f] = 0x01;
				}
			}
		}

		// run virtual machine cpu cycle (fetch, decode, execute opcode)
		cycle(&c8, logfile);

		// draw the screen
		drawScreen(&c8, &scr);
	}

	/* OLD MAIN LOOP - now handled above in SDL event loop
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
		drawScreen(&c8, &scr);

		// update timers
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		long microsecondDelta = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_nsec + start.tv_nsec);
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		uint64_t timerUnitsDelta = microsecondDelta / 16666;
		updateTimers(&c8, timerUnitsDelta);
	}
	*/

	endSDL();
	fclose(logfile);
	return 0;
}