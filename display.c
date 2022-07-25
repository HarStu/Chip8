#include "display.h"

#include <stdio.h>
#include <ncurses.h>
#include "chip8.h"

void startcurses() {
    initscr();
    cbreak();
    noecho();
}

void endcurses() {
    endwin();
}

void drawscreen(Chip8 *c8) {

}