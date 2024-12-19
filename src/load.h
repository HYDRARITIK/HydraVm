// #include "instruction.h"

#pragma once
#include "virtual.h"
#include "instruction.h"
typedef enum _vm_load_result
{
    VM_LOAD_SUCCESS,
    VM_LOAD_INPUT_NOT_FOUND,
    VM_LOAD_INPUT_TOO_LARGE,
} vm_load_result;

typedef enum _vm_run_result
{
    VM_RUN_SUCCESS,
    VM_RUN_UNIMPLEMENTED_OPCODE,
} vm_run_result;


typedef enum _vm_exit_code
{
    VM_EXIT_SUCCESS,
    VM_EXIT_USAGE,
    VM_EXIT_INPUT_INVALID,
    VM_EXIT_OPCODE_INVALID,
} vm_exit_code;

void vm_load_os(vm_impl *vm);
vm_load_result vm_load_file(vm_impl *vm, const char *file);
vm_load_result vm_load_data(vm_impl *vm, unsigned const char *data, uint16_t length);

vm_run_result vm_run(vm_impl *vm);

vm_load_result ld_image(char *fname, uint16_t offset);
void start(uint16_t offset);