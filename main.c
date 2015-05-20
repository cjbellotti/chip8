#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "instructions.h"

#define MEMORY_SIZE  4096
#define STACK_SIZE 16

struct registers_t {
	
	uint8_t v[16];
	
	uint16_t i;
	uint16_t pc;
	uint16_t sp;
	uint16_t dt;
	uint16_t st;
	
};

struct machine_t {
	
	struct registers_t registers;
	uint8_t mem[MEMORY_SIZE];
	uint16_t stack[STACK_SIZE];
	int running;
	
};

void reset(struct machine_t*);
void load_rom(struct machine_t *, char*);

int main(int argc, char *argv[]) {

	struct machine_t machine;
	
	reset(&machine);
	load_rom(&machine, "./roms/PONG.ch8");
	machine.running = 1;
	while(machine.running) {
		if (machine.registers.pc >= MEMORY_SIZE)
			machine.running = 0;
		else
		{
			uint16_t opcode = (machine.mem[machine.registers.pc] << 8) | (machine.mem[++machine.registers.pc]);
			machine.registers.pc++;
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
					break;

				case 7:

					printf("ADD V%x, %x\n", x, kk);	
					break;

				case 8:

					if (n == 0) {

						printf("LD V%x, V%x\n", x, y);
 
					} else if (n == 1) {

						printf("OR V%x, V%x\n", x, y);

					} else if (n == 2) {

						printf("AND V%x, V%x\n", x, y);

					} else if (n == 3) {

						printf("XOR V%x, V%x\n", x, y);

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
	}
	
	return 0;

}

void reset(struct machine_t *machine) {

	int i = 0;
	for (i = 0; i < MEMORY_SIZE; i++)
	{
		machine->mem[i] = 0x00;	
	}
	for (i = 0; i < STACK_SIZE; i++)
	{
		machine->stack[i] = 0x00;
	}
	for (i = 0; i <16; i++)
	{
		machine->registers.v[i] = 0x00;
	}
	

	//machine->registers.pc = 0x200;
	machine->registers.pc = 0x00;
	machine->registers.sp = 0x0f;
	machine->registers.i = 0x0000;
	machine->registers.st = 0x00;
	machine->registers.dt = 0x00;
}

void load_rom(struct machine_t *machine, char *path) {
	FILE *file_rom = fopen(path, "r");
	
	if (file_rom == NULL) 
	{
		fprintf(stderr, "No se puede abrir el archivo %s.", path);
		exit(1);
	}
	
	fseek(file_rom, 0, SEEK_END);
	int length = ftell(file_rom);
	fseek(file_rom, 0, SEEK_SET);

	printf("Longitud del archivo: %d\n", length);
	
	fread(machine->mem, 4096, 1, file_rom); 

	fclose(file_rom);
	
}
