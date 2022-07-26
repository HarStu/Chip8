#include "cpu.h"

#include <stdio.h>
#include "chip8.h"

// shorthand for specific nibble(s) within an opcode
#define X ((opcode & 0x0F00) >> 8)  //0x0X00
#define Y ((opcode & 0x00F0) >> 4)  //0x00Y0
#define N ((opcode & 0x000F))       //0x000N
#define NN ((opcode & 0x00FF))      //0x00NN
#define NNN ((opcode & 0x0FFF))     //0x0NNN

void cycle(Chip8 *c8, FILE *out) {
    // fetch opcode
    // decode opcode
    // execute opcode
    // increment pc by 2 (16 bits)

    // all chip8 opcodes are 16-bit (2-byte) values, which we can represent as an unsigned short
    // here, the value mem[pc] is shifted 8 zeroes to the left, becoming the first 8 bits
    // then, a bitwise OR merges it with the data point next to it, which becomes the next 8 bits
    unsigned short opcode = c8->mem[c8->pc] << 8 | c8->mem[c8->pc+1];

    // declaring this variable here since it cannot be properly
    // used within the switch statement
    unsigned char reg;

    // switch statement based off the first nibble of the opcode
    // the exact opcode executed is determined by other nibbles, 
    // which we've already defined as preprocessor directives
    switch(opcode & 0xF000) {
        case 0x0000: //old
            if (opcode == 0x00EE) {
                //0x00EE
                //return from a subroutine
                c8->sp--;
                c8->pc = c8->stack[c8->sp] + 2;
            }
            else if (opcode == 0x00E0) {
                // 0x00E0
                // clear screen
                for (int x = 0; x < 64; x++) {
                    for (int y = 0; y < 32; y++) {
                        c8->screen[x][y] = 0;
                    }
                }
            }
            else {
                //*TODO*execute machine languge instruction at 0x0NNN
                //we're probably not going to implement this
                //so just forget about it
                c8->pc += 2;
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

        case 0x3000: //old
            //0x3XNN
            //skip the following instruction if vX == NN
            reg = (opcode & 0x0F00) >> 8;
            if (c8->v[reg] == (opcode & 0x00FF)) {
                c8->pc += 4; //values matched, next instruction is skipped
            }
            else {
                c8->pc += 2; //values didn't match, pc is incremented normally
            }
            break;

        case 0x4000: //old
            //0x4XNN
            //skip the following instruction if vX != NN
            reg = (opcode & 0x0F00) >> 8;
            if (c8->v[reg] != (opcode & 0x00FF)) {
                c8->pc += 4;
            }
            else {
                c8->pc += 2;
            }
            break;

        case 0x5000: //old
            //5XY0
            //skip the following instruction if vX is equal to vY
            if(c8->v[(opcode & 0x0F00) >> 8] == c8->v[(opcode & 0x00F0) >> 4]) {
                c8->pc += 4;
            }
            else {
                c8->pc += 2;
            }
            break;

        case 0x6000:
            //6XNN
            //Store the value of NN in register vX
            c8->v[X] = NN;
            c8->pc += 2;
            break;

        case 0x7000:
            //7XNN
            //Add the value of NN to register vX
            c8->v[X] = c8->v[X] + NN;
            c8->pc += 2;
            break;

        case 0x8000: //old
            //8XY0
            //store the value of vY in vX
            if ((opcode & 0x000F) == 0x0000) {
                c8->v[((opcode & 0x0F00) >> 8)] = c8->v[((opcode & 0x00F0) >> 4)];
                c8->pc += 2;
            }
            //8XY1
            //set the value of vX to vX OR vY
            else if ((opcode & 0x000F) == 0x0001) {
                c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] | c8->v[((opcode & 0x00F0) >> 4)]);
                c8->pc += 2;
            }
            //8XY2
            //set the value of vX to vX AND vY
            else if ((opcode & 0x000F) == 0x0002) {
                c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] & c8->v[((opcode & 0x00F0) >> 4)]);
                c8->pc += 2;
            }
            //8XY3
            //set the value of vX to vX XOR vY
            else if ((opcode & 0x000F) == 0x0003) {
                c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] ^ c8->v[((opcode & 0x00F0) >> 4)]);
                c8->pc += 2;
            }
            //8XY4
            //Add the value of vY to vX
            //If a carry occurs, set vF to 01
            //If a carry does not occur, set vF to 00
            else if ((opcode & 0x000F) == 0x0004) {
                if (c8->v[((opcode & 0x0F00) >> 8)] + c8->v[((opcode & 0x00F0) >> 4)] > 255) {
                    //carry occurs
                    c8->v[0xF] = 0x01;
                }
                else {
                    //no carry occurs
                    c8->v[0xF] = 0x00;
                }
                c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] +  c8->mem[c8->pc + 1]) % 256;
                c8->pc += 2;
            }
            //8XY5
            //Subtract the value of vY from vX
            //Set vF to 00 if a borrow occurs
            //Set vF to 01 if a borrow does not occur
            //TODO - Somewhat unclear on the concept of a borrow in Binary math.
            //Some revision may be needed
            else if ((opcode & 0x000F) == 0x0005) {
                if (c8->v[((opcode & 0x00F0) >> 4)] > c8->v[((opcode & 0x0F00) >> 8)]) {
                    //borrow occurs
                    c8->v[0xF] = 0x00;
                }
                else {
                    c8->v[0xF] = 0x01;
                }
                c8->v[((opcode & 0x0F00) >> 8)] = c8->v[((opcode & 0x0F00) >> 8)] - c8->v[((opcode & 0x00F0) >> 4)];
                c8->pc += 2;

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
        case 0xD000:
            // DXYN
            // draw a spite N pixels tall (always 8 bit/1 byte wide),
            // from the memory location held in the index register I
            // at the X coordinate v[X] and the Y coordinate v[Y]
            
            //get starting position for sprites
            sprX = c8->v[X] % 64;
            sprY = c8->v[Y] % 32;

            // set the carry flag to 0
            // if drawing the sprite turns any pixels 'off' again, this will be set back to 1
            c8->v[0xF] = 0;

            // iterate over N rows
            for (int h = 0; h < N; h++) {
                // iterate 8 times, for each bit in the char being read
                for (int b = 0; b < 8; b++) {
                    sprChar = c8->mem[c8->I];
                }
            }
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