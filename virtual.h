#define max_memory (1 << 16)
#define pc_Start 0x3000

#include <assert.h>
typedef unsigned short uint16_t;

// uint16_t memory[max_memory];

extern const char *lc3os_obj;
// extern  int lc3os_obj_len;


// registors
enum
{
    RO,
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
};
// uint16_t reg[R_COUNT];

enum
{
    OP_BR = 0b0000, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
};

// conditional flags

enum
{
    Fl_POS = 1 << 0,
    FL_NEG = 1 << 1,
    FL_ZER = 1 << 2
};

 
// Trap Routines
 enum
{
    TRAP_GETC = 0x20,  /* get character from keyboard, not echoed onto the terminal */
    TRAP_OUT = 0x21,   /* output a character */
    TRAP_PUTS = 0x22,  /* output a word string */
    TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
    TRAP_PUTSP = 0x24, /* output a byte string */
    TRAP_HALT = 0x25   /* halt the program */
};



typedef struct  {
    uint16_t mem[max_memory];
    uint16_t reg[R_COUNT];
} vm_impl;
enum {
    VM_EXIT_SUCCESS,
    VM_EXIT_USAGE,
    VM_EXIT_INPUT_INVALID,
    VM_EXIT_OPCODE_INVALID,
};


// typedef struct vm_impl* vm_impl*;

// MARK: - Creation
#include <stdlib.h>
// vm_impl* vm_create(void) {
//     vm_impl* vm = calloc(1, sizeof( vm_impl));

//     vm->reg[R_PC] = pc_Start;
//     vm->reg[R_COND] = FL_ZER;
//     vm->mem[VM_ADDR_MCR] = VM_STATUS_BIT;

//     return vm;
// }


extern vm_impl* vm;

// void vm_destroy(vm_impl* vm) {
//     free(vm);
// }



typedef enum {
    VM_LOAD_SUCCESS,
    VM_LOAD_INPUT_NOT_FOUND,
    VM_LOAD_INPUT_TOO_LARGE,
} vm_load_result;

typedef enum {
    VM_RUN_SUCCESS,
    VM_RUN_UNIMPLEMENTED_OPCODE,
} vm_run_result;

typedef enum {
    VM_ADDR_KBSR = 0xfe00,
    VM_ADDR_KBDR = 0xfe02,
    VM_ADDR_DSR  = 0xfe04,
    VM_ADDR_DDR  = 0xfe06,
    VM_ADDR_MCR  = 0xfffe,
} vm_addr_special;

#include <stdint.h>
enum {
    VM_SIGN_BIT = 1 << 15,
    VM_STATUS_BIT = 1 << 15,
};

/*fuction declaration*/
vm_impl* vm_create(void);
void vm_destroy(vm_impl* vm);

void vm_load_os(vm_impl* vm);
vm_load_result vm_load_file(vm_impl* vm, const char *file);
vm_load_result vm_load_data(vm_impl* vm, unsigned const char *data, size_t length);

vm_run_result vm_run(vm_impl* vm);

// extern void ld_img(char *fname,uint16_t offset);

// macros

#define DR(i) (((i)>>9)&0x7)
#define SR1(i) (((i)>>6)&0x7)
#define SR2(i) ((i)&0x7)
#define IMM(i) ((i)&0x1F)
#define OPC(i) ((i)>>12)

#define NOPS (16) // number of instructions
typedef void (*op_ex_f)(uint16_t instruction); //type of function pointer

void add(uint16_t instruction);
// void and(uint16_t instruction);
// void br(uint16_t instruction);
// void jmp(uint16_t instruction);
// void jsr(uint16_t instruction);
// void ld(uint16_t instruction);
// void ldi(uint16_t instruction);
// void ldr(uint16_t instruction);
// void lea(uint16_t instruction);
// void not(uint16_t instruction);
// void st(uint16_t instruction);
// void sti(uint16_t instruction);
// void str(uint16_t instruction);
// void rti(uint16_t instruction);
// void trap(uint16_t instruction);
// void res(uint16_t instruction);


// op_ex_f op_ex[NOPS] = { 
//     br, add, ld, st, jsr, and, ldr, str, rti, not, ldi, sti, jmp, res, lea, trap 
// };

// op_ex_f op_ex[NOPS] = { 
//     add, add
// };