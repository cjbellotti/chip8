#include <stdio.h>
#include <stdint.h>
#include "cpu.h"

char keys [] = {
		SDL_SCANCODE_X, // 0
		SDL_SCANCODE_1, // 1
		SDL_SCANCODE_2, // 2
		SDL_SCANCODE_3, // 3
		SDL_SCANCODE_Q, // 4
		SDL_SCANCODE_W, // 5
		SDL_SCANCODE_E, // 6
		SDL_SCANCODE_A, // 7
		SDL_SCANCODE_S, // 8
		SDL_SCANCODE_D, // 9
		SDL_SCANCODE_Z, // a
		SDL_SCANCODE_C, // b
		SDL_SCANCODE_4, // c
		SDL_SCANCODE_R, // d
		SDL_SCANCODE_F, // e
		SDL_SCANCODE_V  // f
};

inline void __drw_vx_vy_n(machine_t *machine, uint8_t vx, uint8_t vy, uint8_t n)
{
	int x = machine->registers.v[vx] & 63;
	int y = machine->registers.v[vy] & 31;
	int tmpY = 0;
	int tmpX = 0;
	uint8_t sprite = 0;
	int i = 0;
	int bitIndex = 0;
	machine->registers.v[0xf] = 0;
	for (i = 0; i < n; i++) 
	{
		sprite = machine->mem[machine->registers.i + i];
		tmpY = y << 6;
		tmpX = x;	
		for (bitIndex = 0; bitIndex < 8; bitIndex++)
		{
			uint8_t pixel = ((sprite >> (7 - bitIndex)) & 0x1);
			machine->registers.v[0xf] = machine->screen[tmpY + tmpX] != 0 && pixel != 0;
			machine->screen[tmpY + tmpX] ^= pixel;
			tmpX++;
		}
		y++;
	}

}

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

				CLS;	

			} else if (opcode == 0x00ee){

				RET;	

			} else {

				//printf("SYS %x\n", nnn);

			} 

			break;

		case 1:

			JP(nnn);
			break;

		case 2:

			CALL(nnn);	
			break;

		case 3:

			SE_Vx_byte(x, kk);
			break;

		case 4:

			SNE_Vx_byte(x, kk);
			break;

		case 5:
			
			if (n == 0) {
				SE_Vx_Vy(x, y);
			}	
			break;

		case 6:

			LD_Vx_byte(x, kk);	
			break;

		case 7:

			ADD_Vx_byte(x, kk);
			break;

		case 8:

			if (n == 0) {

				LD_Vx_Vy(x, y);

			} else if (n == 1) {

				OR_Vx_Vy(x, y);

			} else if (n == 2) {

				AND_Vx_Vy(x, y);

			} else if (n == 3) {

				XOR_Vx_Vy(x, y);

			} else if (n == 4) {

				ADD_Vx_Vy(x, y);

			} else if (n == 5) {

				SUB_Vx_Vy(x, y);

			} else if (n == 6) {

				SHR_Vx(x);

			} else if (n == 7) {

				SUBN_Vx_Vy(x, y);

			} else if (n == 0xe) {

				SHL_Vx(x);

			}
			break;

		case 9:
			
			if (n == 0) {

				SNE_Vx_Vy(x, y);

			}
			break;

		case 0xa:

			LD_I_addr(nnn);
			break;

		case 0xb:

			JP_V0_addr(nnn);
			break;

		case 0xc:

			RND_Vx_byte(x, kk);
			break;

		case 0xd:

			__drw_vx_vy_n(machine, x, y, n);
			break;

		case 0xe:

			if (kk == 0x9e) {

				SKP_Vx(x);

			} else if (kk == 0xa1) {

				SKNP_Vx(x);

			}
			break;

		case 0xf:

			if (kk == 0x07) {

				LD_Vx_DT(x);
			
			} else if (kk == 0x0a) {

				printf("LD V%x, K\n", x);
				LD_Vx_k(x);

			} else if (kk == 0x15) {

				LD_DT_Vx(x);

			} else if (kk == 0x18) {

				LD_ST_Vx(x);

			} else if (kk == 0x1e) {

				ADD_I_Vx(x);

			} else if (kk == 0x29) {

				LD_F_Vx(x);

			} else if (kk == 0x33) {

				machine->mem[machine->registers.i] = (machine->registers.v[x] / 100);
				machine->mem[machine->registers.i + 1] = (machine->registers.v[x] / 10) % 10;
				machine->mem[machine->registers.i + 2] = machine->registers.v[x] % 10;

			} else if (kk == 0x55) {
	
				LD_I_Vx(x);

			} else if (kk == 0x65) {

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
int is_key_pressed(char key)
{
	const Uint8 *sdl_key = SDL_GetKeyboardState(NULL);
	Uint8 real_key = keys[(int) key];
	return sdl_key[real_key];
}
