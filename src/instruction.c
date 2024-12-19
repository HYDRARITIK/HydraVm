
#include "instruction.h"
#include "stdio.h"
#include "virtual.h"

extern uint16_t running;
extern vm_impl *vm;
// INstrunction
/*ADD*/

void ADD(uint16_t i)
{
    uint16_t dest = DR(i);
    uint16_t sr1 = SR1(i);
    uint16_t imm_flag = (i >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(i & 0x1F, 5);
        vm->reg[dest] = vm->reg[sr1] + imm5;
    }
    else
    {
        uint16_t sr2 = SR2(i);
        vm->reg[dest] = vm->reg[sr1] + vm->reg[sr2];
    }
    updateFlags(dest);
}
/*AND*/
void AND(uint16_t i)
{
    uint16_t dest = DR(i);
    uint16_t sr1 = SR1(i);
    uint16_t imm_flag = (i >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(i & 0x1F, 5);
        vm->reg[dest] = vm->reg[sr1] & imm5;
    }
    else
    {
        uint16_t r2 = i & 0x7;
        vm->reg[dest] = vm->reg[sr1] & vm->reg[r2];
    }
    updateFlags(dest);
}

/*ld*/
// load data from main memory to destination register

void LD(uint16_t i)
{
    uint16_t dr = DR(i);
    uint16_t pc_offset = POFF9(i);

    vm->reg[dr] = vm->mem[(vm->reg[R_PC] + pc_offset)];

    updateFlags(dr);
}

/*ldi  load indirect*/

void LDI(uint16_t i)
{
    uint16_t dr = DR(i);
    uint16_t pc_offset = POFF9(i);

    vm->reg[dr] = vm->mem[vm->mem[(vm->reg[R_PC] + pc_offset)]];

    updateFlags(dr);
}

/*ldr            load base +offset */

void LDR(uint16_t i)
{
    uint16_t dr = DR(i);
    uint16_t pc_offset = POFF6(i);
    uint16_t baser = BSR(i);

    vm->reg[dr] = vm->mem[(vm->reg[baser] + pc_offset)];
    updateFlags(dr);
}

/*lea load effective address*/

void LEA(uint16_t i)
{
    uint16_t dr = DR(i);
    uint16_t pc_offset = POFF9(i);
    // uint16_t baser=BSR(i);

    vm->reg[dr] = (vm->reg[R_PC] + pc_offset);
    updateFlags(dr);
}

void NOT(uint16_t i)
{
    vm->reg[DR(i)] = ~vm->reg[SR1(i)];
    updateFlags(DR(i));
}

// store operations
/*st store*/
void ST(uint16_t i)
{
    uint16_t sr = DR(i);
    uint16_t pc_offset = POFF9(i);

    vm->mem[vm->reg[R_PC] + pc_offset] = vm->reg[sr];
}

/*sti store indirect*/
void STI(uint16_t i)
{
    uint16_t sr = DR(i);
    uint16_t pc_offset = POFF9(i);
    vm->mem[vm->mem[vm->reg[R_PC] + pc_offset]] = vm->reg[sr];
}
/*str - Store base + offset

*/

void STR(uint16_t i)
{
    uint16_t sr = DR(i);
    uint16_t baser = BSR(i);
    uint16_t pc_offset = POFF6(i);

    vm->mem[vm->reg[baser] + pc_offset] = vm->reg[sr];
}
/*jump */
void JMP(uint16_t i)
{
    vm->reg[R_PC] = vm->reg[BSR(i)];
}

/*jsr - Jump to subroutines*/

void JSR(uint16_t i)
{
    uint16_t pc_offset = POFF11(i);
    vm->reg[R7] = vm->reg[R_PC];
    vm->reg[R_PC] = ((i >> 10) & 0x1) ? (vm->reg[R_PC] + pc_offset) : (vm->reg[BSR(i)]);
}

/*BR*/

void BR(uint16_t i)
{

    uint16_t pc_offset = POFF9(i);
    if(!pc_offset){
        running=0;
    }
   pc_offset= sign_extend(pc_offset, 9);
    
    printf("pc_offset %d\n", pc_offset);
    uint16_t nzp = NZP(i);
    printf("nzp %d\n", nzp);

    if ((nzp & vm->reg[R_COND]))
    {
        vm->reg[R_PC] += pc_offset;
    }

    printf("pc %d\n", vm->reg[R_PC]);
}


void RTI(uint16_t instr)
{
    // R7 is the stack pointer
    vm->reg[R_PC] = memRead(vm->reg[R7]++);
    vm->reg[R_COND] = memRead(vm->reg[R7]++);
}

void RES(uint16_t instr)
{
    // halt the program
    running = 0;
}

op_ex_f op_ex[NOPS] = {
    BR, ADD, LD, ST, JSR, AND, LDR, STR, RTI, NOT, LDI, STI, JMP, RES, LEA, TRAP};
    

char *opcodes[] = {
    "BR", "ADD", "LD", "ST", "JSR", "AND", "LDR", "STR", "RTI", "NOT", "LDI", "STI", "JMP", "RES", "LEA", "TRAP"};


