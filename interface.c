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

    // clear the ncurses screen
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

    // print input
    mvprintw(0, 76, "input: %x", c8.input);

    // print stack pointer
    mvprintw(4, 76, "sp: %x", c8.sp);

    // print stack
    for (int i = 0; i < 16; i++) {
        mvprintw(5 + i, 76, "stack[%x]: %03x", i, c8.stack[i]);
    }


    // print timers
    mvprintw(2, 65, "dt: %i", c8.dt);
    mvprintw(3, 65, "st: %i", c8.st);

    // print sound timer ouput since ncurses doesn't have sound fx
    if (c8.st > 0) {
        mvprintw(1, 76, "**BEEP**");
    }
}

void updateTimers(Chip8 *c8, unsigned int dec) {
    // update delay timer
    if (dec > c8->dt) {
        c8->dt = 0;
    }
    else {
        c8->dt = (c8->dt) - dec;
    }

    // update sound timer
    if (dec > c8->st) {
        c8->st = 0;
    }
    else {
        c8->st = (c8->st) - dec;
    }
}

void updateInput(Chip8 *c8) {
    char keypress = getch();

    switch(keypress) {
        case '1':
            c8->input = 0x1;
            break;
        case '2':
            c8->input = 0x2;
            break;
        case '3':
            c8->input = 0x3;
            break;
        case '4':
            c8->input = 0xC;
            break;
        case 'q':
            c8->input = 0x4;
            break;
        case 'w':
            c8->input = 0x5;
            break;
        case 'e':
            c8->input = 0x6;
            break;
        case 'r':
            c8->input = 0xD;
            break;
        case 'a':
            c8->input = 0x7;
            break;
        case 's':
            c8->input = 0x8;
            break;
        case 'd':
            c8->input = 0x9;
            break;
        case 'f':
            c8->input = 0xE;
            break;
        case 'z':
            c8->input = 0xA;
            break;
        case 'x':
            c8->input = 0x0;
            break;
        case 'c':
            c8->input = 0xB;
            break;
        case 'v':
            c8->input = 0xF;
            break;
        case ERR:
            c8->input = 0xFF;
    }
}