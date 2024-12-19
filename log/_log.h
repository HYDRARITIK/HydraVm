#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void printf_binary(FILE *f, uint16_t num);
void printf_inst(FILE *f, uint16_t instr);
void printf_mem(FILE *f, uint16_t *mem, uint16_t from_addr, uint16_t to_addr);
void printf_reg(FILE *f, uint16_t *reg, int idx);
void printf_reg_all(FILE *f, uint16_t *reg, int size);