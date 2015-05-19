#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


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
			printf("%x", opcode);
			machine.registers.pc++;
			uint16_t nnn = opcode & 0xfff;
			uint8_t kk = opcode & 0xff;
			uint8_t n = opcode & 0xf;
			uint8_t x = (opcode >> 8) & 0xf;
			uint8_t y = (opcode >> 4) & 0xf;
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
