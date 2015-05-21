/*
 *
 * CPU Data
 *
 */
#include <stdint.h>

#ifndef CPU_H
#define CPU_H

#define MEMORY_SIZE  4096
#define STACK_SIZE 16

struct __registers_t {
	
	uint8_t v[16];
	
	uint16_t i;
	uint16_t pc;
	uint8_t sp;
	uint16_t dt;
	uint16_t st;
	uint8_t vf;
	
};

typedef struct __registers_t registers_t; 

struct __machine_t {
	
	registers_t registers;
	uint8_t mem[MEMORY_SIZE];
	uint16_t stack[STACK_SIZE];
	int running;
	
};

typedef struct __machine_t machine_t;

void exec_opcode(machine_t*, uint16_t);

#endif
