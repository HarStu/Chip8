#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "chip8.h"

// call all the functions necessary to setup the SDL enviroment
void startSDL();

// clean up and terminate the SDL enviroment
void endSDL();

// print the current state of the screen in memory to the terminal
void drawScreen(Chip8 c8);

// print debug information
void drawState(Chip8 c8);

// update timers
void updateTimers(Chip8 *c8, unsigned int dec);

// update the c8.input value
// if key is pressed, input will be set to 0xFF
void updateInput(Chip8 *c8);

#endif // _DISPLAY_H