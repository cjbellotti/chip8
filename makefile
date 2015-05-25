all:
	gcc -Wall $(CFLAGS) chip8emu.c cpu.c -o chip8emu -lSDL2 -lSDL2main
	gcc $(CFLAGS) disasm.c -o disasm
clean:
	rm chip8emu 
	rm disasm
	rm *.o
