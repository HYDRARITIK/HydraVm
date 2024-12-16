#include "virtual.h"
#include "stdio.h"
#include "byteswap.h"
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>

vm_impl *vm = NULL;
const char *lc3os_obj = NULL;

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

// INstrunction
/*ADD*/

void add(uint16_t ins)
{
    // check 5 th bit for mod
    uint16_t flag = ((ins) & (1 << 5));
    uint16_t reg1 = ((ins) & ((0x0007) << 6));
    uint16_t num1 = vm->reg[reg1];
    uint16_t dest = ((ins) & ((0x0007) << 9));
    uint16_t num2 = 0;
    printf("num1----%d\n", num1);
    if (flag == 0)
    {
        uint16_t reg2 = (ins) & (0x0007);
        num2 = vm->reg[reg2];
    }
    else
    {
        num2 = sign_extend((ins) & (0x001F), 5);
    }

    printf("num2----%d\n", num2);
    vm->reg[dest] = num1 + num2;
    updateFlags(dest);
}
/*AND*/
void AND(uint16_t instr)
{
    uint16_t dest = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        vm->reg[dest] = vm->reg[r1] & imm5;
    }
    else
    {
        uint16_t r2 = instr & 0x7;
        vm->reg[dest] = vm->reg[r1] & vm->reg[r2];
    }
    updateFlags(dest);
}
/*NOT*/
void NOT(uint16_t instr)
{
    uint16_t dest = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;

    vm->reg[dest] = ~vm->reg[r1];
    updateFlags(dest);
}

uint16_t memRead(uint16_t addr)
{
    if (addr == VM_ADDR_KBSR)
    {
        static fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        return select(1, &readfds, NULL, NULL, &timeout) ? VM_STATUS_BIT : 0;
    }
    else if (addr == VM_ADDR_KBDR)
    {
        if (memRead( VM_ADDR_KBSR))
        {
            return getchar();
        }
        else
        {
            return 0;
        }
    }
    else if (addr == VM_ADDR_DSR)
    {
        return VM_STATUS_BIT;
    }
    else if (addr == VM_ADDR_DDR)
    {
        return 0;
    }

    return vm->mem[addr];
}
void mem_write(uint16_t addr, uint16_t val)
{

    if (addr == VM_ADDR_KBSR || addr == VM_ADDR_KBDR || addr == VM_ADDR_DSR)
    {
        return;
    }
    else if (addr == VM_ADDR_DDR)
    {
        putchar(val);
        fflush(stdout);
        return;
    }
    vm->mem[addr] = val;
}
void load_indirect(vm_impl *vm, uint16_t ins)
{
    uint16_t dest = ((ins) & ((0x0007) << 9));
    uint16_t PC_offset = ((ins) & ((0x00FF)));

    uint16_t addr = memRead(vm->reg[R_PC] + PC_offset);
    vm->reg[dest] = memRead(addr);
}

// trap Routines

// output a string to console
void PUTS()
{

    uint16_t addr = vm->reg[RO];

    while (memRead(addr) != 0x0000)
    {
        putc((char)(memRead(addr)), stdout);
        addr++;
    }

    fflush(stdout);
}

// read image file
uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

vm_load_result vm_load_data(vm_impl *vm, unsigned const char *data, size_t length)
{
    assert(vm != NULL);

    uint16_t load_addr = swap16(*((uint16_t *)data));
    size_t load_length = (length - sizeof(uint16_t)) / sizeof(uint16_t);

    assert(load_addr + load_length < max_memory);

    // uint16_t *dest = vm->mem + load_addr;
    // uint16_t *source = (uint16_t*)(data + sizeof(uint16_t));

    uint16_t *dest_in_mem = vm->mem + load_addr;
    uint16_t *src_file_ptr = (uint16_t *)(data + sizeof(load_addr));

    if (dest_in_mem + load_length >= vm->mem + max_memory)
    {
        return VM_LOAD_INPUT_TOO_LARGE;
    }

    while (load_length-- > 0)
    {
        *(dest_in_mem++) = swap16(*(src_file_ptr++));
    }

    vm->reg[R_PC] = load_addr;

    return VM_LOAD_SUCCESS;
}
vm_load_result vm_load_file(vm_impl *vm, const char *file)
{
    int fd, ret;
    struct stat statbuf;
    unsigned char *data;

    // try to open the file
    if ((fd = open(file, O_RDONLY)) < 0)
    {
        return VM_LOAD_INPUT_NOT_FOUND;
    }
    // retrieve the file statistics
    if ((ret = fstat(fd, &statbuf)) < 0)
    {
        return VM_LOAD_INPUT_NOT_FOUND;
    }
    // map files into memory using mmap system call
    if ((data = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        return VM_LOAD_INPUT_NOT_FOUND;
    }

    vm_load_result result = vm_load_data(vm, data, statbuf.st_size);

    munmap(data, statbuf.st_size);
    close(fd);

    return result;
}

void vm_load_os(vm_impl *vm)
{
    vm_load_result res = vm_load_file(vm, (char *)lc3os_obj);
    assert(res == VM_LOAD_SUCCESS);
}

vm_run_result vm_perform(vm_impl *vm, uint16_t instr)
{
    uint16_t op = instr >> 12;
    switch (op)
    {
    case OP_ADD:
        add(instr);
        break;
    case OP_AND:
        AND(instr);
        break;
    default:
        break;
    }

    return VM_RUN_SUCCESS;
}
vm_run_result vm_run(vm_impl *vm)
{
    assert(vm != NULL);
    uint16_t cnt = 0;
    while (memRead(VM_ADDR_MCR) & VM_STATUS_BIT)
    {
        vm_run_result res = vm_perform(vm, memRead(vm->reg[R_PC]++));
        cnt++;
        printf("cnt %d\n", cnt);
        if (cnt > 200)
        {
            break;
        }
        if (res != VM_RUN_SUCCESS)
        {
            return res;
        }
    }

    return VM_RUN_SUCCESS;
}


