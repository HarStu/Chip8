#include <stdio.h>

//declaring the registers
//16 registers, from v0 to vF are avalible
unsigned char v[16];

//Program counter
//Index register
unsigned short pc;
unsigned short I;

//stack
//stack pointer
unsigned short stack[16];
unsigned short sp;

//memory of the chip8, where all relevant data is loaded
//4 kilobytes in total
unsigned char mem[4096];

//opcode currently in use
//fetched using bitwise OR
unsigned short opcode;

//loads data from file into mem
void loadData();

//outputs a dump of the data loaded into [mem]
void outputMemDump();

//loads mem full of empty data
void loadEmptyMem();

//prints the value of each register, sp, and pc
void statusDump();

int main() {
	loadData();

	pc = 0;
	int emergencyBreakCounter = 0;
	while (pc < 32) {
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
		opcode = mem[pc] << 8 | mem[pc+1];

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
					sp--;
					pc = stack[sp] + 2;
				}
				else if (opcode == 0x00E0) {
					//*TODO*clear screen
				}
				else {
					//*TODO*execute machine languge instruction at 0x0NNN
					//we're probably not going to implement this
					//so just forget about it
					pc += 2;
				}
				break;

			case 0x1000:
				//0x1NNN
				//jump to address NNN in 0x1NNN
				pc = (opcode & 0x0FFF);	
				break;

			case 0x2000:
				//0x2NNN
				//execute subroutine at NNN
				stack[sp] = pc;
				sp++;
				pc = (opcode & 0x0FFF);
				break;

			case 0x3000:
				//0x3XNN
				//skip the following instruction if VX == NN
				reg = (opcode & 0x0F00) >> 8;
				if (v[reg] == (opcode & 0x00FF)) {
					pc += 4; //values matched, next instruction is skipped
				}
				else {
					pc += 2; //values didn't match, pc is incremented normally
				}
				break;

			case 0x4000:
				//0x4XNN
				//skip the following instruction if VX != NN
				reg = (opcode & 0x0F00) >> 8;
				if (v[reg] != (opcode & 0x00FF)) {
					pc += 4;
				}
				else {
					pc += 2;
				}
				break;

			case 0x5000:
				//5XY0
				//skip the following instruction if VX is equal to VY
				if(v[(opcode & 0x0F00) >> 8] == v[(opcode & 0x00F0) >> 4]) {
					pc += 4;
				}
				else {
					pc += 2;
				}
				break;

			case 0x6000:
				//6XNN
				//Store the value of NN in register VX
				v[((opcode & 0x0F00) >> 8)] = mem[pc + 1];
				pc += 2;
				break;

			case 0x7000:
				//7XNN
				//Add the value of NN to register VX
				v[((opcode & 0x0F00) >> 8)] = (v[((opcode & 0x0F00) >> 8)] +  mem[pc + 1]) % 256;
				pc += 2;
				break;

			case 0x8000:
				//8XY0
				//store the value of VY in VX
				if ((opcode & 0x000F) == 0x0000) {
					v[((opcode & 0x0F00) >> 8)] = v[((opcode & 0x00F0) >> 4)];
					pc += 2;
				}
				//8XY1
				//set the value of VX to VX OR VY
				else if ((opcode & 0x000F) == 0x0001) {
					v[((opcode & 0x0F00) >> 8)] = (v[((opcode & 0x0F00) >> 8)] | v[((opcode & 0x00F0) >> 4)]);
					pc += 2;
				}
				//8XY2
				//set the value of VX to VX AND VY
				else if ((opcode & 0x000F) == 0x0002) {
					v[((opcode & 0x0F00) >> 8)] = (v[((opcode & 0x0F00) >> 8)] & v[((opcode & 0x00F0) >> 4)]);
					pc += 2;
				}
				//8XY3
				//set the value of VX to VX XOR VY
				else if ((opcode & 0x000F) == 0x0003) {
					v[((opcode & 0x0F00) >> 8)] = (v[((opcode & 0x0F00) >> 8)] ^ v[((opcode & 0x00F0) >> 4)]);
					pc += 2;
				}
				//8XY4
				//Add the value of VY to VX
				//If a carry occurs, set VF to 01
				//If a carry does not occur, set VF to 00
				else if ((opcode & 0x000F) == 0x0004) {
					if (v[((opcode & 0x0F00) >> 8)] + v[((opcode & 0x00F0) >> 4)] > 255) {
						//carry occurs
						v[0xF] = 0x01;
					}
					else {
						//no carry occurs
						v[0xF] = 0x00;
					}
					v[((opcode & 0x0F00) >> 8)] = (v[((opcode & 0x0F00) >> 8)] +  mem[pc + 1]) % 256;
					pc += 2;
				}
				//8XY5
				//Subtract the value of VY from VX
				//Set VF to 00 if a borrow occurs
				//Set VF to 01 if a borrow does not occur
				//TODO - Somewhat unclear on the concept of a borrow in Binary math.
				//Some revision may be needed
				else if ((opcode & 0x000F) == 0x0005) {
					if (v[((opcode & 0x00F0) >> 4)] > v[((opcode & 0x0F00) >> 8)]) {
						//borrow occurs
						v[0xF] = 0x00;
					}
					else {
						v[0xF] = 0x01;
					}
					v[((opcode & 0x0F00) >> 8)] = v[((opcode & 0x0F00) >> 8)] - v[((opcode & 0x00F0) >> 4)];
					pc += 2;

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
		emergencyBreakCounter++;
		if (emergencyBreakCounter > 6) {
			break;
		}
	}
	return 0;
}

void loadData() {
	FILE *fp;
	fp = fopen("test.ch8","rb");
	while (fread(mem,1,4096,fp));
	fclose(fp);
}

void outputMemDump() {
	printf("\n");
	for (int i = 0; i < 64; i++) {
		if ((i % 8 == 0) && (i != 0)) {
			printf("\n");
		}
		printf(" %04x", mem[i]);
	}
	printf("\n");
}

void statusDump() {
	printf("\n");
	for (int i = 0x0; i < 0x10; i++) {
		printf("v%x: %02x\n", i, v[i]);
	}
	printf("program counter (after running): %04x\nstack pointer: %d", pc, sp);
}

void loadEmptyMem() {
	for (int i = 0; i < 4097; i++) {
		mem[i] = 0x00;
	}
	for (int i = 0; i < 17; i++) {
		v[i] = 0x0000;
	}
}
