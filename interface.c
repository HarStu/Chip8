#include "interface.h"

#include <stdio.h>
#include <ncurses.h>
#include "chip8.h"

void startcurses() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
}

void endcurses() {
    endwin();
}

void writeScreenBuffer(Chip8 c8) {
    // draws the data currently in c8.screen[][] to the terminal

    //clear the ncurses screen
    clear();

    // print the bits in c8.screen[][] to the screen buffer as binary numbers
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            if (c8.screen[x][y] == 0) {
                mvprintw(y, x, ".");
            } 
            else {
                mvprintw(y, x, "#");
            }
        }
    }
}

void writeStateBuffer(Chip8 c8) {
    // print program counter
    mvprintw(0, 65, "pc: %03x", c8.pc);

    // print index register
    mvprintw(1, 65, "I: %03x", c8.I);
    
    // print registers
    for (int i = 0; i < 0x10; i++) {
        mvprintw(5 + i, 65, "v%x: %02x\n", i, c8.v[i]);
    }

    // print stack pointer
    mvprintw(4, 76, "sp: %x", c8.sp);

    // print stack
    for (int i = 0; i < 16; i++) {
        mvprintw(5 + i, 76, "stack[%x]: %03x", i, c8.stack[i]);
    }
}