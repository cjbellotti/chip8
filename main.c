#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "cpu.h"

uint8_t spritesChars[] = {
		0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
		0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
		0x90, 0x90, 0xf0, 0x10, 0x10, // 4
		0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
		0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
		0xf0, 0x10, 0x20, 0x40, 0x40, // 7
		0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
		0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
		0xf0, 0x90, 0xf0, 0x90, 0x90, // A
		0xe0, 0x90, 0xe0, 0x90, 0xe0, // B
		0xf0, 0x80, 0x80, 0x80, 0xf0, // C
		0xe0, 0x90, 0x90, 0x90, 0xe0, // D
		0xf0, 0x80, 0xf0, 0x80, 0xf0, // E
		0xf0, 0x80, 0xf0, 0x80, 0x80  // F	

};


void reset(machine_t*);
void load_rom(machine_t *, char*);

int main()
{

	machine_t machine;
	reset(&machine);
	load_rom(&machine, "./roms/PONG.ch8");
	//load_rom(&machine, "./roms/INVADERS.ch8");

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *wnd = SDL_CreateWindow("CHIP-8 Emulator",
										SDL_WINDOWPOS_CENTERED,
										SDL_WINDOWPOS_CENTERED,
										640,
										320,
										SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_Renderer *rnd = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture *tex = SDL_CreateTexture(rnd, SDL_PIXELFORMAT_RGBA8888,
										SDL_TEXTUREACCESS_STREAMING,
										64, 32);

	SDL_Surface *surface = SDL_CreateRGBSurface(0, 64, 32, 32, 
													0x00ff0000,
													0x0000ff00,
													0x000000ff,
													0xff000000);


	//SDL_LockTexture(tex, NULL, &surface->pixels, &surface->pitch);


	//SDL_UnlockTexture(tex);
	//
	//Test
	//
	
/*	machine.mem[0x0000] = 0x20;
	machine.mem[0x0001] = 0x60;
	machine.mem[0x0002] = 0x20;
	machine.mem[0x0003] = 0x20;
	machine.mem[0x0004] = 0x70;

	machine.mem[0x200] = 0x61;
	machine.mem[0x201] = 0x0a;
	machine.mem[0x202] = 0x62;
	machine.mem[0x203] = 0x0a;
	machine.mem[0x204] = 0xa0;
	machine.mem[0x205] = 0x00;
	machine.mem[0x206] = 0xd1;
	machine.mem[0x207] = 0x25;
	machine.mem[0x208] = 0x12;
	machine.mem[0x209] = 0x00;*/

	int running = 1;
	SDL_Event ev;
	double last_ticks = 0;
	double last_ticks_cpu = 0;
	while (running)
	{


		//SDL_WaitEvent(&ev);
		while(SDL_PollEvent(&ev)) {
				switch(ev.type)
				{
						case SDL_QUIT:
								running = 0;
								break;
				}
		}

		if (SDL_GetTicks() - last_ticks_cpu > (1000/ 2000))
		{
				if (machine.registers.pc >= MEMORY_SIZE)
						machine.registers.pc = 0x200;
				else
				{
						uint16_t opcode = (machine.mem[machine.registers.pc] << 8) | (machine.mem[machine.registers.pc + 1]);
						machine.registers.pc+=2;
						exec_opcode(&machine, opcode);
				}
				last_ticks_cpu = SDL_GetTicks();
		}	

		if (SDL_GetTicks() - last_ticks > (1000/ 60))
		{

				SDL_LockTexture(tex, NULL, &surface->pixels, &surface->pitch);

				expansion(machine.screen, (Uint32*) surface->pixels);

				SDL_UnlockTexture(tex);

				SDL_RenderClear(rnd);
				SDL_RenderCopy(rnd, tex, NULL, NULL);
				SDL_RenderPresent(rnd);
				last_ticks = SDL_GetTicks();
				if (machine.registers.dt > 0)
						machine.registers.dt--;
				if (machine.registers.st > 0)
						machine.registers.st--;
		}

	}
	// TODO : Logica

	SDL_DestroyRenderer(rnd);
	SDL_DestroyWindow(wnd);
	SDL_Quit();
	return 0;
}
/*int main(int argc, char *argv[]) {

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

}*/

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

	for (i = 0; i < SCREEN_SIZE; i++)
	{
		machine->screen[i] = 0x00;
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

	memcpy(machine->mem + 0x50, spritesChars, 80);
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
