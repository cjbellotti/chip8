/*
 *
 * CPU Data
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#ifndef CPU_H
#define CPU_H

#define MEMORY_SIZE  4096
#define SCREEN_SIZE 2048
#define STACK_SIZE 16

#define PUSH(data) machine->registers.sp++; machine->stack[machine->registers.sp] = data;
#define POP(target) target = machine->stack[machine->registers.sp]; machine->registers.sp--

#define CLS memset(machine->screen, 0, 2048);
#define RET machine->registers.pc = machine->stack[machine->registers.sp]; machine->registers.sp--
#define JP(addr) machine->registers.pc = addr
#define CALL(addr) if(machine->registers.sp < 16){ machine->registers.sp++; machine->stack[machine->registers.sp] = machine->registers.pc;} machine->registers.pc = addr
#define SE_Vx_byte(vx, byte) if (machine->registers.v[vx] == byte) machine->registers.pc = (machine->registers.pc + 2) & 0xfff
#define SNE_Vx_byte(vx, byte) if (machine->registers.v[vx] != byte) machine->registers.pc = (machine->registers.pc + 2) & 0xfff
#define SE_Vx_Vy(vx, vy) if (machine->registers.v[vx] == machine->registers.v[vy]) machine->registers.pc = (machine->registers.pc + 2) & 0xfff
#define LD_Vx_byte(vx, byte)  machine->registers.v[vx] = byte
#define ADD_Vx_byte(vx, byte) machine->registers.v[vx] = (machine->registers.v[vx] + byte) & 0xff
#define LD_Vx_Vy(vx, vy) machine->registers.v[vx] = machine->registers.v[vy] 
#define OR_Vx_Vy(vx, vy) machine->registers.v[vx] |= machine->registers.v[vy] 
#define AND_Vx_Vy(vx, vy) machine->registers.v[vx] &= machine->registers.v[vy]
#define XOR_Vx_Vy(vx, vy) machine->registers.v[vx] ^= machine->registers.v[vy]
#define ADD_Vx_Vy(vx, vy) machine->registers.v[0xf] = ((machine->registers.v[vx] + machine->registers.v[vy]) & 0x100) ? 1 : 0;  machine->registers.v[vx] = (machine->registers.v[vx] + machine->registers.v[vy]) & 0xff
#define SUB_Vx_Vy(vx, vy) machine->registers.v[0xf] = (machine->registers.v[vx] > machine->registers.v[vy]) ? 1 : 0; machine->registers.v[vx] = (machine->registers.v[vx] - machine->registers.v[vy]) & 0xff
#define SHR_Vx(vx) machine->registers.v[0xf] = (machine->registers.v[vx] & 0x01) ? 1: 0; machine->registers.v[vx] >>= 1 
#define SUBN_Vx_Vy(vx, vy) machine->registers.v[0xf] = (machine->registers.v[vy] > machine->registers.v[vx]) ? 1 : 0; machine->registers.v[vx] = (machine->registers.v[vy] - machine->registers.v[vx]) & 0xff
#define SHL_Vx(vx) machine->registers.v[0xf] = (machine->registers.v[vx] & 0x80) ? 1 : 0; machine->registers.v[vx] <<= 1
#define SNE_Vx_Vy(vx, vy) if (machine->registers.v[vx] != machine->registers.v[vy]) machine->registers.pc = (machine->registers.v[vx] + 2) & 0xfff
#define LD_I_addr(addr) machine->registers.i = addr
#define JP_V0_addr(addr)  machine->registers.pc = (machine->registers.v[0] + addr) & 0xfff
#define RND_Vx_byte(vx, byte) machine->registers.v[vx] = rand() & byte
#define DRW_Vx_Vy_n(x, y, n) ;
#define SKP_Vx(vx) if(is_key_pressed(machine->registers.v[vx])) machine->registers.pc = (machine->registers.pc + 2) & 0xfff
#define SKNP_Vx(vx) if(!is_key_pressed(machine->registers.v[vx])) machine->registers.pc = (machine->registers.pc + 2) & 0xfff
#define LD_Vx_DT(vx) machine->registers.v[vx] = machine->registers.dt
#define LD_Vx_k(vx) machine->waiting_key = vx;
#define LD_DT_Vx(vx) machine->registers.dt = machine->registers.v[vx]
#define LD_ST_Vx(vx) machine->registers.st = machine->registers.v[vx]
#define ADD_I_Vx(vx) machine->registers.i += machine->registers.v[vx]; machine->registers.i &= 0xfff
#define LD_F_Vx(vx) machine->registers.i = 0x50 + machine->registers.v[vx & 0xf]*5
#define LD_I_Vx(vx) memcpy(machine->mem + machine->registers.i, machine->registers.v, vx +1) 
#define LD_Vx_I(vx) memcpy(machine->registers.v, machine->mem + machine->registers.i, vx +1) 

struct __registers_t {
	
	uint8_t v[16];
	
	uint16_t i;
	uint16_t pc;
	uint8_t sp;
	uint8_t dt;
	uint8_t st;
	
};

typedef struct __registers_t registers_t; 

struct __machine_t {
	
	registers_t registers;
	uint8_t mem[MEMORY_SIZE];
	char screen[SCREEN_SIZE];
	uint16_t stack[STACK_SIZE];
	int running;
	int waiting_key;
	
};

typedef struct __machine_t machine_t;

void exec_opcode(machine_t*, uint16_t);
void expansion(char*, Uint32*);
int is_key_pressed(char);
#endif
