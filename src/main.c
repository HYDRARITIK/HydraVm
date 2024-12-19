
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/errno.h>
#include <sys/termios.h>


#include "load.h"
//#include "virtual.h"

extern char *opcodes[];
extern vm_impl *vm;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("please add object file path");
    }
    vm = vm_create();
    assert(vm != NULL);
    printf("vm created\n");
    vm_load_result load_result = ld_image(argv[1], 0x0);

    switch (load_result)
    {
    case VM_LOAD_SUCCESS:
        fprintf(stdout, "Loaded successfully\n");
        break;

    case VM_LOAD_INPUT_NOT_FOUND:
        fprintf(stderr, "%s: Failed to load input: %s\n", argv[0], strerror(errno));
        return VM_EXIT_INPUT_INVALID;

    case VM_LOAD_INPUT_TOO_LARGE:
        fprintf(stderr, "%s: Failed to load input: Input exceeded memory space\n", argv[0]);
        return VM_EXIT_INPUT_INVALID;
    }

    start(0x0);

    vm_destroy(vm);

        return 0;
}
