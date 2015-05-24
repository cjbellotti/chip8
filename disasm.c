#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void disasm_opcode(uint16_t opcode)
{
	uint16_t nnn = opcode & 0xfff;
	uint8_t kk = opcode & 0xff;
	uint8_t n = opcode & 0xf;
	uint8_t x = (opcode >> 8) & 0xf;
	uint8_t y = (opcode >> 4) & 0xf;
	uint8_t op = (opcode >> 12);

	printf("%x - ", opcode);

	switch (op)
	{
		case 0:

			if (opcode == 0x00e0){

				printf("CLS\n");	

			} else if (opcode == 0x00ee){

				printf("RET\n");	

			} else {

				//printf("SYS %x\n", nnn);

			} 

			break;

		case 1:

			printf("JP %x\n", nnn);
			break;

		case 2:

			printf("CALL %x\n", nnn);	
			break;

		case 3:

			printf("SE V%x, %x\n", x, kk);
			break;

		case 4:

			printf("SNE V%x, %x\n", x, kk);
			break;

		case 5:
			
			if (n == 0) {

				printf("SE V%x, V%x\n", x, y);

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

				printf("SHR V%x\n", x);

			} else if (n == 7) {

				printf("SUBN V%x, V%x\n", x, y);

			} else if (n == 0xe) {

				printf("SHL V%x\n", x);

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

		default:

			printf("%x - Unidentified\n", opcode);

	}

}

int main (int argc, char *argv[])
{

	if (argc < 2)
	{
		printf("You must to specify rom file name.\n");
		return 0;
	}

	uint8_t mem[4096];
	memset(mem, 0, 4096);

	FILE *file_rom = fopen(argv[1], "r");
	
	if (file_rom == NULL) 
	{
		fprintf(stderr, "No se puede abrir el archivo %s.", argv[1]);
		exit(1);
	}
	
	fseek(file_rom, 0, SEEK_END);
	int length = ftell(file_rom);
	fseek(file_rom, 0, SEEK_SET);

	printf("Longitud del archivo: %d\n", length);
	
	fread(mem, length, 1, file_rom); 

	fclose(file_rom);

	int i = 0;
	uint16_t pc = 0;
	for (i = 0; i < 4096; i++)
	{
	
		printf("%x - ", pc);	
		uint16_t opcode = (mem[pc] << 8) | (mem[pc + 1]);
		pc+=2;
		disasm_opcode(opcode);
	}
	return 0;

}
