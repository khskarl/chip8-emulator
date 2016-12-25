/**
* Master reference: http://mattmik.com/files/chip8/mastering/chip8.html
*/

#include "chip8.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>  // rand()

/* Memory map
	0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	0x200-0xFFF - Program ROM and work RAM
*/

uint8_t fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/* Opcode and Program counter */
uint16_t opcode = 0;
uint16_t pc	= 0x200;

/* Memory */
uint8_t memory[4096];
uint8_t  V[16]; /* CPU registers */
uint16_t I = 0; /* Index register */

/* Timers */
uint8_t delayTimer = 0;
uint8_t soundTimer = 0;

/* Stack and stack pointer */
uint16_t stack[16];
uint16_t sp = 0;

/* Input keys & graphics buffer */
uint8_t keypad[16];
uint8_t display[64 * 32];

bool drawFlag = false;

void FetchOpcode ();
void ExecuteOpcode ();

uint8_t* Chip8::GetDisplayMemory () {
	return display;
}

uint8_t* Chip8::GetKeypadMemory () {
	return keypad;
}

bool Chip8::GetDrawFlag () {
	return drawFlag;
}

void Chip8::Initialize () {
	pc     = 0x200;
	opcode = 0;
	I      = 0;
	sp     = 0;

	for (uint8_t &m : memory) {	m = 0; }

	for (uint16_t &s : stack) {	s = 0; }

	for (uint8_t &v : V) { v = 0; }

	for (uint8_t &pixel : display) { pixel = 0;	}

	for (uint8_t &key : keypad) {	key = 0; }

	// Load fontset into memory
  for(uint8_t i = 0; i < 80; ++i)
    memory[i] = fontset[i];

	delayTimer = 0;
	soundTimer = 0;

	srand(time(NULL));
}

void Chip8::LoadGame (uint8_t *buffer, int size) {
	for (int i = 0; i < size; i++) {
		// printf("%02x%02x\n", buffer[i], buffer[i + 1]);
		memory[i + 0x200]     = buffer[i];
		memory[i + 0x200 + 1] = buffer[i + 1];
	}
}

void Chip8::Cycle () {
	drawFlag = 0;

	FetchOpcode();
	ExecuteOpcode();

	if (delayTimer > 0)
	delayTimer -= 1;

	if (soundTimer > 0) {
		if(soundTimer == 1)
		std::cout << "beep\n";

		soundTimer -= 1;
	}
}

void FetchOpcode () {
	opcode = memory[pc] << 8 | memory[pc + 1];
	pc += 2;
}

void opNull ();     /* Does... nothing */
void op0();         /* 0... instructions */
void opJump ();     /* 1NNN jump to address NNN */
void opCall ();     /* 2NNN call subroutine on address NNN */
void opSkipEqVN ();
void opSkipNeVN ();
void opSkipEqVV ();
void opMoveVN ();   /* 6XNN	Store NN in register VX */
void opAddVN ();
void op8 ();        /* 8... instructions */
void opSkipNeVV ();
void opMoveIA ();
void opJumpV ();
void opRand ();
void opDspr ();
void opE ();        /* E... instructions */
void opF ();        /* F... instructions */

void (*opTable[16]) () = {
	op0, opJump, opCall, opSkipEqVN, opSkipNeVN, opSkipEqVV, opMoveVN, opAddVN,
	op8, opSkipNeVV, opMoveIA, opJumpV, opRand, opDspr, opE, opF
};

void ExecuteOpcode () {
	uint8_t hiByte = (opcode & 0xff00) >> 8;
	uint8_t loByte =  opcode & 0x00ff;

	printf("\n%04x %02x %02x ", pc - 2, hiByte, loByte);
	opTable[hiByte >> 4]();
}

void opNull ()
{
	printf("not yet implemented");
}

/* 0... instructions */
void op0 ()
{
	switch((opcode & 0x0f00) >> 8)
	{
		case 0x0:
		switch (opcode & 0x00ff) {
			/* 00E0	Clear the screen */
			case 0x00e0:
			for (uint8_t& pixel : display)
				pixel = 0;
			break;
			/* 00EE	Return from a subroutine */
			case 0x00ee:
			pc  = stack[--sp];
			break;
		}
		break;

		/* 0NNN Execute machine language subroutine at address NNN */
		default:
			// pc = opcode & 0x0fff;
			opNull();
		break;
	}
}

/* 1NNN */
void opJump ()
{
	pc = opcode & 0xfff;
}

/* 2NNN */
void opCall () {
	stack[sp] = pc;
	sp += 1;
	pc = opcode & 0x0fff;
}

/* 3XNN	Skip instruction if VX == NN */
void opSkipEqVN () {
	uint8_t x = (opcode & 0xf00) >> 8;
	uint8_t n =  opcode & 0x0ff;

	if (V[x] == n)
		pc += 2;
}

/* 4XNN	Skip instruction if VX != NN */
void opSkipNeVN () {
	uint8_t x = (opcode & 0xf00) >> 8;
	uint8_t n =  opcode & 0x0ff;

	if (V[x] != n)
		pc += 2;
}

/* 5XY0	Skip instruction if VX == VY */
void opSkipEqVV () {
	uint8_t x = (opcode & 0xf00) >> 8;
	uint8_t y = (opcode & 0x0f0) >> 4;

	if (V[x] == V[y])
		pc += 2;
}

/* 6XNN	Store number NN in register VX */
void opMoveVN () {
	uint8_t x = (opcode & 0xf00) >> 8;
	uint8_t n =  opcode & 0x0ff;

	V[x] = n;
}

