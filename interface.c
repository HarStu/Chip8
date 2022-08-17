#include "interface.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void startSDL(Screen* scr) {
    // start SDL and throw an error if it fails to start
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not init");
    }
    else {
        // create window
        scr->win = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        // if SDL fails to init the window, throw an error
        if (scr->win == NULL) {
            printf("Window not created! SDL_Error: %s\n", SDL_GetError());
        }
        // otherwise, get the window surface, paint it white, and update it
        scr->sur = SDL_GetWindowSurface(scr->win);
        SDL_FillRect(scr->sur, NULL, SDL_MapRGB((scr->sur)->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(scr->win);
    }
}

void endSDL(Screen* scr) {
    // Destroy window and end SDL process
    SDL_DestroyWindow(scr->win);
    SDL_Quit();
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