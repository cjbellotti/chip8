#include <stdio.h>
#include <stdint.h>
#include "cpu.h"

#define RET machine->registers.pc = machine->stack[machine->registers.sp]; machine->registers.sp--
#define JP(addr) machine->registers.pc = addr
#define CALL(addr) machine->registers.sp++; machine->stack[machine->registers.sp] = machine->registers.pc; machine->registers.pc = addr
#define SE_Vx_byte(vx, byte) if (machine->registers.v[vx] == byte) machine->registers.pc+=2
#define SNE_Vx_byte(vx, byte) if (machine->registers.v[vx] != byte) machine->registers.pc+=2
#define SE_Vx_Vy(vx, vy) if (machine->registers.v[vx] == machine->registers.v[vy]) machine->registers.pc+=2
#define LD_Vx_byte(vx, byte)  machine->registers.v[vx] = byte
#define ADD_Vx_byte(vx, byte) machine->registers.v[vx] += byte
#define LD_Vx_Vy(vx, vy) machine->registers.v[vx] = machine->registers.v[vy]
#define OR_Vx_Vy(vx, vy) machine->registers.v[vx] |= machine->registers.v[vy]
#define AND_Vx_Vy(vx, vy) machine->registers.v[vx] &= machine->registers.v[vy]
#define XOR_Vx_Vy(vx, vy) machine->registers.v[vx] ^= machine->registers.v[vy]
#define ADD_Vx_Vy(vx, vy) machine->registers.vf = ((machine->registers.v[vx] + machine->registers.v[vy]) & 0x100) ? 1 : 0;  machine->registers.v[vx] += machine->registers.v[vy]

void exec_opcode(machine_t *machine, uint16_t opcode)
{

	uint16_t nnn = opcode & 0xfff;
	uint8_t kk = opcode & 0xff;
	uint8_t n = opcode & 0xf;
	uint8_t x = (opcode >> 8) & 0xf;
	uint8_t y = (opcode >> 4) & 0xf;
	uint8_t op = (opcode >> 12);

	switch (op)
	{
		case 0:

			if (opcode == 0x00e0){

				printf("CLS\n");	

			} else if (opcode == 0x00ee){

				printf("RET\n");
				RET;	

			} else {

				//printf("SYS %x\n", nnn);

			} 

			break;

		case 1:

			printf("JP %x\n", nnn);	
			JP(nnn);
			break;

		case 2:

			printf("CALL %x\n", nnn);
			CALL(nnn);	
			break;

		case 3:

			printf("SE V%x, %x\n", x, kk);
			SE_Vx_byte(x, kk);
			break;

		case 4:

			printf("SNE V%x, %x\n", x, kk);	
			SNE_Vx_byte(x, kk);
			break;

		case 5:
			
			if (n == 0) {
				printf("SE V%x, V%x\n", x, y);
				SE_Vx_Vy(x, y);
			}	
			break;

		case 6:

			printf("LD V%x, %x\n", x, kk);
			LD_Vx_byte(x, kk);	
			break;

		case 7:

			printf("ADD V%x, %x\n", x, kk);	
			ADD_Vx_byte(x, kk);
			break;

		case 8:

			if (n == 0) {

				printf("LD V%x, V%x\n", x, y);
				LD_Vx_Vy(x, y);

			} else if (n == 1) {

				printf("OR V%x, V%x\n", x, y);
				OR_Vx_Vy(x, y);

			} else if (n == 2) {

				printf("AND V%x, V%x\n", x, y);
				AND_Vx_Vy(x, y);

			} else if (n == 3) {

				printf("XOR V%x, V%x\n", x, y);
				XOR_Vx_Vy(x, y);

			} else if (n == 4) {

				printf("ADD V%x, V%x\n", x, y);

			} else if (n == 5) {

				printf("SUB V%x, V%x\n", x, y);

			} else if (n == 6) {

				printf("SHR V%x {, V%x}\n", x, y);

			} else if (n == 7) {

				printf("SUBN V%x, V%x\n", x, y);

			} else if (n == 0xe) {

				printf("SHL V%x, {, V%x}\n", x, y);
			}
			break;

		case 9:
			
			if (n == 0) {

				printf("SNE V%x, V%x\n", x, y);

			}
			break;

		case 0xa:

			printf("LD I, %x\n", nnn);
			break;

		case 0xb:

			printf("JP V0, %x\n", nnn);
			break;

		case 0xc:

			printf("RND V%x, %x\n", x, kk);
			break;

		case 0xd:

			printf("DRW V%x, V%x, %x\n", x, y, n);
			break;

		case 0xe:

			if (kk == 0x9e) {

				printf("SKP V%x\n", x);

			} else if (kk == 0xa1) {

				printf("SKNP V%x\n", x);

			}
			break;

		case 0xf:

			if (kk == 0x07) {

				printf("LD V%x, DT\n", x);
			
			} else if (kk == 0x0a) {

				printf("LD V%x, K\n", x);

			} else if (kk == 0x15) {

				printf("LD DT, V%x\n", x);

			} else if (kk == 0x18) {

				printf("LD ST, V%x\n", x);

			} else if (kk == 0x1e) {

				printf("ADD I, V%x\n", x);

			} else if (kk == 0x29) {

				printf("LD F, V%x\n", x);

			} else if (kk == 0x33) {

				printf("LD B, V%x\n", x);

			} else if (kk == 0x55) {
	
				printf("LD [I], V%x\n", x);

			} else if (kk == 0x65) {

				printf("LD V%x, [I]\n", x);

			}

			break;

	}
}
