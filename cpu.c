#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chip8.h"
#include "interface.h"

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

    // print out opcode for debug purposes
    printf("opcode: %04x\n", opcode);

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
            //Set vX to vX minux vY
            //Set vF to 01 if vX > vY (no borrow occurs)
            //Set vF to 00 otherwise
            else if (N == 0x0005) {
                if (c8->v[X] > c8->v[Y]) {
                    c8->v[0xF] = 0x01;
                }
                else {
                    c8->v[0xF] = 0x00;
                }
                c8->v[X] = c8->v[X] - c8->v[Y];
            }
            // 8XY6
            // Set vF to the least significant bit of vY prior to the shift
            // Store the value of vY shifted one bit right in vX
            else if (N == 0x0006) {
                c8->v[0xF] = (c8->v[Y] & 0x1); // store least significant bit in vF
                c8->v[X] = (c8->v[Y] >> 1);
            }
            // 8XY7
            // Set vX to vY minus vX
            // set vF to 01 if vY > vX (no borrow occurs)
            // Set vF to 00 otherwise
            else if (N == 0x0007) {
                if (c8->v[Y] > c8->v[X]) {
                    c8->v[0xF] = 0x01;
                }
                else {
                    c8->v[0xF] = 0x00;
                }
                c8->v[X] = c8->v[Y] - c8->v[X];
            }
            // 8XYE
            // Set vF to the most significant bit of vY prior to the shift
            // Store the value of vY shifted one bit left in vX
            else if (N == 0x000E) {
                c8->v[0xF] = ((c8->v[Y] & 0x8) >> 3); // store most significant bit of vY in vF
                c8->v[X] = (c8->v[Y] << 1);
            }

            break;

        case 0x9000:
            // 9XY0
            // skip following instruction if vX != vY
            if (c8->v[X] != c8->v[Y]) {
                c8->pc += 2;
            }
            break;

        case 0xA000:
            // 0xANNN
            // Store the value of NNN in the index register
            c8->I = NNN;
            break;

        case 0xB000:
            // BNNN
            // jump to address NNN + v0
            c8->pc = (NNN + c8->v[0x0]);
            break;

        case 0xC000:
            // CXNN
            // Generates a random number, binary ANDs that number against NN, store result in VX
            c8->v[X] = ((rand() % 0x100) & NN); // resulting random number is between 0x0 and 0xFF. Ranges won't be perfect but I accept that.
            break;

        case 0xD000:
            // DXYN
            // draw a spite N pixels tall (always 8 bit/1 byte wide),
            // from the memory location held in the index register I
            // at the X coordinate v[X] and the Y coordinate v[Y]
            // get starting position for sprites
            int sprX = c8->v[X] % 64;
            int sprY = c8->v[Y] % 32;
            //unsigned int rows = N;

            // set the vf to 0
            // if drawing the sprite turns any pixels 'off' again, this will be set back to 1
            c8->v[0xF] = 0;

            // iterate over N rows
            for (int h = 0; h < N; h++) {
                // pixelsChar; the row of 8 pixels being read from memory which compromise this row
                unsigned char pixelsChar = c8->mem[((c8->I)+h)];

                // iterate 8 times, for each bit in pixelsChar
                // each bit is one pixel
                for (int b = 0; b < 8; b++) {
                    // 0x80 = 0b10000000
                    // as we iterate through b, we use the bitwise AND to isolate and check each bit/pixel
                    // if the pixel exists, then we potentially need to update the screen
                    if ((pixelsChar & (0x80 >> b)) != 0) { 
                        // check the pixel we're drawing to; if it's already set, we have a collision, and need to set vF
                        if (c8->screen[sprX + b][sprY + h] == 1) {
                            c8->v[0xF] = 0x01;
                        }
                        // XOR to update the screen
                        c8->screen[sprX + b][sprY + h] ^= 1;
                        }
                    }
                }
            break;

        case 0xE000:
            // EX9E 
            // skip one instruction if the key corresponding to the value in vX is pressed
            if (NN == 0x009E) {
                if (c8->keypad[c8->v[X]] != 0x00) {
                     c8->pc += 2;
                }
            }
            // EXA1
            // skip one instruction if the key corresponding to the value in vX is NOT pressed
            else if (NN == 0x00A1) {
                if (c8->keypad[c8->v[X]] == 0x00) {
                     c8->pc += 2;
                 }
            }
            break;

        case 0xF000:
            // FX07
            // set vX to the current value of the delay timer
            if (NN == 0x0007) {
                c8->v[X] = c8->dt;
            }
            // FX0A
            // block further execution until a key is pressed
            // when a key is pressed, store its value in vX
            else if (NN == 0x000A) {
                bool block_flag = true;
                for (int i = 0x00; i < 0x10; i++) {
                    if (c8->keypad[i] != 0x00) {
                        c8->v[X] = i;
                        block_flag = false;
                    }
                }
                if (block_flag == true) {
                    c8->pc -= 2;
                }
            }
            // FX15
            // set the delay time to the value in vX
            else if (NN == 0x0015) {
                c8->dt = c8->v[X];
            }
            // FX18 
            // set the sound timer to the value in vX
            else if (NN == 0x0018) {
                c8->st = c8->v[X];
            }
            // FX1E
            // Add the value of vX to I
            // if vX I overflows, set vF to 1
            else if (NN == 0x001E) {
                if (c8->I + c8->v[X] > 0x0FFF) {
                    c8->v[0xF] = 1;
                }
                c8->I = c8->I + c8->v[X];
            }
            // FX29
            // set I to the address of the hex character stored in vX
            else if (NN == 0x0029) {
                c8->I = (c8->v[X] * 0x05) + 0x50;
            }
            // FX33
            // convert the 3 decimal digits of vX to 3 seperate values
            // store the hundreds-place value at I, tens-place at I+1, and ones-place at I+2
            else if (NN == 0x0033) {
                c8->mem[c8->I] = (c8->v[X] / 100);
                c8->mem[c8->I+0x01] = ((c8->v[X] % 100) / 10);
                c8->mem[c8->I+0x02] = ((c8->v[X] % 100) % 10);
            }
            // FX55
            // store the values of registers v0 through vX inclusive in memory starting at address I
            // I is set to I + X + 1 after operation in earlier implementations (not here)
            // fprintf(out, "\n******storage opcode********");
            else if (NN == 0x0055) {
                for (int reg = 0; reg < X+1; reg++) {
                    // fprintf(out, "\n\nstoring value %02x from v[%02x] in mem[%02x]", c8->v[reg], reg, c8->mem[(c8->I) + reg]);
                    c8->mem[(c8->I) + reg] = c8->v[reg];
                }
            }
            // FX65
            // store the values at memory addresses I through I + X in registers v0 through vX
            // I is set to I + X + 1 after operation in earlier implementations (not here)
            else if (NN == 0x0065) {
                for (int reg = 0; reg < X+1; reg++) {
                    c8->v[reg] = c8->mem[(c8->I) + reg];
                }
            }
            break;

        default:
            printf("broken opcode!");
    }
    fprintf(out, "\nopcode tested: %04x\n", opcode);
    statusDump(c8, out);
    fprintf(out, "\n");
}