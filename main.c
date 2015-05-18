#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

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
	printf("%x - %x", machine.mem[0x200 + 0xf4], machine.mem[0x200 + 0xf45]);
	machine.running = 1;
	while(machine.running) {
		if (machine.registers.pc >= MEMORY_SIZE)
			machine.running = 0;
		else
		{
			uint16_t opcode = (machine.mem[machine.registers.pc] << 8) | (machine.mem[++machine.registers.pc]);
			printf("%x - %x\n ", machine.registers.pc, opcode);
			machine.registers.pc++;
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
	
	machine->registers.pc = 0x200;
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
	
	fread(machine->mem + 0x200, length, 1, file_rom);
	
	fclose(file_rom);
	
}
