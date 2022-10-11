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

void drawScreen(Chip8 *c8, Screen *scr) {
    // draw the virtual machine screen to the SDL screen

    // load images to use for pixels which are turned on and off
    SDL_Surface *onPixel = SDL_LoadBMP("./onPixel.bmp");
    SDL_Surface *offPixel = SDL_LoadBMP("./offPixel.bmp");

    // clear the screen
    SDL_FillRect(scr->sur, NULL, SDL_MapRGB((scr->sur)->format, 0xFF, 0xFF, 0xFF));

    // draw each pixel onto the screen
    for (int w = 0; w < 64; w++) {
        for (int h = 0; h < 32; h++) {
            // struct to represent the pixel being drawn
            SDL_Rect pixel;
            pixel.w = 2;
            pixel.h = 2;
            
            pixel.x = (w * 2) + 5;
            pixel.y = (h * 2) + 5;

            // draw pixel
            if (c8->screen[w][h] == 0x0) {
                SDL_BlitSurface(offPixel, NULL, scr->sur, &pixel);
            }
            else if (c8->screen[w][h] == 0x1) {
                SDL_BlitSurface(onPixel, NULL, scr->sur, &pixel);
            }
        }
    }

    // update the screen to show the buffer which has been drawn to
    SDL_UpdateWindowSurface(scr->win);
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

void updateInput(Chip8 *c8, SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
            case SDLK_1:
                c8->keypad[0x00] = 0x01;
                break;
            case SDLK_2:
                c8->keypad[0x01] = 0x01;
                break;
            case SDLK_3:
                c8->keypad[0x02] = 0x01;
                break;
            case SDLK_4:
                c8->keypad[0x03] = 0x01;
                break;
            case SDLK_q:
                c8->keypad[0x04] = 0x01;
                break;
            case SDLK_w:
                c8->keypad[0x05] = 0x01;
                break;
            case SDLK_e:
                c8->keypad[0x06] = 0x01;
                break;
            case SDLK_r:
                c8->keypad[0x07] = 0x01;
                break;
            case SDLK_a:                  					
                c8->keypad[0x08] = 0x01;
                break;
            case SDLK_s:
                c8->keypad[0x09] = 0x01;
                break;
            case SDLK_d:
                c8->keypad[0x0a] = 0x01;				
                break;
            case SDLK_f:
                c8->keypad[0x0b] = 0x01;
                break;
            case SDLK_z:
                c8->keypad[0x0c] = 0x01;
                break;
            case SDLK_x:
                c8->keypad[0x0d] = 0x01;
                break;
            case SDLK_c:
                c8->keypad[0x0e] = 0x01;
                break;
            case SDLK_v:
                c8->keypad[0x0f] = 0x01;
                break;
        }
    }
}