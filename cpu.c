#include <stdio.h>
#include <stdint.h>
#include "cpu.h"

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
				CLS;	

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
				ADD_Vx_Vy(x, y);

			} else if (n == 5) {

				printf("SUB V%x, V%x\n", x, y);
				SUB_Vx_Vy(x, y);

			} else if (n == 6) {

				printf("SHR V%x {, V%x}\n", x, y);
				SHR_Vx(x);

			} else if (n == 7) {

				printf("SUBN V%x, V%x\n", x, y);
				SUBN_Vx_Vy(x, y);

			} else if (n == 0xe) {

				printf("SHL V%x, {, V%x}\n", x, y);
				SHL_Vx(x);

			}
			break;

		case 9:
			
			if (n == 0) {

				printf("SNE V%x, V%x\n", x, y);
				SNE_Vx_Vy(x, y);

			}
			break;

		case 0xa:

			printf("LD I, %x\n", nnn);
			LD_I_addr(nnn);
			break;

		case 0xb:

			printf("JP V0, %x\n", nnn);
			JP_V0_addr(nnn);
			break;

		case 0xc:

			printf("RND V%x, %x\n", x, kk);
			RND_Vx_byte(x, kk);
			break;

		case 0xd:

			printf("DRW V%x, V%x, %x\n", x, y, n);
			DRW_Vx_Vy_n(x, y, n);
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
				LD_Vx_DT(x);
			
			} else if (kk == 0x0a) {

				printf("LD V%x, K\n", x);

			} else if (kk == 0x15) {

				printf("LD DT, V%x\n", x);
				LD_DT_Vx(x);

			} else if (kk == 0x18) {

				printf("LD ST, V%x\n", x);
				LD_ST_Vx(x);

			} else if (kk == 0x1e) {

				printf("ADD I, V%x\n", x);
				ADD_I_Vx(x);

			} else if (kk == 0x29) {

				printf("LD F, V%x\n", x);

			} else if (kk == 0x33) {

				printf("LD B, V%x\n", x);

			} else if (kk == 0x55) {
	
				printf("LD [I], V%x\n", x);
				LD_I_Vx(x);

			} else if (kk == 0x65) {

				printf("LD V%x, [I]\n", x);
				LD_Vx_I(x);

			}

			break;

	}
}

void expansion (char *from, Uint32 *to) 
{

	int i = 0;
	for (i = 0; i < 2048; i++) 
		to[i] = (from[i]) ? -1 : 0;

}
