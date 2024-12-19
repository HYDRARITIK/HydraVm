
#pragma once

#include "virtual.h"

// #include "virtual.h"
#define NOPS (16) // number of instructions

#define POFF9(i) (i & ((1 << 9) - 1));
#define POFF6(i) (i & ((1 << 6) - 1));
#define POFF11(i) (i & ((1 << 11) - 1));

#define BSR(i) ((i >> 6) & 0x7)
#define NZP(i) (((i) >> 9) & 0x7)

#define DR(i) (((i) >> 9) & 0x7)
#define SR1(i) (((i) >> 6) & 0x7)
#define SR2(i) ((i) & 0x7)
#define IMM(i) ((i) & 0x1F)
#define OPC(i) ((i) >> 12)
// typedef unsigned short uint16_t;

typedef enum _ops
{
    OP_BR = 0b0000, /* branch */
    OP_ADD,         /* add  */
    OP_LD,          /* load */
    OP_ST,          /* store */
    OP_JSR,         /* jump register */
    OP_AND,         /* bitwise and */
    OP_LDR,         /* load register */
    OP_STR,         /* store register */
    OP_RTI,         /* unused */
    OP_NOT,         /* bitwise not */
    OP_LDI,         /* load indirect */
    OP_STI,         /* store indirect */
    OP_JMP,         /* jump */
    OP_RES,         /* reserved (unused) */
    OP_LEA,         /* load effective address */
    OP_TRAP         /* execute trap */
} ops;

void ADD(uint16_t i);
void AND(uint16_t i);
void NOT(uint16_t i);
void LD(uint16_t i);
void LDI(uint16_t i);
void LDR(uint16_t i);
void LEA(uint16_t i);
void ST(uint16_t i);
void STI(uint16_t i);
void STR(uint16_t i);
void BR(uint16_t i);
void JMP(uint16_t i);
void JSR(uint16_t i);
void RTI(uint16_t i);
void TRAP(uint16_t i);
void RES(uint16_t i);

 typedef void (*op_ex_f)(uint16_t instruction); // type of function pointer
extern char *opcodes[NOPS];                           // array of strings
extern op_ex_f op_ex[NOPS];