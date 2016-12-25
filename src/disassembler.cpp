#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*-------------------*/
/* 8XY_ instructions */
/*-------------------*/
void OpMovVV(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8,
					y = (opcode & 0x00f0) >> 4;

	printf("%-10s V%01x,V%01x", "MOV", x, y);
}

void OpOr(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8,
					y = (opcode & 0x00f0) >> 4;

	printf("%-10s V%01x,V%01x", "OR", x, y);
}

void OpAnd(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8,
					y = (opcode & 0x00f0) >> 4;

	printf("%-10s V%01x,V%01x", "AND", x, y);
}

void OpXor(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8,
					y = (opcode & 0x00f0) >> 4;

	printf("%-10s V%01x,V%01x", "XOR", x, y);
}

void OpAddV(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8,
					y = (opcode & 0x00f0) >> 4;

	printf("%-10s V%01x,V%01x", "ADD", x, y);
}

void OpSub(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8,
					y = (opcode & 0x00f0) >> 4;

	printf("%-10s V%01x,V%01x", "SUB", x, y);
}

void OpShr(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8;

	printf("%-10s V%01x", "SHR", x);
}

void OpSubb(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8,
					y = (opcode & 0x00f0) >> 4;

	printf("%-10s V%01x,V%01x", "SUBB", x, y);
}

void OpShl(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8;

	printf("%-10s V%01x", "SHL", x);
}


void (*Op8Table[9])(uint16_t opcode) =
{//	0		1		2		3		4		5		6		7		8
	OpMovVV,	OpOr, 	OpAnd, 	OpXor, 	OpAddV,  OpSub,	OpShr,  OpSubb, OpShl
};

void Op8(uint16_t opcode)
{
	Op8Table[opcode & 0x000f](opcode);
}
/*-------------------*/


void OpNull(uint16_t opcode)
{
	printf("not yet implemented");
};

void Op0(uint16_t opcode) {
	switch (opcode & 0x00ff)
	{
		case 0xE0:
		{
			printf("%-10s", "CLS");
		}
		break;
		case 0xEE:
		{
			printf("%-10s", "RTS");
		}
		break;
		default:
		{
			printf("%-10s $%01x%02x", "CALL RCA", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
		}
		break;
	}
}



/*--------------------*/
/* Other instructions */
/*--------------------*/
void OpJump(uint16_t opcode) {
	printf("%-10s $%01x%02x", "JUMP", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpCall(uint16_t opcode) {
	printf("%-10s $%01x%02x", "CALL", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpSkipEqVN(uint16_t opcode) {
	printf("%-10s V%01x,#$%02x", "SKIP.EQ", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpSkipNeVN(uint16_t opcode) {
	printf("%-10s V%01x,#$%02x", "SKIP.NE", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpSkipEqVV(uint16_t opcode) {
	printf("%-10s V%01x,V%02x", "SKIP.EQ", (opcode & 0x0f00) >> 8, (opcode & 0x00f0) >> 4);
}

void OpMoveVN(uint16_t opcode) {
	printf("%-10s V%01x,#$%02x", "MVI", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpAdd(uint16_t opcode) {
	printf("%-10s V%01x,#$%02x", "ADD", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpSkipNeVV(uint16_t opcode) {
	printf("%-10s V%01x,V%02x", "SKIP.NE", (opcode & 0x0f00) >> 8, (opcode & 0x00f0) >> 4);
}

void OpMoveIA(uint16_t opcode) {
	uint8_t addresshi = (opcode & 0x0f00) >> 8;
	printf("%-10s I,#$%01x%02x", "MVI", addresshi, opcode & 0x00ff);
}

void OpJumpV(uint16_t opcode) {
	printf("%-10s #$%01x%02x, V0", "JUMP", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpRand(uint16_t opcode) {
	printf("%-10s V%01x,#$%02x", "RAND", (opcode & 0x0f00) >> 8, opcode & 0x00ff);
}

void OpDspr(uint16_t opcode) {
	printf("%-10s V%01x,V%01x,#$%02x", "DSPR", (opcode & 0x0f00) >> 8, (opcode & 0x00f0) >> 4, opcode & 0xf);
}
/*------------------*/



/*-------------------*/
/* E... instructions */
/*-------------------*/
void OpE(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8;

	switch (opcode & 0x00ff)
	{
		case 0x9e: printf("%-10s V%01x", "SKIP.KEY", x); break;
		case 0xA1: printf("%-10s V%01x", "SKIP.NOKEY", x); break;
		default: printf("UNKNOWN E"); break;
	}
}
/*-------------------*/



/*-------------------*/
/* F... instructions */
/*-------------------*/
void OpF(uint16_t opcode) {
	uint8_t x = (opcode & 0x0f00) >> 8;
	switch(opcode & 0x00ff)
	{
		case 0x07: printf("%-10s V%01x,DELAY", "MOV", x); break;
		case 0x0a: printf("%-10s V%01x", "KEY", x); break;
		case 0x15: printf("%-10s DELAY,V%01x", "MOV", x); break;
		case 0x18: printf("%-10s SOUND,V%01x", "MOV", x); break;
		case 0x1e: printf("%-10s I,V%01x", "ADI", x); break;
		case 0x29: printf("%-10s I,V%01x", "SPRITECHAR", x); break;
		case 0x33: printf("%-10s (I),V%01x", "MOVBCD", x); break;
		case 0x55: printf("%-10s (I),V0-V%01x", "MOVM", x); break;
		case 0x65: printf("%-10s V0-V%01x,(I)", "MOVM", x); break;
		default: printf("UNKNOWN F"); break;
	}
}
/*-------------------*/

void (*ChipOpTable[17])(uint16_t opcode) = {
/*  0           1         2         3         4          5         6      7 */
	Op0,     OpJump,   OpCall, OpSkipEqVN, OpSkipNeVN, OpSkipEqVV, OpMoveVN, OpAdd,
	Op8, OpSkipNeVV, OpMoveIA,  OpJumpV,   OpRand,    OpDspr,      OpE,   OpF
};

void DisassembleInstruction(uint16_t opcode) {
	uint8_t firstNib = (opcode & 0xf000) >> 12;

	printf("%02x %02x ", opcode >> 8, opcode & 0x00ff);
	ChipOpTable[firstNib](opcode);
}
