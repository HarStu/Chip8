#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "chip8.h"

// call all the functions necessary to setup the ncurses enviroment
void startcurses();

// clean up and terminate the ncurses enviorment
void endcurses();

// print the current state of the screen in memory to the terminal
void writeScreenBuffer(Chip8 c8);

// print debug information
void writeStateBuffer(Chip8 c8);

// update timers
void updateTimers(Chip8 *c8, unsigned int dec);

// update the c8.input value
// if key is pressed, input will be set to 0xFF
void updateInput(Chip8 *c8);

#endif // _DISPLAY_H