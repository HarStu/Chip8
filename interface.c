#include "interface.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"

const int WIDTH = 64;
const int HEIGHT = 32;

int prevTime = 0;

void startSDL(Screen* scr) {
    // start SDL and throw an error if it fails to start
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not init");
    }
    else {
        // init window
        scr->win = SDL_CreateWindow("H's Chip 8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (WIDTH * 10), (HEIGHT * 10), SDL_WINDOW_SHOWN);
        // if SDL fails to init the window, throw an error
        if (scr->win == NULL) {
            printf("Window init failed! SDL_Error: %s\n", SDL_GetError());
        }

        // init renderer
        scr->ren = SDL_CreateRenderer(scr->win, -1, SDL_RENDERER_ACCELERATED);
        // if SDL fails to init renderer, throw an error
        if (scr->ren == NULL) {
            printf("Renderer init failed! SDL_Error: %s\n", SDL_GetError());
        }

        // init texture
        scr->tex = SDL_CreateTexture(scr->ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
        // if SDL fails to init texture, throw an error
        if (scr-> tex == NULL) {
            printf("Texture init failed! SDL_Error: %s\n", SDL_GetError());
        }
    }
}

void endSDL(Screen* scr) {
    // Destroy SDL entities and end SDL process
    SDL_DestroyTexture(scr->tex);
    SDL_DestroyRenderer(scr->ren);
    SDL_DestroyWindow(scr->win);
    scr->tex == NULL;
    scr->ren == NULL;
    scr->win == NULL;
    SDL_Quit();
}

void drawScreen(Chip8 *c8, Screen *scr) {
    // draw the virtual machine screen to the SDL screen
    int *pixel;
    int pitch = 32;
    SDL_SetRenderDrawColor(scr->ren, 255, 255, 255, 255);
    SDL_RenderClear(scr->ren);
    SDL_LockTexture(scr->tex, NULL, (void **)&pixel, &pitch);
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            if (c8->screen[x][y] > 0x00) {
                pixel[x + y * 64] = 0xFFFFFFFF;
            }
            else {
                pixel[x + y * 64] = 0x00000000;
            }
        }
    }
    SDL_UnlockTexture(scr->tex);
    SDL_SetRenderTarget(scr->ren, NULL);
    SDL_Rect texture_rect;
    texture_rect.x = 0;
    texture_rect.y = 0;
    texture_rect.w = (WIDTH * 10);
    texture_rect.h = (HEIGHT * 10);
    SDL_RenderCopy(scr->ren, scr->tex, NULL, &texture_rect);
    SDL_RenderPresent(scr->ren);
}

void writeStateBuffer(Chip8 c8) {
    // TODO
}

void updateTimers(Chip8 *c8) {
    int currentTime = SDL_GetTicks();
    // rounding 16.666 to 17

    if ((currentTime - prevTime) > 17) {

        //printf("\nTIMER UPDATE");
        //printf("\ndt: %02x", c8->dt);
        //printf("\nst: %02x", c8->st);

        if (c8->dt > 0) {
            c8->dt--;
        }

        if (c8->st > 0) {
            c8->st--;
        }
        prevTime = currentTime;
    }
}

void updateInput(Chip8 *c8, SDL_Event e) {
    // check for keys newly pressed down
    if (e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
            case SDLK_1:
                c8->keypad[0x01] = 0x01;
                break;
            case SDLK_2:
                c8->keypad[0x02] = 0x01;
                break;
            case SDLK_3:
                c8->keypad[0x03] = 0x01;
                break;
            case SDLK_4:
                c8->keypad[0x0c] = 0x01;
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
                c8->keypad[0x0d] = 0x01;
                break;
            case SDLK_a:                  					
                c8->keypad[0x07] = 0x01;
                break;
            case SDLK_s:
                c8->keypad[0x08] = 0x01;
                break;
            case SDLK_d:
                c8->keypad[0x09] = 0x01;				
                break;
            case SDLK_f:
                c8->keypad[0x0e] = 0x01;
                break;
            case SDLK_z:
                c8->keypad[0x0a] = 0x01;
                break;
            case SDLK_x:
                c8->keypad[0x00] = 0x01;
                break;
            case SDLK_c:
                c8->keypad[0x0b] = 0x01;
                break;
            case SDLK_v:
                c8->keypad[0x0f] = 0x01;
                break;
        }
    }

    // check for keys newly released
    else if (e.type == SDL_KEYUP) {
        switch(e.key.keysym.sym) {
            case SDLK_1:
                c8->keypad[0x01] = 0x00;
                break;
            case SDLK_2:
                c8->keypad[0x02] = 0x00;
                break;
            case SDLK_3:
                c8->keypad[0x03] = 0x00;
                break;
            case SDLK_4:
                c8->keypad[0x0c] = 0x00;
                break;
            case SDLK_q:
                c8->keypad[0x04] = 0x00;
                break;
            case SDLK_w:
                c8->keypad[0x05] = 0x00;
                break;
            case SDLK_e:
                c8->keypad[0x06] = 0x00;
                break;
            case SDLK_r:
                c8->keypad[0x0d] = 0x00;
                break;
            case SDLK_a:                  					
                c8->keypad[0x07] = 0x00;
                break;
            case SDLK_s:
                c8->keypad[0x08] = 0x00;
                break;
            case SDLK_d:
                c8->keypad[0x09] = 0x00;				
                break;
            case SDLK_f:
                c8->keypad[0x0e] = 0x00;
                break;
            case SDLK_z:
                c8->keypad[0x0a] = 0x00;
                break;
            case SDLK_x:
                c8->keypad[0x00] = 0x00;
                break;
            case SDLK_c:
                c8->keypad[0x0b] = 0x00;
                break;
            case SDLK_v:
                c8->keypad[0x0f] = 0x00;
                break;
        }
    }
}