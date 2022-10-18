#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <SDL2/SDL.h>
#include "chip8.h"

// struct containing both the SDL window and SDL surface inside of it
typedef struct Screen {
    SDL_Window* win;
    SDL_Surface* sur;
} Screen;

// consts dictating SDL window size 
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// call all the functions necessary to setup the SDL enviroment
void startSDL(Screen *scr);

// clean up and terminate the SDL enviroment
void endSDL();

// draw the virtual machine screen to the SDL screen
void drawScreen(Chip8 *c8, Screen *scr);

// print debug information
void drawState(Chip8 c8);

// update timers
void updateTimers(Chip8 *c8);

// update the c8.input value
// if key is pressed, input will be set to 0xFF
void updateInput(Chip8 *c8, SDL_Event e);

#endif // _DISPLAY_H