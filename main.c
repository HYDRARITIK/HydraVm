//
//  Created by Ryan Pendleton on 6/28/18.
//  Copyright © 2018 Ryan Pendleton. All rights reserved.
//

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/errno.h>
#include <sys/termios.h>

#include "virtual.h"


static struct termios original_tio;

static void disable_input_buffering() {
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

static void restore_input_buffering() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

static void handle_signal(int signal) {
    if (signal != SIGINT) return;

    restore_input_buffering();
    printf("\n");
    exit(-2);
}
void ld_image(char *fname,uint16_t offset){
    FILE *fd=fopen(fname,"rb");
    assert(fd!=NULL);

    uint16_t *p=vm->mem+pc_Start+offset;
    fread(p,sizeof(uint16_t),(UINT16_MAX-pc_Start),fd);

    fclose(fd);
}

uint16_t running=1;
void start(uint16_t offset) { 
    vm->reg[R_PC] = pc_Start + offset; // The RPC is set
    while(running<10) {
        uint16_t i = memRead(vm->reg[R_PC]++); // We extract instructions from the memory
                                     // location pointed by RPC           
                                     // We (auto)increment RPC       
        add(i)   ;         // We execute each instruction
        running++;
    }
}
int main(int argc, char **argv) {
    // create a VM
    vm = vm_create();
    assert(vm != NULL);
    printf("vm created\n");
    ld_image(argv[1], 0x0);
    start(0x0);
    return 0;
}

int main2(int argc, const char * argv[]) {
    // if (argc != 2) {
    //     fprintf(stderr, "usage: %s <program.obj>\n", argv[0]);
    //     return VM_EXIT_USAGE;
    // }

    vm = vm_create();
    printf("address of vm %p\n",(void *)vm);
    lc3os_obj="/home/ritik/VM/2048.obj";
    // printf("address of lc3os_obj %p\n",lc3os_obj);
    vm_load_os(vm);

    vm_load_result load_result = vm_load_file(vm, lc3os_obj);

    switch (load_result) {
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

    // disable_input_buffering();
    // signal(SIGINT, handle_signal);

    vm_run_result run_result = vm_run(vm);

    printf("run result %d\n",run_result);
    // restore_input_buffering();

    switch (run_result) {
        case VM_RUN_SUCCESS:
            break;

        case VM_RUN_UNIMPLEMENTED_OPCODE:
            fprintf(stderr, "%s: Failed to execute input: Attempted to execute unimplemented opcode\n", argv[0]);
            return VM_EXIT_OPCODE_INVALID;
    }

    vm_destroy(vm);

    return VM_EXIT_SUCCESS;
}