/* 7XNN Add the value NN to register VX */
void opAddVN () {
	uint8_t x = (opcode & 0xf00) >> 8;
	uint8_t n =  opcode & 0x0ff;

	V[x] += n;
}

/* 8XY. instructions */
void op8 () {
	uint8_t x = (opcode & 0x0f00) >> 8;
	uint8_t y = (opcode & 0x00f0) >> 4;
	switch(opcode & 0x000f)
	{
		/* 8XY0	Store the value of register VY in register VX */
		case 0x0000:
		V[x] = V[y];
		break;
		/* 8XY1	Set VX to VX OR VY */
		case 0x0001:
		V[x] |= V[y];
		break;
		/* 8XY2	Set VX to VX AND VY */
		case 0x0002:
		V[x] &= V[y];
		break;
		/* 8XY3	Set VX to VX XOR VY */
		case 0x0003:
		V[x] ^= V[y];
		break;
		/* 8XY4	Add the value of VY to VX */
		case 0x0004:
		V[0xF] = (V[x] + V[y] > 255);
		V[x] += V[y];
		break;
		/* 8XY5	Subtract the value of VY from VX */
		case 0x0005:
		V[0xF] = (V[x] > V[y]);
		V[x]  -=  V[y];
		break;
		/* 8XY6	Store the value of VY shifted right one bit in VX */
		case 0x0006:
		V[0xf] = V[y] & 1;
		V[x]   = V[y] >> 1;
		break;
		/* 8XY7	Set VX to the value of VY minus VX */
		case 0x0007:
		V[0xf] = (V[y] > V[x]);
		V[x]   =  V[y] - V[x];
		break;
		/* 8XYE	Store the value of VY shifted left one bit in VX */
		case 0x000E:
		V[0xf] = V[y] & 0x80;
		V[x]   = V[y] << 1;
		break;
	}
}
/* ------------------ */

/* 9XY0	Skip next instruction if VX != VY */
void opSkipNeVV () {
	uint8_t x = (opcode & 0xf00) >> 8;
	uint8_t y = (opcode & 0x0f0) >> 4;

	if (V[x] != V[y])
		pc += 2;
}

/* ANNN Store memory address NNN in register I */
void opMoveIA () {
	I = opcode & 0x0fff;
}

/* BNNN Jump to address NNN + V0 */
void opJumpV () {
	pc = (opcode & 0x0fff) + V[0];
}

/* CXNN Set VX to a random number with a mask of NN */
void opRand () {
	uint8_t x = (opcode & 0x0f00) >> 8;
	uint8_t n =  opcode & 0x00ff;

	srand(time(NULL));
	V[x] = n & (rand() % 256) ;
}

/* DXYN Draw a sprite at position VX, VY with N bytes of sprite data starting at
the address stored in I. */
void opDspr () {
	uint8_t x    = V[(opcode & 0x0f00) >> 8];
	uint8_t y    = V[(opcode & 0x00f0) >> 4];
	uint8_t numRows = opcode & 0x000f;

	/* Set VF to 1 if any '1' pixels are changed to '1', and 0 otherwise */
	V[0xf] = 0;
	for (uint8_t i = 0; i < numRows; i++) {
		uint8_t pixel = memory[I + i];

		for (uint8_t j = 0; j < 8; j++) {
			uint8_t bit = (pixel & (0x80 >> j));
			if (bit != 0) {
				if (display[x + j + (y + i) * 64] == 1)
					V[0xf] = 1;

				display[x + j + (y + i) * 64] ^= 1;
			}
		}
	}
	drawFlag = true;
}

/* EX.. instructions */
void opE () {
	uint8_t x = (opcode & 0x0f00) >> 8;
	switch(opcode & 0x00ff)
	{
		/* EX9E Skips the next instruction if the key in VX is pressed  */
		case 0x009e:
		if(keypad[V[x]] != 0)
			pc += 2;
		break;
		/* EXA1 Skips the next instruction if the key in VX is not pressed  */
		case 0x00a1:
		if(keypad[V[x]] == 0)
			pc += 2;
		break;
	}
}

/* FX.. instructions */
void opF () {
	uint8_t x = (opcode & 0x0f00) >> 8;
	switch(opcode & 0x00ff)
	{
		/* FX07	Store the current value of the delay timer in register VX */
		case 0x0007:
		V[x] = delayTimer;
		break;
		/* FX0A	Wait for a keypress and store the result in register VX */
		case 0x000a:
		opNull();
		break;
		/* FX15	Set the delay timer to the value of register VX */
		case 0x0015:
		delayTimer = V[x];
		break;
		/* FX18	Set the sound timer to the value of register VX */
		case 0x0018:
		soundTimer = V[x];
		break;
		/* FX1E	Add the value stored in register VX to register I */
		case 0x001e:
		V[x] += I;
		break;
		/* FX29	Set I to the memadd of the sprite corresponding to the hex digit in VX */
		case 0x0029:
		I = V[x];
		break;
		/* FX33	*/
		case 0x0033:
		memory[I]     =  V[x] / 100;
		memory[I + 1] = (V[x] /  10) % 10;
		memory[I + 2] = (V[x] % 100) % 10;
		break;
		/* FX55	Store the values of registers V0 to VX inclusive in memory starting
		at address I.
		I is set to I + X + 1 after operation */
		case 0x0055:
		for (uint8_t i = 0; i <= x; i++) {
			memory[I + i] = V[i];
		}
		I += x + 1;
		break;
		/* FX65	Fill V0 to VX with the values in memory starting at address I.
		I is set to I + X + 1 after operation */
		case 0x0065:
		for (uint8_t i = 0; i <= x; i++) {
			V[i] = memory[I + i];
		}
		I += x + 1;
		break;
	}
}
