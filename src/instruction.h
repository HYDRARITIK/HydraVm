
#include "virtual.h"

 typedef enum
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
} _ops;


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

extern op_ex_f op_ex[NOPS];