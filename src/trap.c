


#include "trap.h"
#include "stdio.h"
#include "virtual.h"



// extern vm_impl *vm;
trp_ex_f trp_ex[8] = {tgetch, tout, tputs, tin, tputsp, thalt, tinu16, toutu16};


void TRAP(uint16_t instr)
{
    trp_ex[TRP(instr) - trp_offset]();
}

// Trap Function	TRAPVECT	trp_ex[] index	Comments
// tgetc	0x20	0	Reads a character (char) from the keyboard that get copied in R0
// tout	0x21	1	Writes the character (char) from R0 to the console.
// tputs	0x22	2	Writes a string of characters to the console. As a rule, the characters are kept in a contiguous memory location, one char per memory location. Starting with the address specified in R0. If 0x0000 is encountered, printing stops.
// tin	0x23	3	Reads a character (char) from the keyboard, and it gets copied in R0. Afterward, the char is printed on the console.
// tputsp	0x24	4	Not implemented. This trap is used to store 2 characters per memory location instead of 1. Otherwise, it works like tputs. It’s left out as an exercise.
// thalt	0x25	5	Halts execution of the program. The VM stops.
// tinu16	0x26	6	Reads a uint16_t from the keyboard and stores it in R0.
// toutu16	0x27	7	Writes the uint16_t found inside R0.

void tgetch()
{
    vm->reg[RO] = getchar();
}

void tout()
{
    putchar(vm->reg[RO]);
    fflush(stdout);
}

void tputs()
{
    uint16_t *st_addr = (uint16_t *)(vm->mem + vm->reg[RO]);

    while (*(st_addr))
    {
        putchar(*(st_addr));
        (st_addr)++;
    }
}

void tin()
{
    vm->reg[RO] = getchar();
    putchar(vm->reg[RO]);
    fflush(stdout);
}

void tputsp()
{
    uint16_t *st_addr = (uint16_t *)(vm->mem + vm->reg[RO]);

    while (*(st_addr))
    {
        putchar(*(st_addr));
        (st_addr)++;
    }
}

void thalt(uint16_t *running)
{
    running = 0;
}

void tinu16()
{
    scanf("%hu", &vm->reg[RO]);
}

void toutu16()
{
    printf("%hu\n", vm->reg[RO]);
}

