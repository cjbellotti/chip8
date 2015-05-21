all:
	gcc -Wall main.c cpu.c -o main
clean:
	rm main
	rm *.h
