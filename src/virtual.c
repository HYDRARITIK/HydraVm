#include "virtual.h"
#include "stdio.h"

vm_impl *vm = NULL;
uint16_t running = 1;

vm_impl *vm_create(void)
{
    vm_impl *vm = (vm_impl *)calloc(1, sizeof(vm_impl));

    vm->reg[R_PC] = pc_Start;
    vm->reg[R_COND] = FL_ZER;
    vm->mem[VM_ADDR_MCR] = VM_STATUS_BIT;

    return vm;
}

void vm_destroy(vm_impl *vm)
{
    free(vm);
}

// lc3os_obj="/home/ritik/VM/2048.obj";

uint16_t sign_extend(uint16_t x, int bitCNt)
{
    // extenf neg number by extending one to left

    if (x & (1 << (bitCNt - 1)))
    {
        x |= (0xffff >> bitCNt);
    }
    return x;
}

void updateFlags(uint16_t r)
{
    if (vm->reg[r] == 0)
    {
        vm->reg[R_COND] = FL_ZER;
    }
    else if (vm->reg[r] >> 15)
    {
        vm->reg[R_COND] = FL_NEG;
    }
    else
    {
        vm->reg[R_COND] = Fl_POS;
    }
}

uint16_t memRead(uint16_t addr)
{
    return vm->mem[addr];
}
void mem_write(uint16_t addr, uint16_t val)
{
    vm->mem[addr] = val;
}

// read image file
uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}


// uint16_t running=1;

