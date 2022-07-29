#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

// shorthand for specific nibble(s) within an opcode
#define X ((opcode & 0x0F00) >> 8)  //0x0X00
#define Y ((opcode & 0x00F0) >> 4)  //0x00Y0
#define N ((opcode & 0x000F))       //0x000N
#define NN ((opcode & 0x00FF))      //0x00NN
#define NNN ((opcode & 0x0FFF))     //0x0NNN

void cycle(Chip8 *c8, FILE *out) {
    // fetch opcode
    // increment pc by 2 bytes
    // decode opcode
    // execute opcode

    // all chip8 opcodes are 16-bit (2-byte) values, which we can represent as an unsigned short
    // here, the value mem[pc] is shifted 8 zeroes to the left, becoming the first 8 bits
    // then, a bitwise OR merges it with the data point next to it, which becomes the next 8 bits
    unsigned short opcode = c8->mem[c8->pc] << 8 | c8->mem[c8->pc+1];

    // iterate pc here; next time an opcode is fetched, it'll be the one after the current opcode
    c8->pc += 2;

    // switch statement based off the first nibble of the opcode
    // the exact opcode executed is determined by other nibbles, 
    // which we've already defined as preprocessor directives
    switch(opcode & 0xF000) {
        case 0x0000: 
            if (opcode == 0x00EE) {
                //0x00EE
                //return from a subroutine
                c8->sp--;
                c8->pc = c8->stack[c8->sp];
            }
            else if (opcode == 0x00E0) {
                // 0x00E0
                // clear screen
                clearScreen(c8);
            }
            break;

        case 0x1000:
            //0x1NNN
            //jump to address NNN in 0x1NNN
            c8->pc = NNN;
            break;

        case 0x2000:
            //0x2NNN
            //execute subroutine at NNN
            c8->stack[c8->sp] = c8->pc;
            c8->sp++;
            c8->pc = NNN;
            break;

        case 0x3000:
            //0x3XNN
            //skip the following instruction if vX == NN
            if (c8->v[X] == NN) {
                c8->pc += 2; // pc is iterated by another 2 bytes, skipping the next opcode
            }
            break;

        case 0x4000:
            //0x4XNN
            //skip the following instruction if vX != NN
            if (c8->v[X] != NN) {
                c8->pc += 2; // pc is iterated by another 2 bytes, skipping the next opcode
            }
            break;

        case 0x5000:
            //5XY0
            //skip the following instruction if vX is equal to vY
            if(c8->v[X] == c8->v[Y]) {
                c8->pc += 2; // pc is iterated by another 2 bytes, skipping the next opcode
            }
            break;

        case 0x6000:
            //6XNN
            //Store the value of NN in register vX
            c8->v[X] = NN;
            break;

        case 0x7000:
            //7XNN
            //Add the value of NN to register vX
            c8->v[X] = c8->v[X] + NN;
            break;

        case 0x8000:
            //8XY0
            //store the value of vY in vX
            if (N == 0x0000) {
                c8->v[X] = c8->v[Y];
            }
            //8XY1
            //set the value of vX to vX OR vY
            else if (N == 0x0001) {
                c8->v[X] = (c8->v[X] | c8->v[Y]);
            }
            //8XY2
            //set the value of vX to vX AND vY
            else if (N == 0x0002) {
                c8->v[X] = (c8->v[X] & c8->v[Y]);
            }
            //8XY3
            //set the value of vX to vX XOR vY
            else if (N == 0x0003) {
                c8->v[X] = (c8->v[X] ^ c8->v[Y]);
            }
            //8XY4
            //Add the value of vY to vX
            //If a carry occurs, set vF to 01
            //If a carry does not occur, set vF to 00
            else if (N == 0x0004) {
                if (c8->v[X] + c8->v[Y] > 255) {
                    //carry occurs
                    c8->v[0xF] = 0x01;
                }
                else {
                    //no carry occurs
                    c8->v[0xF] = 0x00;
                }
                c8->v[X] = (c8->v[X] + c8->v[Y]) % 256;
            }
            //8XY5
            //Subtract the value of vY from vX
            //Set vF to 00 if a borrow occurs from beyond the highest bit
            //Set vF to 01 if a borrow does not occur from beyond the highest bit
            else if (N == 0x0005) {
                if (c8->v[Y] > c8->v[X]) {
                    //borrow occurs
                    c8->v[0xF] = 0x00;
                }
                else {
                    c8->v[0xF] = 0x01;
                }
                c8->v[X] = c8->v[X] - c8->v[Y];
            }
            // 8XY6
            // Store the value of vY shifted one bit right in vX
            // Set vF to the least significant bit of vY prior to the shift
            /*
            CURRENTLY NOT PASSING TEST 
            else if (N == 0x0006) {
                c8->v[0xF] = (c8->v[Y] & 0x1); // store least significant bit in vF
                c8->v[X] = (c8->v[Y] >> 1);
            }
            */
            // 8XY7
            // Set vX to vY minus vX
            // set vF to 00 if a borrow occurs from beyond the highest bit
            // Set vF to 01 if a borrow does not occur from beyond the highest bit
            else if (N == 0x0007) {
                if (c8->v[Y] > c8-> v[X]) {
                    // borrow occurs
                    c8->v[0xF] = 0x00;
                }
                else {
                    c8->v[0xF] = 0x01;
                }
                c8->v[X] = c8->v[Y] - c8->v[X];
            }

            break;

        case 0x9000:
            break;
        case 0xA000:
            // 0xANNN
            // Store the value of NNN in the index register
            c8->I = NNN;
            break;

        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000: //TODO: NEED PROTECTIONS TO PREVENT DRAWING OUT OF BOUNDS!
            // DXYN
            // draw a spite N pixels tall (always 8 bit/1 byte wide),
            // from the memory location held in the index register I
            // at the X coordinate v[X] and the Y coordinate v[Y]

            fprintf(out,"\n*******************************\n***SCREEN DRAWING OPCODE %04x***\n", opcode);
            
            //get starting position for sprites
            int sprX = c8->v[X] % 64;
            int sprY = c8->v[Y] % 32;
            fprintf(out, "Coordinate to draw at:\n\tX: %x\n\tY: %x\n", sprX, sprY);
            //unsigned int rows = N;
            fprintf(out, "Lines to draw: \n\tN: %x\n\n", N);

            // set the vf to 0
            // if drawing the sprite turns any pixels 'off' again, this will be set back to 1
            c8->v[0xf] = 0;

            // iterate over N rows
            for (int h = 0; h < N; h++) {
                // pixelsChar; the row of 8 pixels being read from memory which compromise this row
                unsigned char pixelsChar = c8->mem[((c8->I)+h)];
                fprintf(out, "row[%x]: pixelsChar: %02x from mem[%03x]\n", h, pixelsChar, ((c8->I)+h));

                // iterate 8 times, for each bit in pixelsChar
                // each bit is one pixel
                for (int b = 0; b < 8; b++) {
                    // 0x80 = 0b10000000
                    // as we iterate through b, we use the bitwise AND to isolate and check each bit/pixel in pixelsChar
                    // pixel is either 0 or 1
                    unsigned char pixel = (pixelsChar & (0x80 >> b));

                    // confirm that the drawing location we're looking at is actually on-screen
                    if (((sprX + b) < 64) && ((sprY + h) < 32)) {
                        // check if the upcoming XOR will disable a currently-on pixel, and set the drawflag accordingly
                        if ((pixel == 1) && (c8->screen[sprX + b][sprY + h] == 1)) {
                            c8->v[0xf] = 1;
                        }
                        
                        // value of the sprite pixel is XOR'd agaisnt the value of the screen in order to update the dislpay
                        c8->screen[sprX + b][sprY + h] ^= pixel;
                    }
                }
            }
            fprintf(out,"*");
            break;

        case 0xE000:
            break;
        case 0xF000:
            break;

        default:
            printf("broken opcode!");
    }
    fprintf(out, "\nopcode tested: %04x\n", opcode);
    statusDump(c8, out);
    fprintf(out, "\n");
}