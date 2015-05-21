#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"


void reset(machine_t*);
void load_rom(machine_t *, char*);

int main(int argc, char *argv[]) {

	machine_t machine;
	
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
			exec_opcode(&machine, opcode);
		}
	}
	
	return 0;

}

void reset(machine_t *machine) {

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

void load_rom(machine_t *machine, char *path) {
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
	
	fread(machine->mem + 0x200, length, 1, file_rom); 

	fclose(file_rom);
	
}
