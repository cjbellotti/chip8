// Instructions

#define CALL(addr)  machine.registers.sp++; machine.stack[machine.registers.sp] = addr
#define RET machine.registers.pc = machine.stack[machine.registers.sp]; machine.registers.sp--
#define JP(addr) machine.registers.pc = addr
#define SE_Vx_byte(vx, byte) if (machine.registers.v[vx] == byte) machine.registers.pc+=2
#define SNE_Vx_byte(vx, byte) if (machine.registers.v[vx] != byte) machine.registers.pc+=2
#define SE_Vx_Vy(vx, vy) if (machine.registers.v[vx] == machine.registers.v[vy]) machine.registers.pc+=2
