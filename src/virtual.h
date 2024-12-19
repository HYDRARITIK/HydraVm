// #include <assert.h>

#define max_memory (1 << 16)
#define pc_Start 0x3000
#define NOPS (16) // number of instructions

typedef unsigned short uint16_t;

typedef enum __Reg
{
    RO = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R_PC,
    R_COND,
    R_COUNT
}_Reg;

// conditional flags

typedef enum __flags
{
    Fl_POS = 1 << 0,
    FL_ZER = 1 << 1,
    FL_NEG = 1 << 2
} _flags;

typedef enum __Sbit 
{
    VM_SIGN_BIT = 1 << 15,
    VM_STATUS_BIT = 1 << 15,
}_Sbit;

typedef struct __vm_impl
{
    uint16_t mem[max_memory];
    uint16_t reg[R_COUNT];
} vm_impl;

typedef enum __vm_addr_special
{
    VM_ADDR_KBSR = 0xfe00,
    VM_ADDR_KBDR = 0xfe02,
    VM_ADDR_DSR = 0xfe04,
    VM_ADDR_DDR = 0xfe06,
    VM_ADDR_MCR = 0xfffe,
} vm_addr_special;

/*fuction declaration*/
vm_impl *vm_create(void);
void vm_destroy(vm_impl *vm);

uint16_t memRead(uint16_t addr);
void mem_write(uint16_t addr, uint16_t val);

