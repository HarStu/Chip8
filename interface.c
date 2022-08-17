#include "interface.h"

#include <stdio.h>
#include "chip8.h"

void startSDL() {
    // TODO
}

void endSDL() {
    // TODO
}

void drawScreen(Chip8 c8) {
    // TODO
}

void writeStateBuffer(Chip8 c8) {
    // TODO
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
    // TODO
}