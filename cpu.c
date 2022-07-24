#include "cpu.h"

#include <stdio.h>
#include "chip8.h"

void cycle(Chip8 *c8) {
    //fetch opcode
    //decode opcode
    //execute opcode
    //(make sure to increase the pc by 2)
    //
    //update timers
    //if the drawflag is set, draw the screen
    //
    //store key press state
    
    //here, the value mem[pc] is shifted 8 zeroes to the left
    //then, a bitwise OR merges it with the data point next to it
    unsigned short opcode = c8->mem[c8->pc] << 8 | c8->mem[c8->pc+1];

    //now that we've obtained the opcode, we'll use a bitwise expression
    //to isolate the first bit. We'll use a switch statement off this
    //to determine which opcode to run
    
    //declaring this variable here since it cannot be properly
    //used within the switch statement
    unsigned char reg;
    switch(opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00EE) {
                //0x00EE
                //return from a subroutine
                c8->sp--;
                c8->pc = c8->stack[c8->sp] + 2;
            }
            else if (opcode == 0x00E0) {
                //*TODO*clear screen
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
            c8->pc = (opcode & 0x0FFF);	
            break;

        case 0x2000:
            //0x2NNN
            //execute subroutine at NNN
            c8->stack[c8->sp] = c8->pc;
            c8->sp++;
            c8->pc = (opcode & 0x0FFF);
            break;

        case 0x3000:
            //0x3XNN
            //skip the following instruction if VX == NN
            reg = (opcode & 0x0F00) >> 8;
            if (c8->v[reg] == (opcode & 0x00FF)) {
                c8->pc += 4; //values matched, next instruction is skipped
            }
            else {
                c8->pc += 2; //values didn't match, pc is incremented normally
            }
            break;

        case 0x4000:
            //0x4XNN
            //skip the following instruction if VX != NN
            reg = (opcode & 0x0F00) >> 8;
            if (c8->v[reg] != (opcode & 0x00FF)) {
                c8->pc += 4;
            }
            else {
                c8->pc += 2;
            }
            break;

        case 0x5000:
            //5XY0
            //skip the following instruction if VX is equal to VY
            if(c8->v[(opcode & 0x0F00) >> 8] == c8->v[(opcode & 0x00F0) >> 4]) {
                c8->pc += 4;
            }
            else {
                c8->pc += 2;
            }
            break;

        case 0x6000:
            //6XNN
            //Store the value of NN in register VX
            c8->v[((opcode & 0x0F00) >> 8)] = c8->mem[c8->pc + 1];
            c8->pc += 2;
            break;

        case 0x7000:
            //7XNN
            //Add the value of NN to register VX
            c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] +  c8->mem[c8->pc + 1]) % 256;
            c8->pc += 2;
            break;

        case 0x8000:
            //8XY0
            //store the value of VY in VX
            if ((opcode & 0x000F) == 0x0000) {
                c8->v[((opcode & 0x0F00) >> 8)] = c8->v[((opcode & 0x00F0) >> 4)];
                c8->pc += 2;
            }
            //8XY1
            //set the value of VX to VX OR VY
            else if ((opcode & 0x000F) == 0x0001) {
                c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] | c8->v[((opcode & 0x00F0) >> 4)]);
                c8->pc += 2;
            }
            //8XY2
            //set the value of VX to VX AND VY
            else if ((opcode & 0x000F) == 0x0002) {
                c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] & c8->v[((opcode & 0x00F0) >> 4)]);
                c8->pc += 2;
            }
            //8XY3
            //set the value of VX to VX XOR VY
            else if ((opcode & 0x000F) == 0x0003) {
                c8->v[((opcode & 0x0F00) >> 8)] = (c8->v[((opcode & 0x0F00) >> 8)] ^ c8->v[((opcode & 0x00F0) >> 4)]);
                c8->pc += 2;
            }
            //8XY4
            //Add the value of VY to VX
            //If a carry occurs, set VF to 01
            //If a carry does not occur, set VF to 00
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
            //Subtract the value of VY from VX
            //Set VF to 00 if a borrow occurs
            //Set VF to 01 if a borrow does not occur
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
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;

        default:
            printf("broken opcode!");
    }
    printf("\nopcode tested: %04x", opcode);
    statusDump();
    printf("\n");
}