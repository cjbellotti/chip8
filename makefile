all:
	gcc -Wall $(CFLAG) main.c cpu.c -o main
clean:
	rm main
	rm *.h
