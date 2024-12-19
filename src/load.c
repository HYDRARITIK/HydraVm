
#include "virtual.h"
#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

#include "load.h"

#include "assert.h"



extern char *opcodes[];
extern vm_impl *vm;
extern uint16_t running;


vm_load_result vm_load_data(vm_impl *vm, unsigned const char *data, uint16_t length)
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

vm_load_result ld_image(char *fname, uint16_t offset)
{
    FILE *fd = fopen(fname, "rb");
    if (fd == NULL)
    {
        fprintf(stderr, "%s: Failed to load input image: ", strerror(errno));
        return VM_LOAD_INPUT_NOT_FOUND;
    }

    uint16_t *p = (uint16_t *)(vm->mem + pc_Start + offset);
    fread(p, sizeof(uint16_t), (__UINT16_MAX__ - pc_Start), fd);

    fclose(fd);

    return VM_LOAD_SUCCESS;
}


void start(uint16_t offset)
{
    vm->reg[R_PC] = pc_Start + offset; // The RPC is set
    while (running)
    {
        uint16_t i = memRead(vm->reg[R_PC]++); // We extract instructions from the memory
                                               // location pointed by RPC
                                               // We (auto)increment RPC
        printf("instruction opcode %d %s\n", OPC(i), opcodes[OPC(i)]);
        op_ex[OPC(i)](i); // We execute each instruction
        // running++;
    }
}
