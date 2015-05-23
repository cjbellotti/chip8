all:
	gcc -Wall $(CFLAGS) main.c cpu.c -o main -lSDL2 -lSDL2main
clean:
	rm main
	rm *.o
