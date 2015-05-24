all:
	gcc -Wall $(CFLAGS) main.c cpu.c -o main -lSDL2 -lSDL2main
	gcc $(CFLAGS) disasm.c -o disasm
clean:
	rm main
	rm disasm
	rm *.o
