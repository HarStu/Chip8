#include "interface.h"

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

void drawScreen(Chip8 c8) {
    // draws the data currently in c8.screen[][] to the terminal

    //clear the ncurses screen
    clear();

    // print the bits in c8.screen[][] to the screen buffer as binary numbers
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            if (c8.screen[x][y] == 0) {
                mvprintw(x, y, "0");
            } 
            else {
                mvprintw(x, y, "H");
            }
        }
    }
    // refresh the screen to display what's been printed to the buffer
    refresh();
}